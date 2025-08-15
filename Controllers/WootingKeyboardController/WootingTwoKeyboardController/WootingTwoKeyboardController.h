/*---------------------------------------------------------*\
| WootingTwoKeyboardController.h                            |
|                                                           |
|   Driver for Wooting Two keyboard                         |
|                                                           |
|   Chris M (Dr_No)                             09 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "WootingKeyboardController.h"

class WootingTwoKeyboardController : public WootingKeyboardController
{
public:
    WootingTwoKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type, std::string dev_name);
    ~WootingTwoKeyboardController();

    void                SendDirect(RGBColor* colors, uint8_t colour_count);

private:
    void                SendInitialize();
    bool                wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* report_buffer);
};
