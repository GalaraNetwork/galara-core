# Galara Testnet Fresh Bootstrap Verification

Galara Testnet fresh-node bootstrap synchronization was verified using a third
independent node with a new datadir.

## Existing network state

Node A and Node B were already synchronized at height 3.

Current testnet tip:

`0000000010e22d372b4b6553c1abf0bd7b4054e7f01eddd38a1b3082197866d1`

Chainwork:

`0000000000000000000000000000000000000000000000000000000400040004`

## Fresh Node C

Node C was started with a fresh datadir and connected only to Node A.

Node C successfully synchronized the Galara Testnet chain from the native
Galara genesis through height 3.

After synchronization Node C reported:

- chain: `test`
- blocks: 3
- headers: 3
- best block:
  `0000000010e22d372b4b6553c1abf0bd7b4054e7f01eddd38a1b3082197866d1`
- bits: `1d00ffff`
- difficulty: `1`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000400040004`
- initialblockdownload: false
- no warnings

Nodes A, B, and C all reported the same best block hash.

This verifies that a brand-new Galara Testnet node can bootstrap from an
existing Galara peer, download and validate the available chain history, and
arrive at the same active chain tip.
