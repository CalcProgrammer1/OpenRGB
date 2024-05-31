/*---------------------------------------------------------*\
| StringUtils.h                                             |
|                                                           |
|   String utility functions                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

class StringUtils
{
public:
    static const char *wchar_to_char(const wchar_t *pwchar);
    static const std::string remove_null_terminating_chars(std::string input);
};

#endif // STRING_UTILS_H
