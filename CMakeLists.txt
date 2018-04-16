cmake_minimum_required(VERSION 3.10)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(wdemo)

option(WDEMO_INSTALL "enable to install progoram" OFF)

set(dist_targets wdemo)
#add_subdirectory(src/raspi)
add_subdirectory(src)
