#ifndef _BOXMAP_HPP
#define _BOXMAP_HPP

#include "../../../adt/source/include/Box.hpp"
#include "MapInterface.hpp"

namespace r2d2
{
    class BoxMap : public SaveLoadMap
    {

    public:

        const BoxInfo get_box_info(const Box box) override;
        const Box get_map_bounding_box() override;

        void set_box_info(const Box box, const BoxInfo box_info) override;

        void save(std::string filename) override;
        void load(std::string filename) override;

        int get_map_size();


    private:
        std::vector<std::pair<Box, BoxInfo>> map;

    };
}
#endif //_BOXMAP_HPP