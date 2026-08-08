#!/usr/bin/env python3

import base64
import hashlib
import json
import os
import struct
import urllib.request


RPC_URL = "http://127.0.0.1:29492/"
RPC_COOKIE = os.path.expanduser("~/.galara-launch-test/.cookie")

EXTRANONCE1 = bytes.fromhex("00000001")
EXTRANONCE2_SIZE = 4

# Temporary test payout script only.
# OP_TRUE is useful for construction testing, but MUST NOT be used for
# Galara's real launch miner payout.
MINER_SCRIPT = bytes.fromhex(
    "00146de876d7867d34cab179a077342b0aa5310a384b"
)


def rpc_call(method, params=None):
    if params is None:
        params = []

    with open(RPC_COOKIE, "r", encoding="utf-8") as f:
        auth = base64.b64encode(
            f.read().strip().encode()
        ).decode()

    payload = json.dumps({
        "jsonrpc": "2.0",
        "id": "galara-job-test",
        "method": method,
        "params": params,
    }).encode()

    request = urllib.request.Request(
        RPC_URL,
        data=payload,
        headers={
            "Content-Type": "application/json",
            "Authorization": f"Basic {auth}",
        },
    )

    with urllib.request.urlopen(request, timeout=10) as response:
        reply = json.loads(response.read().decode())

    if reply.get("error"):
        raise RuntimeError(reply["error"])

    return reply["result"]


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
    raise ValueError("push_data helper only supports short values")


def serialize_output(value, script):
    return (
        struct.pack("<Q", value)
        + varint(len(script))
        + script
    )


def build_coinbase_parts(template):
    height = template["height"]

    if height != 1:
        raise RuntimeError(
            f"This test expects Galara launch block 1, got height {height}"
        )

    required = template.get(
        "galara_required_coinbase_outputs",
        []
    )

    if len(required) != 1:
        raise RuntimeError(
            f"Expected exactly one Galara required output, got {len(required)}"
        )

    premine = required[0]

    premine_value = premine["value"]
    premine_script = bytes.fromhex(
        premine["scriptPubKey"]
    )

    total_value = template["coinbasevalue"]
    miner_value = total_value - premine_value

    # Height 1 encoded as CScript number 1, then pushed.
    height_data = bytes.fromhex("01")

    script_prefix = (
        push_data(height_data)
        + b"/Galara/"
    )

    # extranonce1 and extranonce2 will be inserted here.
    script_suffix = b""

    version = struct.pack("<I", 2)

    prevout_hash = bytes(32)
    prevout_index = struct.pack("<I", 0xffffffff)

    script_length = (
        len(script_prefix)
        + len(EXTRANONCE1)
        + EXTRANONCE2_SIZE
        + len(script_suffix)
    )

    sequence = struct.pack("<I", 0xffffffff)

    coinbase1 = (
        version
        + b"\x01"
        + prevout_hash
        + prevout_index
        + varint(script_length)
        + script_prefix
    )

    outputs = (
        serialize_output(miner_value, MINER_SCRIPT)
        + serialize_output(
            premine_value,
            premine_script,
        )
    )

    coinbase2 = (
        script_suffix
        + sequence
        + varint(2)
        + outputs
        + struct.pack("<I", 0)
    )

    return {
        "coinbase1": coinbase1.hex(),
        "coinbase2": coinbase2.hex(),
        "miner_value": miner_value,
        "premine_value": premine_value,
        "premine_script": premine_script.hex(),
    }


def sha256d(data):
    return hashlib.sha256(
        hashlib.sha256(data).digest()
    ).digest()


def main():
    template = rpc_call(
        "getblocktemplate",
        [{
            "rules": [
                "segwit",
                "galara-premine",
            ]
        }]
    )

    parts = build_coinbase_parts(template)

    extranonce2 = bytes(EXTRANONCE2_SIZE)

    coinbase = (
        bytes.fromhex(parts["coinbase1"])
        + EXTRANONCE1
        + extranonce2
        + bytes.fromhex(parts["coinbase2"])
    )

    coinbase_txid_internal = sha256d(coinbase)

    print("Galara Stratum job construction test")
    print("height:", template["height"])
    print("version:", f"{template['version']:08x}")
    print("bits:", template["bits"])
    print("ntime:", f"{template['curtime']:08x}")
    print("previousblockhash:", template["previousblockhash"])

    print()
    print("miner reward sats:", parts["miner_value"])
    print("miner reward GLRA:", parts["miner_value"] / 100_000_000)

    print("premine sats:", parts["premine_value"])
    print("premine GLRA:", parts["premine_value"] / 100_000_000)
    print("premine script:", parts["premine_script"])

    print()
    print("coinbase1:", parts["coinbase1"])
    print("extranonce1:", EXTRANONCE1.hex())
    print("extranonce2 size:", EXTRANONCE2_SIZE)
    print("test extranonce2:", extranonce2.hex())
    print("coinbase2:", parts["coinbase2"])

    print()
    print("coinbase bytes:", len(coinbase))
    print(
        "coinbase txid:",
        coinbase_txid_internal[::-1].hex()
    )

    print()
    print("GBT transaction count:", len(template["transactions"]))
    print(
        "Merkle branches needed:",
        len(template["transactions"]),
    )

    assert parts["miner_value"] == 50 * 100_000_000
    assert parts["premine_value"] == 700800 * 100_000_000
    assert (
        parts["premine_script"]
        ==
        "00200f7f09c48be501a04d15a6553e7dfb4f9dec46972da5a1ea01e0fa41b7732c25"
    )
    assert len(template["transactions"]) == 0

    print()
    print("PASS: Galara block 1 job values are internally consistent.")


if __name__ == "__main__":
    main()
