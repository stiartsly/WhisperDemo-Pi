# wmdemo.so

SET(COMPILE_DEFINITIONS -Werror)

include_directories("/opt/vc/include")
include_directories(${CMAKE_SOURCE_DIR}/../src/raspi})

link_directories(/opt/vc/lib)

set (RASPI_SOURCES
        RaspiCamControl.c
        RaspiCLI.c
        RaspiPreview.c
        RaspiVid.c)

add_library(wmdemo_ext SHARED ${RASPI_SOURCES})

set (MMAL_LIBS mmal_core mmal_util mmal_vc_client)

target_link_libraries(wmdemo_ext ${MMAL_LIBS} vcos bcm_host)

