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
		std::vector<std::pair<Box, BoxInfo>> new_boxes;

		r2d2::Translation axis_size{box.get_axis_size()};
		bool has_x = axis_size.get_x() > 0 * r2d2::Length::METER,
				has_y = axis_size.get_y() > 0 * r2d2::Length::METER,
				has_z = axis_size.get_z() > 0 * r2d2::Length::METER;

		for (auto it = map.begin(); it != map.end();) {
			// iterator used instead of int to be able to remove elements efficiently
			// this implementation is separate from the get_intersecting one,
			// as it does not support returning iterators

			// do some wizzardry with length zero detection and stuff to circumvent some bugs
			if ((   (it->first.get_top_right().get_x() > box.get_bottom_left().get_x() &&
			         it->first.get_bottom_left().get_x() < box.get_top_right().get_x())
			        || (!has_x &&
			            (it->first.get_bottom_left().get_x() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_x() / r2d2::Length::METER))
			    )
			    &&
			    (   (it->first.get_top_right().get_y() > box.get_bottom_left().get_y() &&
			         it->first.get_bottom_left().get_y() < box.get_top_right().get_y())
			        || (!has_y &&
			            (it->first.get_bottom_left().get_y() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_y() / r2d2::Length::METER))
			    )
			    &&
			    (   (it->first.get_top_right().get_z() > box.get_bottom_left().get_z() &&
			         it->first.get_bottom_left().get_z() < box.get_top_right().get_z())
			        || (!has_z &&
			            (it->first.get_bottom_left().get_z() / r2d2::Length::METER) ==
			            (box.get_bottom_left().get_z() / r2d2::Length::METER)))) {
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
		for (std::pair<Box, BoxInfo> box_cut : new_boxes) {
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

	void ArrayBoxMap::add_box(Box box, BoxInfo info) {
		map.push_back({box, info});
	}

}