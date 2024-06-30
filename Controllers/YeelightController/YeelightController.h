/*---------------------------------------------------------*\
| YeelightController.h                                      |
|                                                           |
|   Driver for Yeelight                                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <thread>
#include <vector>
#include "RGBController.h"
#include "net_port.h"

class YeelightController
{
public:
    YeelightController(std::string ip, std::string host_ip, bool music_mode_val);
    ~YeelightController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    bool        GetMusicMode();

    void SetMusicMode();
    void SetPower();
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string         location;
    std::string         host_ip;
    net_port            port;
    bool                music_mode;
    unsigned int        music_mode_port;
    net_port            music_mode_server;
    SOCKET *            music_mode_sock;
};
