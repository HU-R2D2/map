
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>
#include "../include/MapInterface.hpp"
#include "../include/BoxMap.hpp"

int main(int argc, char ** argv){
    r2d2::BoxMap testMap{};
    srand(time(NULL));
    int max = 5;
    if (argc > 1){max = atoi(argv[1]); }
    for (int i = 0; i < max; i++){
        testMap.set_box_info(Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } }, r2d2::BoxInfo{ rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0 });
        cout << "\n" << testMap.get_map_size() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    cout << testMap.get_map_size() << "\n";
    for (int i = 0; i < 10; i++){
        cout << testMap.get_box_info(Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } }).get_has_obstacle();
        cout << "\n";// << testMap.get_map_size() << "\n";
    }
    cout << "\n" << testMap.get_map_size() << "\n";
    cout << testMap.get_map_bounding_box();
    return 0;
}