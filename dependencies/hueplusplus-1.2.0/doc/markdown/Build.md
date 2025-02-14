# Build and install {#build}

[TOC]

# Basic installation {#basic-install}

## Clone from github {#clone}
To get the newest version of the hueplusplus library, clone it directly from [github](https://github.com/enwi/hueplusplus).
The master branch contains the latest tested and stable version, while the development branch is more unstable.
```{.sh}
~ $ git clone https://github.com/enwi/hueplusplus.git
```
This creates a folder hueplusplus with the library sources.

When you want to update the library for a new version, use pull with rebase.
```{.sh}
~/hueplusplus $ git pull --rebase
```

## Build with CMake {#build-cmake}
To build the library, you need to use [CMake](https://cmake.org) version 3.10 or higher.
It is easiest to create a separate build directory where the build files are stored.
```{.sh}
~/hueplusplus $ mkdir build
~/hueplusplus $ cd build
~/hueplusplus/build $ cmake ..
~/hueplusplus/build $ make
```

To install or uninstall the library use the make targets.
```{.sh}
~/hueplusplus/build $ make install
~/hueplusplus/build $ make uninstall
```

## Use in a CMake project {#import-cmake}
If you have a project that already uses CMake you probably want to add the hueplusplus library directly in your cmake file.
For that the best way is to use find_package().
```{.cmake}
find_package(hueplusplus REQUIRED)
```
But this will only work if the hueplusplus library is already installed.
Instead, if you have the hueplusplus repository included in your project repository (as a submodule) or know where the folder lives you can do the following:
```{.cmake}
find_package(hueplusplus QUIET)
if(NOT hueplusplus_FOUND)
    message(STATUS "-- hueplusplus not found, building it")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/<path to directory>/hueplusplus" "${CMAKE_CURRENT_BINARY_DIR}/hueplusplus")
endif()
```
This will check if the hueplusplus library was found by find_package() and if not it will use the specified path to the library source and compile it during the build process.

The cmake project defines two library targets: `hueplusplusstatic` to link as a static library and `hueplusplusshared` to link as a shared library.
```{.cmake}
target_link_libraries(<executable> PUBLIC hueplusplusstatic)
```

## Use in another project {#import-other}
When you are not using CMake, you have to install hueplusplus and change your build configuration to link to the compiled library.
The header files in the include directory need to be added to the include path.
How you do this depends on the build system.

## Building tests {#build-tests}
If you additionally want to run the tests use cmake with the option -Dhueplusplus_TESTS=ON. Testing is done with Google gtest and gmock. Note that you wont need to install gtest/gmock yourself, because cmake will automatically download them and include them during the build.
The custom target `unittest` compiles and executes all tests.
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

## Building examples {#build-examples}
There are some small [example programs](@ref examples) using this library in the examples folder. To build them,
set `hueplusplus_EXAMPLES=ON`. The target `hueplusplus_examples` builds all examples into build/examples.
```{.sh}
mkdir build
cd build
cmake .. -Dhueplusplus_EXAMPLES=ON
make hueplusplus_examples
```

## External libraries
Hueplusplus requires a few external libraries  (e.g. Mbed TLS and GTest), which are included automatically. If these are pre-installed on your system, those versions will be used by default. This can potentially cause issues if your installed versions are incompatible.
In this case, set `hueplusplus_NO_EXTERNAL_LIBRARIES=ON` to force using the embedded versions instead of the installed libraries.