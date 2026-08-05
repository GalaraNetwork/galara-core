# Galara Upstream Source Record

## Upstream project

- Project: Bitcoin Core
- Version: v31.1
- Exact commit: 9be056a8a72b624dae9623b2f7bded92c2a21c91
- License: MIT
- Upstream repository: https://github.com/bitcoin/bitcoin

## Verified environment

- WSL Ubuntu
- GCC 15.2.0
- CMake 4.2.3
- Ninja 1.13.2
- Python 3.14.4
- IPC disabled for the initial build

## Verification completed

- bitcoind compiled successfully
- bitcoin-cli compiled successfully
- wallet support enabled
- 721 unit tests passed
- regtest node started successfully
- wallet created successfully
- 101 regtest blocks generated
- mature balance confirmed

## Galara development policy

Galara consensus changes must be isolated, documented, tested, and committed separately from cosmetic branding changes.

The original Bitcoin Core source will remain available through the `upstream` Git remote.

Existing Bitcoin Core copyright and license notices must remain intact.
