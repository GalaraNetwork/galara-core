# Galara Network and Consensus Specification

**Document status:** Implemented pre-release specification
**Codebase:** Bitcoin Core v31.1
**Network name:** Galara
**Currency ticker:** GLRA

This document records the implemented Galara network and consensus rules.

Consensus values must not be changed casually. Consensus changes after public
mainnet launch may require coordinated network upgrades or a hard fork.

## 1. Project identity

| Property | Value | Status |
|---|---:|---|
| Network name | Galara | Confirmed |
| Currency ticker | GLRA | Confirmed |
| Client name | Galara Core | Confirmed |
| Source repository | GalaraNetwork/galara-core | Confirmed |
| Upstream base | Bitcoin Core v31.1 | Confirmed |
| Smallest unit | 0.00000001 GLRA | Confirmed |

## 2. Proof of work

| Property | Value | Status |
|---|---:|---|
| Proof-of-work algorithm | SHA-256d | Confirmed |
| Mining hardware | SHA-256 ASICs, including Bitaxe-class miners | Confirmed |
| Target block spacing | 180 seconds | Confirmed |
| Blocks per hour | 20 | Derived |
| Blocks per day | 480 | Derived |
| Approximate blocks per year | 175,200 | Derived |

Galara will be an independent SHA-256d proof-of-work network. It will not
require merged mining with Bitcoin, Bloodstone, Stone, or another blockchain.

## 3. Block subsidy

| Property | Value | Status |
|---|---:|---|
| Initial ordinary block subsidy | 50 GLRA | Confirmed |
| Halving interval | 700,800 blocks | Confirmed |
| Approximate time between halvings | 4 years | Derived |
| Tail emission | None | Confirmed |
| Ordinary miner subsidy after final halving | 0 GLRA | Derived |

At a target spacing of 180 seconds, 700,800 blocks equal 1,460 days,
which is approximately four years.

### Subsidy schedule

| Era | Starting height | Subsidy |
|---:|---:|---:|
| 0 | 0 | 50 GLRA |
| 1 | 700,800 | 25 GLRA |
| 2 | 1,401,600 | 12.5 GLRA |
| 3 | 2,102,400 | 6.25 GLRA |
| 4 | 2,803,200 | 3.125 GLRA |
| 5 | 3,504,000 | 1.5625 GLRA |
| 6 | 4,204,800 | 0.78125 GLRA |

The one-time treasury premine is separate from the ordinary block subsidy.
It occurs at block height 1 and does not alter the subsidy era boundaries.

## 4. Supply

### Ordinary mining supply

The ideal geometric-series estimate is 70,080,000 GLRA.

Because block rewards are represented in integer base units and truncated after
each halving, the exact maximum ordinary subsidy using Bitcoin-style integer
arithmetic is approximately 70,079,999.922912 GLRA.

### Treasury premine

| Property | Value | Status |
|---|---:|---|
| Premine amount | 700,800 GLRA | Confirmed |
| Premine block height | 1 | Confirmed |
| Premine recipient | Galara Network Treasury 2-of-3 native SegWit multisig | Confirmed |
| Premine scriptPubKey | `00200f7f09c48be501a04d15a6553e7dfb4f9dec46972da5a1ea01e0fa41b7732c25` | Confirmed |

The block-1 treasury payment is a separate one-time issuance and is enforced
by consensus. Exactly one output must pay 700,800 GLRA to the configured
treasury script.

### Total maximum supply

The nominal rounded maximum is 70,780,800 GLRA.

The approximate exact maximum, including the treasury premine, is
70,780,799.922912 GLRA.

## 5. Difficulty adjustment

| Property | Value | Status |
|---|---:|---|
| Mainnet difficulty algorithm | ASERT | Confirmed |
| ASERT half-life | 172,800 seconds (2 days) | Confirmed |
| Target block spacing | 180 seconds (3 minutes) | Confirmed |
| ASERT activation height | 3 | Confirmed |
| Initial post-genesis difficulty bits | `0x1b014f8a` | Confirmed |
| Emergency difficulty adjustment | None | Confirmed |
| Minimum-difficulty blocks on mainnet | Disabled | Confirmed |
| Testnet minimum-difficulty blocks | Disabled | Confirmed |
| Regtest difficulty behavior | Easy local mining | Confirmed |

Galara uses ASERT to adjust proof-of-work difficulty toward the target
three-minute block interval. The configured ASERT half-life is two days.

The ASERT implementation is intended to:

- Respond to sudden hashrate changes.
- Recover from periods with little or no mining.
- Adjust continuously rather than at large periodic retarget boundaries.
- Support a new network with low and variable hashrate.
- Remain deterministic and testable by every validating node.

Physical Bitaxe mining validation confirmed that difficulty increased as rapid
blocks were accepted after an extended period of low mining activity.

## 6. Genesis blocks

### Mainnet genesis

