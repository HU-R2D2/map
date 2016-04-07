
#include "../../../adt/source/include/Box.hpp"
#include "../include/BoxMap.hpp"

namespace r2d2
{

    const BoxInfo BoxMap::get_box_info(const Box box)
    {
        bool temp_has_obstacle = false;
        bool temp_has_unknown = false;
        bool temp_has_navigatable = false;

        for (std::pair<Box, BoxInfo> known_box : map){
            if (known_box.first.intersects(box)){
                temp_has_obstacle = temp_has_obstacle || known_box.second.get_has_obstacle();
                temp_has_unknown = temp_has_unknown || known_box.second.get_has_unknown();
                temp_has_navigatable = temp_has_navigatable || known_box.second.get_has_navigatable();
            }
        }

        return BoxInfo{ temp_has_obstacle, temp_has_unknown, temp_has_navigatable };
    }

    const Box BoxMap::get_map_bounding_box()
    {
        if (map.empty()){ return Box{}; }
        Box temp_box{ map[0].first };
        for (std::pair<Box, BoxInfo> box : map){
            temp_box = temp_box.get_union_box(box.first);
        }
        return temp_box;
    }

    void BoxMap::set_box_info(const Box box, const BoxInfo box_info)
    {

        std::vector<int> to_be_removed;
        std::vector<std::pair<Box, BoxInfo>> new_boxes;

        for (unsigned int j = 0; j < map.size(); j++){
            if (box.intersects(map[j].first)){

                to_be_removed.push_back(j);
                if (!box.contains(map[j].first)){
                    std::pair<Box, BoxInfo> temp_box = map[j];

                    if (temp_box.first.get_bottom_left().get_x() < box.get_bottom_left().get_x()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ temp_box.first.get_bottom_left(), Coordinate{ box.get_bottom_left().get_x(), temp_box.first.get_top_right().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{ Box{ Coordinate{ box.get_bottom_left().get_x(), temp_box.first.get_bottom_left().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second };
                    }
                    if (temp_box.first.get_top_right().get_x() > box.get_bottom_left().get_x()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ Coordinate{ box.get_top_right().get_x(), temp_box.first.get_bottom_left().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{ Box{ temp_box.first.get_bottom_left(), Coordinate{ box.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second };
                    }

                    if (temp_box.first.get_bottom_left().get_y() < box.get_bottom_left().get_y()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ temp_box.first.get_bottom_left(), Coordinate{ temp_box.first.get_top_right().get_x(), box.get_bottom_left().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{ Box{ Coordinate{ temp_box.first.get_bottom_left().get_x(), box.get_bottom_left().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second};
                    }
                    if (temp_box.first.get_top_right().get_y() > box.get_top_right().get_y()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ Coordinate{ temp_box.first.get_bottom_left().get_x(), box.get_top_right().get_y(), temp_box.first.get_bottom_left().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{Box{ temp_box.first.get_bottom_left(), Coordinate{ temp_box.first.get_top_right().get_x(), box.get_top_right().get_y(), temp_box.first.get_top_right().get_z() } }, temp_box.second};
                    }

                    if (temp_box.first.get_top_right().get_z() < box.get_top_right().get_z()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ Coordinate{ temp_box.first.get_bottom_left().get_x(), temp_box.first.get_bottom_left().get_y(), box.get_top_right().get_z() }, temp_box.first.get_top_right() }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{Box{ temp_box.first.get_bottom_left(), Coordinate{ temp_box.first.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), box.get_top_right().get_z() } }, temp_box.second};
                    }

                    if (temp_box.first.get_bottom_left().get_z() > box.get_bottom_left().get_z()){
                        new_boxes.push_back(std::pair<Box, BoxInfo>{Box{ temp_box.first.get_bottom_left(), Coordinate{ temp_box.first.get_top_right().get_x(), temp_box.first.get_top_right().get_y(), box.get_bottom_left().get_z() } }, temp_box.second});
                        temp_box = std::pair<Box, BoxInfo>{ Box{ Coordinate{ temp_box.first.get_bottom_left().get_x(), temp_box.first.get_bottom_left().get_y(), box.get_top_right().get_z() }, temp_box.first.get_top_right() }, temp_box.second};
                    }
                }
            }
        }

        for (int i : to_be_removed){

            map.erase(map.begin() + i);
        }
        map.shrink_to_fit();
        for (std::pair<Box, BoxInfo> box_cut : new_boxes){
            map.push_back(box_cut);
        }
        map.push_back(std::pair<Box, BoxInfo>{box, box_info});
    }

    void BoxMap::save(std::string filename)
    {

    }


    void BoxMap::load(std::string filename)
    {

    }

    int BoxMap::get_map_size(){
        return map.size();
    }
}