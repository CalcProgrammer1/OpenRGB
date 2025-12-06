/*---------------------------------------------------------*\
| MNTKeyboardController.h                                   |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <cstring>
#include <string>

#include "Detector.h"
#include "LogManager.h"

#define KBD_ROWS 6
#define KBD_COLOR_SIZE 3

#define CMD_PREFIX "xXRGB"
#define CMD_OFFSET (sizeof("xXRGB"))
#define CMD_PREFIX_LEN CMD_OFFSET - 1

#define KBD_VID 0x1209
#define KBD_INTERFACE 0
#define HID_USAGE_PAGE_DESKTOP 0x01
#define HID_USAGE_DESKTOP_KEYBOARD 0x06

class MNTKeyboardController
{
  public:
    ~MNTKeyboardController();
    void SendColorMatrix(unsigned char *color_map);
    std::string location;
    unsigned char kbd_cols;

  protected:
    hid_device *dev;
};
