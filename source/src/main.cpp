
#include <iostream>
#include <string>
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

}