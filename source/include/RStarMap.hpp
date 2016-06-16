// ++--++
// @file RStarMap.hpp
// @date Created: <08-05-16>
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

#ifndef MAP_RSTARMAP_HPP
#define MAP_RSTARMAP_HPP

#include "MapInterface.hpp"
#include "RTreeRoot.hpp"
#include "BoxMap.hpp"

namespace r2d2 {
    //! the minimum amount of nodes a branch can have
    #define MIN_NODES 3
	//! the maximum amount of nodes a branch can have
    #define MAX_NODES 8
    typedef RTreeRoot<MIN_NODES, MAX_NODES, const BoxInfo> root_type;
    typedef RTree<MIN_NODES, MAX_NODES, const BoxInfo> node_type;

    //! implementation of an R-tree
    //!
    //! the structure is described in it's corresponding paper: http://pages.cs.wisc.edu/~cs764-1/rtree.pdf
    class RStarMap : public BoxMap {
    public:
        RStarMap();

        virtual const Box get_map_bounding_box() override;

        virtual void set_box_info(const Box box, const BoxInfo box_info) override;

        virtual int get_map_size() const override;

        virtual std::vector<std::pair<Box, BoxInfo>> get_intersecting(const Box &bounds) const override;

        virtual std::ostream &print(std::ostream &lhs) {
            return lhs << *map;
        }

    private:
        virtual void add_box(Box box, BoxInfo info) override;

        //! insert a leaf into the map
        void insert(std::shared_ptr<node_type> node);

        std::shared_ptr<root_type> map;

    };
}

#endif //MAP_RSTARMAP_HPP
