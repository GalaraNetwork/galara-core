# Galara Testnet Quickstart

This guide explains how to build Galara Core and join the public Galara Testnet.

Galara Testnet uses built-in fixed seeds, so a normal testnet node does not
need a manually supplied peer address.

## 1. Install build dependencies

On Ubuntu or Ubuntu under WSL:

    sudo apt update
    sudo apt install -y build-essential cmake ninja-build pkg-config python3 git libevent-dev libboost-dev libsqlite3-dev

## 2. Clone Galara Core

    mkdir -p ~/galara-development
    cd ~/galara-development
    git clone https://github.com/GalaraNetwork/galara-core.git bitcoin
    cd bitcoin
    git checkout galara/stratum-integration
    git pull

Confirm the current branch:

    git status
    git log -1 --oneline

## 3. Configure Galara Core

    cmake -B build -GNinja -DBUILD_GUI=OFF -DBUILD_TESTS=ON -DENABLE_IPC=OFF

## 4. Build Galara Core

A conservative build is recommended:

    ninja -C build -j1

Verify the main binaries:

    ls -lh build/bin/galarad build/bin/galara-cli

## 5. Start Galara Testnet

Create a testnet datadir:

    mkdir -p "$HOME/.galara-testnet"

Start Galara Testnet:

    build/bin/galarad -datadir="$HOME/.galara-testnet" -chain=test -daemon -discover=0 -dnsseed=0 -server=1 -dbcache=64

No -connect, -addnode, or -seednode option is required.

Galara Core contains built-in Galara Testnet fixed seeds and will use them to
find the public test network.

## 6. Check synchronization

Wait about 60 to 90 seconds, then run:

    build/bin/galara-cli -datadir="$HOME/.galara-testnet" -chain=test getblockchaininfo

Look for:

- chain: test
- a valid block height
- a valid best block hash
- no warnings

## 7. Check peers

    build/bin/galara-cli -datadir="$HOME/.galara-testnet" -chain=test getpeerinfo

A healthy peer should normally report version 70016, outbound-full-relay, and
P2P v2 transport.

## 8. Stop the node

Always stop Galara Core cleanly:

    build/bin/galara-cli -datadir="$HOME/.galara-testnet" -chain=test stop

## Public Galara Testnet bootstrap

Galara Testnet currently has independent public bootstrap infrastructure on
separate Internet connections.

The built-in fixed-seed system has been tested with one seed intentionally
offline. A fresh node successfully discovered the remaining seed and
synchronized the Galara Testnet chain.

Users normally do not need to configure bootstrap addresses manually.

## Important security notes

Galara RPC should not be exposed directly to the public Internet.

If you choose to make your node publicly reachable, expose only the Galara
Testnet P2P port needed for peer connections and keep RPC bound to localhost
or otherwise securely restricted.

Never share wallet private keys, wallet passphrases, recovery information, or
RPC authentication credentials.

## Testnet warning

Galara Testnet coins have no intended monetary value.

Testnet may be reset, upgraded, reorganized, or changed as Galara development
continues.

Do not treat Testnet funds, addresses, blocks, or balances as permanent.
