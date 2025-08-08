/*---------------------------------------------------------*\
| GaiZhongGaiController.h                                   |
|                                                           |
|   Driver for GaiZhongGai keyboard                         |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| GaiZhongGai vendor ID                                 |
\*-----------------------------------------------------*/
#define GAIZHONGGAI_VID                 0x3061

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define GAIZHONGGAI_68_PRO_PID          0x4700
#define GAIZHONGGAI_42_PRO_PID          0x4701
#define GAIZHONGGAI_17_TOUCH_PRO_PID    0x4770
#define GAIZHONGGAI_17_PRO_PID          0x4771
#define GAIZHONGGAI_20_PRO_PID          0x4772

/*-----------------------------------------------------*\
| Other product IDs                                     |
\*-----------------------------------------------------*/
#define GAIZHONGGAI_LIGHT_BOARD_PID     0x4710
#define GAIZHONGGAI_RGB_HUB_GREEN_PID   0x4711
#define GAIZHONGGAI_RGB_HUB_BLUE_PID    0x4712
#define GAIZHONGGAI_DIAL_PID            0x4720

class GaiZhongGaiKeyboardController
{
public:
    GaiZhongGaiKeyboardController(hid_device* dev_handle, hid_device_info* info, std::string dev_name);
    ~GaiZhongGaiKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();
    std::string     GetVersion();
    unsigned short  GetUSBPID();
    uint8_t*        GetDataFlash();
    uint16_t        GetChannelLen(uint8_t ch);
    void            SetChannelLen(uint8_t ch , uint16_t len);

    void SendColors
    (
    unsigned char*  color_data,
    unsigned int    color_data_size
    );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
    std::string             version;
    unsigned short          usb_pid;
    uint8_t                 data_flash[128];
};
