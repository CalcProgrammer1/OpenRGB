cmake_minimum_required(VERSION 3.0.0)

# read and parse version file
file(READ version PROJECT_VERSION)
string(STRIP ${PROJECT_VERSION} PROJECT_VERSION)
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" VERSION_MAJOR ${PROJECT_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" VERSION_MINOR ${PROJECT_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" VERSION_MICRO ${PROJECT_VERSION})

set(HEADERDEF "${VERSION_MAJOR}_${VERSION_MINOR}_${VERSION_MICRO}")

project(rpi_ws281x VERSION ${PROJECT_VERSION})

option(BUILD_SHARED "Build as shared library" OFF)
option(BUILD_TEST "Build test application" ON)

set(CMAKE_C_STANDARD 11)

set(LIB_TARGET ws2811)
set(TEST_TARGET test)

set(LIB_PUBLIC_HEADERS
    ws2811.h
    rpihw.h
    pwm.h
    clk.h
    dma.h
    gpio.h
    mailbox.h
    pcm.h
)

set(LIB_SOURCES
    mailbox.c
    ws2811.c
    pwm.c
    pcm.c
    dma.c
    rpihw.c
)

set(TEST_SOURCES
    main.c
)

include(GNUInstallDirs)

configure_file(version.h.in version.h)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libws2811.pc"
    @ONLY
)

set(DEST_HEADERS_DIR include/ws2811)
set(DEST_LIB_DIR lib)

if(BUILD_SHARED)
    add_library(${LIB_TARGET} SHARED ${LIB_SOURCES})
else()
    add_library(${LIB_TARGET} ${LIB_SOURCES})
endif()

target_link_libraries(${LIB_TARGET} m)
set_target_properties(${LIB_TARGET} PROPERTIES PUBLIC_HEADER "${LIB_PUBLIC_HEADERS}")

install(TARGETS ${LIB_TARGET}
    ARCHIVE DESTINATION ${DEST_LIB_DIR}
    PUBLIC_HEADER DESTINATION ${DEST_HEADERS_DIR}
)

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/libws2811.pc"
        DESTINATION lib/pkgconfig)

install(CODE "execute_process(COMMAND /sbin/ldconfig
                              RESULT_VARIABLE EXIT_STATUS
                              ERROR_QUIET)
             if (NOT EXIT_STATUS EQUAL 0)
                 message(\"Warning: Could not run ldconfig. You may need to manually run ldconfig as root to cache the newly installed libraries.\")
             endif()")

if(BUILD_TEST)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    add_executable(${TEST_TARGET} ${TEST_SOURCES})
    target_link_libraries(${TEST_TARGET} ${LIB_TARGET})
endif()
