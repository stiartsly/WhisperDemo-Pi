cmake_minimum_required (VERSION 3.1)
project(wmdemo)

set(proj_include_DIR ${CMAKE_SOURCE_DIR}/../deps/include)
set(proj_library_DIR ${CMAKE_SOURCE_DIR}/../deps/libs)

add_definitions(-std=c++11)
set(cmake_cxx_flag "-DDEBUG=1 -g -O0 -Wall")

link_directories(${proj_library_DIR})

add_executable(wmdemo
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

target_include_directories(wmdemo PRIVATE
               ${proj_include_DIR}
               json/json)

target_link_libraries(wmdemo
               confuse
               mp4v2
               wmcommon 
               wmcore
               wmsession)
