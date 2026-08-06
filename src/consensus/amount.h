// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_AMOUNT_H
#define BITCOIN_CONSENSUS_AMOUNT_H

#include <cstdint>

/** Amount in Galara base units (can be negative). */
typedef int64_t CAmount;

/** The number of base units in one whole coin. */
static constexpr CAmount COIN = 100000000;

/** No amount larger than this number of base units is valid.
 *
 * This constant is not Galara's issued money supply. It is a consensus-critical
 * sanity ceiling set above the planned maximum supply. As this sanity check
 * is used by consensus-critical
 * validation code, the exact value of the MAX_MONEY constant is consensus
 * critical; in unusual circumstances like a(nother) overflow bug that allowed
 * for the creation of coins out of thin air modification could lead to a fork.
 * */
static constexpr CAmount MAX_MONEY = 100000000 * COIN;
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

#endif // BITCOIN_CONSENSUS_AMOUNT_H
