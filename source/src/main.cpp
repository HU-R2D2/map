// ++--++
// @file main.cpp
// @date Created: <07-04-16>
// @version <1.0.0>
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

#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>
#include "../include/MapInterface.hpp"
#include "../include/BoxMap.hpp"

int main(int argc, char ** argv){

    r2d2::BoxMap bm1{};
    Length l1 = 201.43 * Length::METER;
    Length l2 = 202.43 * Length::METER;
    Length l3 = 203.43 * Length::METER;
    Coordinate c1{ l1, l2, l3 };
    Box boxA{};
    r2d2::BoxInfo boxAInfo{ false, false, false };
    Box boxB{c1,c1};
    r2d2::BoxInfo boxBInfo{ false, true, false };
    Box boxC{};
    r2d2::BoxInfo boxCInfo{ true, true, true };
    bm1.set_box_info(boxA, boxAInfo);
    bm1.set_box_info(boxB, boxBInfo);
    bm1.set_box_info(boxC, boxCInfo);

    //bm1.load("C:/Users/Anas/Documents/GitHub/map/source/src/test.json");
    bm1.save("C:/Users/Anas/Documents/GitHub/map/source/src/MAPPPPPPP.json");
    //std::cout << "Test";


    return 0;
}