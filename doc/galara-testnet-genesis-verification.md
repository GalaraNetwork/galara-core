# Galara Testnet Genesis Verification

Galara Testnet was verified using a fresh isolated datadir and a newly built
Galara Core daemon.

## Testnet genesis

- Chain: test
- Height: 0
- Genesis hash:
  `0000015a20de47f9991c005549e1224cda9f599e7d708319a8924a5103ae0cf2`
- Merkle root:
  `3fc33817a8b9e73c08a62a4f4f7911e7dce504c22e7890fdc4b419103ed9c4ae`
- Time: `1786918620`
- Nonce: `1467599`
- Bits: `0x1e0b8ef1`
- Target:
  `00000b8ef1000000000000000000000000000000000000000000000000000000`
- Version: `1`
- Genesis reward: `50 GLRA`
- Genesis output: `OP_RETURN` (intentionally unspendable)
- Coinbase timestamp:
  `16/Aug/2026 Galara Testnet - Independent testing begins`

## Verification

The following checks were completed successfully:

1. Independent Python SHA-256d genesis calculation.
2. `pow_tests`: 18 test cases, no errors.
3. `key_io_tests`: 3 test cases, no errors.
4. Fresh Galara testnet datadir startup.
5. `getblockhash 0` returned the expected Galara testnet genesis hash.
6. `getblockchaininfo` reported:
   - chain `test`
   - blocks `0`
   - headers `0`
   - expected genesis as best block
   - bits `1e0b8ef1`
   - `initialblockdownload: false`
   - no warnings
7. `getblock` confirmed the expected hash, merkle root, nonce, bits,
   coinbase transaction, and genesis timestamp.
8. The isolated smoke-test daemon was stopped cleanly and RPC became
   unavailable after shutdown.

This verifies that Galara Testnet starts from its own native genesis block
rather than the inherited Bitcoin Testnet3 genesis.
