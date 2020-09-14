file(GLOB hueplusplus_HEADERS include/*.h include/*.hpp)
set(hueplusplus_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseHttpHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtendedColorHueStrategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtendedColorTemperatureStrategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Hue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HueCommandAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HueException.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HueLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleBrightnessStrategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleColorHueStrategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleColorTemperatureStrategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UPnP.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
)

# on windows we want to compile the WinHttpHandler
if(WIN32)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/WinHttpHandler.cpp
    )
endif()
# whereas on linux we want the LinHttpHandler
if(UNIX)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/LinHttpHandler.cpp
    )
endif()
if(ESP_PLATFORM)
    set(hueplusplus_SOURCES
        ${hueplusplus_SOURCES}
       ${CMAKE_CURRENT_SOURCE_DIR}/LinHttpHandler.cpp
    )
endif()


# Set global includes BEFORE adding any targets for legacy CMake versions
if(CMAKE_VERSION VERSION_LESS 2.8.12)
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/json")
endif()


# hueplusplus shared library
add_library(hueplusplusshared SHARED ${hueplusplus_SOURCES})
set_property(TARGET hueplusplusshared PROPERTY CXX_STANDARD 14)
set_property(TARGET hueplusplusshared PROPERTY CXX_EXTENSIONS OFF)
if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
    target_include_directories(hueplusplusshared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()
install(TARGETS hueplusplusshared DESTINATION lib)

# hueplusplus static library
add_library(hueplusplusstatic STATIC ${hueplusplus_SOURCES})
set_property(TARGET hueplusplusstatic PROPERTY CXX_STANDARD 14)
set_property(TARGET hueplusplusstatic PROPERTY CXX_EXTENSIONS OFF)
install(TARGETS hueplusplusstatic DESTINATION lib)
if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
    target_include_directories(hueplusplusstatic PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()
install(FILES ${hueplusplus_HEADERS} DESTINATION include/hueplusplus)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE hueplusplus)
# Create the hueplusplus-config.cmake
configure_file (hueplusplus-config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hueplusplus-config.cmake" @ONLY)
# Install hueplusplus-config.cmake
install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hueplusplus-config.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# if the user decided to use tests add the subdirectory
if(hueplusplus_TESTS)
    set(HuePlusPlus_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory("test")
endif()
