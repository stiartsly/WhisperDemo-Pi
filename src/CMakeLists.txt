cmake_minimum_required (VERSION 3.1)
project(twining)

set(proj_include_DIR ${CMAKE_SOURCE_DIR}/../deps/include)
set(proj_library_DIR ${CMAKE_SOURCE_DIR}/../deps/libs)

add_definitions(-std=c++11)
set(cmake_cxx_flag "-DDEBUG=1 -g -O0 -Wall")

link_directories(${proj_library_DIR})

add_executable(twining
               vlog.cpp
               cfg.cpp
               input.cpp
               gadget.cpp
               cmd.cpp
               agent.cpp
               main.cpp
               json/jsoncpp.cpp)

target_include_directories(twining PRIVATE
               ${proj_include_DIR}
               json/json)

target_link_libraries(twining 
               confuse
               wmcommon 
               wmcore)
