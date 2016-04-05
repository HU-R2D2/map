#pragma once
#include "gtest/gtest.h"
#include <vector>
#include <memory>

#include "../adt/source/include/Box.hpp"

namespace r2d2
{
	namespace map
   {
       class BoxInfo
       {

       public:
          BoxInfo(const bool has_obstacle = false, const bool has_unknown = false, const bool has_navigatable = false);

          const bool get_has_obstacle();
          const bool get_has_unknown();
          const bool get_has_navigatable();

          bool operator== (const BoxInfo & rhs) const;
          bool operator!= (const BoxInfo & rhs) const;

       private:
          const bool has_obstacle;
          const bool has_unknown;
          const bool has_navigatable;

       };

       class ReadOnlyMap
       {

       public:
           virtual BoxInfo & get_box_info(const Box & box) = 0;
           virtual Box & get_map_bounding_box() = 0;
           virtual std::shared_ptr<BoxInfo**> get_map_area_2d(const Box & area, const Box & pixel_size) = 0;
       };

       class ReadWriteMap : public ReadOnlyMap
       {

       public:
           virtual void set_box_info(const Box & box, const BoxInfo & box_info) = 0;
       };

       class SaveLoadMap : public ReadWriteMap
       {

       public:
           virtual void save(const std::string filename) = 0;
           virtual void load(const std::string filename) = 0;
       };
   }
}