# wmdemo.so

SET(COMPILE_DEFINITIONS -Werror)

#include_directories("/opt/vc/include")
include_directories(${CMAKE_SOURCE_DIR}/../src/raspi})

#link_directories(/opt/vc/lib)

set (RASPI_SOURCES
        camera.c
        matrix.c)

add_library(wmdemo_ext SHARED ${RASPI_SOURCES})

set (DEP_LIBS wiringPi)

target_link_libraries(wmdemo_ext ${DEP_LIBS})
