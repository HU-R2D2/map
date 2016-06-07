// ++--++
// @file ArrayBoxMap.cpp
// @date Created: <07-04-16>
// @version <1.0.0>
//
// @author Sander Kolman
// @author Maikel Bolderdijk
//
// @section LICENSE
// License: newBSD
//
// Copyright � 2016, HU University of Applied Sciences Utrecht.
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

#include "../include/ArrayBoxMap.hpp"

namespace r2d2 {

	const BoxInfo ArrayBoxMap::get_box_info(const Box box) {
		bool temp_has_obstacle = false;
		bool temp_has_navigatable = false;
		bool temp_has_unknown = false;

		// calculate the total area of the requested box
		Translation axis_size = box.get_axis_size();
		// adt has no unit for volume, we'll have to use a double for now
		double vol = axis_size.get_x() / Length::METER
		             * axis_size.get_y() / Length::METER
		             * axis_size.get_z() / Length::METER;

		for (std::pair<Box, BoxInfo> known_box : get_intersecting(box)){
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

			// subtract the volume that this box takes in from the total box area
			axis_size = known_box.first.get_intersection_box(box).get_axis_size();
			vol -= axis_size.get_x() / Length::METER
			       * axis_size.get_y() / Length::METER
			       * axis_size.get_z() / Length::METER;

			if (temp_has_obstacle && temp_has_unknown && temp_has_navigatable) {
				// if all three are true then the result must be as such
				return BoxInfo{
						temp_has_obstacle,
						temp_has_unknown,
						temp_has_navigatable
				};
			}
		}

		static const double significant = 0.0001;
		// if there is some amount of uncovered area within the requested box
		// then there is unknown in the area
		if (vol > significant || !(temp_has_obstacle || temp_has_navigatable)) {
			temp_has_unknown = true;
		}

		return BoxInfo{
				temp_has_obstacle,
				temp_has_navigatable,
				temp_has_unknown
		};
	}

	const Box ArrayBoxMap::get_map_bounding_box()
	{
		if (map.empty()) {
			return Box{};
		}
		Box temp_box{ map[0].first };
		for (std::pair<Box, BoxInfo> box : map){
			temp_box = temp_box.get_union_box(box.first);
		}
		return temp_box;
	}

	void ArrayBoxMap::set_box_info(const Box box, const BoxInfo box_info) {
		std::vector<std::pair<Box, BoxInfo>> new_boxes;

		for (auto it = map.begin(); it != map.end();) {
			// iterator used instead of int to be able to remove elements efficiently
			// this implementation is separate from the get_intersecting one,
			// as it does not support returning iterators
			if (box.intersects(it->first)) {
				// current box has to be cut because of intersecting with the insertion box

				if (it->first.get_bottom_left().get_x()
				    < box.get_bottom_left().get_x()) {
					// the current box extends the new box in the -x direction

					// insert the extension
					new_boxes.push_back(
							{Box{it->first.get_bottom_left(), Coordinate{
									box.get_bottom_left().get_x(),
									it->first.get_top_right().get_y(),
									it->first.get_top_right().get_z()
							}},
							 it->second}
					);

					// resize the current box as to not include the newly made box
					it->first = Box{Coordinate{
							box.get_bottom_left().get_x(),
							it->first.get_bottom_left().get_y(),
							it->first.get_bottom_left().get_z()
					}, it->first.get_top_right()};
				}

				if (it->first.get_top_right().get_x()
				    > box.get_top_right().get_x()) {

					//Make new box
					new_boxes.push_back(
							{Box{Coordinate{
									box.get_top_right().get_x(),
									it->first.get_bottom_left().get_y(),
									it->first.get_bottom_left().get_z()
							}, it->first.get_top_right()},
							 it->second
							});

					//Cut-away old
					it->first = Box{it->first.get_bottom_left(), Coordinate{
							box.get_top_right().get_x(),
							it->first.get_top_right().get_y(),
							it->first.get_top_right().get_z()
					}};
				}

				if (it->first.get_bottom_left().get_y()
				    < box.get_bottom_left().get_y()) {
					new_boxes.push_back(
							{Box{it->first.get_bottom_left(), Coordinate{
									it->first.get_top_right().get_x(),
									box.get_bottom_left().get_y(),
									it->first.get_top_right().get_z()
							}},
							 it->second
							});

					it->first = Box{Coordinate{
							it->first.get_bottom_left().get_x(),
							box.get_bottom_left().get_y(),
							it->first.get_bottom_left().get_z()
					}, it->first.get_top_right()};
				}

				if (it->first.get_top_right().get_y()
				    > box.get_top_right().get_y()) {
					new_boxes.push_back(
							{Box{Coordinate{
									it->first.get_bottom_left().get_x(),
									box.get_top_right().get_y(),
									it->first.get_bottom_left().get_z()
							}, it->first.get_top_right()},
							 it->second
							});

					it->first = Box{it->first.get_bottom_left(), Coordinate{
							it->first.get_top_right().get_x(),
							box.get_top_right().get_y(),
							it->first.get_top_right().get_z()
					}};
				}

				if (it->first.get_top_right().get_z()
				    > box.get_top_right().get_z()) {
					new_boxes.push_back(
							{Box{Coordinate{
									it->first.get_bottom_left().get_x(),
									it->first.get_bottom_left().get_y(),
									box.get_top_right().get_z()
							}, it->first.get_top_right()},
							 it->second
							});

					// last box cutaway removed
				}

				if (it->first.get_bottom_left().get_z()
				    < box.get_bottom_left().get_z()) {
					new_boxes.push_back(
							{Box{it->first.get_bottom_left(), Coordinate{
									it->first.get_top_right().get_x(),
									it->first.get_top_right().get_y(),
									box.get_bottom_left().get_z()
							}},
							 it->second
							});

					// last box cutaway removed
				}

				// remove the old box as it will be split into multiple
				it = map.erase(it);
			} else {
				++it;
			}
		}

		//Add newly cut boxes
		for (std::pair<Box, BoxInfo> box_cut : new_boxes){
			map.push_back(box_cut);
		}

		// if the new box has neither navigable or an obstacle
		// then don't bother adding it as the empty area is unknown by default
//		if (box_info.get_has_navigatable() || box_info.get_has_obstacle()) {
			//Add the actual new box
			map.push_back(std::pair<Box, BoxInfo>{box, box_info});
//		}
	}

