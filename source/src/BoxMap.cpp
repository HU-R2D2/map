
#include "../../../adt/source/include/Box.hpp"
#include "../include/BoxMap.hpp"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/filewritestream.h"
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>

namespace r2d2
{
    BoxInfo BoxMap::get_box_info(Box & box)
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

    Box BoxMap::get_map_bounding_box()
    {
        Box temp_box = Box();
        for (std::pair<Box, BoxInfo> box : map){
            temp_box = temp_box.get_union_box(box.first);
        }
        return temp_box;
    }

    std::shared_ptr<BoxInfo*> BoxMap::get_map_area_2d(Box & area, Box & pixel_size)
    {
        std::shared_ptr < BoxInfo*> bs;
        return bs;
    }

    void BoxMap::set_box_info(Box & box, BoxInfo & box_info)
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

    std::string removechars(std::string str) {
        for ( auto & c : str ) {
            if ( isalpha( c ) ) c = ' ';
        }
        return str;
    }

    template <typename T>
    string NumberToString ( T Number )
    {
        stringstream ss;
        ss << Number;
        return ss.str();
    }

    void BoxMap::save(std::string filename)
    {
        rapidjson::Document d;
        d.SetObject();

        rapidjson::Value leftCoordinates(rapidjson::kArrayType);
        rapidjson::Value rightCoordinates(rapidjson::kArrayType);
        rapidjson::Value boxInfos(rapidjson::kArrayType);

        for(int j = 0; j < map.size(); j++) {
            rapidjson::Value leftCoordinate(rapidjson::kObjectType);
                rapidjson::Value lcX;
                lcX.SetDouble(map[j].first.get_bottom_left().get_x() / Length::METER);

                rapidjson::Value lcY(rapidjson::kStringType);
                lcY.SetDouble(map[j].first.get_bottom_left().get_y() / Length::METER);

                rapidjson::Value lcZ(rapidjson::kStringType);
                lcZ.SetDouble(map[j].first.get_bottom_left().get_z() / Length::METER);

            rapidjson::Value rightCoordinate(rapidjson::kObjectType);
                rapidjson::Value rcX;
                rcX.SetDouble(map[j].first.get_top_right().get_x() / Length::METER);

                rapidjson::Value rcY(rapidjson::kStringType);
                rcY.SetDouble(map[j].first.get_top_right().get_y() / Length::METER);

                rapidjson::Value rcZ(rapidjson::kStringType);
                rcZ.SetDouble(map[j].first.get_top_right().get_z() / Length::METER);

            rapidjson::Value boxInfo(rapidjson::kObjectType);
                rapidjson::Value has_obstacle;
                has_obstacle.SetBool(map[j].second.get_has_obstacle());

                rapidjson::Value has_unknown;
                has_unknown.SetBool(map[j].second.get_has_unknown());

                rapidjson::Value has_navigatable;
                has_navigatable.SetBool(map[j].second.get_has_navigatable());

            leftCoordinate.AddMember("x", lcX, d.GetAllocator());
            leftCoordinate.AddMember("y", lcY, d.GetAllocator());
            leftCoordinate.AddMember("z", lcZ, d.GetAllocator());

            rightCoordinate.AddMember("x", rcX, d.GetAllocator());
            rightCoordinate.AddMember("y", rcY, d.GetAllocator());
            rightCoordinate.AddMember("z", rcZ, d.GetAllocator());

            boxInfo.AddMember("has_obstacle", has_obstacle, d.GetAllocator());
            boxInfo.AddMember("has_unknown", has_unknown, d.GetAllocator());
            boxInfo.AddMember("has_navigatable", has_navigatable, d.GetAllocator());

            leftCoordinates.PushBack(leftCoordinate, d.GetAllocator());
            rightCoordinates.PushBack(rightCoordinate, d.GetAllocator());
            boxInfos.PushBack(boxInfo, d.GetAllocator());
        }

        d.AddMember("leftCoordinates", leftCoordinates, d.GetAllocator());
        d.AddMember("rightCoordinates", rightCoordinates, d.GetAllocator());
        d.AddMember("boxInfo", boxInfos, d.GetAllocator());
        FILE* fp = fopen(filename.c_str(), "wb");
        char writeBuffer[65536];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
        d.Accept(writer);
    }


    void BoxMap::load(std::string filename)
    {
        FILE* pFILE = fopen(filename.c_str() , "rb");
        char buff[65536];
        rapidjson::FileReadStream is(pFILE, buff, sizeof(buff));
        rapidjson::Document d;

        d.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
        const rapidjson::Value& a = d["leftCoordinates"];

        for(rapidjson::SizeType i = 0; i < a.Size(); i++) {
            Box pBox = Box(Coordinate((d["leftCoordinates"][i]["x"].GetDouble() * Length::METER), (d["leftCoordinates"][i]["y"].GetDouble() * Length::METER), (d["leftCoordinates"][i]["z"].GetDouble() * Length::METER)), Coordinate((d["rightCoordinates"][i]["x"].GetDouble() * Length::METER), (d["rightCoordinates"][i]["y"].GetDouble() * Length::METER), (d["rightCoordinates"][i]["z"].GetDouble() * Length::METER)));
            BoxInfo pBoxInfo(d["boxInfo"][i]["has_obstacle"].GetBool(),d["boxInfo"][i]["has_unknown"].GetBool(),d["boxInfo"][i]["has_navigatable"].GetBool());
            set_box_info(pBox, pBoxInfo);
        }
    }
}