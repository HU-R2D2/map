
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include "../include/MapInterface.hpp"
#include "../include/BoxMap.hpp"

int main(int argc, char ** argv){
    r2d2::BoxMap testMap{};

    for (int i = 0; i < 2; i++){
        testMap.set_box_info(Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } }, r2d2::BoxInfo{ rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0 });
        cout << "\n" << testMap.get_map_size() << "\n";
    }

    cout << testMap.get_map_size() << "\n";
    for (int i = 0; i < 0; i++){
        testMap.get_box_info(Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } });
        cout <<"\n"<<testMap.get_map_size()<< "\n";
    }

    return 0;
}