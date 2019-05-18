cmake_minimum_required(VERSION 3.14)
project(pasha_db_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(pasha_db_server
        Database.cpp
        Database.h
        gen.cpp
        gen.h
        main.cpp Query.cpp Query.h)
