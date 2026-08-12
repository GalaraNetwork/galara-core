#!/usr/bin/env python3

import hashlib
import struct
import threading
import time
import base64
import json
import os
import socketserver
import urllib.request


RPC_URL = "http://127.0.0.1:29492/"
RPC_COOKIE = os.path.expanduser(
    "~/.galara-launch-test/.cookie"
)

STRATUM_HOST = "0.0.0.0"
STRATUM_PORT = 3333


def read_rpc_cookie():
    with open(RPC_COOKIE, "r", encoding="utf-8") as cookie_file:
        return cookie_file.read().strip()


def rpc_call(method, params=None):
    if params is None:
        params = []

    payload = json.dumps({
        "jsonrpc": "2.0",
        "id": "galara-stratum",
        "method": method,
        "params": params,
    }).encode()

    auth = base64.b64encode(
        read_rpc_cookie().encode()
    ).decode()

    request = urllib.request.Request(
        RPC_URL,
        data=payload,
        headers={
            "Content-Type": "application/json",
            "Authorization": f"Basic {auth}",
        },
    )

    with urllib.request.urlopen(request, timeout=10) as response:
        result = json.loads(response.read().decode())

    if result.get("error"):
        raise RuntimeError(result["error"])

    return result["result"]
def get_galara_template():
    return rpc_call(
        "getblocktemplate",
        [{
            "rules": [
                "segwit",
                "galara-premine",
            ]
        }]
    )


EXTRANONCE2_SIZE = 4
MINER_SCRIPT = bytes.fromhex(
    "00146de876d7867d34cab179a077342b0aa5310a384b"
)


def varint(value):
    if value < 0xfd:
        return bytes([value])
    if value <= 0xffff:
        return b"\xfd" + struct.pack("<H", value)
    if value <= 0xffffffff:
        return b"\xfe" + struct.pack("<I", value)
    return b"\xff" + struct.pack("<Q", value)


def push_data(data):
    if len(data) < 0x4c:
        return bytes([len(data)]) + data
    raise ValueError("push_data only supports short values")


def encode_script_num(value):
    if value < 0:
        raise ValueError(
            "Galara block height cannot be negative"
        )

    if value == 0:
        return b""

    result = bytearray()

    while value:
        result.append(value & 0xff)
        value >>= 8

    if result[-1] & 0x80:
        result.append(0x00)

    return bytes(result)


def encode_bip34_height(height):
    if height == 0:
        return b"\x00"

    if 1 <= height <= 16:
        return bytes([0x50 + height])

    return push_data(
        encode_script_num(height)
    )


def serialize_output(value, script):
    return (
        struct.pack("<Q", value)
        + varint(len(script))
        + script
    )


def build_coinbase_parts(template):
    required = template.get(
        "galara_required_coinbase_outputs",
        []
    )

    if template["height"] == 1 and len(required) != 1:
        raise RuntimeError(
            "Galara block 1 requires exactly one treasury output"
        )

    premine_value = 0
    premine_script = b""

    if required:
        premine_value = required[0]["value"]
        premine_script = bytes.fromhex(
            required[0]["scriptPubKey"]
        )

    miner_value = template["coinbasevalue"] - premine_value

    height = template["height"]
    height_script = encode_bip34_height(height)

    script_prefix = (
        height_script
        + b"/Galara/"
    )
    # extranonce1 + extranonce2 are inserted between these pieces.
    script_suffix = b""

    script_length = (
        len(script_prefix)
        + 4
        + EXTRANONCE2_SIZE
        + len(script_suffix)
    )

    coinbase1 = (
        struct.pack("<I", 2)
        + b"\x01"
        + bytes(32)
        + struct.pack("<I", 0xffffffff)
        + varint(script_length)
        + script_prefix
    )

    outputs = serialize_output(
        miner_value,
        MINER_SCRIPT,
    )

    if required:
        outputs += serialize_output(
            premine_value,
            premine_script,
        )

    witness_commitment_hex = template.get(
        "default_witness_commitment"
    )

    if witness_commitment_hex:
        witness_commitment = bytes.fromhex(
            witness_commitment_hex
        )

        outputs += serialize_output(
            0,
            witness_commitment,
        )

    output_count = (
        1
        + len(required)
        + (1 if witness_commitment_hex else 0)
    )

    coinbase2 = (
        script_suffix
        + struct.pack("<I", 0xffffffff)
        + varint(output_count)
        + outputs
        + struct.pack("<I", 0)
    )

    return coinbase1.hex(), coinbase2.hex()


