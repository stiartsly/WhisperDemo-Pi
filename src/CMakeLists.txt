cmake_minimum_required (VERSION 3.1)
project(wmdemo)

add_subdirectory(raspi)

include_directories(${CMAKE_SOURCE_DIR}/../deps/include)
include_directories(${CMAKE_SOURCE_DIR}/../src/json/json)

link_directories(${CMAKE_SOURCE_DIR}/../deps/libs)

add_definitions(-std=c++11)

set(cmake_cxx_flag "-DDEBUG=1 -g -O0 -Wall")
set(WMLIBS confuse wmcommon wmcore wmsession)

set (WMDEMO_SOURCES
        vlog.cpp
        cfg.cpp
        input.cpp
        gadget.cpp
        cmd.cpp
        agent.cpp
        session.cpp
        lock.cpp
        rwlock.cpp
        rtp.cpp
        mp4.cpp
        main.cpp
        json/jsoncpp.cpp)

add_executable(wmdemo ${WMDEMO_SOURCES})
target_link_libraries(wmdemo ${WMLIBS} mp4v2 pthread dl)