	void ArrayBoxMap::save(std::string filename)
	{
		// Create a DOM document
		rapidjson::Document d;
		d.SetObject();

		// Create the first level of json
		// { "left_coordinates":[{}],
		//   "right_coordinates":[{}],
		//   "box_infos":[{}]
		// }
		rapidjson::Value boxes(rapidjson::kArrayType);

		for(int j = 0; j < map.size(); j++) {
			rapidjson::Value box(rapidjson::kObjectType);
			// Create coordinate placeholders
			// "left_coordinates":
			// [{
			// "x":left_coordinate_x,
			// "y":left_coordinate_y,
			// "z":left_coordinate_z
			// }]
			rapidjson::Value left_coordinate(rapidjson::kObjectType);
			rapidjson::Value left_coordinate_x;
			left_coordinate_x.SetDouble(
					map[j].first.get_bottom_left().get_x() / Length::METER);

			rapidjson::Value left_coordinate_y(rapidjson::kStringType);
			left_coordinate_y.SetDouble(
					map[j].first.get_bottom_left().get_y() / Length::METER);

			rapidjson::Value left_coordinate_z(rapidjson::kStringType);
			left_coordinate_z.SetDouble(
					map[j].first.get_bottom_left().get_z() / Length::METER);

			// Create coordinate placeholders
			// "right_coordinates":[{
			// "x":right_coordinate_x,
			// "y":right_coordinate_y,
			// "z":right_coordinate_z
			// }]
			rapidjson::Value right_coordinate(rapidjson::kObjectType);
			rapidjson::Value right_coordinate_x;
			right_coordinate_x.SetDouble(
					map[j].first.get_top_right().get_x() / Length::METER);

			rapidjson::Value right_coordinate_y(rapidjson::kStringType);
			right_coordinate_y.SetDouble(
					map[j].first.get_top_right().get_y() / Length::METER);

			rapidjson::Value right_coordinate_z(rapidjson::kStringType);
			right_coordinate_z.SetDouble(
					map[j].first.get_top_right().get_z() / Length::METER);

			// Create box_info placeholders
			// "box_infos":[{
			// "has_obstacle":has_obstacle,
			// "has_unknown":has_unknown,
			// "has_navigatable":has_navigatable
			// }]
			rapidjson::Value box_info(rapidjson::kObjectType);
			rapidjson::Value has_obstacle;
			has_obstacle.SetBool(map[j].second.get_has_obstacle());

			rapidjson::Value has_navigatable;
			has_navigatable.SetBool(map[j].second.get_has_navigatable());

			rapidjson::Value has_unknown;
			has_unknown.SetBool(map[j].second.get_has_unknown());

			// Add indexes to Objects
			left_coordinate.AddMember("x", left_coordinate_x, d.GetAllocator());
			left_coordinate.AddMember("y", left_coordinate_y, d.GetAllocator());
			left_coordinate.AddMember("z", left_coordinate_z, d.GetAllocator());

			right_coordinate.AddMember(
					"x",
					right_coordinate_x,
					d.GetAllocator()
			);
			right_coordinate.AddMember(
					"y",
					right_coordinate_y,
					d.GetAllocator()
			);
			right_coordinate.AddMember(
					"z",
					right_coordinate_z,
					d.GetAllocator()
			);

			box_info.AddMember(
					"has_obstacle",
					has_obstacle,
					d.GetAllocator()
			);
			box_info.AddMember(
					"has_navigatable",
					has_navigatable,
					d.GetAllocator()
			);
			box_info.AddMember(
					"has_unknown",
					has_unknown,
					d.GetAllocator()
			);

			box.AddMember("left_coordinate", left_coordinate, d.GetAllocator());
			box.AddMember("right_coordinate", right_coordinate, d.GetAllocator());
			box.AddMember("box_info", box_info, d.GetAllocator());

			boxes.PushBack(box, d.GetAllocator());
		}

		d.AddMember("boxes", boxes, d.GetAllocator());

		// Write the file to given filename
		FILE* pFILE = fopen(filename.c_str(), "wb");
		if(!pFILE) {
			fclose(pFILE);
			throw std::exception();
		}
		char writeBuffer[65536];
		rapidjson::FileWriteStream os(pFILE, writeBuffer, sizeof(writeBuffer));
		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		d.Accept(writer);

		// Reset writer for re-use and close the file
		writer.Reset(os);
		fclose(pFILE);
	}

