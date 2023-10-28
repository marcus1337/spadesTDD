
#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <string>
#include <optional>
#include "data/card/Card.h"
#include "data/Player.h"

namespace spd
{
    class TrumpVariation {
        protected:
        public:
        TrumpVariation() = default;
        virtual ~TrumpVariation() = default;
    };

    class AceHigh : public TrumpVariation {
        protected:
        public:
        AceHigh() = default;
    };
}
