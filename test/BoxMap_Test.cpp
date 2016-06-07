// ++--++
// @file BoxMap_Test.cpp
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

#include "../source/include/ArrayBoxMap.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <random>
#include <time.h>

/*
* Constructor
*/
TEST(BoxMap, DefaultConstructor){
    r2d2::ArrayBoxMap bm{};
    ASSERT_EQ(0, bm.get_map_size());
}

/*
* BoxMap::set_box_info() test
*/
TEST(BoxMap, AddBox){
    r2d2::ArrayBoxMap bm{};
    bm.set_box_info(r2d2::Box{}, r2d2::BoxInfo{true, false, false});
    ASSERT_EQ(1, bm.get_map_size());
}

/*
* BoxMap::get_box_info() test
*/
TEST(BoxMap, GetBoxInfo1){
    srand(time(NULL));
    r2d2::ArrayBoxMap bm{};
    for (int i = 0; i < 8; i++){

        r2d2::BoxInfo temp{
            i % 2 == 0,
            (i / 2) % 2 == 0,
            (i / 4) % 2 == 0
        };

        bm.set_box_info(
            r2d2::Box{
            r2d2::Coordinate{},
            r2d2::Coordinate{
                2 * r2d2::Length::METER,
                2 * r2d2::Length::METER,
                2 * r2d2::Length::METER
            }
        }, temp);

        if (!(temp.get_has_obstacle() || temp.get_has_navigatable())) {
            temp = {temp.get_has_obstacle(), temp.get_has_navigatable(), true};
        }

        ASSERT_EQ(temp,
            bm.get_box_info(
            r2d2::Box{
            r2d2::Coordinate{
                0.5*r2d2::Length::METER,
                0.5*r2d2::Length::METER,
                0.5*r2d2::Length::METER
            },
            r2d2::Coordinate{
                    1.5*r2d2::Length::METER,
                    1.5*r2d2::Length::METER,
                    1.5*r2d2::Length::METER
                }
        }));
    }
}


/*
* BoxMap::get_box_info() test
*/
TEST(BoxMap, GetBoxInfo2){
    srand(time(NULL));
    r2d2::ArrayBoxMap bm{};

    bm.set_box_info(
        r2d2::Box{
        r2d2::Coordinate{
            rand() % 20 * r2d2::Length::METER,
            rand() % 20 * r2d2::Length::METER,
            rand() % 20 * r2d2::Length::METER
        },
        r2d2::Coordinate{
                rand() % 20 * r2d2::Length::METER,
                rand() % 20 * r2d2::Length::METER,
                rand() % 20 * r2d2::Length::METER
            }
    },
    r2d2::BoxInfo{ false, true, false }
                );

    bm.set_box_info(
        r2d2::Box{
        r2d2::Coordinate{
            rand() % 20 * r2d2::Length::METER,
            rand() % 20 * r2d2::Length::METER,
            rand() % 20 * r2d2::Length::METER
        },
        r2d2::Coordinate{
                rand() % 20 * r2d2::Length::METER,
                rand() % 20 * r2d2::Length::METER,
                rand() % 20 * r2d2::Length::METER
            }
    },
    r2d2::BoxInfo{ false, false, true }
                );

    ASSERT_EQ((r2d2::BoxInfo{ false, true, true }),
              bm.get_box_info(
                      r2d2::Box{
                              r2d2::Coordinate{
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER
                              },
                              r2d2::Coordinate{
                                      20 * r2d2::Length::METER,
                                      20 * r2d2::Length::METER,
                                      20 * r2d2::Length::METER
                              }
                      }
              ));
}

/*
* BoxMap::get_bounding_box() test
*/
TEST(BoxMap, BoundingBox){
    r2d2::ArrayBoxMap bm{};

    bm.set_box_info(
        r2d2::Box{
        r2d2::Coordinate{
            10 * r2d2::Length::METER,
            10 * r2d2::Length::METER,
            10 * r2d2::Length::METER
        },
        r2d2::Coordinate{
                20 * r2d2::Length::METER,
                20 * r2d2::Length::METER,
                20 * r2d2::Length::METER
            }
    },
    r2d2::BoxInfo{ false, true, true }
                );

    bm.set_box_info(
        r2d2::Box{
        r2d2::Coordinate{
            15 * r2d2::Length::METER,
            15 * r2d2::Length::METER,
            15 * r2d2::Length::METER
        },
        r2d2::Coordinate{
                60 * r2d2::Length::METER,
                60 * r2d2::Length::METER,
                60 * r2d2::Length::METER
            }
    },
    r2d2::BoxInfo{ false, false, true }
                );

    r2d2::Box bounding = bm.get_map_bounding_box();

    ASSERT_TRUE(
        (bounding.get_bottom_left().get_x() / r2d2::Length::METER == 10) &&
        (bounding.get_bottom_left().get_y() / r2d2::Length::METER == 10) &&
        (bounding.get_bottom_left().get_z() / r2d2::Length::METER == 10) &&
        (bounding.get_top_right().get_x() / r2d2::Length::METER == 60) &&
        (bounding.get_top_right().get_y() / r2d2::Length::METER == 60) &&
        (bounding.get_top_right().get_z() / r2d2::Length::METER == 60)
        );
}

/*
* Real world test / stress test
*/
TEST(BoxMap, UsageExample){
    r2d2::ArrayBoxMap bm{};
    r2d2::Box box1 =  r2d2::Box{
            r2d2::Coordinate{
                    -83.9178867229331 *r2d2::Length::METER,
                    -50.005615540355247 *r2d2::Length::METER,
                    52.06871684497102 *r2d2::Length::METER
            },
            r2d2::Coordinate{
                    32.52173379550348*r2d2::Length::METER,
                    80.63101146674475*r2d2::Length::METER,
                    57.487075248474159*r2d2::Length::METER
            }
    };

    r2d2::Box box2 =  r2d2::Box{
            r2d2::Coordinate{
                    85*r2d2::Length::METER,
                    55*r2d2::Length::METER,
                    50*r2d2::Length::METER
            },
            r2d2::Coordinate{
                    32*r2d2::Length::METER,
                    80*r2d2::Length::METER,
                    57*r2d2::Length::METER
            }
    };

    ASSERT_TRUE(box1.intersects(box2));

    bm.set_box_info(box1, r2d2::BoxInfo{ false, false, false} );

    r2d2::BoxInfo gBox1 = bm.get_box_info(box1);

    ASSERT_TRUE((!gBox1.get_has_navigatable()) &&
                (!gBox1.get_has_obstacle()));

    bm.set_box_info(box2, r2d2::BoxInfo{ true, true, false} );
    r2d2::BoxInfo gBox2 = bm.get_box_info(box2);

    ASSERT_TRUE((gBox2.get_has_navigatable()) &&
                (gBox2.get_has_obstacle()));

}