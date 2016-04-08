// ++--++
// @file BoxMap.cpp
// @date Created: <07-04-16>
// @version <1.0.0>
//
// @author Sander Kolman
//
// @section LICENSE
// License: newBSD
//
// Copyright © 2016, HU University of Applied Sciences Utrecht.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of the HU University of Applied Sciences Utrecht nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE HU UNIVERSITY OF APPLIED SCIENCES UTRECHT
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ++--++

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

    const BoxInfo BoxMap::get_box_info(const Box box)
    {
        bool temp_has_obstacle = false;
        bool temp_has_unknown = false;
        bool temp_has_navigatable = false;

        for (std::pair<Box, BoxInfo> known_box : map){
            if (known_box.first.intersects(box)){

                temp_has_obstacle = (
                    temp_has_obstacle ||
                    known_box.second.get_has_obstacle()
                );

                temp_has_unknown = (
                    temp_has_unknown || 
                    known_box.second.get_has_unknown()
                );

                temp_has_navigatable = (
                    temp_has_navigatable || 
                    known_box.second.get_has_navigatable()
                );
            }
        }

        return BoxInfo{ 
            temp_has_obstacle, 
            temp_has_unknown, 
            temp_has_navigatable 
        };
    }

    const Box BoxMap::get_map_bounding_box()
    {
        if (map.empty()) return Box{};
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
                    Box temp_box = map[j].first;
                    r2d2::BoxInfo temp_info = map[j].second;

                    //Cut-away Left exterior
                    if (temp_box.get_bottom_left().get_x()
                                        < box.get_bottom_left().get_x()){

                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{
                                temp_box.get_bottom_left(), 
                                Coordinate{ 
                                    box.get_bottom_left().get_x(), 
                                    temp_box.get_top_right().get_y(), 
                                    temp_box.get_top_right().get_z() 
                                } 
                            }, 
                            temp_info
                        });

                        //Cut-away old
                        temp_box = Box{
                            Coordinate{ 
                                box.get_bottom_left().get_x(), 
                                temp_box.get_bottom_left().get_y(), 
                                temp_box.get_bottom_left().get_z() 
                            },
                            temp_box.get_top_right()
                        };
                    }

                    //Cut-away Right exterior
                    if (temp_box.get_top_right().get_x() 
                                        > box.get_bottom_left().get_x()){

                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{ 
                                Coordinate{ 
                                    box.get_top_right().get_x(), 
                                    temp_box.get_bottom_left().get_y(), 
                                    temp_box.get_bottom_left().get_z() 
                                }, 
                                temp_box.get_top_right() 
                            }, 
                            temp_info
                        });

                        //Cut-away old
                        temp_box = Box{ 
                            temp_box.get_bottom_left(), 
                            Coordinate{ 
                                box.get_top_right().get_x(), 
                                temp_box.get_top_right().get_y(), 
                                temp_box.get_top_right().get_z() 
                            } 
                        };
                    }

                    //Cut-away Bottom exterior
                    if (temp_box.get_bottom_left().get_y() 
                                        < box.get_bottom_left().get_y()){

                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{ 
                                temp_box.get_bottom_left(), 
                                Coordinate{ 
                                    temp_box.get_top_right().get_x(), 
                                    box.get_bottom_left().get_y(), 
                                    temp_box.get_top_right().get_z() 
                                } 
                            }, 
                            temp_info
                        });
                        
                        //Cut-away old
                        temp_box = Box{ 
                            Coordinate{ 
                                temp_box.get_bottom_left().get_x(), 
                                box.get_bottom_left().get_y(), 
                                temp_box.get_bottom_left().get_z() 
                            }, 
                            temp_box.get_top_right() 
                        };
                    }

                    //Cut-away Top exterior
                    if (temp_box.get_top_right().get_y() 
                                        > box.get_top_right().get_y()){

                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{ 
                                Coordinate{ 
                                    temp_box.get_bottom_left().get_x(), 
                                    box.get_top_right().get_y(), 
                                    temp_box.get_bottom_left().get_z() 
                                }, 
                                temp_box.get_top_right() 
                            }, 
                            temp_info
                        });

                        //Cut-away old
                        temp_box = Box{ 
                            temp_box.get_bottom_left(), 
                            Coordinate{ 
                                temp_box.get_top_right().get_x(), 
                                box.get_top_right().get_y(), 
                                temp_box.get_top_right().get_z() 
                            } 
                        };
                    }

                    //Cut-away Front exterior
                    if (temp_box.get_top_right().get_z() 
                                        < box.get_top_right().get_z()){

                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{ 
                                Coordinate{ 
                                    temp_box.get_bottom_left().get_x(), 
                                    temp_box.get_bottom_left().get_y(), 
                                    box.get_top_right().get_z() 
                                }, 
                                temp_box.get_top_right() 
                            }, 
                            temp_info
                        });

                        //Cut-away old
                        temp_box = Box{ 
                            temp_box.get_bottom_left(), 
                            Coordinate{ 
                                temp_box.get_top_right().get_x(), 
                                temp_box.get_top_right().get_y(), 
                                box.get_top_right().get_z() 
                            } 
                        };
                    }

                    //Cut-away Back exterior
                    if (temp_box.get_bottom_left().get_z() 
                                        > box.get_bottom_left().get_z()){
                        
                        //Make new box
                        new_boxes.push_back(std::pair<Box, BoxInfo>{
                            Box{ 
                                temp_box.get_bottom_left(), 
                                Coordinate{ 
                                    temp_box.get_top_right().get_x(), 
                                    temp_box.get_top_right().get_y(), 
                                    box.get_bottom_left().get_z() 
                                } 
                            }, 
                            temp_info
                        });

                        //Cut-away old
                        temp_box = Box{ 
                            Coordinate{ 
                                temp_box.get_bottom_left().get_x(), 
                                temp_box.get_bottom_left().get_y(), 
                                box.get_top_right().get_z() 
                            }, 
                            temp_box.get_top_right() 
                        };
                    }
                }
            }
        }

        //Remove old boxes
        for (int i : to_be_removed){
            map.erase(map.begin() + i);
        }
        map.shrink_to_fit();

        //Add newly cut boxes
        for (std::pair<Box, BoxInfo> box_cut : new_boxes){
            map.push_back(box_cut);
        }
        //Add the actual new box
        map.push_back(std::pair<Box, BoxInfo>{box, box_info});
    }

    void BoxMap::save(std::string filename)
    {
        int filenameIndex = 0;

        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        writer.StartObject();
        writer.String("leftcoordinate");
        writer.StartArray();

        for (std::pair<Box, BoxInfo> Boxes : map){
            //leftcoordinate
            writer.StartObject();
            writer.String("x");
            writer.Double(Boxes.first.get_bottom_left().get_x() / Length::METER); //zetWaarde
            writer.String("y");
            writer.Double(Boxes.first.get_bottom_left().get_y() / Length::METER); //zetWaarde
            writer.String("z");
            writer.Double(Boxes.first.get_bottom_left().get_z() / Length::METER); //zetWaarde
            writer.EndObject();

        }
        writer.EndArray();
        writer.String("rightcoordinate");
        writer.StartArray();
        for (std::pair<Box, BoxInfo> Boxes : map){
            //rightcoordinate
            writer.StartObject();
            writer.String("x");
            writer.Double(Boxes.first.get_top_right().get_x() / Length::METER); //zetWaarde
            writer.String("y");
            writer.Double(Boxes.first.get_top_right().get_y() / Length::METER); //zetWaarde
            writer.String("z");
            writer.Double(Boxes.first.get_top_right().get_z() / Length::METER); //zetWaarde
            writer.EndObject();

        }
        writer.EndArray();
        writer.String("boxinfo");
        writer.StartArray();
        for (std::pair<Box, BoxInfo> Boxes : map){
            //boxinfo
            writer.StartObject();
            writer.String("has_obstacle");
            writer.Bool(Boxes.second.get_has_obstacle()); //zetWaarde

            writer.String("has_unknown");
            writer.Bool(Boxes.second.get_has_unknown()); //zetWaarde
            writer.String("has_navigatable");
            writer.Bool(Boxes.second.get_has_navigatable()); //zetWaarde
            writer.EndObject();

        }
        writer.EndArray();
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

        const rapidjson::Value& elementSize = d["leftCoordinates"];

        for(rapidjson::SizeType i = 0; i < elementSize.Size(); i++) {
            Box mBox = Box(Coordinate((d["leftCoordinates"][i]["x"].GetDouble() * Length::METER), (d["leftCoordinates"][i]["y"].GetDouble() * Length::METER), (d["leftCoordinates"][i]["z"].GetDouble() * Length::METER)), Coordinate((d["rightCoordinates"][i]["x"].GetDouble() * Length::METER), (d["rightCoordinates"][i]["y"].GetDouble() * Length::METER), (d["rightCoordinates"][i]["z"].GetDouble() * Length::METER)));
            BoxInfo mBoxInfo(d["boxInfo"][i]["has_obstacle"].GetBool(),d["boxInfo"][i]["has_unknown"].GetBool(),d["boxInfo"][i]["has_navigatable"].GetBool());
            set_box_info(mBox, mBoxInfo);
        }
    }

    int BoxMap::get_map_size(){
        return map.size();
    }
}