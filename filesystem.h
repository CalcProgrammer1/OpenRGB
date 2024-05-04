/*---------------------------------------------------------*\
| filesystem.h                                              |
|                                                           |
|   Selects between std and experimental filesystem library |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#if defined(__has_include) && __has_include(<filesystem>)

#include <filesystem>

// Debian 10 provides the header, but does not enable the feature, so we additionally check for the feature test macro
// MSVC below 2017 does not provide feature test macros, so we leave an exception for them
#if defined(__cpp_lib_filesystem) || defined (_MSC_VER)
namespace filesystem = std::filesystem;
#define STD_FILESYSTEM_FOUND
#endif

#endif

#ifndef STD_FILESYSTEM_FOUND

#if defined(__has_include) && !__has_include(<experimental/filesystem>)
#error Neither <filesystem> nor <experimental/filesystem> could be found on the system! Please report an issue to OpenRGB developers providing your system info.
#endif

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

#endif // C++17

#endif // FILESYSTEM_H