def add_coinbase_witness(
    stripped_coinbase,
    reserved_value=None,
):
    if reserved_value is None:
        reserved_value = bytes(32)

    if len(reserved_value) != 32:
        raise ValueError(
            "Witness reserved value must be 32 bytes"
        )

    if len(stripped_coinbase) < 8:
        raise ValueError(
            "Coinbase serialization is unexpectedly short"
        )

    # SegWit serialization:
    # version | marker | flag | vin/vout |
    # witness | locktime
    return (
        stripped_coinbase[:4]
        + b"\x00\x01"
        + stripped_coinbase[4:-4]
        + b"\x01\x20"
        + reserved_value
        + stripped_coinbase[-4:]
    )


def build_job(template):
    coinbase1, coinbase2 = build_coinbase_parts(template)

    merkle_branch = [
        item.hex()
        for item in build_coinbase_merkle_branch(
            template
        )
    ]

    raw_prevhash = bytes.fromhex(
        template["previousblockhash"]
    )

    stratum_prevhash = b"".join(
        raw_prevhash[i:i+4]
        for i in range(28, -1, -4)
    ).hex()

    return {
        "job_id": f"galara-{template['height']}-{template['curtime']}",
        "prevhash": stratum_prevhash,
        "coinbase1": coinbase1,
        "coinbase2": coinbase2,
        "merkle_branch": merkle_branch,
        "version": f"{template['version']:08x}",
        "nbits": template["bits"],
        "ntime": f"{template['curtime']:08x}",
        "clean_jobs": True,
    }

def sha256d(data):
    return hashlib.sha256(
        hashlib.sha256(data).digest()
    ).digest()


def build_coinbase_merkle_branch(template):
    tx_hashes = [
        bytes.fromhex(tx["txid"])[::-1]
        for tx in template["transactions"]
    ]

    if not tx_hashes:
        return []

    # None represents the coinbase path at index 0.
    level = [None] + tx_hashes
    branch = []

    while len(level) > 1:
        # The coinbase path is always the left-most node.
        if len(level) > 1:
            branch.append(level[1])

        next_level = []

        if len(level) % 2:
            level.append(level[-1])

        for i in range(0, len(level), 2):
            left = level[i]
            right = level[i + 1]

            if left is None or right is None:
                next_level.append(None)
            else:
                next_level.append(
                    sha256d(left + right)
                )

        level = next_level

    return branch


def apply_merkle_branch(
    coinbase_hash,
    branch,
):
    root = coinbase_hash

    for sibling in branch:
        root = sha256d(
            root + sibling
        )

    return root


def compact_to_target(bits_hex):
    bits = int(bits_hex, 16)
    exponent = bits >> 24
    mantissa = bits & 0x007fffff

    if exponent <= 3:
        return mantissa >> (8 * (3 - exponent))

    return mantissa << (8 * (exponent - 3))

JOBS = {}
JOBS_LOCK = threading.Lock()
JOB_REFRESH_SECONDS = 2

EXTRANONCE1_LOCK = threading.Lock()
EXTRANONCE1_COUNTER = 1
VERSION_MASK = 0x1FFFE000


def allocate_extranonce1():
    global EXTRANONCE1_COUNTER

    with EXTRANONCE1_LOCK:
        if EXTRANONCE1_COUNTER > 0xffffffff:
            raise RuntimeError(
                "Galara extranonce1 space exhausted"
            )

        value = EXTRANONCE1_COUNTER
        EXTRANONCE1_COUNTER += 1

    return value.to_bytes(4, "big")


