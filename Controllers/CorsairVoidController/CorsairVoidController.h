/*---------------------------------------------------------*\
| CorsairVoidController.h                                   |
|                                                           |
|   Driver for Corsair Void Pro headsets                    |
|                                                           |
|   Derek Ross                                  15 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*---------------------------------------------------------*\
| Lighting control report: C8 01 hands lighting to the      |
| host, C8 00 returns it to the headset firmware.  The mic  |
| mute LED only works while the firmware has control.       |
\*---------------------------------------------------------*/
#define CORSAIR_VOID_LIGHTING_REPORT_ID         0xC8
#define CORSAIR_VOID_LIGHTING_REPORT_SIZE       3
#define CORSAIR_VOID_LIGHTING_FIRMWARE          0x00
#define CORSAIR_VOID_LIGHTING_HOST              0x01

/*---------------------------------------------------------*\
| A wireless headset that is switched off and on returns to |
| firmware lighting, so host control is re-requested at     |
| this interval while colors are being sent                 |
\*---------------------------------------------------------*/
#define CORSAIR_VOID_LIGHTING_REFRESH_MS        1000

/*---------------------------------------------------------*\
| Register write report: CB <count> followed by <count>     |
| (register, value) pairs                                   |
\*---------------------------------------------------------*/
#define CORSAIR_VOID_REGISTER_REPORT_ID         0xCB
#define CORSAIR_VOID_REGISTER_REPORT_SIZE       20

enum
{
    CORSAIR_VOID_REGISTER_LEFT_GREEN    = 0x16,
    CORSAIR_VOID_REGISTER_LEFT_BLUE     = 0x17,
    CORSAIR_VOID_REGISTER_RIGHT_GREEN   = 0x18,
    CORSAIR_VOID_REGISTER_RIGHT_BLUE    = 0x19,
    CORSAIR_VOID_REGISTER_LEFT_RED      = 0x1C,
    CORSAIR_VOID_REGISTER_RIGHT_RED     = 0x1D,
};

class CorsairVoidController
{
public:
    CorsairVoidController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~CorsairVoidController();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();

    void                SetColors(RGBColor left, RGBColor right);

private:
    hid_device*         dev;
    std::string         location;
    std::string         name;
    bool                host_control;
    std::chrono::steady_clock::time_point last_host_request;

    void                SetLightingControl(unsigned char owner);
};
