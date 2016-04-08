
#include "../../../adt/source/include/Box.hpp"
#include "../include/BoxMap.hpp"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/filewritestream.h"
#include"../include/rapidjson/writer.h"
#include <fstream>
#include <string> 
#include <sstream>

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

    void BoxMap::save(std::string filename)
    {
        int filenameIndex = 0;

        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        writer.StartObject();

        for (std::pair<Box, BoxInfo> Boxes : map){
            filenameIndex++;
            string Result;//string which will contain the result
            stringstream convert; // stringstream used for the conversion
            convert << filenameIndex;//add the value of Number to the characters in the stream
            Result = convert.str();//set Result to the content of the stream
            const char * waarde = Result.c_str();// c_string type

            writer.String(waarde);
            writer.StartObject();
            writer.String("leftcoordinate");
            writer.StartObject();
            writer.String("x");
            writer.Double(Boxes.first.get_bottom_left().get_x() / Length::METER); //zetWaarde
            writer.String("y");
            writer.Double(Boxes.first.get_bottom_left().get_y() / Length::METER); //zetWaarde
            writer.String("z");
            writer.Double(Boxes.first.get_bottom_left().get_z() / Length::METER); //zetWaarde
            writer.EndObject();
            writer.String("leftcoordinate");
            writer.StartObject();
            writer.String("x");
            writer.Double(Boxes.first.get_top_right().get_x() / Length::METER); //zetWaarde
            writer.String("y");
            writer.Double(Boxes.first.get_top_right().get_y() / Length::METER); //zetWaarde
            writer.String("z");
            writer.Double(Boxes.first.get_top_right().get_z() / Length::METER); //zetWaarde
            writer.EndObject();
            writer.String("boxinfo");
            writer.StartObject();
            writer.String("has_obstacle");
            writer.Bool(Boxes.second.get_has_obstacle()); //zetWaarde
            writer.String("has_obstacle");
            writer.Bool(Boxes.second.get_has_unknown()); //zetWaarde
            writer.String("has_obstacle");
            writer.Bool(Boxes.second.get_has_navigatable()); //zetWaarde
            writer.EndObject();
            writer.EndObject();

        }
        writer.EndObject();
        std::cout << s.GetString() << endl;
        std::ofstream fs(filename);
        if (!fs)
        {
            std::cerr << "Cannot open the output file." << std::endl;
        }
        fs << s.GetString() << endl;// output to fs


    }


    void BoxMap::load(std::string filename)
    {
        FILE* pFILE = fopen(filename.c_str(), "rb");
        char buff[65536];
        rapidjson::FileReadStream is(pFILE, buff, sizeof(buff));
        // 1. Parse a JSON string into DOM
        rapidjson::Document d;
        d.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

        Box pBox = Box(Coordinate((d["box0"]["leftCoordinate"]["x"].GetDouble() * Length::METER), (d["box0"]["leftCoordinate"]["y"].GetDouble() * Length::METER), (d["box0"]["leftCoordinate"]["z"].GetDouble() * Length::METER)), Coordinate((d["box0"]["rightCoordinate"]["x"].GetDouble() * Length::METER), (d["box0"]["rightCoordinate"]["y"].GetDouble() * Length::METER), (d["box0"]["rightCoordinate"]["z"].GetDouble() * Length::METER)));
        BoxInfo pBoxInfo(d["box0"]["boxInfo"]["has_obstacle"].GetBool(), d["box0"]["boxInfo"]["has_unknown"].GetBool(), d["box0"]["boxInfo"]["has_navigatable"].GetBool());
        set_box_info(pBox, pBoxInfo);
    }
}