#include "../source/include/BoxMap.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <random>
#include <time.h>

/*
* Constructor
*/
TEST(BoxMap, DefaultConstructor){
    r2d2::BoxMap bm{};
    ASSERT_EQ(bm.get_map_size(), 0);
}

/*
* BoxMap::set_box_info() test
*/
TEST(BoxMap, AddBox){
    r2d2::BoxMap bm{};
    bm.set_box_info(Box{}, r2d2::BoxInfo{});
    ASSERT_EQ(bm.get_map_size(), 1);
}

/*
* BoxMap::get_box_info() test
*/
TEST(BoxMap, GetBoxInfo1){
    srand(time(NULL));
    r2d2::BoxMap bm{};
    for (int i = 0; i < 5; i++){

        r2d2::BoxInfo temp{
            rand() % 2 == 0,
            rand() % 2 == 0,
            rand() % 2 == 0
        };

        bm.set_box_info(
            Box{
            Coordinate{},
            Coordinate{
                2 * Length::METER,
                2 * Length::METER,
                2 * Length::METER
            }
        }, temp);

        ASSERT_EQ(
            bm.get_box_info(
            Box{
            Coordinate{
                0.5*Length::METER,
                0.5*Length::METER,
                0.5*Length::METER
            },
            Coordinate{
                    1.5*Length::METER,
                    1.5*Length::METER,
                    1.5*Length::METER
                }
        }
        ),
        temp
        );
    }
}


/*
* BoxMap::get_box_info() test
*/
TEST(BoxMap, GetBoxInfo2){
    srand(time(NULL));
    r2d2::BoxMap bm{};

    bm.set_box_info(
        Box{
        Coordinate{
            rand() % 20 * Length::METER,
            rand() % 20 * Length::METER,
            rand() % 20 * Length::METER
        },
        Coordinate{
                rand() % 20 * Length::METER,
                rand() % 20 * Length::METER,
                rand() % 20 * Length::METER
            }
    },
    r2d2::BoxInfo{ false, true, false }
                );

    bm.set_box_info(
        Box{
        Coordinate{
            rand() % 20 * Length::METER,
            rand() % 20 * Length::METER,
            rand() % 20 * Length::METER
        },
        Coordinate{
                rand() % 20 * Length::METER,
                rand() % 20 * Length::METER,
                rand() % 20 * Length::METER
            }
    },
    r2d2::BoxInfo{ false, false, true }
                );

    ASSERT_EQ(
        bm.get_box_info(
        Box{
        Coordinate{
            0 * Length::METER,
            0 * Length::METER,
            0 * Length::METER
        },
        Coordinate{
                20 * Length::METER,
                20 * Length::METER,
                20 * Length::METER
            }
    }
    ),
    (r2d2::BoxInfo{ false, true, true })
    );
}

/*
* BoxMap::get_bounding_box() test
*/
TEST(BoxMap, BoundingBox){
    r2d2::BoxMap bm{};

    bm.set_box_info(
        Box{
        Coordinate{
            10 * Length::METER,
            10 * Length::METER,
            10 * Length::METER
        },
        Coordinate{
                20 * Length::METER,
                20 * Length::METER,
                20 * Length::METER
            }
    },
    r2d2::BoxInfo{ false, true, true }
                );

    bm.set_box_info(
        Box{
        Coordinate{
            15 * Length::METER,
            15 * Length::METER,
            15 * Length::METER
        },
        Coordinate{
                60 * Length::METER,
                60 * Length::METER,
                60 * Length::METER
            }
    },
    r2d2::BoxInfo{ false, false, true }
                );

    Box bounding = bm.get_map_bounding_box();

    ASSERT_TRUE(
        (bounding.get_bottom_left().get_x() / Length::METER == 10) &&
        (bounding.get_bottom_left().get_y() / Length::METER == 10) &&
        (bounding.get_bottom_left().get_z() / Length::METER == 10) &&
        (bounding.get_top_right().get_x() / Length::METER == 60) &&
        (bounding.get_top_right().get_y() / Length::METER == 60) &&
        (bounding.get_top_right().get_z() / Length::METER == 60)
        );
}

/*
* Real world test / stress test
*/
TEST(BoxMap, UsageExample){
    std::uniform_real_distribution<double> random_real(-100.0, 100.0);
    std::default_random_engine re(time(NULL));
    srand(time(NULL));
    r2d2::BoxMap bm{};
    cout << "May take a minute or 2...\n";

    for (int i = 0; i < 50; i++){
        bm.set_box_info(
            Box{
            Coordinate{
                random_real(re)*Length::METER,
                random_real(re)*Length::METER,
                random_real(re)*Length::METER
            },
            Coordinate{
                    random_real(re)*Length::METER,
                    random_real(re)*Length::METER,
                    random_real(re)*Length::METER
                }
        },
        r2d2::BoxInfo{ rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0 }
                    );
    }
    ASSERT_GT(bm.get_map_size(), 9);
    ASSERT_TRUE(
        (bm.get_box_info(bm.get_map_bounding_box())
        == r2d2::BoxInfo{ true, true, true })
        );

    Box bounding = bm.get_map_bounding_box();
    ASSERT_TRUE(
        (bounding.get_bottom_left().get_x() / Length::METER < 0) &&
        (bounding.get_bottom_left().get_y() / Length::METER < 0) &&
        (bounding.get_bottom_left().get_z() / Length::METER < 0) &&
        (bounding.get_top_right().get_x() / Length::METER > 0) &&
        (bounding.get_top_right().get_y() / Length::METER > 0) &&
        (bounding.get_top_right().get_z() / Length::METER > 0)
        );
}