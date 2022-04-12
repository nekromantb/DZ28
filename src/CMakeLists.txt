cmake_minimum_required(VERSION 3.21)

add_executable(DZ28 main.cpp task1.cpp task2.cpp task3.cpp)

target_include_directories(DZ28 PUBLIC ${CMAKE_SOURCE_DIR}/include)