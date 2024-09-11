/*---------------------------------------------------------*\
| SteelSeriesAerox3WirelessController.h                     |
|                                                           |
|   Driver for SteelSeries Aerox 3 Wireless                 |
|                                                           |
|   Ensar S (esensar)                           09 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesMouseController.h"

#define STEELSERIES_AEROX3_WIRELESS_NAME            "SteelSeries Aerox 3 Wireless (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX3_WIRELESS_WIRED_NAME      "SteelSeries Aerox 3 Wireless (wired mode)"
#define STEELSERIES_AEROX3_PACKET_SIZE              64
#define STEELSERIES_AEROX3_TIMEOUT                  250
#define STEELSERIES_AEROX3_WIRELESS_FLAG            0b01000000

static const steelseries_mouse aerox_3_wireless =
{
    {   0x04, 0x05  },
    {
        {"Front",           0 },
        {"Middle",          1 },
        {"Rear",            2 },
    }
};

class SteelSeriesAerox3WirelessController: public SteelSeriesMouseController
{
public:
    SteelSeriesAerox3WirelessController(hid_device* dev_handle, steelseries_type proto_type, const char* path);
    ~SteelSeriesAerox3WirelessController();

    std::string         GetFirmwareVersion() override;
    steelseries_mouse   GetMouse() override;

    void                Save() override;
    void                SetLightEffectAll(uint8_t effect) override;
    void                SetColor
                            (
                            unsigned char   zone_id,
                            unsigned char   red,
                            unsigned char   green,
                            unsigned char   blue,
                            unsigned char   brightness
                            ) override;
private:
    void                SendInit();
    void                WriteBuffer(uint8_t* buffer);
    void                SetBrightness(uint8_t brightness);
    uint8_t             current_brightness;
};
