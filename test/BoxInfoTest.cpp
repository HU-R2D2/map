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

#include "../source/include/MapInterface.hpp"
#include "../source/include/BoxMap.hpp"
#include "gtest/gtest.h"

/**
* Constructor
*/
//BoxInfo(bool has_obstacle = false, bool has_unknown = false, bool has_navigatable = false);

TEST(BoxInfo, Constructor) {
	r2d2::BoxInfo b1{false, false, false};
	r2d2::BoxInfo b2(true, true, true);
	ASSERT_FALSE(b1.get_has_obstacle() || b1.get_has_unknown() || b1.get_has_navigatable()) << "failed, should return false";
	ASSERT_TRUE(b2.get_has_obstacle() && b2.get_has_unknown() && b2.get_has_navigatable()) << "failed, should return true";
}

/**
* OPERATORS
*/
//Operator==
//bool operator==(const BoxInfo rhs) const;

TEST(BoxInfo, IsEqual) {
	r2d2::BoxInfo b1{ true, true, true };
	r2d2::BoxInfo b2{ true, true, true };
	ASSERT_TRUE(b1 == b2) << "failed, should return true but did not";
}

//Operator!=
//bool operator!=(const BoxInfo rhs) const;
TEST(BoxInfo, NotEqual) {
	r2d2::BoxInfo b1{ true, true, true };
	r2d2::BoxInfo b2{ false, false, false };
	ASSERT_TRUE(b1 != b2);
	
}

//has_navigatable getter
TEST(BoxInfo, GetHasNavigatable1) {
	r2d2::BoxInfo ghn{};
	ASSERT_FALSE(ghn.get_has_navigatable()) << "failed, should return false";
	r2d2::BoxInfo ghn2{true, true, true};
	ASSERT_TRUE(ghn2.get_has_navigatable()) << "failed, should return true";
}

//has_obstacle getter
TEST(BoxInfo, GetHasObstacle1) {
	r2d2::BoxInfo gho{};
	ASSERT_FALSE(gho.get_has_obstacle()) << "failed, should return false";
	r2d2::BoxInfo gho2{true, true, true};
	ASSERT_TRUE(gho2.get_has_obstacle()) << "failed, should return true";
}

//has_unknown getter
TEST(BoxInfo, GetHasUnknown1) {
	r2d2::BoxInfo ghu{};
	ASSERT_FALSE(ghu.get_has_unknown()) << "failed, should return false";
	r2d2::BoxInfo ghu2{true, true, true};
	ASSERT_TRUE(ghu2.get_has_unknown()) << "failed, should return true";
}