def build_block_candidate(
    job,
    template,
    extranonce1,
    extranonce2,
    ntime,
    nonce,
    version_bits=None,
):
    stripped_coinbase = bytes.fromhex(
        job["coinbase1"]
        + extranonce1.hex()
        + extranonce2
        + job["coinbase2"]
    )

    merkle_root = apply_merkle_branch(
        sha256d(stripped_coinbase),
        [
            bytes.fromhex(item)
            for item in job["merkle_branch"]
        ],
    )

    stratum_prevhash = bytes.fromhex(
        job["prevhash"]
    )

    display_prevhash = b"".join(
        stratum_prevhash[i:i+4]
        for i in range(28, -1, -4)
    )

    header_prevhash = display_prevhash[::-1]

    job_version = int(job["version"], 16)

    if version_bits is not None:
        submitted_bits = int(version_bits, 16)

        header_version = (
            (job_version & ~VERSION_MASK)
            | (submitted_bits & VERSION_MASK)
        )
    else:
        header_version = job_version

    header = (
        struct.pack("<I", header_version)
        + header_prevhash
        + merkle_root
        + struct.pack("<I", int(ntime, 16))
        + bytes.fromhex(job["nbits"])[::-1]
        + struct.pack("<I", int(nonce, 16))
    )

    if template.get("default_witness_commitment"):
        block_coinbase = add_coinbase_witness(
            stripped_coinbase
        )
    else:
        block_coinbase = stripped_coinbase

    block = (
        header
        + varint(1 + len(template["transactions"]))
        + block_coinbase
    )

    for tx in template["transactions"]:
        block += bytes.fromhex(tx["data"])

    return block

