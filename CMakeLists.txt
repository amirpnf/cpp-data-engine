cmake_minimum_required(VERSION 3.10)
project(DataEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(pybind11 REQUIRED)

set(COMMON_SOURCES 
    src/csv_reader.cpp 
    src/operations.cpp 
    src/pipeline.cpp
)

pybind11_add_module(cpp_data_engine src/bindings.cpp ${COMMON_SOURCES})
target_include_directories(cpp_data_engine PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(data_engine src/main.cpp ${COMMON_SOURCES})
target_include_directories(data_engine PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(Threads REQUIRED)
target_link_libraries(data_engine PRIVATE Threads::Threads)