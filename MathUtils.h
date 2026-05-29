/*---------------------------------------------------------*\
| MathUtils.h                                               |
|                                                           |
|   Math utility functions                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <stdint.h>

class MathUtils
{
public:
    static int IntInterpolate(int y0, int y1, int x0, int x1, int x);
};