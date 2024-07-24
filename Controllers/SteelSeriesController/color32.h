/*---------------------------------------------------------*\
| color32.h                                                 |
|                                                           |
|   Class to hold 32-bit color data                         |
|                                                           |
|   David Lee (RAMChYLD)                        15 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} color32;
