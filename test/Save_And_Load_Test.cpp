// ++--++
// @file BoxMap_Test.cpp
// @date Created: <07-04-16>
// @version <1.0.0>
//
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

#include "../source/include/BoxMap.hpp"
#include "../source/include/MapInterface.hpp"
#include "gtest/gtest.h"
#include "../source/include/ArrayBoxMap.hpp"
#include <iostream>
#include <random>
#include <time.h>

//! Tests saving and loading to its full extends
//! Generates a map, saves it and then loads and compares the saved map and currently loaded map with eachother if they are equal
TEST(BoxMap, saveAndLoad) {
    std::uniform_real_distribution<double> random_real(-100.0, 100.0);
    std::default_random_engine re(time(NULL));
    srand(time(NULL));
    r2d2::ArrayBoxMap bm{};
    cout << "May take a while... Please wait" << endl;
    int generate_box_count = 5;
    for (int i = 0; i < generate_box_count; i++) {
        bm.set_box_info(
                r2d2::Box{
                        r2d2::Coordinate{
                                random_real(re) * r2d2::Length::METER,
                                random_real(re) * r2d2::Length::METER,
                                random_real(re) * r2d2::Length::METER
                        },
                        r2d2::Coordinate{
                                random_real(re) * r2d2::Length::METER,
                                random_real(re) * r2d2::Length::METER,
                                random_real(re) * r2d2::Length::METER
                        }
                },
                r2d2::BoxInfo{rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0}
        );
    }
    cout << "Saving...";
    bm.save("save_and_load_test.json");

    cout << "Loading..." << endl;
    r2d2::ArrayBoxMap bm2{};
    bm2.load("save_and_load_test.json");
    int rounds = 50;
    cout << "Comparing with " << rounds << " rounds" << endl;
    while (rounds >= 0) {
        r2d2::Box temp{
            r2d2::Coordinate{
                    random_real(re) * r2d2::Length::METER,
                    random_real(re) * r2d2::Length::METER,
                    random_real(re) * r2d2::Length::METER
            },
            r2d2::Coordinate{
                    random_real(re) * r2d2::Length::METER,
                    random_real(re) * r2d2::Length::METER,
                    random_real(re) * r2d2::Length::METER
            }
        };

        ASSERT_TRUE((bm.get_box_info(temp) == bm2.get_box_info(temp)));
        --rounds;
    }
}