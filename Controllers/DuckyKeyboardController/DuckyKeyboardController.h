/*---------------------------------------------------------*\
| DuckyKeyboardController.h                                 |
|                                                           |
|   Driver for Ducky keyboard                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| Ducky vendor ID                                       |
\*-----------------------------------------------------*/
#define DUCKY_VID                       0x04D9

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define DUCKY_SHINE_7_ONE_2_RGB_PID     0x0348
#define DUCKY_ONE_2_RGB_TKL_PID         0x0356

class DuckyKeyboardController
{
public:
    DuckyKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name);
    ~DuckyKeyboardController();

    std::string     GetLocationString();
    std::string     GetNameString();
    std::string     GetSerialString();
    unsigned short  GetUSBPID();

    void SendColors
            (
            unsigned char*  color_data,
            unsigned int    color_data_size
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
    unsigned short          usb_pid;

    void SendInitialize();
    void SendInitializeColorPacket();
    unsigned int SendColorDataPacket
        (
        unsigned char   packet_id,
        unsigned char*  color_data,
        unsigned int    color_size
        );
    void SendTerminateColorPacket();
};
