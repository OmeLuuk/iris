#include "geometry.h"

#include <vector>

// Definition of the static member
std::vector<float> LookupTable::acos_table(LookupTable::ACOS_TABLE_PRECISION + 1);

void LookupTable::initialize()
{
    for (int i = 0; i <= ACOS_TABLE_PRECISION; ++i)
    {
        float dot_product = static_cast<float>(i) / ACOS_TABLE_PRECISION;
        // intensity formula f(x) = 1 - acos(dot/length)/(0.5Pi)
        acos_table[i] = std::clamp(1 - 0.63662 * std::acos(dot_product), 0.0, 1.0);
    }
}
