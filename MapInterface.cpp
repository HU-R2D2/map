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

        bool BoxInfo::operator==(const BoxInfo & lhs) const
        {
            return has_obstacle == lhs.has_obstacle && has_unknown == lhs.has_unknown && has_navigatable == lhs.has_navigatable;
        }

        bool BoxInfo::operator!=(const BoxInfo & lhs) const
        {
            return !operator==(lhs);
        }
    }
}