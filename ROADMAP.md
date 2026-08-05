# Galara Development Roadmap

## Phase 0 — Governance and ownership

- Create Galara Network GitHub organization.
- Enable two-factor authentication.
- Store recovery codes offline.
- Create `galara-core` repository.
- Approve specification and contribution rules.
- Reserve domain and major account names.
- Establish contractor access policy.

**Exit gate:** Mark controls every core account and at least one secure backup exists.

## Phase 1 — Upstream selection

- Compare maintained Bitcoin-derived codebases.
- Confirm license compatibility.
- Record exact upstream commit.
- Produce a change inventory.
- Decide supported build platforms.
- Select a tested difficulty-adjustment design.

**Exit gate:** Written architecture decision approved before code import.

## Phase 2 — Regtest prototype

- Rename network and binaries.
- Create isolated data directories.
- Implement preliminary subsidy schedule.
- Implement difficulty rules.
- Configure regtest identifiers and ports.
- Build Linux daemon and command-line wallet.
- Add automated consensus tests.

**Exit gate:** Blocks, transactions, wallet restore, and subsidy tests pass locally.

## Phase 3 — Private testnet

- Generate testnet genesis.
- Operate three nodes on separate systems.
- Add peer discovery.
- Deploy explorer.
- Deploy Stratum server.
- Connect Bitaxe.
- Test solo and pool mining.
- Simulate large hash-rate arrival and departure.

**Exit gate:** Network survives node loss and difficulty stress tests.

## Phase 4 — Public testnet

- Publish signed binaries and checksums.
- Release mining and node guides.
- Open faucet.
- Recruit independent node operators and miners.
- Track bugs publicly.
- Conduct backup and disaster-recovery drill.
- Obtain external code review.

**Exit gate:** Stable public testnet and documented remediation of critical findings.

## Phase 5 — Mainnet preparation

- Freeze consensus parameters.
- Verify supply calculations.
- Finalize premine policy and addresses.
- Generate mainnet genesis transparently.
- Configure seed nodes and checkpoints.
- Prepare public launch documentation.
- Prepare incident-response contacts.

**Exit gate:** Signed launch checklist approved.

## Phase 6 — Mainnet

- Publish source and signed releases.
- Start seed nodes and explorer.
- Open mining pool.
- Publish premine addresses.
- Monitor chain health.
- Avoid exchanges or bridges until basic stability is demonstrated.

## Phase 7 — Ecosystem

- Galara Forge
- Galara Gives
- optional merchant tools
- optional game integrations
- possible wrapped token only after independent security review
