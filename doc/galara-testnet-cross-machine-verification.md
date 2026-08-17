# Galara Testnet Cross-Machine Synchronization Verification

Galara Testnet synchronization was successfully verified across two independent
physical computers on a local network.

## Source node

CryptoIsland hosted Galara Testnet Node A inside WSL.

Network path:

- CryptoIsland Windows LAN address: `10.0.0.12`
- CryptoIsland WSL address: `172.30.207.120`
- Galara Testnet P2P port: `39601`
- Galara RPC remained bound to localhost only

Windows forwarded TCP port `39601` from the LAN interface to the WSL Galara
Testnet node.

## Fresh remote node

A separate physical computer, Izal-Good, was used as Node D.

Node D:

- LAN address: `10.0.0.25`
- built Galara Core independently from the verified repository branch
- used a fresh Galara Testnet datadir
- connected only to `10.0.0.12:39601`

The TCP connection to the Galara P2P endpoint was verified before launch.

## Synchronization result

Node D successfully synchronized the Galara Testnet chain through height 3.

Node D reported:

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

Peer information confirmed:

- remote peer: `10.0.0.12:39601`
- connection type: `manual`
- transport protocol: `v2`
- synced headers: 3
- synced blocks: 3

This verifies that a freshly built Galara Testnet node on a separate physical
computer can connect over a real LAN, obtain Galara headers and blocks from an
existing peer, independently validate them, and reach the same active chain tip.
