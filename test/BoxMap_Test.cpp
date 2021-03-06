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
#include "../source/include/RStarMap.hpp"
#include "../source/include/DefaultBoxMap.hpp"
#include <gtest/gtest.h>

/*
* Constructor
*/
void default_constructor_test(r2d2::BoxMap &bm) {
    ASSERT_EQ(0, bm.get_map_size());
}

TEST(ArrayBoxMap, DefaultConstructor) {
    r2d2::ArrayBoxMap bm{};
    default_constructor_test(bm);
}

TEST(RStarMap, DefaultConstructor) {
    r2d2::RStarMap bm{};
    default_constructor_test(bm);
}

/*
* ArrayBoxMap::set_box_info() / ArrayBoxMap::get_map_size() test
*/
void add_box_test(r2d2::BoxMap &bm) {
    bm.set_box_info(r2d2::Box{r2d2::Coordinate{-1 * r2d2::Length::METER,
                                               -1 * r2d2::Length::METER,
                                               -1 * r2d2::Length::METER},
                              r2d2::Coordinate{1 * r2d2::Length::METER,
                                               1 * r2d2::Length::METER,
                                               1 * r2d2::Length::METER}},
                    r2d2::BoxInfo{true, false, false});
    ASSERT_EQ(1, bm.get_map_size());
}

TEST(ArrayBoxMap, AddBox) {
    r2d2::ArrayBoxMap bm{};
    add_box_test(bm);
}

TEST(RStarMap, AddBox) {
    r2d2::RStarMap bm{};
    add_box_test(bm);
}

/*
* ArrayBoxMap::get_box_info() test, automatic loop over possible values
*/
void get_boxinfo_1_test(r2d2::BoxMap &bm) {
    for (int i = 0; i < 8; i++) {

        r2d2::BoxInfo temp{
                i % 2 == 0,
                (i / 2) % 2 == 0,
                (i / 4) % 2 == 0
        };

        bm.set_box_info(
                r2d2::Box{
                        r2d2::Coordinate{
                                0 * r2d2::Length::METER,
                                1 * r2d2::Length::METER,
                                2 * r2d2::Length::METER
                        },
                        r2d2::Coordinate{
                                2 * r2d2::Length::METER,
                                3 * r2d2::Length::METER,
                                4 * r2d2::Length::METER
                        }
                }, temp
        );

        // if the temp has neither obstacle or navigable, then set unknown to true
        if (!(temp.get_has_obstacle() || temp.get_has_navigatable())) {
            temp = {false, false, true};
        }

        ASSERT_EQ(temp,
                  bm.get_box_info(
                          r2d2::Box{
                                  r2d2::Coordinate{
                                          0.5 * r2d2::Length::METER,
                                          1.5 * r2d2::Length::METER,
                                          2.5 * r2d2::Length::METER
                                  },
                                  r2d2::Coordinate{
                                          1.5 * r2d2::Length::METER,
                                          2.5 * r2d2::Length::METER,
                                          3.5 * r2d2::Length::METER
                                  }
                          }));
    }
}

TEST(ArrayBoxMap, GetBoxInfo1) {
    r2d2::ArrayBoxMap bm{};
    get_boxinfo_1_test(bm);
}

TEST(RStarMap, GetBoxInfo1) {
    r2d2::RStarMap bm{};
    get_boxinfo_1_test(bm);
}

/*
* ArrayBoxMap::get_box_info() test, manual creation
*/
void get_boxinfo_2_test(r2d2::BoxMap &bm) {
    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            0 * r2d2::Length::METER,
                            1 * r2d2::Length::METER,
                            2 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            15 * r2d2::Length::METER,
                            16 * r2d2::Length::METER,
                            17 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{true, false, false}
    );

    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            5 * r2d2::Length::METER,
                            6 * r2d2::Length::METER,
                            7 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            20 * r2d2::Length::METER,
                            21 * r2d2::Length::METER,
                            22 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{false, true, false}
    );

    ASSERT_EQ((r2d2::BoxInfo{true, true, true}),
              bm.get_box_info(
                      r2d2::Box{
                              r2d2::Coordinate{
                                      0 * r2d2::Length::METER,
                                      1 * r2d2::Length::METER,
                                      2 * r2d2::Length::METER
                              },
                              r2d2::Coordinate{
                                      20 * r2d2::Length::METER,
                                      21 * r2d2::Length::METER,
                                      22 * r2d2::Length::METER
                              }
                      }
              ));
}

