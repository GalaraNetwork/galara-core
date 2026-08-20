# Galara Testnet Dual-Seed Failover Verification

Galara Testnet fixed-seed redundancy was successfully verified using two
independent public bootstrap nodes on different Internet service providers.

## Public fixed seeds

Seed #1:

`107.205.210.9:39601`

Seed #2:

`97.241.193.100:39641`

Both addresses are included in the Galara Testnet built-in fixed-seed list.

## Failover test

Seed #1 was intentionally taken offline by stopping only the Galara Testnet
Node A service.

A separate physical machine, Izal-Good, then started a completely fresh
Galara Testnet Node H using a new datadir.

Node H was started without:

- `-connect`
- `-addnode`
- `-seednode`

DNS seeds were disabled.

## Seed #1 state

While Seed #1 was offline, Node H attempted to reach:

`107.205.210.9:39601`

but the connection did not complete.

Peer information showed:

- version: 0
- bytes received: 0
- synced headers: -1
- synced blocks: -1

## Seed #2 failover

Node H independently connected to:

`97.241.193.100:39641`

The connection reported:

- network: `ipv4`
- version: `70016`
- services: `0000000000000c09`
- connection type: `outbound-full-relay`
- transport protocol: `v2`
- synced headers: 3
- synced blocks: 3

## Chain result

Despite Seed #1 being unavailable, Node H successfully reached:

- chain: `test`
- blocks: 3
- headers: 3
- best block:
  `00000000f5c171ffe62231e06a28e354d1ef6b4e8caca87364fbb9e5530430f3`
- bits: `1d00ffff`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000400040004`
- no warnings

After verification, Seed #1 was restarted successfully and returned to the
same Galara Testnet chain tip.

This verifies that Galara Testnet can bootstrap through Seed #2 when Seed #1
is unavailable, providing functional fixed-seed redundancy across independent
public nodes and separate Internet service providers.
