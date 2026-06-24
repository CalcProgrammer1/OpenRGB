/*---------------------------------------------------------*\
| QMKCommon.h                                               |
|                                                           |
|   Common QMK definitions                                  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

typedef struct
{
    unsigned char   x;          /* X position in RGB matrix (0-224) */
    unsigned char   y;          /* Y position in RGB matrix (0-64)  */
    unsigned char   flags;      /* LED flags                        */
    unsigned char   row;        /* Row in key matrix                */
    unsigned char   col;        /* Column in key matrix             */
    bool            valid;      /* Is this LED valid?               */
} qmk_rgb_matrix_led_info;