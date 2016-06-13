// ++--++
// @file BoxMap.hpp
// @date Created: <07-06-16>
// @version <1.0.0>
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

#ifndef _BOXMAP_HPP
#define _BOXMAP_HPP

#include <fstream>
#include <Box.hpp>
#include "MapInterface.hpp"

namespace r2d2 {

    class BoxMap : public SaveLoadMap {
    public:
        virtual const BoxInfo get_box_info(const Box box) override;

        virtual void save(std::string filename) override;
        virtual void load(std::string filename) override;

        //! @brief  gets the amount of boxes stored within this map
        //!         this does not take into account any other boxes that might
        //!         be used to store/access the information
        //! @return int BoxMap::map.size()
        virtual int get_map_size() const = 0;

        //! @brief  get all the boxes that intersect with a given area
        //! @parameter bounds
        //!
        //! @return an array of all the boxes intersecting with the bounds
        virtual std::vector<std::pair<Box, BoxInfo>> get_intersecting(const Box &bounds) const = 0;

        virtual std::ostream &print(std::ostream &lhs) = 0;

    private:
        //! add a box without doing any checks for overlapping
        virtual void add_box(Box box, BoxInfo info) = 0;

    };
}
#endif //_BOXMAP_HPP