TEST(ArrayBoxMap, GetBoxInfo2) {
    r2d2::ArrayBoxMap bm{};
    get_boxinfo_2_test(bm);
}

TEST(RStarMap, GetBoxInfo2) {
    r2d2::RStarMap bm{};
    get_boxinfo_2_test(bm);
}

/*
* ArrayBoxMap::get_bounding_box() test
*/
void bounding_box_test(r2d2::BoxMap &bm) {
    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            10 * r2d2::Length::METER,
                            11 * r2d2::Length::METER,
                            12 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            20 * r2d2::Length::METER,
                            21 * r2d2::Length::METER,
                            22 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{false, true, true}
    );

    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            15 * r2d2::Length::METER,
                            16 * r2d2::Length::METER,
                            17 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            60 * r2d2::Length::METER,
                            61 * r2d2::Length::METER,
                            62 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{true, false, true}
    );

    r2d2::Box bounding = bm.get_map_bounding_box();

    ASSERT_TRUE(
            (bounding.get_bottom_left().get_x() / r2d2::Length::METER == 10) &&
            (bounding.get_bottom_left().get_y() / r2d2::Length::METER == 11) &&
            (bounding.get_bottom_left().get_z() / r2d2::Length::METER == 12) &&
            (bounding.get_top_right().get_x() / r2d2::Length::METER == 60) &&
            (bounding.get_top_right().get_y() / r2d2::Length::METER == 61) &&
            (bounding.get_top_right().get_z() / r2d2::Length::METER == 62)
    );
}

TEST(ArrayBoxMap, BoundingBox) {
    r2d2::ArrayBoxMap bm{};
    bounding_box_test(bm);
}

TEST(RStarMap, BoundingBox) {
    r2d2::RStarMap bm{};
    bounding_box_test(bm);
}

/**
 * test to test that getting information boxes of volume
 * zero will still work correctly
 */
void zero_size_test(r2d2::BoxMap &bm) {
    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            10 * r2d2::Length::METER,
                            11 * r2d2::Length::METER,
                            0 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            20 * r2d2::Length::METER,
                            21 * r2d2::Length::METER,
                            0 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{true, true, false}
    );
    ASSERT_EQ((r2d2::BoxInfo{true, true, true}),
              (bm.get_box_info(
                      r2d2::Box{
                              r2d2::Coordinate{
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER
                              }, r2d2::Coordinate{
                                      30 * r2d2::Length::METER,
                                      30 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER
                              }
                      }
              )));
    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            10 * r2d2::Length::METER,
                            11 * r2d2::Length::METER,
                            0 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            20 * r2d2::Length::METER,
                            21 * r2d2::Length::METER,
                            0 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{false, true, false}
    );
    ASSERT_EQ((r2d2::BoxInfo{false, true, true}),
              (bm.get_box_info(
                      r2d2::Box{
                              r2d2::Coordinate{
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER
                              }, r2d2::Coordinate{
                                      30 * r2d2::Length::METER,
                                      30 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER
                              }
                      }
              )));
    bm.set_box_info(
            r2d2::Box{
                    r2d2::Coordinate{
                            10 * r2d2::Length::METER,
                            11 * r2d2::Length::METER,
                            0 * r2d2::Length::METER
                    },
                    r2d2::Coordinate{
                            20 * r2d2::Length::METER,
                            21 * r2d2::Length::METER,
                            1 * r2d2::Length::METER
                    }
            },
            r2d2::BoxInfo{false, true, false}
    );
    ASSERT_EQ((r2d2::BoxInfo{false, true, true}),
              (bm.get_box_info(
                      r2d2::Box{
                              r2d2::Coordinate{
                                      0 * r2d2::Length::METER,
                                      0 * r2d2::Length::METER,
                                      0.5 * r2d2::Length::METER
                              }, r2d2::Coordinate{
                                      30 * r2d2::Length::METER,
                                      30 * r2d2::Length::METER,
                                      0.5 * r2d2::Length::METER
                              }
                      }
              )));
}

