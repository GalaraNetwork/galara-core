# Galara systemd examples

These files document the service layout used during Galara launch and
Stratum integration testing.

They are examples, not directly installable production units.

Before using them, replace:

- `GALARA_USER` with the Linux service account.
- `GALARA_HOME` with that account's home directory.
- `GALARA_REPO` with the Galara Core source/build directory.

## Startup order

The current isolated stack starts in this order:

1. `galara-peer.service`
2. `galara-launch.service`
3. `galara-stratum.service`

The launch node requires the local peer. The Stratum bridge requires the
launch node.

The Stratum unit also performs an RPC readiness check before starting the
bridge. This prevents the bridge from starting before the Galara RPC cookie
and RPC server are available.

## Development-only network isolation

The example peer and launch units reproduce the isolated development
environment used during launch testing. They are not a final Galara public
network topology.

The launch node currently uses:

- `-listen=0`
- `-discover=0`
- `-dnsseed=0`
- `-connect=127.0.0.1:29601`
- `-maxtipage=604800`
- `-fallbackfee=0.0001`

These settings should be reviewed before a production/public-network
deployment.

`-maxtipage=604800` was used to tolerate an old chain tip during controlled
launch testing.

`-fallbackfee=0.0001` is an operational wallet setting and is not a Galara
consensus rule.

## Stratum block submission

`GALARA_STRATUM_SUBMIT_BLOCKS=1` enables actual network block submission.

Operators who only want to test Stratum behavior should leave block
submission disabled.
