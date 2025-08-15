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
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define PACKET_SIZE                 65

#define TOTAL_LED_BYTES             528
#define LED_BYTES_IN_CHUNK          56

#define SUBCOMMAND_NONE             0x00

#define MODE_OFFLINE                0x04
#define MODE_ONLINE                 0x05

#define LE_DEFINE_SET               0x01
#define LE_DEFINE_SAVE              0x02

class SkyloongGK104ProController
{
public:
    SkyloongGK104ProController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~SkyloongGK104ProController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void            Ping();
    void            SetMode(int mode);
    void            SendCommand(char command, char sub_command);
    void            SendColorPacket(std::vector<RGBColor> colors, std::vector<led> *leds, int brightness);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    uint16_t        Crc16CcittFalse(const uint8_t *buffer, uint16_t size);
    void            SetLEDefine(int address, unsigned char *le_data, int le_data_length);
    void            SaveLEDefine();
};
