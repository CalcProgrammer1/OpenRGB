/*---------------------------------------------------------*\
| LogitechG560Controller.h                                  |
|                                                           |
|   Driver for Logitech G560                                |
|                                                           |
|   Cheerpipe                                   28 Oct 2020 |
|   based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi/hidapi.h>
#include "RGBController.h"

#define LOGI_G560_LED_PACKET_SIZE               20
#define LOGI_G560_LED_COMMAND_SEND_RETRIES      3

enum
{
    LOGITECH_G560_MODE_OFF                      = 0x00,
    LOGITECH_G560_MODE_DIRECT                   = 0x01,
    LOGITECH_G560_MODE_CYCLE                    = 0x02,
    LOGITECH_G560_MODE_BREATHING                = 0x03,
};

class LogitechG560Controller
{
public:
    LogitechG560Controller(hid_device* dev_handle, const char* path);
    ~LogitechG560Controller();

    std::string GetDeviceLocation();

    void SetDirectMode(uint8_t zone);
    void SetOffMode(uint8_t zone);

    void        SendSpeakerMode
                   (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                   );

private:
    hid_device*             dev;
    std::string             location;

    void fail_retry_write(hid_device *device, const unsigned char *data, size_t length);
};


