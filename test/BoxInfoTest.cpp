// ++--++
// @file BoxInfoTest.cpp
// @date Created: <07-04-16>
// @version <1.0.0>
//
// @author Rutger de Gruijter
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

#include "../source/include/MapInterface.hpp"
#include "gtest/gtest.h"
#include "../source/include/BoxMap.hpp"

/**
* Constructor
*/
//BoxInfo(bool has_obstacle = false, bool has_unknown = false, bool has_navigatable = false);

TEST(BoxInfo, Constructor)
{
	r2d2::BoxInfo bi{};
	
}

/**
* OPERATORS
*/
//Operator==
//bool operator==(const BoxInfo rhs) const;
/*
TEST(BoxInfo, IsEqual)
{
	r2d2::BoxInfo::operator== 

}

//Operator!=
//bool operator!=(const BoxInfo rhs) const;
TEST(BoxInfo, NotEqual)
{
	
}


TEST(BoxInfo, get_has_obstacle)
{

}

TEST(BoxInfo, get_has_unknown)
{

}

TEST(BoxInfo, get_has_unknown)
{

}
*/