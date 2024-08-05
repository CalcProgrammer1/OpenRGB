# Documentation for the hueplusplus library
A simple and easy to use library for Philips Hue Lights.

[TOC]

## Features
* find bridges with SSDP or set an ip manually
* all common light functions (brightness, color, temperature)
* extended alert() functions, which alert in a specific color (good for notifications)
* supports sensors, rules, groups, scenes and schedules
* streaming with entertainment mode
* documented with doxygen
* tested with google test, google mock and gcov/lcov

## Compatibility
* Linux
* Windows
* MacOS
* Espressif ESP32 SDK & Arduino

## How to use
[Getting started](@ref getting-started)
### Searching for Bridges
To start searching for a Hue Bridge you will need to choose an IHttpHandler and create one. The options are a [WinHttpHandler](@ref hueplusplus::WinHttpHandler) (for windows) or a [LinHttpHandler](@ref hueplusplus::LinHttpHandler) (for linux or linux-like).

Then create a [BridgeFinder](@ref hueplusplus::BridgeFinder) object with the handler.
The handler is needed, because it tells the finder which functions to use to communicate with a bridge or your local network.
After that you can call [findBridges()](@ref hueplusplus::BridgeFinder::findBridges), which will return a vector containing the ip and mac address of all found Bridges.
\snippet Snippets.cpp search-bridge

At this point you may want to decide whether to use a [shared state](@ref shared-state) cache model or keep the default settings.

### Authenticate Bridges
If you have found the Bridge you were looking for, you can then move on with the authentication process.
To get a new username from the Bridge (for now) you simply call [getBridge(bridges[\<index\>])](@ref hueplusplus::BridgeFinder::getBridge),
where index is your preferred Bridge from the part [Searching for Bridges](#searchingBridges). This requires the user to press the link button.
\snippet Snippets.cpp get-bridge-1

If you on the other hand already have a username you can add your bridge like so
\snippet Snippets.cpp get-bridge-2

If you do not want to use the BridgeFinder or you already know the ip and username of your bridge you have the option to create your own Bridge object.
Here you will need to provide the ip address, the port number, a username and an HttpHandler
\snippet Snippets.cpp get-bridge-3


### Controlling lights
If you have your Bridge all set up, you can now control its lights.
For that create a new Light object and call [lights().get(\<id\>)](@ref hueplusplus::ResourceList::get) on your bridge object to get a reference to a specific light, where id
is the id of the light set internally by the Hue Bridge.
\snippet Snippets.cpp light-1

If you don't know the id of a specific light or want to get an overview over all lights that are controlled by your bridge, 
you can get a vector containing them by calling [getAll()](@ref hueplusplus::ResourceList::getAll) on your bridge object. If no lights are found the vector will be empty.
\snippet Snippets.cpp light-2

If you now want to control a light, call a specific function of it.
\snippet Snippets.cpp light-3

But keep in mind that some light types do not have all functions available. So you might call a
specific function, but nothing will happen. For that you might want to check what type
of a light you are controlling. For that you can call the function [getColorType()](@ref hueplusplus::Light::getColorType()), which will return
a ColorType.
\snippet Snippets.cpp light-4

There's also a new way to check whether specific functions of a light are available:
\snippet Snippets.cpp light-5

These will either return true(light has specified function) or false(light lacks specified function).

## Build and install
[Build and install guide](@ref build)

### Basic installation
If you want to build the library you can use cmake (at least version 3.8). First create a build folder and then execute cmake.
```bash
mkdir build
cd build
cmake ..
```
Then compile the code with make. If you are inpatient use the option -j\<number\>, where number specifies how many files are compiled at the same time. Note this number should not exceed the number of cores*2 of your machine.
```bash
make
```
```bash
make -j4
```
If you want to install the library use
```bash
make install
```
To remove it
```bash
make uninstall
```

### Advanced usage
If you have a project that already uses CMake you probably want to add the hueplusplus library directly in your cmake file.
For that the best way is to use find_package().
When cmake finds the hueplusplus library you can then link against either the shared or static version of the library.
```cmake
find_package(hueplusplus REQUIRED)

target_link_libraries(<executable> PUBLIC hueplusplusstatic)
```
But this will only work if the hueplusplus library is already installed.
To get around this problem there is a pretty awesome way.
If you have the hueplusplus repository included in your project repository (as a submodule) or know where the folder lives you can do the following:
```cmake
find_package(hueplusplus QUIET)
if(NOT hueplusplus_FOUND)
    message(STATUS "-- hueplusplus not found, building it")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/<path to directory>/hueplusplus" "${CMAKE_CURRENT_BINARY_DIR}/hueplusplus")
endif()

target_link_libraries(<executable> PUBLIC hueplusplusstatic)
```
This will check if the hueplusplus library was found by find_package() and if not it will use the specified path to the library source and compile it during the build process.

### Running tests
If you additionally want to run the tests use cmake with the option -Dhueplusplus_TESTS=ON. Testing is done with Google gtest and gmock. Note that you wont need to install gtest/gmock yourself, because cmake will automatically download them and include them during the build. Since I added a custom target you will only need to call "make unittest" and the tests are compiled and executed.
```bash
mkdir build
cd build
cmake .. -Dhueplusplus_TESTS=ON
make unittest
```
If you also want to execute coverage tests you will need to install gcov and lcov yourself. To run the coverage test use
```bash
make coveragetest
```


## Other pages
- [Getting started](@ref getting-started)
- [Build and install](@ref build)
- [Shared state cache](@ref shared-state)
- [Transactions](@ref transactions)
- [Sensors](@ref sensors)
