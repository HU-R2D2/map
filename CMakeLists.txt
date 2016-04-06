cmake_minimum_required(VERSION 2.8.9)
project( map )
find_package (Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(GLOB SOURCES "source/src/*.cpp" "source/include/*.hpp" "source/include/*.c" "source/include/*.h" "source/include/rapidjson/*.h")
file(GLOB SOURCES_GTEST "source/src/*.cpp" "source/include/*.hpp" "test/*.cpp" "test/*.hpp")
list(REMOVE_ITEM SOURCES_GTEST ${CMAKE_CURRENT_SOURCE_DIR}/source/src/main.cpp)

#include_directories(
#        ../deps/gtest-1.7.0/include
#        ../deps/gtest-1.7.0)

#link_directories(../deps/gtest-1.7.0/src)
#ADD_LIBRARY(gtest ../deps/gtest-1.7.0/src/gtest-all.cc)
#ADD_LIBRARY(gtest_main ../deps/gtest-1.7.0/src/gtest_main.cc)

add_executable(map ${SOURCES})
#add_executable(map_test ${GTEST} ${SOURCES_GTEST})
#target_link_libraries(map_test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})