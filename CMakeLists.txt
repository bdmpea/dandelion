cmake_minimum_required(VERSION 3.23)
project(dandelion)

set(CMAKE_CXX_STANDARD 17)

include_directories(client/include)
include_directories(client/include/tests)
include_directories(common)
include_directories(server/include)
include_directories(server/include/tests)

add_executable(dandelion
        client/src/main.cpp
        client/tests/run_tests.cpp
        server/src/main.cpp
        server/tests/run_tests.cpp)
