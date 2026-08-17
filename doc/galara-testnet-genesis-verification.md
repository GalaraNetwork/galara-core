# Galara Testnet Genesis Verification

Galara Testnet was verified using a fresh isolated datadir and a newly built
Galara Core daemon.

## Testnet genesis

- Chain: test
- Height: 0
- Genesis hash:
  `000000007c1ad2c2d90adda5fac8d523a97112d34332a953febba17fd4c4f5ff`
- Merkle root:
  `3fc33817a8b9e73c08a62a4f4f7911e7dce504c22e7890fdc4b419103ed9c4ae`
- Time: `1786918621`
- Nonce: `77312931`
- Bits: `0x1d00ffff`
- Target:
  `00000000ffff0000000000000000000000000000000000000000000000000000`
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
   - bits `1d00ffff`
   - `initialblockdownload: false`
   - no warnings
7. `getblock` confirmed the expected hash, merkle root, nonce, bits,
   coinbase transaction, and genesis timestamp.
8. The isolated smoke-test daemon was stopped cleanly and RPC became
   unavailable after shutdown.

This verifies that Galara Testnet starts from its own native genesis block
rather than the inherited Bitcoin Testnet3 genesis.

## ASERT launch handoff verification

The final ASERT-compatible testnet configuration was verified using a fresh
datadir.

- Height 0 used bits `1d00ffff`.
- Height 1 mined successfully with bits `1d00ffff`.
- Height 2 mined successfully with bits `1d00ffff`.
- Height 3 block-template creation entered the ASERT path successfully and
  returned a normal template without assertion failure or daemon abort.
- A regression check was added to `pow_tests` requiring ASERT-enabled networks
  to keep the highest 32 bits of `powLimit` zero, matching the invariant
  enforced by `CalculateASERT()`.

Verified launch blocks:

- Height 1:
  `000000008d5f341b2b6f9aebb186687ff89e993970a4104dadc30b41674069a6`
- Height 2:
  `000000003d00b2ff419b0e1f3965e49b3c312e53c2ac12edd2b27e305799cd7e`

The height 3 template returned:

- height: `3`
- bits: `1d00ffff`
- target:
  `00000000ffff0000000000000000000000000000000000000000000000000000`

This confirms the Galara Testnet launch handoff from the fixed launch target
through ASERT activation is operational.

