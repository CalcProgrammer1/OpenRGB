#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#if defined(__has_include) && __has_include(<filesystem>)

#include <filesystem>
namespace filesystem = std::filesystem;

#else

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

#endif // C++17

#endif // FILESYSTEM_H
