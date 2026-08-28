# Galara Mainnet Bitaxe Validation

## Validation date

27 August 2026

## Hardware

Bitaxe SHA-256 miner using BM1370 firmware.

## Network path

Bitaxe -> Galara Stratum V1 bridge -> Galara Core mainnet node.

## Validation results

The Bitaxe successfully:

- connected to the Galara Stratum server
- negotiated version rolling
- subscribed to mining work
- authorized as a worker
- received Galara mainnet mining jobs
- submitted SHA-256 shares
- produced network-target block candidates
- passed Galara Core proposal validation
- submitted valid blocks through submitblock
- advanced the Galara mainnet chain
- received replacement jobs after new chain tips

Galara Core accepted multiple Bitaxe-mined blocks.

The Stratum bridge detected new chain tips and generated fresh mining work for
subsequent heights.

## ASERT observation

Following an extended period with no mining activity, Galara difficulty had
declined as expected under ASERT.

When mining resumed, blocks were initially found rapidly.

Difficulty increased continuously as the rapid blocks were accepted, confirming
that Galara ASERT was responding in the intended direction.

The Galara project has elected to retain the configured two-day ASERT half-life.

## Result

PASS

A physical Bitaxe has successfully mined valid Galara mainnet blocks through
the Galara Stratum bridge.
