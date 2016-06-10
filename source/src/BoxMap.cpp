//// ++--++
// @file BoxMap.hpp
// @date Created: 08/06/16
// @version <1.1.0>
//
// @author Chiel Douwes
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

#include "../include/BoxMap.hpp"

namespace r2d2 {

	const BoxInfo BoxMap::get_box_info(const Box box) {
		bool temp_has_obstacle = false;
		bool temp_has_unknown = false;
		bool temp_has_navigatable = false;

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

		for (auto &found_box : get_intersecting(box)) {
			if (found_box.second.get_has_obstacle()) {
				temp_has_obstacle = true;
			}

			if (found_box.second.get_has_navigatable()) {
				temp_has_navigatable = true;
			}

			if (found_box.second.get_has_unknown()) {
				temp_has_unknown = true;
			}

			// subtract the volume that this box takes in from the total box area
			axis_size = found_box.first.get_intersection_box(box).get_axis_size();
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

	union BoxInfoByte {
		char byte;
		struct {
			bool navigable : 1,
					obstacle : 1,
					unknown : 1;
		} info;
	};

	void BoxMap::save(std::string filename) {
		std::ofstream ofs{filename, std::ios_base::out | std::ios_base::binary};
		char buf[sizeof(double) * 6];
		double *dvals{(double*)&buf};

		for (auto item : get_intersecting(get_map_bounding_box())) {
			Box &box = item.first;
			Coordinate bot{box.get_bottom_left()},
					top{box.get_top_right()};
			BoxInfoByte info;
			info.info.navigable = item.second.get_has_navigatable();
			info.info.obstacle = item.second.get_has_obstacle();
			info.info.unknown = item.second.get_has_unknown();
			dvals[0] = bot.get_x() / r2d2::Length::METER;
			dvals[1] = bot.get_y() / r2d2::Length::METER;
			dvals[2] = bot.get_z() / r2d2::Length::METER;
			dvals[3] = top.get_x() / r2d2::Length::METER;
			dvals[4] = top.get_y() / r2d2::Length::METER;
			dvals[5] = top.get_z() / r2d2::Length::METER;
			ofs.write(buf, sizeof(buf)) << info.byte;
		}

		ofs.close();
	}

	void BoxMap::load(std::string filename) {
		set_box_info(get_map_bounding_box(), {false, false, false});

		// Open file
		std::ifstream ifs{filename, std::ios_base::in | std::ios_base::binary};
		char buf[sizeof(double) * 6];
		double *dvals{(double*)&buf};

		BoxInfoByte info;
		while (ifs.read(buf, sizeof(buf)) >> info.byte) {
			set_box_info({Coordinate{dvals[0] * r2d2::Length::METER,
			                         dvals[1] * r2d2::Length::METER,
			                         dvals[2] * r2d2::Length::METER},
			              Coordinate{dvals[3] * r2d2::Length::METER,
			                         dvals[4] * r2d2::Length::METER,
			                         dvals[5] * r2d2::Length::METER}},
			             {info.info.obstacle, info.info.navigable, info.info.unknown});
		}

		ifs.close();
	}

}
