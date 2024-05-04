/*---------------------------------------------------------*\
| OpenRGBThemeManager.h                                     |
|                                                           |
|   Functionality for managing dark theme mode              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

class OpenRGBThemeManager
{
public:
    static void Init();
    static void SetDarkTheme();
    static bool IsDarkTheme();
};
