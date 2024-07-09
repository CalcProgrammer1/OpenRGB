/*---------------------------------------------------------*\
| SkyloongGK104ProController.h                              |
|                                                           |
|   Driver for Skyloong GK104 Pro                           |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi/hidapi.h>

class SkyloongGK104ProController
{
public:
    SkyloongGK104ProController(hid_device* dev_handle, const char* path);
    ~SkyloongGK104ProController();

    std::string     GetDeviceLocation();

    void            Ping();
    void            SetMode(int mode);
    void            SendCommand(char command, char sub_command);
    void            SendColorPacket(std::vector<RGBColor> colors, std::vector<led> *leds, int brightness);

private:
    hid_device*             dev;
    std::string             location;
	
    uint16_t                Crc16CcittFalse(const uint8_t *buffer, uint16_t size);
    void                    SetLEDefine(int address, unsigned char *le_data, int le_data_length);
    void                    SaveLEDefine();
};
