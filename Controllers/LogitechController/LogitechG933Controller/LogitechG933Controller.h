/*---------------------------------------------------------*\
| LogitechG933Controller.h                                  |
|                                                           |
|   Driver for Logitech G933                                |
|                                                           |
|   Edbgon                                      21 Jun 2021 |
|   Based on TheRogueZeta                       31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LOGI_G933_LED_PACKET_SIZE               20
#define LOGI_G933_LED_COMMAND_SEND_RETRIES      3

enum
{
    LOGITECH_G933_MODE_OFF                      = 0x00,
    LOGITECH_G933_MODE_DIRECT                   = 0x01,
    LOGITECH_G933_MODE_CYCLE                    = 0x02,
    LOGITECH_G933_MODE_BREATHING                = 0x03,
};

class LogitechG933Controller
{
public:
    LogitechG933Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LogitechG933Controller();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetDirectMode(uint8_t zone);
    void        SetOffMode(uint8_t zone);

    void        SendHeadsetMode
                   (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                   );

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void        fail_retry_write(hid_device *device, const unsigned char *data, size_t length);
};


