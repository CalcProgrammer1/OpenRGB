cmake_minimum_required(VERSION 3.10.2...3.28)

# Add cmake dir to module path, so Find*.cmake can be found
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(hueplusplus VERSION 1.2.0 LANGUAGES CXX)

# check whether hueplusplus is compiled directly or included as a subdirectory
if(NOT DEFINED hueplusplus_master_project)
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		set(hueplusplus_master_project ON)
	else()
		set(hueplusplus_master_project OFF)
	endif()
endif()

# options to set
option(hueplusplus_TESTS "Build tests" OFF)
option(hueplusplus_EXAMPLES "Build examples" OFF)
option(hueplusplus_NO_EXTERNAL_LIBRARIES "Do not try to use external libraries" OFF)

# Try to find installed packages
if(NOT hueplusplus_NO_EXTERNAL_LIBRARIES)
    # Suppress warnings if libraries are not found, they will be built from submodules
	find_package(MbedTLS QUIET)
	find_package(nlohmann_json QUIET)
endif()

set(NEED_SUBMODULES NOT (${MbedTLS_FOUND} AND ${nlohmann_json_FOUND}))

option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")
if(CLANG_TIDY_EXE)
	if(CLANG_TIDY_FIX)
		set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-fix")
	else()
		set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
	endif()
endif()

# update submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE AND NEED_SUBMODULES)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

# Set default build type if none was specified
set(default_build_type "Release")
if(hueplusplus_master_project AND (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES))
	message(STATUS "Setting build type to '${default_build_type}' as none was specified")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
	# Set possible values for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# get the correct installation directory for add_library() to work
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/hueplusplus)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# target for uninstall
if(NOT TARGET uninstall)
    configure_file(
        "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# if we are on a apple machine this is needed
if (1 AND APPLE)
	set(CMAKE_MACOSX_RPATH 1)
endif()

if(NOT MbedTLS_FOUND)
	# Build mbedtls if not installed
	message(STATUS "MbedTLS was not found, the submodule is used.")
	set(USE_STATIC_MBEDTLS_LIBRARY ON)
	set(USE_SHARED_MBEDTLS_LIBRARY OFF)
	add_subdirectory("lib/mbedtls" EXCLUDE_FROM_ALL)

	# Compile the mbedtls library as a static with position independent code,
	# because we need it for both a shared and static library
	set_property(TARGET mbedtls PROPERTY POSITION_INDEPENDENT_CODE ON)
	set_property(TARGET mbedcrypto PROPERTY POSITION_INDEPENDENT_CODE ON)
	set_property(TARGET mbedx509 PROPERTY POSITION_INDEPENDENT_CODE ON)

	if(CMAKE_VERSION VERSION_LESS 3.18)
		# Aliases for compatibility with find_package, newer cmake versions add these already
		add_library(MbedTLS::mbedtls ALIAS mbedtls)
		add_library(MbedTLS::mbedcrypto ALIAS mbedcrypto)
		add_library(MbedTLS::mbedx509 ALIAS mbedx509)
	endif()
endif()

if(NOT nlohmann_json_FOUND)
	# Use embedded json
	message(STATUS "nlohmann_json was not found, the submodule is used.")
	# disable tests for json
	set(JSON_BuildTests OFF CACHE INTERNAL "")
	add_subdirectory("lib/json" EXCLUDE_FROM_ALL)
endif()

add_subdirectory(src)

# if the user decided to use tests add the subdirectory
if(hueplusplus_TESTS)
    add_subdirectory("test")
endif()

if(hueplusplus_EXAMPLES)
	add_subdirectory("examples")
endif()
