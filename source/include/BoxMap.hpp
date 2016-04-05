#pragma once
#include "gtest/gtest.h"
#include "MapInterface.hpp"

namespace r2d2
{
    namespace map
    {
        class BoxMap : public SaveLoadMap, ReadWriteMap, ReadOnlyMap
        {

        public:

            BoxMap::BoxMap();
            
            BoxInfo & ReadOnlyMap::get_box_info(const Box & box) override;
            Box & ReadOnlyMap::get_map_bounding_box() override;
            std::shared_ptr<BoxInfo**> ReadOnlyMap::get_map_area_2d(const Box & area, const Box & pixel_size) override;

            void ReadWriteMap::set_box_info(const Box & box, const BoxInfo & box_info) override;

            void SaveLoadMap::save(const std::string filename) override;
            void SaveLoadMap::load(const std::string filename) override;


        private:
            std::vector<std::pair<Box, BoxInfo>> map;

        };
    }
}