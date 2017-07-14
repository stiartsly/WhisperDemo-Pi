# libraspi.so

SET(COMPILE_DEFINITIONS -Werror)

include_directories(${CMAKE_SOURCE_DIR}/../src/raspi})

set (RASPI_SOURCES
        camera.c
        matrix.c)

add_library(raspi SHARED ${RASPI_SOURCES})

set (DEP_LIBS wiringPi)

target_link_libraries(raspi ${DEP_LIBS})
