/*---------------------------------------------------------*\
| GameSirController.h                                       |
|                                                           |
|   GameSir RGB Device                                      |
|                                                           |
|   Added by OpenRGB Community                  08 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

class GameSirController
{
public:
    GameSirController(hid_device* dev_handle, const char* path);
    ~GameSirController();

    std::string GetLocation();
    std::string GetSerialString();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device* dev;
    std::string location;
};
