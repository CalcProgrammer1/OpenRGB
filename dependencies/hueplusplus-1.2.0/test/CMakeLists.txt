if(NOT hueplusplus_NO_EXTERNAL_LIBRARIES)
    find_package(GTest)
    find_package(GMock)
endif()

if(NOT GTest_FOUND OR NOT GMock_FOUND)
    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download"
    )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download"
    )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src EXCLUDE_FROM_ALL
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL
    )
    target_compile_features(gmock PUBLIC cxx_std_14)
    target_compile_features(gtest PUBLIC cxx_std_14)
endif()


# define all test sources
set(TEST_SOURCES
    test_Action.cpp
    test_APICache.cpp
    test_BaseDevice.cpp
    test_BaseHttpHandler.cpp
    test_Bridge.cpp
    test_BridgeConfig.cpp
    test_SensorImpls.cpp
    test_ColorUnits.cpp
    test_ExtendedColorHueStrategy.cpp
    test_ExtendedColorTemperatureStrategy.cpp
    test_Group.cpp
    test_HueCommandAPI.cpp
    test_Light.cpp
    test_LightFactory.cpp
    test_Main.cpp
    test_NewDeviceList.cpp
    test_UPnP.cpp
    test_ResourceList.cpp
    test_Rule.cpp
    test_Scene.cpp
    test_Schedule.cpp
    test_Sensor.cpp
    test_SensorList.cpp
    test_SimpleBrightnessStrategy.cpp
    test_SimpleColorHueStrategy.cpp
    test_SimpleColorTemperatureStrategy.cpp
    test_StateTransaction.cpp
    test_TimePattern.cpp)

set(HuePlusPlus_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

# test executable
add_executable(test_HuePlusPlus ${TEST_SOURCES})
#if(DO_CLANG_TIDY)
#	set_target_properties(test_HuePlusPlus PROPERTIES CXX_CLANG_TIDY ${DO_CLANG_TIDY})
#endif()
target_compile_features(test_HuePlusPlus PUBLIC cxx_std_14)
set_property(TARGET test_HuePlusPlus PROPERTY CXX_EXTENSIONS OFF)

target_link_libraries(test_HuePlusPlus PUBLIC hueplusplusstatic)
target_link_libraries(test_HuePlusPlus PUBLIC gtest gmock)
target_include_directories(test_HuePlusPlus PUBLIC ${GTest_INCLUDE_DIRS})
# add custom target to make it simple to run the tests
add_custom_target("unittest"
    # Run the executable
    COMMAND test_HuePlusPlus
    # Depends on test_HuePlusPlus
    DEPENDS test_HuePlusPlus
)

# Check for coverage test prerequisites
find_program( GCOV_PATH gcov )
find_program( LCOV_PATH lcov )

mark_as_advanced(GCOV_PATH)
mark_as_advanced(LCOV_PATH)

if(LCOV_PATH AND GCOV_PATH)
    # GCov
    include(CodeCoverage.cmake)
    add_executable(testcov_HuePlusPlus ${TEST_SOURCES} ${hueplusplus_SOURCES})
    target_include_directories(testcov_HuePlusPlus PUBLIC "${PROJECT_SOURCE_DIR}/include")
    target_compile_features(testcov_HuePlusPlus PUBLIC cxx_std_14)
    set_property(TARGET testcov_HuePlusPlus PROPERTY CXX_EXTENSIONS OFF)

    target_link_libraries(testcov_HuePlusPlus PRIVATE mbedtls)
    target_link_libraries(testcov_HuePlusPlus PUBLIC nlohmann_json::nlohmann_json gtest gmock)
    target_include_directories(testcov_HuePlusPlus PUBLIC ${GTest_INCLUDE_DIRS})
    # this will be already done by APPEND_COVERAGE_COMPILER_FLAGS()
    #set_target_properties(
    #        testcov_HuePlusPlus PROPERTIES
    #        COMPILE_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage"
    #)
    # Normally this would be -lgcov, but on mac only -Lgcov works
    #set_target_properties(
    #        testcov_HuePlusPlus PROPERTIES
    #        LINK_FLAGS "-O0 -g -Lgcov -fprofile-arcs -ftest-coverage"
    #)
    # exclude some special files we do not want to profile
    set(COVERAGE_EXCLUDES
        '/usr/*'           # unix
        '*/hueplusplus/build/*'
        '*/json*'
        '*/test/*'
        '*/v1/*'           # iOS
    )
    APPEND_COVERAGE_COMPILER_FLAGS()
    SETUP_TARGET_FOR_COVERAGE(
            NAME "coveragetest"
            EXECUTABLE testcov_HuePlusPlus
            DEPENDENCIES testcov_HuePlusPlus
    )
endif()
