/*---------------------------------------------------------*\
| WootingV3KeyboardController.h                             |
|                                                           |
|   Driver for Wooting keyboards with v3 firmware           |
|                                                           |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "WootingKeyboardController.h"

class WootingV3KeyboardController : public WootingKeyboardController
{
public:
    WootingV3KeyboardController(hid_device* dev_handle, const char *path, WOOTING_DEVICE_TYPE wooting_type, std::string dev_name);
    ~WootingV3KeyboardController();

    void                SendDirect(RGBColor* colors, uint8_t colour_count);
};
