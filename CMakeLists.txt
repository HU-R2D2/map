cmake_minimum_required(VERSION 2.8.9)

project(map)

find_package (Threads)

set(SOURCES_GTEST
		source/src/ArrayBoxMap.cpp
		source/src/MapInterface.cpp
		test/BoxInfoTest.cpp
		test/BoxMap_Test.cpp
		test/Main_TestAll.cpp
		test/Save_And_Load_Test
		../adt/source/include/ADT_Base.hpp
		../adt/source/src/Length.cpp
		../adt/source/src/Box.cpp
		../adt/source/src/Coordinate.cpp
		../adt/source/src/Translation.cpp
		source/include/RTree.hpp
		source/include/RTreeLeaf.hpp
		source/include/RTreeBranch.hpp
		source/src/RStarMap.cpp source/include/RStarMap.hpp
		source/include/RTreeRoot.hpp)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


include_directories(

  ../adt/source/include
  ../deps/gtest-1.7.0/include

  ../deps/gtest-1.7.0)



link_directories(../deps/gtest-1.7.0/src ../adt/source/src)

ADD_LIBRARY(gtest ../deps/gtest-1.7.0/src/gtest-all.cc)

ADD_LIBRARY(gtest_main ../deps/gtest-1.7.0/src/gtest_main.cc)


add_executable(map_test ${SOURCES_GTEST})

target_link_libraries(map_test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
