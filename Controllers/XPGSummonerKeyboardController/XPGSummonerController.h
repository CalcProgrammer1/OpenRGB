/*---------------------------------------------------------*\
| XPGSummonerController.h                                   |
|                                                           |
|   Driver for XPG Summoner keyboard                        |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "hidapi.h"
#include "RGBController.h"

/*-----------------------------------------------------*\
| XPG vendor ID                                         |
\*-----------------------------------------------------*/
#define XPG_VID 0x125F

/*-----------------------------------------------------*\
| Keyboard product ID                                   |
\*-----------------------------------------------------*/
#define XPG_SUMMONER_PID 0x9418

enum
{
    XPG_SUMMONER_MODE_OFF           =    0x00,
    XPG_SUMMONER_MODE_DIRECT        =    0x01,
    XPG_SUMMONER_MODE_STATIC        =    0x02,
    XPG_SUMMONER_MODE_STARS         =    0x03
};

class XPGSummonerController
{
public:
    XPGSummonerController(hid_device *dev_handle, const char *path, const unsigned short pid, std::string dev_name);
    ~XPGSummonerController();

    std::string GetLocationString();
    std::string GetNameString();
    std::string GetSerialString();
    unsigned short GetUSBPID();

    void SendColors
    (
        unsigned char *color_data,
        unsigned int color_data_size
    );

    unsigned int SendColorDataPacket
    (
        unsigned char packet_id,
        unsigned char *color_data,
        unsigned int color_size
    );

    void SendTerminateColorPacket();

    void SendInitialize();

private:
    hid_device *dev;
    std::string location;
    std::string name;
    unsigned short usb_pid;
};
