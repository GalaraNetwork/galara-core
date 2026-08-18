# Galara Testnet WAN Synchronization Verification

Galara Testnet synchronization was successfully verified across the public
internet between two independent machines using different Internet service
providers.

## Source node

CryptoIsland hosted Galara Testnet Node A.

Network path:

- AT&T public IPv4 address: `107.205.210.9`
- Galara Testnet public P2P port: `39601`
- Windows LAN address: `10.0.0.9`
- WSL address: `172.30.207.120`
- Galara RPC remained private and was not exposed to the Internet

The router forwarded TCP port `39601` to CryptoIsland Windows, which forwarded
the connection into WSL and Galara Testnet Node A.

## Remote node

A separate mini PC connected through Verizon was used as Node E.

Node E:

- ran Galara Core inside Ubuntu WSL
- was built independently from the Galara repository
- used a fresh Galara Testnet datadir
- connected to `107.205.210.9:39601`

## Synchronization result

Node E successfully synchronized Galara Testnet through height 3.

Node E reported:

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

- remote peer: `107.205.210.9:39601`
- network: `ipv4`
- connection type: `manual`
- transport protocol: `v2`
- synced headers: 3
- synced blocks: 3
- observed ping approximately 85 to 112 milliseconds

This verifies that a freshly built Galara Testnet node on a separate Internet
connection can connect over public IPv4, obtain Galara headers and blocks from
an existing peer, independently validate them, and reach the same active chain
tip across different Internet service providers.
