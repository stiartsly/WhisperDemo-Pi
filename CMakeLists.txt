cmake_minimum_required(VERSION 3.7)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(wdemo)

option(WDEMO_INSTALL "enable to install progoram" OFF)
option(ENABLE_PI "enable run on raspi" OFF)

set(dist_targets wdemo)

if (${ENABLE_PI})
    add_subdirectory(src/raspi)
endif()

add_subdirectory(src)
