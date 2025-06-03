#-----------------------------------------------------------#
# CMakeLists.txt                                            #
#                                                           #
#   CMake configuration for building OpenRGB                #
#                                                           #
#   Adam Honse (CalcProgrammer1)                24 May 2025 #
#                                                           #
#   This file is part of the OpenRGB project                #
#   SPDX-License-Identifier: GPL-2.0-only                   #
#-----------------------------------------------------------#

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

#-----------------------------------------------------------#
# Project Definition                                        #
#-----------------------------------------------------------#
project(OpenRGB LANGUAGES CXX)

#-----------------------------------------------------------#
# Executable                                                #
#-----------------------------------------------------------#
add_executable(OpenRGB)

#-----------------------------------------------------------#
# CMake Configuration                                       #
#-----------------------------------------------------------#
set(CMAKE_CXX_STANDARD          17                          )
set(CMAKE_CXX_STANDARD_REQUIRED ON                          )
set(CMAKE_AUTOMOC               ON                          )
set(CMAKE_AUTORCC               ON                          )
set(CMAKE_AUTOUIC               ON                          )
set(CMAKE_INCLUDE_CURRENT_DIR   ON                          )

#-----------------------------------------------------------#
# Qt Configuration                                          #
#-----------------------------------------------------------#
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

#-----------------------------------------------------------#
# Version                                                   #
#-----------------------------------------------------------#
set(MAJOR       "0"                                         )
set(MINOR       "9"                                         )
set(SUFFIX      "git"                                       )

#-----------------------------------------------------------#
# Git Information                                           #
#-----------------------------------------------------------#
set(LASTTAG     "release_${MAJOR}.${MINOR}"                 )

execute_process(COMMAND git rev-parse --short=7 HEAD
                OUTPUT_VARIABLE SHORTHASH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND git rev-list --count ${LASTTAG}..HEAD
                OUTPUT_VARIABLE COMMITS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/git-get-branch.sh
                OUTPUT_VARIABLE GITBRANCH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

#-----------------------------------------------------------#
# Version Strings                                           #
#-----------------------------------------------------------#
set(VERSION_NUM "${MAJOR}.${MINOR}.${COMMITS}"              )
set(VERSION_STR "${MAJOR}.${MINOR}"                         )

set(VERSION_DEB ${VERSION_NUM}                              )
set(VERSION_WIX ${VERSION_NUM}                              )
set(VERSION_AUR ${VERSION_NUM}                              )
set(VERSION_RPM ${VERSION_NUM}                              )

#-----------------------------------------------------------#
# If suffix is "git", use package-specific git versioning   #
# Otherwise, append suffix if it is not empty               #
#-----------------------------------------------------------#
if("${SUFFIX}" STREQUAL "git")
set(VERSION_STR "${VERSION_STR}+ (${SUFFIX}${COMMITS})"     )
set(VERSION_DEB "${VERSION_DEB}~git${SHORTHASH}"            )
set(VERSION_AUR "${VERSION_AUR}.g${SHORTHASH}"              )
set(VERSION_RPM "${VERSION_RPM}^git${SHORTHASH}"            )
elseif(NOT "${SUFFIX}" STREQUAL "")
set(VERSION_STR "${VERSION_STR}+ (${SUFFIX})"               )
set(VERSION_DEB "${VERSION_DEB}~${SUFFIX}"                  )
set(VERSION_AUR "${VERSION_AUR}.${SUFFIX}"                  )
set(VERSION_RPM "${VERSION_RPM}^${SUFFIX}"                  )
endif()

#-----------------------------------------------------------#
# Print build configuration information                     #
#-----------------------------------------------------------#
message(STATUS "OpenRGB ${VERSION_STR} CMake Build"         )
message(STATUS "-------------------------------------------")
message(STATUS "  Last release tag:      ${LASTTAG}"        )
message(STATUS "  Shorthash:             ${SHORTHASH}"      )
message(STATUS "  Commits since release: ${COMMITS}"        )
message(STATUS "  Git branch:            ${GITBRANCH}"      )
message(STATUS "  Version:  Main:        ${VERSION_STR}"    )
message(STATUS "            Windows:     ${VERSION_WIX}"    )
message(STATUS "            Debian:      ${VERSION_DEB}"    )
message(STATUS "            Arch (AUR):  ${VERSION_AUR}"    )
message(STATUS "            Fedora:      ${VERSION_RPM}"    )

#-----------------------------------------------------------#
# Build list of controller headers, include directories,    #
# and source files                                          #
#-----------------------------------------------------------#
file(GLOB_RECURSE children LIST_DIRECTORIES true
    "${CMAKE_SOURCE_DIR}/Controllers/*"                     )

foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND CONTROLLER_INC ${child})
    endif()
endforeach()

file(GLOB_RECURSE CONTROLLER_H
    "${CMAKE_SOURCE_DIR}/Controllers/*.h"                   )

file(GLOB_RECURSE CONTROLLER_CPP
    "${CMAKE_SOURCE_DIR}/Controllers/*.cpp"                 )

