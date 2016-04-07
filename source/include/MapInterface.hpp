#ifndef _MAPINTERFACE_HPP
#define _MAPINTERFACE_HPP

#include <vector>
#include <memory>

#include "Box.hpp"

namespace r2d2
{
    class BoxInfo
    {

    public:
        BoxInfo(
            bool has_obstacle = false,
            bool has_unknown = false, 
            bool has_navigatable = false
        );

        bool get_has_obstacle() const;
        bool get_has_unknown() const;
        bool get_has_navigatable() const;

        bool operator==(const BoxInfo rhs) const;
        bool operator!=(const BoxInfo rhs) const;

    private:
        bool has_obstacle;
        bool has_unknown;
        bool has_navigatable;

    };

    class ReadOnlyMap
    {

    public:
        virtual const BoxInfo get_box_info(const Box box) = 0;
        virtual const Box get_map_bounding_box() = 0;
    };

    class ReadWriteMap : public ReadOnlyMap
    {

    public:
        virtual void set_box_info(const Box box, const BoxInfo box_info) = 0;
    };

    class SaveLoadMap : public ReadWriteMap
    {

    public:
        virtual void save(std::string filename) = 0;
        virtual void load(std::string filename) = 0;
    };
}

#endif //_MAPINTERFACE_HPP