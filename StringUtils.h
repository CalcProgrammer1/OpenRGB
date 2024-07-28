/*---------------------------------------------------------*\
| StringUtils.h                                             |
|                                                           |
|   String utility functions                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

class StringUtils
{
public:
    static const char* wchar_to_char(const wchar_t* pwchar);
    static std::string wstring_to_string(const std::wstring wstring);
    static const std::string remove_null_terminating_chars(std::string input);
};