#-----------------------------------------------------------#
# Remove OS-specific files from the overall controller      #
# headers and sources lists.  The suffixes _Windows, _Linux,#
# _FreeBSD, and _MacOS are usable to denote that a file     #
# only applies to one or more OSes.  The suffixes may be    #
# combined such as <file>_Windows_Linux.cpp.                #
#-----------------------------------------------------------#
file(GLOB_RECURSE CONTROLLER_H_WINDOWS
    "${CMAKE_SOURCE_DIR}/Controllers/*_Windows*.h"          )
file(GLOB_RECURSE CONTROLLER_CPP_WINDOWS
    "${CMAKE_SOURCE_DIR}/Controllers/*_Windows*.cpp"        )
file(GLOB_RECURSE CONTROLLER_H_LINUX
    "${CMAKE_SOURCE_DIR}/Controllers/*_Linux*.h"            )
file(GLOB_RECURSE CONTROLLER_CPP_LINUX
    "${CMAKE_SOURCE_DIR}/Controllers/*_Linux*.cpp"          )
file(GLOB_RECURSE CONTROLLER_H_MACOS
    "${CMAKE_SOURCE_DIR}/Controllers/*_MacOS*.h"            )
file(GLOB_RECURSE CONTROLLER_CPP_MACOS
    "${CMAKE_SOURCE_DIR}/Controllers/*_MacOS*.cpp"          )
file(GLOB_RECURSE CONTROLLER_H_FREEBSD
    "${CMAKE_SOURCE_DIR}/Controllers/*_FreeBSD*.h"          )
file(GLOB_RECURSE CONTROLLER_CPP_FREEBSD
    "${CMAKE_SOURCE_DIR}/Controllers/*_FreeBSD*.cpp"        )

list(REMOVE_ITEM CONTROLLER_H   ${CONTROLLER_H_WINDOWS}     )
list(REMOVE_ITEM CONTROLLER_CPP ${CONTROLLER_CPP_WINDOWS}   )
list(REMOVE_ITEM CONTROLLER_H   ${CONTROLLER_H_LINUX}       )
list(REMOVE_ITEM CONTROLLER_CPP ${CONTROLLER_CPP_LINUX}     )
list(REMOVE_ITEM CONTROLLER_H   ${CONTROLLER_H_MACOS}       )
list(REMOVE_ITEM CONTROLLER_CPP ${CONTROLLER_CPP_MACOS}     )
list(REMOVE_ITEM CONTROLLER_H   ${CONTROLLER_H_FREEBSD}     )
list(REMOVE_ITEM CONTROLLER_CPP ${CONTROLLER_CPP_FREEBSD}   )

target_sources(OpenRGB PRIVATE ${CONTROLLER_CPP}            )
target_include_directories(OpenRGB PRIVATE ${CONTROLLER_INC})

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/AutoStart)

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/dmiinfo)
target_sources(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/dmiinfo/dmiinfo.cpp)

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/hidapi_wrapper)

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/i2c_smbus)

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/i2c_tools)
target_sources(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/i2c_tools/i2c_tools.cpp)

target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/interop)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/KeyboardLayoutManager)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/net_port)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/pci_ids)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/qt)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/RGBController)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/scsiapi)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/serial_port)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/SPDAccessor)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/super_io)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/SuspendResume)
target_include_directories(OpenRGB PRIVATE ${CMAKE_SOURCE_DIR}/wmi)

#-----------------------------------------------------------#
# CRCpp                                                     #
#-----------------------------------------------------------#
set(CRCPP_DIR ${CMAKE_SOURCE_DIR}/dependencies/CRCpp)

add_library(CRCpp INTERFACE)
target_include_directories(CRCpp INTERFACE ${CRCPP_DIR}/)

target_link_libraries(OpenRGB CRCpp)

#-----------------------------------------------------------#
# hidapi                                                    #
#-----------------------------------------------------------#
find_package(hidapi REQUIRED)
target_link_libraries(OpenRGB hidapi::hidapi)

#-----------------------------------------------------------#
# httplib                                                   #
#-----------------------------------------------------------#
set(HTTPLIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/httplib)

add_library(httplib INTERFACE)
target_include_directories(httplib INTERFACE ${HTTPLIB_DIR}/)

target_link_libraries(OpenRGB httplib)

#-----------------------------------------------------------#
# libe131                                                   #
#-----------------------------------------------------------#
set(LIBE131_DIR ${CMAKE_SOURCE_DIR}/dependencies/libe131)

add_library(libe131 STATIC)
target_include_directories(libe131 PUBLIC ${LIBE131_DIR}/src)
target_sources(libe131 PRIVATE ${LIBE131_DIR}/src/e131.c)

target_link_libraries(OpenRGB libe131)

#-----------------------------------------------------------#
# libusb                                                    #
#-----------------------------------------------------------#
find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb-1.0 REQUIRED IMPORTED_TARGET GLOBAL libusb-1.0)

target_link_libraries(OpenRGB PkgConfig::libusb-1.0)

#-----------------------------------------------------------#
# hueplusplus                                               #
#-----------------------------------------------------------#
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/hueplusplus-1.2.0)
target_link_libraries(OpenRGB hueplusplusstatic)

#-----------------------------------------------------------#
# nlohmann_json                                             #
#-----------------------------------------------------------#
find_package(nlohmann_json REQUIRED)
target_link_libraries(OpenRGB nlohmann_json)
