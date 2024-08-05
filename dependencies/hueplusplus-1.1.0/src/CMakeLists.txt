set(hueplusplus_SOURCES
    Action.cpp
    APICache.cpp
    BaseDevice.cpp
    BaseHttpHandler.cpp
    Bridge.cpp
    BridgeConfig.cpp
    CLIPSensors.cpp
    ColorUnits.cpp
    EntertainmentMode.cpp
    ExtendedColorHueStrategy.cpp
    ExtendedColorTemperatureStrategy.cpp
    Group.cpp
    HueCommandAPI.cpp
    HueDeviceTypes.cpp
    HueException.cpp
    Light.cpp
    ModelPictures.cpp
    NewDeviceList.cpp
    Rule.cpp
    Scene.cpp
    Schedule.cpp
    Sensor.cpp
    SimpleBrightnessStrategy.cpp
    SimpleColorHueStrategy.cpp
    SimpleColorTemperatureStrategy.cpp
    StateTransaction.cpp
    TimePattern.cpp
    UPnP.cpp
    Utils.cpp 
    ZLLSensors.cpp)

# on windows we want to compile the WinHttpHandler
if(WIN32)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
        WinHttpHandler.cpp
    )
endif()
# whereas on linux we want the LinHttpHandler
if(UNIX)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
        LinHttpHandler.cpp
    )
endif()
if(ESP_PLATFORM)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
        LinHttpHandler.cpp
    )
endif()

# append current source dir before files
foreach(src ${hueplusplus_SOURCES})
    list(APPEND _srcList "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
endforeach()
set(hueplusplus_SOURCES ${_srcList} PARENT_SCOPE)

# hueplusplus shared library
add_library(hueplusplusshared SHARED ${hueplusplus_SOURCES})
target_link_libraries(hueplusplusshared PRIVATE mbedtls)
target_compile_features(hueplusplusshared PUBLIC cxx_std_14)
target_include_directories(hueplusplusshared PUBLIC $<BUILD_INTERFACE:${hueplusplus_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
install(TARGETS hueplusplusshared DESTINATION lib)

# hueplusplus static library
add_library(hueplusplusstatic STATIC ${hueplusplus_SOURCES})
target_link_libraries(hueplusplusstatic PRIVATE mbedtls)
target_compile_features(hueplusplusstatic PUBLIC cxx_std_14)
install(TARGETS hueplusplusstatic DESTINATION lib)
target_include_directories(hueplusplusstatic PUBLIC $<BUILD_INTERFACE:${hueplusplus_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE hueplusplus)
# Create the hueplusplus-config.cmake
configure_file ("${PROJECT_SOURCE_DIR}/cmake/hueplusplus-config.cmake.in" "${hueplusplus_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hueplusplus-config.cmake" @ONLY)
# Install hueplusplus-config.cmake
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hueplusplus-config.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
