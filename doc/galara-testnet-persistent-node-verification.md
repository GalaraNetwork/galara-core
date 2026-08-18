# Galara Testnet Persistent Public Node Verification

Galara Testnet Node A was converted from a manually started test node into
persistent public testnet infrastructure.

## Linux service

Node A runs under systemd using:

`galara-testnet-node-a.service`

The service is enabled for automatic startup and configured to restart on
failure.

Node A uses:

- Testnet P2P port: `39601`
- Testnet RPC port: `39602`
- P2P bind: `0.0.0.0:39601`
- RPC bind: `127.0.0.1`
- Testnet datadir: `.galara-testnet-node-a`

The service was verified active and preserved the existing Galara Testnet
chain through height 3.

## LAN reservation

CryptoIsland has a reserved LAN address:

- IP: `10.0.0.9`
- MAC: `EC:63:D7:CC:BD:07`

This prevents the router-side Galara port forward from depending on a changing
DHCP address.

## Windows to WSL forwarding

Windows forwards:

`10.0.0.9:39601`

to the current WSL Galara node address on TCP port `39601`.

A PowerShell repair script was created at:

`C:\Galara\Repair-GalaraTestnetPort.ps1`

The script discovers the current WSL IPv4 address and recreates only the
Galara Testnet `39601` portproxy entry.

## Automatic repair

Windows Task Scheduler runs:

`Galara Testnet WSL Port Repair`

The task:

- runs with elevated privileges
- runs at logon
- repeats every 5 minutes
- refreshes the Galara Testnet portproxy using the current WSL address

The task returned:

`LastTaskResult: 0`

and exposed a valid future `NextRunTime`.

## Recovery verification

The Galara Testnet `39601` portproxy entry was deliberately deleted.

The scheduled repair task was then run and successfully recreated:

`10.0.0.9:39601 -> 172.30.207.120:39601`

The unrelated TCP `3333` forwarding rule remained unchanged.

After repair, an independent Verizon-connected Galara system successfully
reached the public Galara Testnet endpoint again at:

`107.205.210.9:39601`

This verifies that Galara Testnet Node A has persistent service startup and
automatic Windows-to-WSL P2P forwarding recovery.
