# Galara Testnet Fixed Seed Bootstrap Verification

Galara Testnet fixed-seed bootstrap was successfully verified using a fresh
node on a separate Internet connection.

## Bootstrap configuration

Galara Testnet contains a built-in fixed seed for:

`107.205.210.9:39601`

DNS seeds remained disabled.

The fresh test node was started without:

- `-connect`
- `-addnode`
- `-seednode`

## Fresh Node F

Node F ran on the Verizon-connected mini PC using a fresh Galara Testnet
datadir.

After startup, Node F automatically discovered and connected to:

`107.205.210.9:39601`

Peer information reported:

- network: `ipv4`
- connection type: `outbound-full-relay`
- transport protocol: `v2`
- synced headers: 3
- synced blocks: 3

## Synchronization result

Node F successfully reached:

- chain: `test`
- blocks: 3
- headers: 3
- best block:
  `00000000f5c171ffe62231e06a28e354d1ef6b4e8caca87364fbb9e5530430f3`
- bits: `1d00ffff`
- difficulty: `1`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000400040004`
- initialblockdownload: false
- no warnings

This verifies that a fresh Galara Testnet node can discover the public testnet
through Galara's built-in fixed seed and synchronize without manual peer
configuration.
