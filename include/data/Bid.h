#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "data/Card.h"
#include <string>
#include <optional>

namespace spd
{
    struct Bid{
        int tricks;
        bool blind;
    };

}