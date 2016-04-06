
#include <iostream>
#include <limits.h>
#include "gtest/gtest.h"
#include "../source/include/MapInterface.hpp"

int main(int argc, char ** argv){
    std::cout << "test";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}