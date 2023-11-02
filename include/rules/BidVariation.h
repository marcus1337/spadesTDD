
#pragma once
#include <vector>
#include "data/State.h"

namespace spd
{
    class BidVariation
    {
    protected:
        std::vector<int> getAdjustedBidsBasedOnTeam(const Seat &seat, const State &state) const;
        std::vector<int> getBidEqualToSpades(const Seat &seat, const State &state) const;
        virtual std::optional<int> getStandardBidResult(const Seat &seat, const State &state) const;

    public:
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const = 0;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const = 0;
        virtual std::optional<int> getBidResult(const Seat &seat, const State &state) const = 0;
        virtual ~BidVariation() = default;
    };

    class DoubleBlindNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override;
        virtual std::optional<int> getBidResult(const Seat &seat, const State &state) const;
    };
    class DoubleNil : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override;
        virtual std::optional<int> getBidResult(const Seat &seat, const State &state) const;
    };

    class Mirror : public BidVariation
    {
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override;
        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override;
        virtual std::optional<int> getBidResult(const Seat &seat, const State &state) const;
    };

    class Suicide : public BidVariation
    {
        std::vector<int> getPossibleNonNilBids() const;

        std::vector<int> getPossibleFirstBids() const;

        std::vector<int> getPossibleSecondaryBids(const Seat &seat, const State &state) const;

        bool isFirstBidderInTeam(const Seat &seat, const State &state) const;

    public:
        virtual std::vector<int> getBids(const Seat &seat, const State &state) const override;

        virtual std::vector<BidOption> getBidOptions(const Seat &seat, const State &state) const override;

        virtual std::optional<int> getBidResult(const Seat &seat, const State &state) const override;
    };

}
