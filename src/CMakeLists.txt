include_directories (
    "${PROJECT_SOURCE_DIR}/deps/include"
    "${PROJECT_SOURCE_DIR}/src/json/json"
    "${PROJECT_SOURCE_DIR}/src"
)

link_directories(
    "${PROJECT_SOURCE_DIR}/deps/libs"
)

add_definitions(-std=c++11)
set(cmake_cxx_flag "-DDEBUG=1 -g -O0 -Wall")

add_executable(wdemo
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
    main.cpp
    json/jsoncpp.cpp
)

target_link_libraries(wdemo
    wcommon
    wcore
    wsession
    confuse
    pthread
    dl
)

if (WDEMO_INSTALL)
    install(TARGETS wdemo DESTINATION bin)
endif()
