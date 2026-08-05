# Galara Network Project Charter and Blockchain Specification v0.1

**Status:** Preliminary approved design  
**Network:** Galara  
**Native coin:** Galara  
**Working ticker:** GLRA  
**Tagline:** Family founded. Faith guided. Purpose driven.  
**Mining message:** Mine with Purpose.

## 1. Purpose

Galara Network is an independent, family-founded technology ecosystem connecting faith, healthcare, wellness, creativity, gaming, community service, and decentralized technology.

The Galara blockchain must continue operating if all Bloodstone, STONE, ROD, AZURE, Bitcoin, third-party pools, seed nodes, websites, and original developer services become unavailable.

## 2. Family foundation

The name Galara was inspired by Gage, Lillian, Amara, and Ayla. It represents the family foundation behind the project and the intention to build something lasting, responsible, and useful.

## 3. Ecosystem

- **Galara Coin:** Native cryptocurrency of the Galara blockchain.
- **Galara Forge:** Future mining, node, infrastructure, and development division.
- **Galara Gives:** Future community-service and charitable initiative.
- **Azure Guardians:** Separate gaming and creative project. AZURE may remain connected to Bloodstone.
- **Redeemed Medical:** Separate founding service partner.
- **Redeemed Wellness & Fitness:** Separate founding wellness partner.

## 4. Core principles

1. Independence
2. Transparency
3. Open-source development
4. Responsible stewardship
5. Family-centered purpose
6. Faith-guided service
7. Direct and understandable mining
8. Public coin economics
9. No hidden founder controls
10. No required parent blockchain

No secret function may permit unauthorized coin creation, balance seizure, private transaction reversal, or hidden ledger alteration.

## 5. Preliminary blockchain design

| Parameter | Specification |
|---|---|
| Blockchain type | Independent Bitcoin-derived blockchain |
| Consensus | Proof of Work |
| Mining algorithm | SHA-256d |
| Parent blockchain | None |
| Mandatory merged mining | None |
| Target block time | 180 seconds |
| Starting block reward | 50 GLRA |
| Halving interval | 700,800 blocks |
| Approximate halving period | 4 years |
| Decimal places | 8 |
| Difficulty adjustment | Every block or short rolling window |
| Solo mining | Supported |
| Pool mining | Supported |
| Entry hardware | Bitaxe |
| Larger hardware | Other SHA-256 ASICs |
| Smart contracts | Not included initially |
| Testnet | Required |

## 6. Supply

The estimated mineable supply is **70,080,000 GLRA**.

The proposed premine is **700,800 GLRA**.

The proposed maximum total supply is therefore **70,780,800 GLRA**, subject to verification against the implemented subsidy rules and rounding behavior.

No additional issuance may occur outside the published consensus rules.

## 7. Mining requirements

Miners must earn GLRA directly.

Intended path:

`SHA-256 miner → Galara pool or solo server → Galara node → GLRA wallet`

Miners must not be required to mine another coin, use another project’s wallet address, receive payout in another asset, use mandatory merged mining, or depend on Bloodstone infrastructure.

Pool requirements:

- Standard SHA-256 hardware support
- Bitaxe compatibility
- Low share difficulty
- Wallet-address usernames
- Worker names
- Direct GLRA payouts
- Public blocks and payment records
- Transparent fees

## 8. Difficulty requirements

Bitcoin’s unmodified 2,016-block retarget must not be adopted without evidence that it is safe for a small, variable-hash-rate network.

Testing must cover:

- sudden large ASIC arrival;
- sudden hash-rate departure;
- low-hash-rate Bitaxe-only periods;
- timestamp manipulation;
- oscillating hash rate;
- chain recovery after outages.

The final algorithm must be documented and independently reviewed before mainnet.

## 9. Ownership and control

The founder-controlled organization must retain administrative access to:

- GitHub organization and repositories
- domains and project email
- seed nodes
- mining pool
- explorer
- website and release storage
- documentation
- social accounts
- signing keys
- server credentials
- backups and recovery codes

No contractor may be the sole owner of essential infrastructure.

## 10. Open-source requirements

The repository must document:

- upstream source and exact commit;
- applicable license;
- consensus modifications;
- security-related modifications;
- dependencies;
- build and release process;
- upgrade and recovery procedures.

Compiled-only distribution is prohibited.

## 11. Development stages

1. Regtest
2. Private testnet with at least three nodes
3. Public testnet
4. Mainnet after acceptance requirements are met

Testnet coins have no monetary value and do not automatically convert to mainnet GLRA.

## 12. Testnet acceptance requirements

Before mainnet:

1. Official source builds on supported systems.
2. At least three independent nodes connect.
3. Wallet creation, backup, restore, and recovery work.
4. SHA-256d blocks validate.
5. Bitaxe connects through Stratum.
6. Rewards pay directly in test GLRA.
7. Solo and pool mining work.
8. Difficulty responds safely to rising and falling hash rate.
9. Network continues after a node failure.
10. Full recovery works from documentation and backups.
11. A second developer can build and operate the network.
12. No external blockchain is required.

## 13. Mainnet launch requirements

- final reviewed source;
- final genesis block;
- final ports and identifiers;
- three or more seed nodes;
- pool and solo-mining instructions;
- Bitaxe guide;
- block explorer;
- Windows wallet and Linux daemon;
- backup infrastructure;
- public supply and premine documentation;
- signed releases and checksums;
- disaster-recovery procedure;
- public launch notice.

## 14. Project separation

AZURE and GLRA are separate assets. They must not be represented as interchangeable unless a future transparent exchange mechanism is developed.

Redeemed Medical and Redeemed Wellness & Fitness remain legally, financially, and operationally separate. GLRA ownership does not guarantee treatment, prescriptions, discounts, insurance coverage, or healthcare access.

## 15. Mission statement

Galara Network is a family-founded, faith-guided ecosystem connecting health, technology, creativity, and service.

Technology should be developed responsibly, explained clearly, and used to support meaningful work.

**Family founded. Faith guided. Purpose driven.**
