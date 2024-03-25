cmake_minimum_required(VERSION 3.27)
project(DA2324_PRJ1_G29 CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(DA2324_PRJ1_G29
        Reservoir.cpp
        Station.cpp
        Station.h
        City.cpp
        City.h
        main.cpp
        DataManager.cpp
        DataManager.h
        Graph.h
        Menu.cpp
        Menu.h
)
