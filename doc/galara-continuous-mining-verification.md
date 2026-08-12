# Galara Continuous Mining Verification

## Status

Verified on the Galara mainnet development chain using:

- Galara Core
- Galara Stratum V1 bridge
- Bitaxe SHA-256 miner
- Two independent Galara Core node datadirs
- Dedicated Galara miner wallet

This verification demonstrates the complete mining lifecycle from Stratum job
generation through automatic block submission, peer propagation, persistence,
restart, and resumed mining.

## Verified mining lifecycle

The Galara Stratum bridge successfully:

1. Obtained a valid `getblocktemplate` from Galara Core.
2. Issued Stratum V1 mining jobs to a Bitaxe miner.
3. Assigned a unique extranonce to the miner connection.
4. Accepted and verified submitted shares.
5. Detected a share meeting the Galara network target.
6. Constructed the complete candidate block.
7. Validated the candidate through `getblocktemplate` proposal mode.
8. Automatically submitted the valid block to Galara Core.
9. Detected the new Galara chain tip.
10. Automatically issued the next-height mining job.

## Block 4 automatic submission

The Bitaxe found Galara mainnet block 4:

`000000000000ef490a55387fe8331088c297712ee6a2918c6052d1c6c218ef2a`

The Stratum bridge reported:

- Network target satisfied
- Proposal validation result: `None`
- `submitblock` result: `None`
- Successful automatic block submission
- Automatic transition to a height-5 mining job

Block 4 was independently inspected through Galara Core.

Verified properties included:

- Height: 4
- Miner reward: 50 GLRA
- Miner payout destination: dedicated Galara miner wallet
- No repeated treasury premine payment
- Valid SegWit coinbase witness
- Valid witness commitment output
- Valid SHA-256d proof of work

## Multi-block operation

Continuous Bitaxe mining advanced the chain through height 10 without manual
block submission.

At the height-10 checkpoint:

- Launch node height: 10
- Peer node height: 10
- Both nodes had the same active chain tip
- Miner wallet immature balance: 500 GLRA

Height-10 chain tip:

`0000000000008c4a45f446cae4f29f3086767ffea54bd4397e83a9c69578f90b`

## Clean shutdown and persistence test

The miner and Stratum bridge were stopped and both Galara Core test nodes were
shut down cleanly through RPC.

After shutdown, no test `galarad` process remained running.

The peer node and launch node were then restarted from their existing datadirs.

Both nodes independently restored:

- Height: 10
- Best block:
  `0000000000008c4a45f446cae4f29f3086767ffea54bd4397e83a9c69578f90b`

The launch node successfully reconnected to the peer.

No reindex, block repair, or manual chain recovery was required.

## Mining resume after restart

After the clean restart, the Stratum bridge immediately received a valid
height-11 Galara block template.

The Bitaxe reconnected and:

- Completed Stratum configuration
- Subscribed successfully
- Authorized successfully
- Received a new unique extranonce
- Received a height-11 mining job
- Immediately resumed valid share submissions

Example resumed job:

`galara-11-1786509142`

This verified that Galara mining can resume from the correct next chain height
after a clean node, bridge, and miner restart.

## Transaction and SegWit support

Before the continuous-mining test, the bridge was separately verified using an
isolated regtest environment containing a real wallet-created SegWit
transaction.

The generated non-empty block candidate included:

- A real mempool transaction
- Correct transaction merkle branch
- Correct non-witness block merkle root
- Correct SegWit witness commitment
- Correct coinbase witness reserved value

Galara Core accepted the candidate through proposal validation with:

`proposal result: None`

## Conclusion

The Galara mining stack has demonstrated the complete core mining lifecycle:

`Bitaxe -> Stratum -> Galara Core -> block validation -> automatic submission -> peer propagation -> next job`

It has also demonstrated:

`clean shutdown -> persistent chain state -> node restart -> peer reconnect -> Stratum restart -> miner reconnect -> correct next-height mining`

The core continuous-mining path is therefore verified for the Galara
development mainnet.

Further work should focus on production operations and hardening rather than
basic mining functionality, including service management, monitoring, restart
automation, stale-job cleanup, and longer-duration operational testing.
