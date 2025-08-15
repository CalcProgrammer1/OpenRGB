/*---------------------------------------------------------*\
| WootingOneKeyboardController.h                            |
|                                                           |
|   Driver for Wooting One keyboard                         |
|                                                           |
|   Diogo Trindade (diogotr7)                   04 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "WootingKeyboardController.h"

class WootingOneKeyboardController : public WootingKeyboardController
{
public:
    WootingOneKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type, std::string dev_name);
    ~WootingOneKeyboardController();

    void                SendDirect(RGBColor* colors, uint8_t colour_count);

private:
    void                SendInitialize();
    bool                wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* report_buffer);
};