	void ArrayBoxMap::load(std::string filename)
	{
		// Open file
		FILE* pFILE = fopen(filename.c_str() , "rb");
		if(!pFILE) {
			fclose(pFILE);
			throw std::exception();
		}

		char buff[65536];
		rapidjson::FileReadStream frs(pFILE, buff, sizeof(buff));
		rapidjson::Document d;

		// Push contents into DOM element
		d.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(frs);

		if(!d.HasMember("boxes"))
		{
			fclose(pFILE);
			throw rapidjson::ParseErrorCode::kParseErrorObjectMissName;
		}

		// Clear current map
		map.clear();

		// Allocate row_size
		const rapidjson::SizeType row_size = d["boxes"].Size();

		// Load values from DOM element
		for(rapidjson::SizeType i = 0; i < row_size; i++) {
			if(d["boxes"][i]["left_coordinate"] == NULL,
					d["boxes"][i]["right_coordinate"] == NULL,
					d["boxes"][i]["box_info"] == NULL)
			{
				fclose(pFILE);
				throw rapidjson::ParseErrorCode::kParseErrorObjectMissName;
			}

			Box map_Box =
					Box(Coordinate(
							(d["boxes"][i]["left_coordinate"]["x"].GetDouble()
							 * Length::METER),
							(d["boxes"][i]["left_coordinate"]["y"].GetDouble()
							 * Length::METER),
							(d["boxes"][i]["left_coordinate"]["z"].GetDouble()
							 * Length::METER)),

					    Coordinate(
							    (d["boxes"][i]["right_coordinate"]["x"].GetDouble()
							     * Length::METER),
							    (d["boxes"][i]["right_coordinate"]["y"].GetDouble()
							     * Length::METER),
							    (d["boxes"][i]["right_coordinate"]["z"].GetDouble()
							     * Length::METER)));

			BoxInfo map_BoxInfo(d["boxes"][i]["box_info"]["has_obstacle"].GetBool(),
			                    d["boxes"][i]["box_info"]["has_navigatable"].GetBool(),
			                    d["boxes"][i]["box_info"]["has_unknown"].GetBool());

			std::pair<Box, BoxInfo> psh(map_Box, map_BoxInfo);
			map.push_back(psh);
		}

		// Close file
		fclose(pFILE);
	}

	int ArrayBoxMap::get_map_size() const {
		return int(map.size());
	}

	std::vector<std::pair<Box, BoxInfo>> ArrayBoxMap::get_intersecting(const Box &bounds) const {
		std::vector<std::pair<Box, BoxInfo>> intersecting;
		// naive algorithm for collision detection
		for (auto item : map) {
			if (bounds.intersects(item.first)) {
				intersecting.push_back(item);
			}
		}
		return intersecting;
	}

}