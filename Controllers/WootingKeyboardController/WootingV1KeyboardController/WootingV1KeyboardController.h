/*---------------------------------------------------------*\
| WootingV1KeyboardController.h                             |
|                                                           |
|   Driver for Wooting keyboards with v1 firmware           |
|                                                           |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "WootingKeyboardController.h"

#define WOOTING_REPORT_SIZE             129
#define WOOTING_ONE_KEY_CODE_LIMIT      95
#define WOOTING_TWO_KEY_CODE_LIMIT      116

class WootingV1KeyboardController : public WootingKeyboardController
{
public:
    WootingV1KeyboardController(hid_device* dev_handle, const char *path, WOOTING_DEVICE_TYPE wooting_type, std::string dev_name);
    ~WootingV1KeyboardController();

    uint8_t             key_code_limit;

    void                SendDirect(RGBColor* colors, uint8_t colour_count);

private:
    bool                wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* report_buffer);
};