| Property | Value |
|---|---|
| Timestamp message | `06/Aug/2026 Galara Network - A new chain begins` |
| Output script | `OP_RETURN` |
| Reward | 50 GLRA |
| Time | 1786062300 |
| Nonce | 1047001877 |
| Difficulty bits | `0x1d00ffff` |
| Version | 1 |
| Merkle root | `196a88852b529de6fa784c49e0c53b7985d2a0437475a59aefde952e2c026303` |
| Genesis hash | `000000001c00b80243bd1bcfd7a4ee9711707bfd4bd3bbd292792ec87538dff3` |

The mainnet genesis output is intentionally unspendable. The treasury premine
is separate and occurs at block height 1.

### Testnet genesis

Galara Testnet uses its own independent genesis block and network identity.

| Property | Value |
|---|---|
| Timestamp message | `16/Aug/2026 Galara Testnet - Independent testing begins` |
| Output script | `OP_RETURN` |
| Reward | 50 GLRA |
| Time | 1786918621 |
| Nonce | 77312931 |
| Difficulty bits | `0x1d00ffff` |
| Version | 1 |
| Merkle root | `3fc33817a8b9e73c08a62a4f4f7911e7dce504c22e7890fdc4b419103ed9c4ae` |
| Genesis hash | `000000007c1ad2c2d90adda5fac8d523a97112d34332a953febba17fd4c4f5ff` |

Signet and regtest retain development-oriented inherited structures unless
changed separately in source.

## 7. Network identity

### Mainnet

| Property | Value |
|---|---|
| Default P2P port | 29471 |
| Default RPC port | 29472 |
| Message-start bytes | `30 b8 e8 52` |
| Base58 public-key address prefix | 38 (`0x26`), producing `G...` |
| Base58 script address prefix | 97 (`0x61`), producing `g...` |
| Base58 private-key prefix | 150 (`0x96`), producing compressed WIF `P...` |
| Bech32 HRP | `glra` |
| Extended public-key prefix | `01 49 0a ff` (`0x01490aff`), producing `Gpub...` |
| Extended private-key prefix | `01 49 06 c5` (`0x014906c5`), producing `Gprv...` |

### Testnet

| Property | Value |
|---|---|
| Default P2P port | 39471 |
| Default RPC port | 39472 |
| Message-start bytes | `47 b2 ad c9` |
| Base58 public-key address prefix | 65 (`0x41`), producing `T...` |
| Base58 script address prefix | 127 (`0x7f`), producing `t...` |
| Base58 private-key prefix | 183 (`0xb7`), producing compressed WIF `U...` |
| Bech32 HRP | `tglra` |
| Extended public-key prefix | `02 2d bf 5b` (`0x022dbf5b`), producing `Tpub...` |
| Extended private-key prefix | `02 2d bb 21` (`0x022dbb21`), producing `Tprv...` |

### Regtest

| Property | Value |
|---|---|
| Default P2P port | 49471 |
| Default RPC port | 49472 |
| Message-start bytes | `7f 1b 68 3c` |
| Base58 public-key address prefix | 65 (`0x41`), producing `T...` |
| Base58 script address prefix | 127 (`0x7f`), producing `t...` |
| Base58 private-key prefix | 163 (`0xa3`), producing compressed WIF `R...` |
| Bech32 HRP | `rglra` |
| Extended public-key prefix | `02 2d bf 5b` (`0x022dbf5b`), producing `Tpub...` |
| Extended private-key prefix | `02 2d bb 21` (`0x022dbb21`), producing `Tprv...` |

Every message-start sequence must be distinct from Bitcoin and from known
networks likely to run on the same machines.

The selected message-start values were deterministically derived from the
SHA-256 hashes of the labels `Galara mainnet`, `Galara testnet`, and
`Galara regtest`. They are distinct from each other and from the inherited
Bitcoin network values reviewed in the source tree.

Ports should avoid conflicts with Bitcoin Core and other projects associated
with the Galara community.

The selected Galara ports were checked against the source repository, active
local listeners, and `/etc/services`. No direct conflicts were found.

## 8. Address format goals

Galara addresses must be visibly distinguishable from Bitcoin addresses.

Desired properties:

- Native SegWit addresses have a Galara-specific HRP.
- Legacy addresses should not resemble valid Bitcoin mainnet addresses.
- Test addresses must be clearly distinguishable from Galara mainnet addresses.
- Private-key exports must use a Galara-specific prefix.
- Extended keys should not be confused with Bitcoin xpub or xprv keys unless
  deliberate compatibility is chosen and clearly documented.

Mainnet Base58 prefixes were selected by generating Base58Check examples
across multiple payloads and private keys.

The selected mainnet visual identity is:

- Legacy public-key addresses: `G...`
- Legacy script addresses: `g...`
- Native SegWit addresses: `glra1...`
- Compressed private-key WIF: `P...`

Extended-key prefixes were selected by calculating the four-byte Base58Check
version ranges capable of producing stable, network-specific prefixes across
multiple serialized extended-key payloads.

The selected extended-key identity is:

