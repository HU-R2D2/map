#ifndef _MAPINTERFACE_HPP
#define _MAPINTERFACE_HPP

#include <vector>
#include <memory>

#include "../../../adt/source/include/Box.hpp"

/*
class Length{
};

class Coordinate{
public:
    Coordinate(Length, Length, Length);

    Length get_x();
    Length get_y();
    Length get_z();
};

class Box{
public:
    Box();
    bool Box::intersects(Box box);
    bool contains(Box & box);
    Box get_union_box(Box & box);

    Box get_intersection_box(Box & box);

    Coordinate get_bottom_left();
};
*/
namespace r2d2
{
    class BoxInfo
    {

    public:
        BoxInfo(bool has_obstacle = false, bool has_unknown = false, bool has_navigatable = false);

        bool get_has_obstacle();
        bool get_has_unknown();
        bool get_has_navigatable();

        bool operator==(BoxInfo & rhs);
        bool operator!=(BoxInfo & rhs);

    private:
        bool has_obstacle;
        bool has_unknown;
        bool has_navigatable;

    };

    class ReadOnlyMap
    {

    public:
        virtual BoxInfo get_box_info(Box & box) = 0;
        virtual Box get_map_bounding_box() = 0;
        virtual std::shared_ptr<BoxInfo*> get_map_area_2d(Box & area, Box & pixel_size) = 0;
    };

    class ReadWriteMap : public ReadOnlyMap
    {

    public:
        virtual void set_box_info(Box & box, BoxInfo & box_info) = 0;
    };

    class SaveLoadMap : public ReadWriteMap
    {

    public:
        virtual void save(std::string filename) = 0;
        virtual void load(std::string filename) = 0;
    };
}

#endif
