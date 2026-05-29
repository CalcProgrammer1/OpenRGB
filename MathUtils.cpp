/*---------------------------------------------------------*\
| MathUtils.cpp                                             |
|                                                           |
|   Math utility functions                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <math.h>
#include "MathUtils.h"

int MathUtils::IntInterpolate(int y0, int y1, int x0, int x1, int x)
{
    if(x1 == x0)
        return y0;

    if(y0 == y1)
        return y0;

    double t = (double)(x - x0) / (double)(x1 - x0);
    double y = y0 * (1.0 - t) + y1 * t;
    return (int)round(y);
}