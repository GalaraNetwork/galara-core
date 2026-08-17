# Galara Testnet Two-Node Verification

Galara Testnet was verified using two independent fresh node datadirs
running locally on CryptoIsland.

## Nodes

Node A:
- P2P port: 39601
- RPC port: 39602

Node B:
- P2P port: 39611
- RPC port: 39612
- Connected manually to Node A at 127.0.0.1:39601

Both nodes started from the final Galara Testnet genesis:

`000000007c1ad2c2d90adda5fac8d523a97112d34332a953febba17fd4c4f5ff`

The nodes successfully negotiated a P2P v2 connection.

## Block relay verification

Node A mined testnet block 1:

`00000000238c04456fdeea7790718cdf6dc5777261c3a39e9e0ab92e8c87f5f0`

Node B received and independently validated the block through the Galara
peer-to-peer network.

After synchronization, both nodes reported:

- blocks: 1
- headers: 1
- best block:
  `00000000238c04456fdeea7790718cdf6dc5777261c3a39e9e0ab92e8c87f5f0`
- bits: `1d00ffff`
- difficulty: `1`
- chainwork:
  `0000000000000000000000000000000000000000000000000000000200020002`
- initialblockdownload: false
- no warnings

This verifies that Galara Testnet nodes can establish a native Galara P2P
connection and successfully relay, validate, and activate newly mined Galara
blocks.
