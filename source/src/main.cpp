// ++--++
// @file main.cpp
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

#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>
#include "../include/MapInterface.hpp"
#include "../include/BoxMap.hpp"

Box random_box(){
    return Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } };
}

//! [map_interface_example]
void make_map_interface(){
    r2d2::ReadWriteMap * map = new r2d2::BoxMap{};
    map->set_box_info(random_box(), r2d2::BoxInfo{});
    cout << map->get_map_bounding_box() << endl;
    delete map;
}
//! [map_interface_example]

int main(int argc, char ** argv){
    Length l1 = 200.43 * Length::METER;

    r2d2::BoxMap testMap{};
    srand(time(NULL));
    int max = 5;
    if (argc > 1){max = atoi(argv[1]); }
    for (int i = 0; i < max; i++){
        testMap.set_box_info(Box{ Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER }, Coordinate{ rand() % 100 * Length::METER, rand() % 100 * Length::METER, rand() % 100 * Length::METER } }, r2d2::BoxInfo{ rand() % 2 == 0, rand() % 2 == 0, rand() % 2 == 0 });
        //cout << "\n" << testMap.get_map_size() << "\n";
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
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