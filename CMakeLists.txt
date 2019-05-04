cmake_minimum_required (VERSION 2.6)
project (cpputilities)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )

# GTest
ADD_SUBDIRECTORY (googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(testsuite testsuite.cpp src/gc.cpp)
target_link_libraries(testsuite gtest gtest_main)
add_test(testsuite testsuite)


