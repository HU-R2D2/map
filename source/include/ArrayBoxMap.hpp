// ++--++
// @file BoxMap.hpp
// @date Created: <07-04-16>
// @version <1.1.0>
//
// @author Sander Kolman
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


#ifndef MAP_ARRAYBOXMAP_HPP
#define MAP_ARRAYBOXMAP_HPP

#include "BoxMap.hpp"
#include <ostream>
#include <string>
#include <stdlib.h>

namespace r2d2 {

	//! @author     Anas Shehata
	//! @date       22-04-16
	//! @version    1.0
	//! @brief      ArrayBoxMap is an implementation of BoxMap. a Box map type which retrieves and saves information about a certain area in a 3D Box map.
	//!             The implementation works by putting all the boxes in a giant array, and then looping over it to check collision
	class ArrayBoxMap : public BoxMap {
	public:
		virtual const Box get_map_bounding_box() override;

		virtual void set_box_info(const Box box, const BoxInfo box_info) override;

		virtual int get_map_size() const override;

		virtual std::vector<std::pair<Box, BoxInfo>> get_intersecting(const Box &bounds) const override;

	private:
		virtual void add_box(Box box, BoxInfo info) override;

		//! stores pairs of boxes with their corresponding boxinfos
		std::vector<std::pair<Box, BoxInfo>> map;

	};

}

#endif //MAP_ARRAYBOXMAP_HPP
