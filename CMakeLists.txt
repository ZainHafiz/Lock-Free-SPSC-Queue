# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Name your project and specify the language (CXX = C++)
project(SPSCQueueBenchmark CXX)

# Force the compiler to use modern C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on maximum performance optimizations (-O3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Tell CMake to create an executable named 'benchmark' from 'main.cpp'
add_executable(benchmark main.cpp)

# Find and link the threading library (required for std::thread to work properly on Linux/Mac)
find_package(Threads REQUIRED)
target_link_libraries(benchmark PRIVATE Threads::Threads)