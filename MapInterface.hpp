#pragma once

#include <vector>
#include <memory>
/*
Include from adt repo, but still changing

#include "../adt/Box.hpp"
*/
namespace r2d2
{
    namespace adt
    {
        class Box{}; //temporary mock class until Box is finished
        class Coordinate{};
    }
}

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

          bool operator== (const BoxInfo & lhs) const;
          bool operator!= (const BoxInfo & lhs) const;


       private:
          const bool has_obstacle;
          const bool has_unknown;
          const bool has_navigatable;

       };

       class ReadOnlyMap
       {

       public:
           virtual BoxInfo & get_box_info(const adt::Box & box) = 0;
           virtual adt::Box & get_map_bounding_box() = 0;
           virtual std::shared_ptr<BoxInfo**> get_map_area_2d(const adt::Box & area, const adt::Box & pixel_size) = 0;
       };

       class ReadWriteMap : public ReadOnlyMap
       {

       public:
           virtual void set_box_info(const adt::Box & box, const BoxInfo & box_info) = 0;
       };

       class SaveLoadMap : public ReadWriteMap
       {

       public:
           virtual void save(const std::string filename) = 0;
           virtual void load(const std::string filename) = 0;
       };
   }
}