class StratumHandler(socketserver.StreamRequestHandler):
    def setup(self):
        super().setup()
        self.write_lock = threading.Lock()
        self.stop_event = threading.Event()
        self.refresh_thread = None
        self.current_prevhash = None
        self.extranonce1 = allocate_extranonce1()

        print(
            "Assigned extranonce1",
            self.extranonce1.hex(),
            "to",
            self.client_address,
        )

    def send_response(self, request_id, result, error=None):
        response = {
            "id": request_id,
            "result": result,
            "error": error,
        }

        with self.write_lock:
            self.wfile.write(
                (json.dumps(response) + "\n").encode()
            )
            self.wfile.flush()

    def send_notification(self, method, params):
        message = {
            "id": None,
            "method": method,
            "params": params,
        }

        with self.write_lock:
            self.wfile.write(
                (json.dumps(message) + "\n").encode()
            )
            self.wfile.flush()

    def send_job(self, template):
        job = build_job(template)

        with JOBS_LOCK:
            for stored in JOBS.values():
                if (
                    stored["template"]["previousblockhash"]
                    != template["previousblockhash"]
                ):
                    stored["stale"] = True

            JOBS[job["job_id"]] = {
                "job": job,
                "template": template,
                "stale": False,
            }

        self.current_prevhash = template[
            "previousblockhash"
        ]

        self.send_notification(
            "mining.notify",
            [
                job["job_id"],
                job["prevhash"],
                job["coinbase1"],
                job["coinbase2"],
                job["merkle_branch"],
                job["version"],
                job["nbits"],
                job["ntime"],
                job["clean_jobs"],
            ],
        )

        print(
            "Sent Galara job:",
            job["job_id"],
            "height",
            template["height"],
        )

        return job

    def job_refresh_loop(self):
        while not self.stop_event.wait(
            JOB_REFRESH_SECONDS
        ):
            try:
                template = get_galara_template()

                new_prevhash = template[
                    "previousblockhash"
                ]

                if new_prevhash == self.current_prevhash:
                    continue

                print()
                print(
                    "New Galara chain tip detected:",
                    new_prevhash,
                )

                self.send_job(template)

            except Exception as exc:
                if not self.stop_event.is_set():
                    print(
                        "Galara job refresh error:",
                        exc,
                    )

    def verify_submit(self, params):
        if len(params) not in (5, 6):
            raise ValueError(
                "mining.submit requires 5 or 6 parameters"
            )

        worker = params[0]
        job_id = params[1]
        extranonce2 = params[2]
        ntime = params[3]
        nonce = params[4]

        version_bits = None
        if len(params) == 6:
            version_bits = params[5]

        with JOBS_LOCK:
            stored = JOBS.get(job_id)

            if stored is None:
                return False, [
                    21,
                    "Job not found",
                    None,
                ]

            if stored.get("stale", False):
                return False, [
                    21,
                    "Stale job",
                    None,
                ]

            job = stored["job"]

        if len(extranonce2) != EXTRANONCE2_SIZE * 2:
            return False, [
                20,
                "Incorrect extranonce2 size",
                None,
            ]

        try:
            bytes.fromhex(extranonce2)
            bytes.fromhex(ntime)
            bytes.fromhex(nonce)
        except ValueError:
            return False, [
                20,
                "Invalid hexadecimal submit parameter",
                None,
            ]

        if len(ntime) != 8 or len(nonce) != 8:
            return False, [
                20,
                "ntime and nonce must be 4-byte hex values",
                None,
            ]

        coinbase = bytes.fromhex(
            job["coinbase1"]
            + self.extranonce1.hex()
            + extranonce2
            + job["coinbase2"]
        )

        merkle_root = apply_merkle_branch(
            sha256d(coinbase),
            [
                bytes.fromhex(item)
                for item in job["merkle_branch"]
            ],
        )

        stratum_prevhash = bytes.fromhex(
            job["prevhash"]
        )

        display_prevhash = b"".join(
            stratum_prevhash[i:i+4]
            for i in range(28, -1, -4)
        )

        header_prevhash = display_prevhash[::-1]

        job_version = int(job["version"], 16)

        if version_bits is not None:
            try:
                submitted_bits = int(version_bits, 16)
            except ValueError:
                return False, [
                    20,
                    "Invalid version_bits",
                    None,
                ]

            if submitted_bits & ~VERSION_MASK:
                return False, [
                    20,
                    "Version bits outside negotiated mask",
                    None,
                ]

            header_version = (
                (job_version & ~VERSION_MASK)
                | (submitted_bits & VERSION_MASK)
            )
        else:
            header_version = job_version

        header = (
            struct.pack("<I", header_version)
            + header_prevhash
            + merkle_root
            + struct.pack("<I", int(ntime, 16))
            + bytes.fromhex(job["nbits"])[::-1]
            + struct.pack("<I", int(nonce, 16))
        )

        if len(header) != 80:
            raise RuntimeError(
                f"Unexpected header length: {len(header)}"
            )

        hash_internal = sha256d(header)
        hash_display = hash_internal[::-1].hex()
        hash_value = int.from_bytes(
            hash_internal,
            "little",
        )

        test_easy_shares = (
            os.environ.get(
                "GALARA_STRATUM_TEST_EASY_SHARES",
                "0",
            )
            == "1"
        )

        if test_easy_shares:
            share_target = (1 << 256) - 1
        else:
            share_target = compact_to_target(
                "1d00ffff"
            )

        network_target = compact_to_target(
            job["nbits"]
        )

        meets_share = hash_value <= share_target

        test_force_network = (
            os.environ.get(
                "GALARA_STRATUM_TEST_FORCE_NETWORK",
                "0",
            )
            == "1"
        )

        meets_network = (
            hash_value <= network_target
            or test_force_network
        )

        print()
        print("Galara mining.submit received")
        print("worker:", worker)
        print("job:", job_id)
        print("extranonce2:", extranonce2)
        print("ntime:", ntime)
        print("nonce:", nonce)

        if version_bits is not None:
            print("version_bits:", version_bits)

        print("candidate hash:", hash_display)
        print("meets configured share target:", meets_share)
        print("meets Galara network target:", meets_network)

        if meets_network:
            print(
                "*** GALARA NETWORK-TARGET "
                "BLOCK CANDIDATE FOUND ***"
            )

            block = build_block_candidate(
                job,
                stored["template"],
                self.extranonce1,
                extranonce2,
                ntime,
                nonce,
                version_bits,
            )

            proposal_result = rpc_call(
                "getblocktemplate",
                [{
                    "mode": "proposal",
                    "data": block.hex(),
                    "rules": [
                        "segwit",
                        "galara-premine",
                    ],
                }]
            )

            print(
                "galarad proposal result:",
                proposal_result,
            )

            submit_enabled = (
                os.environ.get(
                    "GALARA_STRATUM_SUBMIT_BLOCKS",
                    "0",
                )
                == "1"
            )

            if proposal_result is not None:
                print(
                    "BLOCK NOT SUBMITTED: "
                    "proposal validation rejected it."
                )

            elif not submit_enabled:
                print(
                    "BLOCK NOT SUBMITTED: "
                    "GALARA_STRATUM_SUBMIT_BLOCKS "
                    "is disabled."
                )

            else:
                submit_result = rpc_call(
                    "submitblock",
                    [block.hex()],
                )

                print(
                    "galarad submitblock result:",
                    submit_result,
                )

                if submit_result is None:
                    print(
                        "*** GALARA BLOCK SUBMITTED "
                        "SUCCESSFULLY ***"
                    )
                else:
                    print(
                        "Galara block submission rejected:",
                        submit_result,
                    )

        if not meets_share:
            return False, [
                23,
                "Low difficulty share",
                None,
            ]

        return True, None

    def handle(self):
        print(f"Miner connected: {self.client_address}")

        while True:
            line = self.rfile.readline()

            if not line:
                break

            try:
                message = json.loads(
                    line.decode().strip()
                )
                print("RX:", message)

                request_id = message.get("id")
                method = message.get("method")
                params = message.get("params", [])

                if method == "mining.configure":
                    self.send_response(
                        request_id,
                        {
                            "version-rolling": True,
                            "version-rolling.mask":
                                f"{VERSION_MASK:08x}",
                        },
                    )

                elif method == "mining.subscribe":
                    self.send_response(
                        request_id,
                        [
                            [
                                [
                                    "mining.set_difficulty",
                                    "galara-sub-1",
                                ],
                                [
                                    "mining.notify",
                                    "galara-sub-1",
                                ],
                            ],
                            self.extranonce1.hex(),
                            EXTRANONCE2_SIZE,
                        ],
                    )

                elif method == "mining.authorize":
                    self.send_response(
                        request_id,
                        True,
                    )

                    template = get_galara_template()

                    self.send_notification(
                        "mining.set_difficulty",
                        [1],
                    )

                    self.send_job(template)

                    if self.refresh_thread is None:
                        self.refresh_thread = (
                            threading.Thread(
                                target=self.job_refresh_loop,
                                daemon=True,
                                name=(
                                    "galara-job-refresh-"
                                    f"{self.client_address[0]}"
                                ),
                            )
                        )
                        self.refresh_thread.start()

                elif method == "mining.submit":
                    accepted, error = self.verify_submit(
                        params
                    )

                    self.send_response(
                        request_id,
                        accepted,
                        error,
                    )

                elif method == "mining.suggest_difficulty":
                    self.send_response(
                        request_id,
                        True,
                    )

                elif method == "mining.extranonce.subscribe":
                    self.send_response(
                        request_id,
                        True,
                    )

                else:
                    self.send_response(
                        request_id,
                        None,
                        [
                            20,
                            f"Unsupported method: {method}",
                            None,
                        ],
                    )

            except Exception as exc:
                print("ERROR:", exc)

                try:
                    self.send_response(
                        message.get("id"),
                        False,
                        [
                            20,
                            str(exc),
                            None,
                        ],
                    )
                except Exception:
                    pass

        self.stop_event.set()
        print(f"Miner disconnected: {self.client_address}")


def main():
    template = get_galara_template()

    print("Galara template received")
    print("height:", template["height"])
    print("bits:", template["bits"])
    print("coinbasevalue:", template["coinbasevalue"])

    required = template.get(
        "galara_required_coinbase_outputs",
        []
    )

    print("required premine outputs:")
    print(json.dumps(required, indent=2))

    with socketserver.ThreadingTCPServer(
        (STRATUM_HOST, STRATUM_PORT),
        StratumHandler,
    ) as server:
        print(
            f"Galara Stratum test server listening on "
            f"{STRATUM_HOST}:{STRATUM_PORT}"
        )

        server.serve_forever()


if __name__ == "__main__":
    main()
