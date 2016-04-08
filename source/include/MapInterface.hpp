// ++--++
// @file MapInterface.hpp
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

#ifndef _MAPINTERFACE_HPP
#define _MAPINTERFACE_HPP

#include <vector>
#include <memory>

#include "Box.hpp"

namespace r2d2
{
    class BoxInfo
    {

    public:
        BoxInfo(
            bool has_obstacle = false,
            bool has_unknown = false, 
            bool has_navigatable = false
        );

        bool get_has_obstacle() const;
        bool get_has_unknown() const;
        bool get_has_navigatable() const;

        bool operator==(const BoxInfo rhs) const;
        bool operator!=(const BoxInfo rhs) const;

    private:
        bool has_obstacle;
        bool has_unknown;
        bool has_navigatable;

    };

    class ReadOnlyMap
    {

    public:
        virtual const BoxInfo get_box_info(const Box box) = 0;
        virtual const Box get_map_bounding_box() = 0;
    };

    class ReadWriteMap : public ReadOnlyMap
    {

    public:
        virtual void set_box_info(const Box box, const BoxInfo box_info) = 0;
    };

    class SaveLoadMap : public ReadWriteMap
    {

    public:
        virtual void save(std::string filename) = 0;
        virtual void load(std::string filename) = 0;
    };
}

#endif //_MAPINTERFACE_HPP