# Galara Core

**Galara Network**  
**Native coin:** Galara  
**Working ticker:** GLRA  
**Consensus:** Independent SHA-256d Proof of Work  
**Tagline:** Family founded. Faith guided. Purpose driven.  
**Mining message:** Mine with Purpose.

## Project status

Galara is in the **design and pre-development stage**. No mainnet exists yet, no GLRA has been issued, and no coins are currently for sale.

The first objective is to create a transparent, independently operated Bitcoin-derived blockchain that can be mined directly with SHA-256 hardware, including Bitaxe devices.

## Core requirements

Galara must:

- operate without Bloodstone, STONE, ROD, AZURE, Bitcoin, or another parent chain;
- pay mining rewards directly in GLRA;
- support solo mining and Stratum pool mining;
- use open-source code and reproducible documentation;
- remain recoverable using repositories, backups, and documented procedures controlled by Galara Network;
- launch on mainnet only after regtest, private testnet, and public testnet validation.

## Preliminary chain settings

| Parameter | Working value |
|---|---|
| Algorithm | SHA-256d |
| Target block time | 180 seconds |
| Initial reward | 50 GLRA |
| Halving interval | 700,800 blocks |
| Approximate mineable supply | 70,080,000 GLRA |
| Proposed premine | 700,800 GLRA |
| Proposed maximum total supply | 70,780,800 GLRA |
| Difficulty adjustment | Per block or short rolling window |
| Mandatory merged mining | None |
| Decimal places | 8 |

These parameters remain subject to testnet validation and formal approval before genesis.

## Repository map

- `docs/GALARA_SPECIFICATION.md` — official working chain blueprint
- `docs/ROADMAP.md` — development stages and acceptance gates
- `docs/PREMINE_POLICY.md` — proposed transparent allocation
- `SECURITY.md` — vulnerability reporting and key-control expectations
- `CONTRIBUTING.md` — contribution rules
- `LICENSE-PLAN.md` — licensing requirements before source import
- `branding/BRAND_GUIDE.md` — working identity
- `.github/ISSUE_TEMPLATE/` — issue templates

## Important separation

AZURE may remain associated with Azure Guardians and the Bloodstone ecosystem. GLRA will be a separate native asset on the independent Galara blockchain.

Redeemed Medical and Redeemed Wellness & Fitness are separate legal and operational entities. No ownership of GLRA guarantees healthcare, prescriptions, discounts, or other services.

## Development order

1. Approve specification.
2. Establish project-controlled GitHub organization and accounts.
3. Select and document the licensed upstream codebase.
4. Build regtest.
5. Launch a three-node private testnet.
6. Add wallet, explorer, and Stratum mining.
7. Test Bitaxe compatibility and sudden hash-rate changes.
8. Launch public testnet.
9. Conduct security review and recovery drill.
10. Finalize genesis and launch mainnet.

## Warning

Do not download or trust unofficial Galara binaries. Official release signing, checksums, and download locations have not yet been established.
