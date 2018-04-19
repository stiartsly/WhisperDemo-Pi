# libraspi.so

set(COMPILE_DEFINITIONS -Werror)

include_directories(
    "${PROJECT_SOURCE_DIR}/deps/include"
    "${PROJECT_SOURCE_DIR}/src/raspi"
)

add_library(raspi SHARED
    camera.c
    matrix.c
)

target_link_libraries(raspi
    wiringPi
)
