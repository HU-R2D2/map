#ifndef _BOXMAP_HPP
#define _BOXMAP_HPP

#include "../../../adt/source/include/Box.hpp"
#include "MapInterface.hpp"

namespace r2d2
{
    class BoxMap : public SaveLoadMap
    {

    public:

        BoxMap();

        BoxInfo get_box_info(Box & box) override;
        Box get_map_bounding_box() override;
        std::shared_ptr<BoxInfo*> get_map_area_2d(Box & area, Box & pixel_size) override;

        void set_box_info(Box & box, BoxInfo & box_info) override;

        void save(std::string filename) override;
        void load(std::string filename) override;


    private:
        std::vector<std::pair<Box, BoxInfo>> map;

    };
}
#endif //_BOXMAP_HPP