#include "MapInterface.hpp"

namespace r2d2
{
    namespace map
    {
        BoxInfo::BoxInfo(const bool has_obstacle, const bool has_unknown, const bool has_navigatable) :
            has_obstacle{ has_obstacle },
            has_unknown{ has_unknown },
            has_navigatable{ has_navigatable }
        {}

        bool BoxInfo::operator==(const BoxInfo & rhs) const
        {
            return has_obstacle == rhs.has_obstacle && has_unknown == rhs.has_unknown && has_navigatable == rhs.has_navigatable;
        }

        bool BoxInfo::operator!=(const BoxInfo & rhs) const
        {
            return !operator==(rhs);
        }
    }
}