TEST(ArrayBoxMap, ZeroSizeUnknown) {
    r2d2::ArrayBoxMap bm{};
    zero_size_test(bm);
}

TEST(RStarMap, ZeroSizeUnknown) {
    r2d2::RStarMap bm{};
    zero_size_test(bm);
}

// defines the size of the grid of squares that will be inserted in the stress test
#define MAP_TEST_SIZE 100
/**
 * stress test to see how performant the code is, also used to verify that the
 * map will have the correct amount of boxes, and return a mixed boxinfo result
 */
void stress_test(r2d2::BoxMap &bm) {
    for (int y = 0; y < MAP_TEST_SIZE; ++y) {
        for (int x = 0; x < MAP_TEST_SIZE; ++x) {
            r2d2::Coordinate new_pos{
                    x * r2d2::Length::METER,
                    y * r2d2::Length::METER,
                    0 * r2d2::Length::METER
            };

            r2d2::BoxInfo info{
                    rand() % 2 == 0,
                    rand() % 2 == 0,
                    rand() % 2 == 0};

            bm.set_box_info(
                    r2d2::Box{
                            new_pos,
                            new_pos
                            + r2d2::Translation{
                                    2 * r2d2::Length::METER,
                                    2 * r2d2::Length::METER,
                                    0 * r2d2::Length::METER
                            }
                    },
                    info.get_has_obstacle() || info.get_has_navigatable() ? info : r2d2::BoxInfo{
                            (x ^ y) % 2 == 0,
                            (x ^ y) % 2 == 1,
                            info.get_has_unknown()
                    }
            );
        }
    }

    ASSERT_EQ(((MAP_TEST_SIZE + 1) * MAP_TEST_SIZE) - 1, bm.get_map_size());
    ASSERT_TRUE(
            (bm.get_box_info(bm.get_map_bounding_box())
             == r2d2::BoxInfo{true, true, true})
    );
}

TEST(RStarMap, PerformanceTest) {
    r2d2::RStarMap bm{};
    stress_test(bm);
}

TEST(ArrayBoxMap, PerformanceTest) {
    r2d2::ArrayBoxMap bm{};
    stress_test(bm);
}

/*
* Real world test
*/
void usage_example_test(r2d2::BoxMap &bm) {
    r2d2::Box box1 = r2d2::Box{
            r2d2::Coordinate{
                    -83.9178867229331 * r2d2::Length::METER,
                    -50.005615540355247 * r2d2::Length::METER,
                    52.06871684497102 * r2d2::Length::METER
            },
            r2d2::Coordinate{
                    32.52173379550348 * r2d2::Length::METER,
                    80.63101146674475 * r2d2::Length::METER,
                    57.487075248474159 * r2d2::Length::METER
            }
    };

    r2d2::Box box2 = r2d2::Box{
            r2d2::Coordinate{
                    85 * r2d2::Length::METER,
                    55 * r2d2::Length::METER,
                    50 * r2d2::Length::METER
            },
            r2d2::Coordinate{
                    32 * r2d2::Length::METER,
                    80 * r2d2::Length::METER,
                    57 * r2d2::Length::METER
            }
    };

    ASSERT_TRUE(box1.intersects(box2));

    bm.set_box_info(box1, r2d2::BoxInfo{false, false, false});

    r2d2::BoxInfo gBox1 = bm.get_box_info(box1);

    ASSERT_TRUE((!gBox1.get_has_navigatable()) &&
                (!gBox1.get_has_obstacle()));

    bm.set_box_info(box2, r2d2::BoxInfo{true, true, false});
    r2d2::BoxInfo gBox2 = bm.get_box_info(box2);

    ASSERT_TRUE((gBox2.get_has_navigatable()) &&
                (gBox2.get_has_obstacle()));
}

TEST(ArrayBoxMap, UsageExample) {
    r2d2::ArrayBoxMap bm{};
    usage_example_test(bm);
}

TEST(RStarMap, UsageExample) {
    r2d2::RStarMap bm{};
    usage_example_test(bm);
}