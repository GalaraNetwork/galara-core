# Galara Mainnet Launch Verification

## Launch date

27 August 2026

## Release candidate

Galara mainnet launch was performed from:

    galara-mainnet-rc1

Release-candidate commit:

    32213d139985

## Mainnet consensus

Galara mainnet launched with the finalized consensus specification, including:

- SHA-256d proof of work
- 180-second target block spacing
- 50 GLRA initial ordinary block subsidy
- 700,800-block halving interval
- 700,800 GLRA one-time treasury premine at block 1
- 2-of-3 native SegWit treasury custody
- ASERT difficulty adjustment
- Two-day ASERT half-life
- ASERT activation at height 3

## Public mainnet node

CryptoIsland operates the first verified public Galara mainnet P2P node.

Public endpoint:

    107.205.210.9:29471

RPC is not exposed publicly.

External connectivity was verified from a separate physical machine and
Internet connection.

The external Galara node successfully:

- established TCP connectivity to the public endpoint
- completed a Galara protocol handshake
- negotiated protocol version 70016
- used P2P v2 transport
- synchronized headers
- synchronized blocks
- reached the same Galara mainnet chain tip
- reported no blockchain warnings

## Mainnet mining

A physical Bitaxe BM1370 SHA-256 miner was connected through the Galara
Stratum V1 bridge.

The Bitaxe successfully:

- subscribed to Galara mining jobs
- authorized as a worker
- received current mainnet work
- submitted proof-of-work candidates
- produced network-target block candidates
- passed Galara Core proposal validation
- submitted valid blocks through submitblock
- advanced the Galara mainnet chain
- received fresh jobs after new chain tips

Multiple mainnet blocks were accepted from the physical Bitaxe.

## Difficulty behavior

Following an earlier period with little or no mining activity, ASERT had
reduced Galara difficulty.

When continuous mining resumed, blocks were initially produced rapidly and
difficulty increased continuously in response.

The project elected to retain the configured two-day ASERT half-life.

## Infrastructure

The launch infrastructure includes:

- Galara mainnet peer node
- Galara launch/mining node
- Galara Stratum V1 bridge
- Public TCP P2P forwarding
- Windows firewall restriction for P2P
- WSL port-forward self-repair task
- Private RPC interfaces

The public P2P forwarding repair task automatically updates the Windows-to-WSL
mapping when the WSL IPv4 address changes.

## Launch result

PASS

Galara mainnet has demonstrated:

- working consensus validation
- functioning public P2P connectivity
- independent external synchronization
- working Stratum mining
- physical Bitaxe compatibility
- accepted SHA-256d mainnet blocks
- functioning ASERT difficulty response

Galara mainnet networking and mining are operational.
