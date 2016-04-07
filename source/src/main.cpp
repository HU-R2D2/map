
#include <iostream>
#include <string>
#include "../include/MapInterface.hpp"
#include "../include/BoxMap.hpp"

int main(int argc, char ** argv){
    Length l1 = 200.43 * Length::METER;
    r2d2::BoxMap bm1{};

    bm1.load("C:/Users/MaikelB/Source/Repos/map/source/src/test.json");
    bm1.save("cookies");
}