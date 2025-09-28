/*---------------------------------------------------------*\
| BloodyB820RController.h                                   |
|                                                           |
|   Driver for A4Tech Bloody B820R Keyboard                 |
|                                                           |
|   Mohammed Julfikar Ali Mahbub (o-julfikar)   01 Apr 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define HID_MAX_STR                     255

#define BLOODY_B820R_RGB_BUFFER_SIZE    58
#define BLOODY_B820R_RGB_OFFSET         6
#define BLOODY_B820R_PACKET_SIZE        64
#define BLOODY_B820R_KEYCOUNT           104
#define BLOODY_B820R_MODE_BYTE          3
#define BLOODY_B820R_DATA_BYTE          8
#define BLOODY_B820R_GAIN_CONTROL       0x01
#define BLOODY_B820R_RELEASE_CONTROL    0x00

/*---------------------------------------------------------*\
| Bloody B820R product ID                                   |
\*---------------------------------------------------------*/
#define BLOODY_B820R_PID                            0xFA10

enum
{
    BLOODY_B820R_MODE_DIRECT     = 0x01,    // Direct LED control - Independently set LEDs in zone
};

class BloodyB820RController
{
public:
    BloodyB820RController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~BloodyB820RController();

    std::string     GetSerial();
    std::string     GetLocation();
    std::string     GetName();

    void            SetLEDDirect(std::vector<RGBColor> colors);
    void            SendControlPacket(uint8_t data);
private:
    std::string     location;
    std::string     name;
    hid_device*     dev;
};
