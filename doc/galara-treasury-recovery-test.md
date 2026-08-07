# Galara Treasury Recovery Verification

**Date:** 2026-08-07  
**Status:** PASSED

## Treasury Design

The Galara Network Treasury uses a 2-of-3 native SegWit multisignature wallet.

Treasury address:

`glra1qpalsn3ytu5q6qng45e2nul0mf7w7c35h9kj6r6spurayrdmn9sjs782m47`

Treasury scriptPubKey:

`00200f7f09c48be501a04d15a6553e7dfb4f9dec46972da5a1ea01e0fa41b7732c25`

Public keys:

1. `023244bc1009dee74eb204cab0da3f38f2f77c43f704529d4ba0010167ae038bd3`
2. `03c30d959828247fe6be9c1ef19c106b9646ab4033703c6032e34391170e1797c2`
3. `026ebfb6f9516b19a43d513ed93d720319c52729f2530ed98e0aa6e9186104840b`

Master fingerprints:

1. `8e6f6932`
2. `bb3e3e0c`
3. `1ab9c9e8`

Key derivation path used for the treasury keys:

`m/84h/0h/0h/0/0`

## Backup Recovery Test

All three encrypted wallet backups were restored successfully on a fresh,
isolated Galara mainnet node.

Each recovered wallet was verified to control its expected treasury public key.

## 2-of-3 Signing Test

A non-broadcastable PSBT was constructed using a deliberately nonexistent
transaction input.

Recovered Treasury Key 1 produced a valid partial signature.

Recovered Treasury Key 2 added the second signature.

Galara Core finalized the transaction and returned:

`complete: true`

This confirms that two independently recovered treasury wallets can satisfy
the Galara 2-of-3 treasury script.

The test transaction cannot be broadcast because its input does not exist.

## Physical Backup Verification

Three encrypted wallet backups were copied to separate USB devices.

Each USB copy was compared to its source using SHA-256 and verified
byte-for-byte identical.

No wallet passphrases, private keys, seed phrases, private descriptors,
or wallet backup files are stored in this repository.

## Result

**Galara treasury backup recovery and 2-of-3 signing verification: PASSED**
