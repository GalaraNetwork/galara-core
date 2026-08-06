# Galara Network and Consensus Specification

**Document status:** Draft
**Codebase:** Bitcoin Core v31.1
**Network name:** Galara
**Currency ticker:** GLRA

This document defines the intended Galara network and consensus rules before
those rules are implemented in source code.

Consensus values must not be changed casually after the public network begins.
Any value marked **TBD** remains undecided and must not be treated as final.

## 1. Project identity

| Property | Value | Status |
|---|---:|---|
| Network name | Galara | Confirmed |
| Currency ticker | GLRA | Confirmed |
| Client name | Galara Core | Confirmed |
| Source repository | GalaraNetwork/galara-core | Confirmed |
| Upstream base | Bitcoin Core v31.1 | Confirmed |
| Smallest unit | 0.00000001 GLRA | Proposed inherited behavior |

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

### Proposed subsidy schedule

| Era | Starting height | Subsidy |
|---:|---:|---:|
| 0 | 0 or post-premine start | 50 GLRA |
| 1 | 700,800 | 25 GLRA |
| 2 | 1,401,600 | 12.5 GLRA |
| 3 | 2,102,400 | 6.25 GLRA |
| 4 | 2,803,200 | 3.125 GLRA |
| 5 | 3,504,000 | 1.5625 GLRA |
| 6 | 4,204,800 | 0.78125 GLRA |

The exact interaction between the premine height and the first subsidy era is
still **TBD**.

## 4. Supply

### Ordinary mining supply

The ideal geometric-series estimate is 70,080,000 GLRA.

Because block rewards are represented in integer base units and truncated after
each halving, the exact maximum ordinary subsidy using Bitcoin-style integer
arithmetic is approximately 70,079,999.922912 GLRA.

### Proposed premine

| Property | Value | Status |
|---|---:|---|
| Proposed premine | 700,800 GLRA | Proposed |
| Premine block height | TBD | Undecided |
| Premine recipient script/address | TBD | Undecided |
| Premine custody and disclosure policy | TBD | Undecided |

### Total maximum supply

The nominal rounded maximum is 70,780,800 GLRA.

The approximate exact maximum, including the proposed premine, is
70,780,799.922912 GLRA.

## 5. Difficulty adjustment

| Property | Value | Status |
|---|---:|---|
| Mainnet difficulty algorithm | TBD | Undecided |
| Candidate algorithm | ASERT | Under consideration |
| Emergency difficulty adjustment | TBD | Undecided |
| Minimum-difficulty blocks on mainnet | Proposed disabled | Proposed |
| Testnet minimum-difficulty behavior | TBD | Undecided |
| Regtest difficulty behavior | Easy local mining | Expected |

The difficulty algorithm must be chosen before the genesis block and public
network launch.

The chosen algorithm should:

- Respond safely to sudden hashrate changes.
- Avoid long periods without blocks.
- Avoid oscillation between difficulty levels.
- Support a new network beginning with low and variable hashrate.
- Remain understandable and testable.

## 6. Genesis blocks

A separate genesis block is required for every applicable network.

| Network | Genesis block | Status |
|---|---|---|
| Mainnet | TBD | Undecided |
| Testnet | TBD | Undecided |
| Signet | TBD | Undecided |
| Regtest | TBD or inherited development structure | Undecided |

Required mainnet genesis inputs:

- Human-readable timestamp message
- Genesis output public key or script
- Genesis reward policy
- Block timestamp
- Nonce
- Difficulty bits
- Version
- Merkle root
- Genesis block hash

The genesis block must not be generated until the timestamp message, output
script, initial difficulty, and network identity values are final.

## 7. Network identity

### Mainnet

| Property | Value |
|---|---|
| Default P2P port | 29471 |
| Default RPC port | 29472 |
| Message-start bytes | TBD |
| Base58 public-key address prefix | TBD |
| Base58 script address prefix | TBD |
| Base58 private-key prefix | TBD |
| Bech32 HRP | TBD |
| Extended public-key prefix | TBD |
| Extended private-key prefix | TBD |

### Testnet

| Property | Value |
|---|---|
| Default P2P port | 39471 |
| Default RPC port | 39472 |
| Message-start bytes | TBD |
| Base58 address prefixes | TBD |
| Bech32 HRP | TBD |

### Regtest

| Property | Value |
|---|---|
| Default P2P port | 49471 |
| Default RPC port | 49472 |
| Message-start bytes | TBD |
| Bech32 HRP | TBD |

Every message-start sequence must be distinct from Bitcoin and from known
networks likely to run on the same machines.

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

Final prefix values are **TBD**.

## 9. Consensus deployments and activations

Bitcoin Core v31.1 contains inherited consensus rules and deployment
infrastructure. Galara must decide whether features are active from genesis or
activated at defined heights.

Items requiring review include:

- BIP34 height in coinbase
- BIP65 CHECKLOCKTIMEVERIFY
- BIP66 strict DER signatures
- CSV
- SegWit
- Taproot
- Minimum chain work
- Assume-valid block
- Version-bits deployment settings
- Buried deployment heights

Preferred direction:

Modern, established Bitcoin consensus features active from genesis where safe.

Exact values remain **TBD** until source locations and testing requirements are
reviewed.

## 10. Chain safety values

| Property | Value | Status |
|---|---:|---|
| Minimum chain work | Zero during private development | Proposed |
| Assume-valid block | Null during private development | Proposed |
| DNS seeds | None initially | Proposed |
| Fixed seeds | None initially | Proposed |
| Checkpoints | Genesis only initially | Proposed |

These values must be updated as the public network develops.

## 11. Premine policy requirements

Before a premine is implemented, the project must publish:

- Exact amount
- Exact recipient address or output script
- Block height and transaction method
- Intended allocation
- Custody arrangements
- Spending transparency plan
- Whether funds are locked, vested, multisignature-controlled, or immediately
  spendable

The premine must be reproducible from source code and visible to every node.

## 12. Implementation order

Consensus implementation should occur in this order:

1. Finalize this specification.
2. Choose network ports and message-start bytes.
3. Choose address and key prefixes.
4. Finalize the subsidy and premine implementation.
5. Select and document the difficulty algorithm.
6. Decide inherited activation heights.
7. Finalize the genesis timestamp and output script.
8. Implement mainnet parameters.
9. Implement testnet and regtest parameters.
10. Generate and verify genesis blocks.
11. Add consensus and chain-parameter tests.
12. Perform clean builds and isolated-network testing.
13. Launch a private multi-node development network.
14. Perform public testnet testing.
15. Consider mainnet launch only after testnet validation.

## 13. Decisions log

| Date | Decision | Rationale |
|---|---|---|
| TBD | SHA-256d proof of work | ASIC and Bitaxe compatibility |
| TBD | 180-second target spacing | Faster confirmation than Bitcoin while retaining proof of work |
| TBD | 50 GLRA initial subsidy | Familiar Bitcoin-style initial emission |
| TBD | 700,800-block halving | Approximately four years at 180-second blocks |
| TBD | 700,800 GLRA premine | Proposed project and ecosystem allocation |

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
