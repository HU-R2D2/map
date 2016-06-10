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

		Translation axis_size = box.get_axis_size();
		double xlen = axis_size.get_x() / Length::METER,
				ylen = axis_size.get_y() / Length::METER,
				zlen = axis_size.get_z() / Length::METER;
		// calculate the total area of the requested box
		// if one of the axes is length zero then it won't be counted
		// in the volume, as it will in result in zero volume
		bool has_x = xlen > 0, has_y = ylen > 0, has_z = zlen > 0;
		// adt has no unit for volume, we'll have to use a double for now
		double vol = (has_x ? xlen : 1) *
				(has_x ? xlen : 1) *
				(has_x ? xlen : 1);

		for (std::pair<Box, BoxInfo> known_box : get_intersecting(box)){
			temp_has_obstacle = (
					temp_has_obstacle ||
					known_box.second.get_has_obstacle()
			);

			temp_has_navigatable = (
					temp_has_navigatable ||
					known_box.second.get_has_navigatable()
			);

			temp_has_unknown = (
					temp_has_unknown ||
					known_box.second.get_has_unknown()
			);

			// subtract the volume that this box takes in from the total box area
			axis_size = known_box.first.get_intersection_box(box).get_axis_size();
			vol -= (has_x ? axis_size.get_x() / Length::METER : 1)
			       * (has_x ? axis_size.get_y() / Length::METER : 1)
			       * (has_x ? axis_size.get_z() / Length::METER : 1);

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

	const Box ArrayBoxMap::get_map_bounding_box() {
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
		Translation axis_size = box.get_axis_size();
		// boxes of size zero should not be a problem here because
		std::vector<std::pair<Box, BoxInfo>> new_boxes;

		for (auto it = map.begin(); it != map.end();) {
			// iterator used instead of int to be able to remove elements efficiently
			// this implementation is separate from the get_intersecting one,
			// as it does not support returning iterators

			// use the old version of the intersection detection, as otherwise there will be boxes of length zero
			if (it->first.get_top_right().get_x() > box.get_bottom_left().get_x() &&
			    it->first.get_bottom_left().get_x() < box.get_top_right().get_x() &&
			    it->first.get_top_right().get_y() > box.get_bottom_left().get_y() &&
			    it->first.get_bottom_left().get_y() < box.get_top_right().get_y() &&
			    it->first.get_top_right().get_z() > box.get_bottom_left().get_z() &&
			    it->first.get_bottom_left().get_z() < box.get_top_right().get_z()) {
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
		if (box_info.get_has_navigatable() || box_info.get_has_obstacle()) {
			//Add the actual new box
			map.push_back(std::pair<Box, BoxInfo>{box, box_info});
		}
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