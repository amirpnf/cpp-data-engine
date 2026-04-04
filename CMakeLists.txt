cmake_minimum_required(VERSION 3.10)
project(DataEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(data_engine
    src/main.cpp
    src/csv_reader.cpp
)