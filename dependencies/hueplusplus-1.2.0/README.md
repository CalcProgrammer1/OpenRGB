# hueplusplus
A simple and easy to use library for Philips Hue Lights

| Branch | CI | Codecov | LGTM
|:-|:-|:-|:-|
| [Master](https://github.com/enwi/hueplusplus/tree/master) | [![CI](https://github.com/enwi/hueplusplus/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/enwi/hueplusplus/actions/workflows/build.yml)| [![codecov](https://codecov.io/gh/enwi/hueplusplus/branch/master/graph/badge.svg)](https://codecov.io/gh/enwi/hueplusplus) | [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/enwi/hueplusplus.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/enwi/hueplusplus/context:cpp) |
| [Development](https://github.com/enwi/hueplusplus/tree/development) | [![CI](https://github.com/enwi/hueplusplus/actions/workflows/build.yml/badge.svg?branch=development)](https://github.com/enwi/hueplusplus/actions/workflows/build.yml) | [![codecov](https://codecov.io/gh/enwi/hueplusplus/branch/development/graph/badge.svg)](https://codecov.io/gh/enwi/hueplusplus) | |

## Features
* find bridges with SSDP or set an ip manually
* all common light functions (brightness, color, temperature)
* extended alert() functions, which alert in a specific color (good for notifications)
* supports sensors, rules, groups, scenes and schedules
* streaming with entertainment mode
* [documented with doxygen](https://enwi.github.io/hueplusplus/)
* tested with google test, google mock and gcov/lcov

## Compatibility
* Linux
* Windows
* MacOS
* Espressif ESP32 SDK & Arduino

## How to use
### <a name="searchingBridges"></a>Searching for Bridges
To start searching for a Hue Bridge you will need to choose an IHttpHandler and create one. The options are a "WinHttpHandler" (for windows) or a "LinHttpHandler" (for linux).
Then create a BridgeFinder object with the handler.
The handler is needed, because it tells the finder which functions to use to communicate with a bridge or your local network.
After that you can call findBridges(), which will return a vector containing the ip and mac address of all found Bridges.
```C++
// For windows use std::make_shared<hueplusplus::WinHttpHandler>();
handler = std::make_shared<hueplusplus::LinHttpHandler>();
hueplusplus::BridgeFinder finder(handler);
std::vector<hueplusplus::BridgeFinder::BridgeIdentification> bridges = finder.findBridges();
if (bridges.empty())
{
	std::cerr << "No bridges found\n";
	return;
}

```

### Authenticate Bridges
If you have found the Bridge you were looking for, you can then move on with the authentication process.
To get a new username from the Bridge (for now) you simply call getBridge(bridges[\<index\>]),
where index is your preferred Bridge from the part [Searching for Bridges](#searchingBridges).
```C++
hueplusplus::Bridge bridge = finder.getBridge(bridges[0]);
```
If you on the other hand already have a username you can add your bridge like so
```C++
finder.addUsername(bridges[0].mac, "<username>");
hueplusplus::Bridge bridge = finder.getBridge(bridges[0]);
```
If you do not want to use the BridgeFinder or you already know the ip and username of your bridge you have the option to create your own Bridge object.
Here you will need to provide the ip address, the port number, a username and an HttpHandler
```C++
// For windows use std::make_shared<hueplusplus::WinHttpHandler>();
handler = std::make_shared<hueplusplus::LinHttpHandler>();
hueplusplus::Bridge bridge("192.168.2.102", 80, "<username>", handler);
```

### Controlling lights
If you have your Bridge all set up, you can now control its lights.
For that create a new Light object and call lights().get(\<id\>) on your bridge object to get a reference to a specific light, where id
is the id of the light set internally by the Hue Bridge.
```C++
hueplusplus::Light light1 = bridge.lights().get(1);
```
If you don't know the id of a specific light or want to get an overview over all lights that are controlled by your bridge, 
you can get a vector containing them by calling getAll(). If no lights are found the vector will be empty.
```C++
std::vector<hueplusplus::Light> lights = bridge.lights().getAll();
```
If you now want to control a light, call a specific function of it.
```C++
light1.on();
light1.setBrightness(120);
light1.alertHueSaturation(25500, 255);
light1.setColorLoop(true);
light1.setColorRGB(255, 128, 0);
lights[1].off();
lights.at(1).setColorHue(4562);
```
But keep in mind that some light types do not have all functions available. So you might call a
specific function, but nothing will happen. For that you might want to check what type
of a light you are controlling. For that you can call the function getColorType(), which will return
a ColorType.
```C++
hueplusplus::ColorType type1 = light1.getColorType();
```
There's also a new way to check whether specific functions of a light are available:
```C++
light1.hasBrightnessControl();
light1.hasTemperatureControl();
light1.hasColorControl();
```
These will either return true(light has specified function) or false(light lacks specified function).

### Further reading
If you want to know more about all functions just look inside the doxygen documentation. It can be found [here](https://enwi.github.io/hueplusplus/)

## Build and install
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


## Copyright
Copyright (c) 2017 Jan Rogall & Moritz Wirger. See LICENSE for further details.
