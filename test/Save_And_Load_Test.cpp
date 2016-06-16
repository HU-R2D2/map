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

#include "../source/include/ArrayBoxMap.hpp"
#include "../source/include/RStarMap.hpp"
#include <gtest/gtest.h>
#include <random>

//! Tests saving and loading to its full extends
//! Generates a map, saves it and then loads and compares the saved map and currently loaded map with eachother if they are equal
template<typename T>
void save_load_test() {
    std::uniform_real_distribution<double> random_real(-100.0, 100.0);
    std::default_random_engine re((unsigned int)(time(NULL)));
    srand((unsigned int)(time(NULL)));
    T bm{};
    std::cout << "May take a while... Please wait" << std::endl;
    int generate_box_count = 10;
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
    std::cout << "Saving...";
    bm.save("save_and_load_test.map");

    std::cout << "Loading..." << std::endl;
    T bm2{};
    bm2.load("save_and_load_test.map");
    int rounds = 50;
    std::cout << "Comparing with " << rounds << " rounds" << std::endl;
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

        ASSERT_EQ(bm.get_box_info(temp), bm2.get_box_info(temp));
        --rounds;
    }
    ASSERT_EQ(bm.get_map_size(), bm2.get_map_size());
}

TEST(ArrayBoxMap, SaveLoad) {
    save_load_test<r2d2::ArrayBoxMap>();
}

TEST(RStarMap, SaveLoad) {
    save_load_test<r2d2::RStarMap>();
}