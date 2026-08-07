// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util/check.h>

#include <cstdlib>

arith_uint256 CalculateASERT(
    const arith_uint256& ref_target,
    int64_t target_spacing,
    int64_t time_diff,
    int64_t height_diff,
    const arith_uint256& pow_limit,
    int64_t half_life)
{
    assert(ref_target > 0 && ref_target <= pow_limit);
    assert((pow_limit >> 224) == 0);
    assert(height_diff >= 0);
    assert(half_life > 0);

    // ASERT fixed-point exponent, with 16 fractional bits.
    assert(std::llabs(time_diff - target_spacing * height_diff) <
           (int64_t{1} << (63 - 16)));

    const int64_t exponent =
        ((time_diff - target_spacing * (height_diff + 1)) * 65536) /
        half_life;

    static_assert((int64_t{-1} >> 1) == int64_t{-1},
                  "ASERT requires arithmetic right shifts");

    int64_t shifts = exponent >> 16;
    const uint16_t frac = static_cast<uint16_t>(exponent);

    assert(exponent == shifts * 65536 + frac);

    // Cubic fixed-point approximation of 2^x for 0 <= x < 1.
    const uint32_t factor =
        65536 +
        ((195766423245049ULL * frac +
          971821376ULL * frac * frac +
          5127ULL * frac * frac * frac +
          (1ULL << 47)) >>
         48);

    arith_uint256 next_target = ref_target * factor;

    // factor contains 16 fractional bits.
    shifts -= 16;

    if (shifts <= 0) {
        next_target >>= -shifts;
    } else {
        const arith_uint256 shifted = next_target << shifts;

        // Detect discarded high bits from a 256-bit overflow.
        if ((shifted >> shifts) != next_target) {
            next_target = pow_limit;
        } else {
            next_target = shifted;
        }
    }

    if (next_target == 0) {
        next_target = arith_uint256{1};
    } else if (next_target > pow_limit) {
        next_target = pow_limit;
    }

    return next_target;
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    const int next_height = pindexLast->nHeight + 1;

    // Galara mainnet uses a dedicated launch target for block 1.
    // Genesis retains its historical PoW target, while live mining
    // begins from nInitialPowBits before ASERT activates at block 2.
    if (params.nInitialPowBits != 0 && next_height == 1) {
        return params.nInitialPowBits;
    }

    if (params.nASERTHalfLife > 0 &&
        next_height >= params.nASERTActivationHeight) {

        const CBlockIndex* anchor = pindexLast->GetAncestor(
            params.nASERTActivationHeight - 1);
        assert(anchor != nullptr);

        arith_uint256 anchor_target;
        bool negative{false};
        bool overflow{false};
        anchor_target.SetCompact(anchor->nBits, &negative, &overflow);

        assert(!negative);
        assert(!overflow);
        assert(anchor_target > 0);
        assert(anchor_target <= UintToArith256(params.powLimit));

        const int64_t height_diff =
            pindexLast->nHeight - anchor->nHeight;

    // ASERT measures time from the parent of the anchor block.
    // When the anchor is genesis, use the genesis timestamp itself,
    // matching the canonical ASERT genesis special case.
    const int64_t time_diff =
        pindexLast->GetBlockTime() -
        (anchor->pprev
            ? anchor->pprev->GetBlockTime()
            : anchor->GetBlockTime());

        return CalculateASERT(
            anchor_target,
            params.nPowTargetSpacing,
            time_diff,
            height_diff,
            UintToArith256(params.powLimit),
            params.nASERTHalfLife).GetCompact();
    }

    // Legacy periodic difficulty adjustment.
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2 target spacings
            // then it MUST be a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back one difficulty adjustment interval
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;

    // Special difficulty rule for Testnet4
    if (params.enforce_BIP94) {
        // Here we use the first block of the difficulty period. This way
        // the real difficulty is always preserved in the first block as
        // it is not allowed to use the min-difficulty exception.
        int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
        const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
        bnNew.SetCompact(pindexFirst->nBits);
    } else {
        bnNew.SetCompact(pindexLast->nBits);
    }

    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// Check that on difficulty adjustments, the new difficulty does not increase
// or decrease beyond the permitted limits.
bool PermittedDifficultyTransition(const Consensus::Params& params, int64_t height, uint32_t old_nbits, uint32_t new_nbits)
{
    if (params.fPowAllowMinDifficultyBlocks) return true;

    if (params.nASERTHalfLife > 0 &&
        height >= params.nASERTActivationHeight) {
        return DeriveTarget(new_nbits, params.powLimit).has_value();
    }

    if (height % params.DifficultyAdjustmentInterval() == 0) {
        int64_t smallest_timespan = params.nPowTargetTimespan/4;
        int64_t largest_timespan = params.nPowTargetTimespan*4;

        const arith_uint256 pow_limit = UintToArith256(params.powLimit);
        arith_uint256 observed_new_target;
        observed_new_target.SetCompact(new_nbits);

        // Calculate the largest difficulty value possible:
        arith_uint256 largest_difficulty_target;
        largest_difficulty_target.SetCompact(old_nbits);
        largest_difficulty_target *= largest_timespan;
        largest_difficulty_target /= params.nPowTargetTimespan;

        if (largest_difficulty_target > pow_limit) {
            largest_difficulty_target = pow_limit;
        }

        // Round and then compare this new calculated value to what is
        // observed.
        arith_uint256 maximum_new_target;
        maximum_new_target.SetCompact(largest_difficulty_target.GetCompact());
        if (maximum_new_target < observed_new_target) return false;

        // Calculate the smallest difficulty value possible:
        arith_uint256 smallest_difficulty_target;
        smallest_difficulty_target.SetCompact(old_nbits);
        smallest_difficulty_target *= smallest_timespan;
        smallest_difficulty_target /= params.nPowTargetTimespan;

        if (smallest_difficulty_target > pow_limit) {
            smallest_difficulty_target = pow_limit;
        }

        // Round and then compare this new calculated value to what is
        // observed.
        arith_uint256 minimum_new_target;
        minimum_new_target.SetCompact(smallest_difficulty_target.GetCompact());
        if (minimum_new_target > observed_new_target) return false;
    } else if (old_nbits != new_nbits) {
        return false;
    }
    return true;
}

// Bypasses the actual proof of work check during fuzz testing with a simplified validation checking whether
// the most significant bit of the last byte of the hash is set.
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    if (EnableFuzzDeterminism()) return (hash.data()[31] & 0x80) == 0;
    return CheckProofOfWorkImpl(hash, nBits, params);
}

std::optional<arith_uint256> DeriveTarget(unsigned int nBits, const uint256 pow_limit)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(pow_limit))
        return {};

    return bnTarget;
}

bool CheckProofOfWorkImpl(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    auto bnTarget{DeriveTarget(nBits, params.powLimit)};
    if (!bnTarget) return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
