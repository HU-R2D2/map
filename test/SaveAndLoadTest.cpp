#include "../source/include/BoxMap.hpp"
#include "../source/include/MapInterface.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <random>
#include <time.h>

TEST(BoxMap, saveAndLoad) {
    std::uniform_real_distribution<double> random_real(-100.0, 100.0);
    std::default_random_engine re(time(NULL));
    srand(time(NULL));
    r2d2::BoxMap bm{};
    cout << "May take a while... Please wait" << endl;
    int generate_box_count = 10;
    for (int i = 0; i < generate_box_count; i++) {
        bm.set_box_info(
            Box{
        Coordinate{
            random_real(re) * Length::METER,
            random_real(re) * Length::METER,
            random_real(re) * Length::METER
        },
        Coordinate{
            random_real(re) * Length::METER,
            random_real(re) * Length::METER,
            random_real(re) * Length::METER
        }
    },
        r2d2::BoxInfo{ rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0 }
    );
    }
    cout << "Saving...";
    bm.save("save_and_load_test");


    cout << "Loading..." << endl;
    r2d2::BoxMap bm2{};
    bm2.load("save_and_load_test");

    int rounds = 50;
    cout << "Comparing with " << rounds << "rounds";

    while (rounds >= 0) {
        Box temp{
        Coordinate{
            random_real(re) * Length::METER,
            random_real(re) * Length::METER,
            random_real(re) * Length::METER
        },
        Coordinate{
            random_real(re) * Length::METER,
            random_real(re) * Length::METER,
            random_real(re) * Length::METER
        }
    };

        ASSERT_TRUE((bm.get_box_info(temp) == bm2.get_box_info(temp)));
        --rounds;
    }
}