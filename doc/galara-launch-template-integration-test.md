# Galara Block 1 Mining Template Integration Test

**Date:** 2026-08-07
**Status:** PASSED

## Purpose

Verify the Galara block 1 mining interface using the actual `galarad`
binary on an isolated Galara mainnet instance.

The test used two local Galara nodes connected only to each other.

## Block 1 Template

A miner advertising support for:

`segwit`

and:

`galara-premine`

received a valid block template for height 1.

Verified values:

- Height: `1`
- Previous block: Galara mainnet genesis
- Difficulty bits: `1b014f8a`
- Total coinbase value: `70085000000000` satoshis
- Total coinbase value: `700850 GLRA`
- Normal block subsidy: `50 GLRA`
- Required treasury premine: `700800 GLRA`

The returned rules included:

`!galara-premine`

## Required Treasury Output

The template exposed the required Galara coinbase output:

Value:

`70080000000000`

scriptPubKey:

`00200f7f09c48be501a04d15a6553e7dfb4f9dec46972da5a1ea01e0fa41b7732c25`

This matches the Galara Network Treasury destination defined by consensus.

## Miner Compatibility Protection

A second `getblocktemplate` request advertised only:

`segwit`

and did not advertise `galara-premine`.

Galara Core rejected the request with:

`Galara block 1 requires the galara-premine rule`

This confirms mining software that is unaware of the mandatory Galara
block 1 treasury output will not receive a launch block template.

## Result

**Galara block 1 mining template integration verification: PASSED**