- Mainnet public extended keys: `Gpub...`
- Mainnet private extended keys: `Gprv...`
- Testnet and regtest public extended keys: `Tpub...`
- Testnet and regtest private extended keys: `Tprv...`

Regtest deliberately shares its extended-key versions with testnet, while its
ports, message-start bytes, Bech32 HRP, and chain data remain separate.

## 9. Consensus deployments and activations

Galara mainnet uses the following activation settings:

| Rule | Mainnet setting |
|---|---|
| BIP34 | Height 1 |
| BIP65 CHECKLOCKTIMEVERIFY | Height 1 |
| BIP66 strict DER signatures | Height 1 |
| CSV | Height 1 |
| SegWit | Height 0 |
| Taproot | Always active from launch |
| TESTDUMMY deployment | Never active |
| Minimum BIP9 warning height | 0 |

Taproot uses the inherited version-bits deployment structure but is configured
`ALWAYS_ACTIVE` with minimum activation height 0.

The TESTDUMMY deployment remains present for development infrastructure but is
configured `NEVER_ACTIVE`.

## 10. Chain safety and bootstrap values

| Property | Current mainnet value | Status |
|---|---:|---|
| Minimum chain work | Zero | Current pre-release value |
| Assume-valid block | Null | Current pre-release value |
| DNS seeds | None | Current pre-release value |
| Fixed seeds | None | Current pre-release value |

These are operational bootstrap and chain-safety values rather than monetary
consensus parameters. They should be reviewed and updated as Galara mainnet
accumulates stable public history and independent nodes.

## 11. Treasury premine policy

Galara implements a one-time treasury premine with the following consensus
properties:

- Amount: 700,800 GLRA
- Height: block 1
- Destination: Galara Network Treasury
- Custody: 2-of-3 native SegWit multisignature
- ScriptPubKey:
  `00200f7f09c48be501a04d15a6553e7dfb4f9dec46972da5a1ea01e0fa41b7732c25`

The treasury output is reproducible from source code and visible to every
validating node.

Consensus validation requires the block-1 coinbase to contain exactly one
output paying the full premine amount to the configured treasury script.
Missing, redirected, incorrectly valued, or duplicated treasury outputs are
invalid.

## 12. Implementation and validation status

The major Galara consensus implementation stages have been completed:

1. Network ports and message-start bytes selected.
2. Address and extended-key prefixes selected.
3. Subsidy and halving schedule implemented.
4. Block-1 treasury premine implemented and consensus-enforced.
5. ASERT difficulty adjustment implemented.
6. Mainnet activation heights configured.
7. Mainnet genesis finalized and verified.
8. Galara Testnet genesis finalized and verified.
9. Consensus and chain-parameter tests added.
10. Multi-node testnet synchronization verified.
11. Public fixed-seed discovery and failover verified on Testnet.
12. Galara Stratum V1 mining integration implemented.
13. Physical Bitaxe mainnet mining and block submission verified.

The remaining work before a formal release is release-readiness review,
documentation cleanup, focused regression testing, build verification, and
release-candidate tagging.

## 13. Decisions log

| Status | Decision | Rationale |
|---|---|---|
| Confirmed | SHA-256d proof of work | ASIC and Bitaxe compatibility |
| Confirmed | 180-second target spacing | Faster confirmation than Bitcoin while retaining proof of work |
| Confirmed | 50 GLRA initial subsidy | Bitcoin-style declining block subsidy |
| Confirmed | 700,800-block halving | Approximately four years at 180-second blocks |
| Confirmed | 700,800 GLRA treasury premine | One-time Galara Network Treasury allocation |
| Confirmed | Two-day ASERT half-life | Continuous difficulty response to changing hashrate |
| Confirmed | 2-of-3 native SegWit treasury custody | Reduces reliance on any single treasury key |

## 14. Source implementation inventory

The primary implementation locations in Bitcoin Core v31.1 are:

| Concern | Primary source location |
|---|---|
| Network and consensus parameters | `src/kernel/chainparams.cpp` |
| Consensus parameter structure | `src/consensus/params.h` |
| Base RPC ports and network selection | `src/chainparamsbase.cpp` |
| Ordinary block subsidy | `src/validation.cpp` |
| Difficulty calculation | `src/pow.cpp` |
| Coinbase reward construction | `src/node/miner.cpp` |
| Subsidy tests | `src/test/validation_tests.cpp` |
| Difficulty tests | `src/test/pow_tests.cpp` |

### Inventory conclusions

- The ordinary subsidy currently begins at 50 coins and is right-shifted once
  per subsidy-halving interval.
- The inherited difficulty algorithm is Bitcoin's periodic retarget algorithm.
- ASERT would require new consensus logic and dedicated test vectors.
- The inherited genesis transaction output is not spendable.
- A usable premine should therefore not rely on the genesis reward alone.
- Mainnet currently contains Bitcoin activation heights, seeds, chain work,
  assume-valid values, address prefixes, ports, and genesis assertions that
  must all be replaced before Galara can operate as an independent network.
- Network-specific values must be finalized before generating Galara genesis
  blocks.
