#include "BoxMap.hpp"

namespace r2d2
{
    namespace map
    {

        BoxInfo & BoxMap::get_box_info(const adt::Box & box)
        {
            bool temp_has_obstacle = false;
            bool temp_has_unknown = false;
            bool temp_has_navigatable = false;
            
            for (std::pair<adt::Box, BoxInfo> known_box : map){
                if (known_box.first.intersects(box)){
                    temp_has_obstacle = temp_has_obstacle || known_box.second.get_has_obstacle();
                    temp_has_unknown = temp_has_unknown || known_box.second.get_has_unknown();
                    temp_has_navigatable = temp_has_navigatable || known_box.second.get_has_navigatable();
                }
            }
            return BoxInfo{temp_has_obstacle, temp_has_unknown, temp_has_navigatable};
        }

        adt::Box & BoxMap::get_map_bounding_box()
        {
            adt::Box temp_box{};
            for (std::pair<adt::Box, BoxInfo> box : map){
                temp_box = temp_box.get_union_box(box);
            }
            return temp_box;
        }

        std::shared_ptr<BoxInfo**> BoxMap::get_map_area_2d(const adt::Box & area, const adt::Box & pixel_size)
        {
            std::shared_ptr <BoxInfo**> bs;
            return bs;
        }

        void BoxMap::set_box_info(const adt::Box & box, const BoxInfo & box_info)
        {

            std::vector<int> to_be_removed;
            std::vector<std::pair<adt::Box, BoxInfo>> new_boxes;
            
            for (int j = 0; j < map.size(); j++){
                if (box.intersects(map[j].first)){

                    to_be_removed.push_back(j);

                    if (!box.contains(map[j].first)){
                        std::pair<adt::Box, BoxInfo> temp_box = map[j];

                        if (temp_box.first.get_bottom_left().get_x() < box.get_bottom_left().get_x()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ box.get_bottom_left().get_x(), temp_box.first.get_top_right().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{ adt::Box{ adt::Coordinate{ box.get_bottom_left().get_x(), temp_box.first.get_bottom_left().get_y(), temp_box.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second };
                        }
                        if (temp_box.first.get_top_right().get_x() > box.get_bottom_left().get_x()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ adt::Coordinate{ box.get_top_right().get_x(), temp_box.first.get_bottom_left().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{ adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ box.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), temp_box.get_top_right().get_z() } }, temp_box.second };
                        }

                        if (temp_box.first.get_bottom_left().get_y() < box.get_bottom_left().get_y()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ temp_box.first.get_top_right().get_x(), box.get_bottom_left().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{ adt::Box{ adt::Coordinate{ temp_box.first.get_bottom_left().get_x(), box.get_bottom_left().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second};
                        }
                        if (temp_box.first.get_top_right().get_y() > box.get_top_right().get_y()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ adt::Coordinate{ temp_box.first.get_bottom_left().get_x(), box.get_top_right().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ temp_box.first.get_top_right().get_x(), box.get_top_right().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second};
                        }

                        if (temp_box.first.get_top_right().get_z() < box.get_top_right().get_z()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ adt::Coordinate{ temp_box.first.get_bottom_left().get_x(), temp_box.first.get_bottom_left().get_y(), box.get_top_right().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ temp_box.first.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), box.get_top_right().get_z() } }, temp_box.second};
                        }

                        if (temp_box.first.get_bottom_left().get_z() > box.get_bottom_left().get_z()){
                            new_boxes.push_back(std::pair<adt::Box, BoxInfo>{adt::Box{ temp_box.first.get_bottom_left(), adt::Coordinate{ temp_box.first.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), box.get_bottom_left().get_z() } }, temp_box.second});
                            temp_box = std::pair<adt::Box, BoxInfo>{adt::Box{ adt::Coordinate{ temp_box.first.get_bottom_left().get_x(), temp_box.get_bottom_left().get_y(), box.get_top_right().get_z() }, temp_box.first.get_top_right() }, temp_box.second};
                        }
                    }
                }
            }

            for (int i : to_be_removed){
                map.erase(map.begin() + i);
            }
            map.shrink_to_fit();
            for (std::pair<adt::Box, BoxInfo> box_cut : new_boxes){
                map.push_back(box_cut);
            }
            map.push_back(std::pair<adt::Box, BoxInfo>{box, box_info});
        }

        void BoxMap::save(const std::string filename)
        {

        }


        void BoxMap::load(const std::string filename)
        {

        }
    }
}