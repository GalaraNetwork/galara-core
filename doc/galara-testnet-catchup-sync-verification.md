# Galara Testnet Catch-Up Synchronization Verification

Galara Testnet catch-up synchronization was verified using two independent
local nodes.

## Starting state

Both nodes initially shared Galara Testnet block 1:

`00000000238c04456fdeea7790718cdf6dc5777261c3a39e9e0ab92e8c87f5f0`

Node B was then shut down cleanly.

## Node A advancement

While Node B was offline, Node A mined blocks 2 and 3.

Block 2:

`0000000041e0e8430d08598c692eb16834d6dad36851f6d8fcb0becfb62d94fd`

Block 3:

`0000000010e22d372b4b6553c1abf0bd7b4054e7f01eddd38a1b3082197866d1`

Node A reached:

- blocks: 3
- headers: 3
- bits: `1d00ffff`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000400040004`
- no warnings

This also crossed the Galara Testnet ASERT activation height.

## Catch-up synchronization

Node B was restarted and reconnected to Node A.

Within seconds, Node B synchronized from height 1 to height 3.

After synchronization both nodes reported the same:

- blocks: 3
- headers: 3
- best block:
  `0000000010e22d372b4b6553c1abf0bd7b4054e7f01eddd38a1b3082197866d1`
- bits: `1d00ffff`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000400040004`
- initialblockdownload: false
- no warnings

This verifies that a Galara Testnet node can fall behind, reconnect, obtain
historical headers and blocks from a peer, validate them, and catch up to the
current active chain.
