cmake_minimum_required(VERSION 3.14)
project(pasha_db_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package ( Threads REQUIRED )
add_executable(pasha_db_server
        Database.cpp
        Database.h
        gen.cpp
        gen.h
        th_serv.cpp Query.cpp Query.h)
add_executable(pasha_http_server
        Database.cpp
        Database.h
        gen.cpp
        gen.h
        Query.cpp Query.h http.cpp)
add_executable(client
        client.cpp)
target_link_libraries ( pasha_db_server ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries ( pasha_http_server ${CMAKE_THREAD_LIBS_INIT})
