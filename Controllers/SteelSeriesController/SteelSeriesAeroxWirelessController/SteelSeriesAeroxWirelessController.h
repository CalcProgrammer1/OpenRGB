/*---------------------------------------------------------*\
| SteelSeriesAeroxWirelessController.h                     |
|                                                           |
|   Driver for SteelSeries Aerox 3, 5 and 9 Wireless        |
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

#define STEELSERIES_AEROX3_WIRELESS_NAME                    "SteelSeries Aerox 3 Wireless (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX3_WIRELESS_WIRED_NAME              "SteelSeries Aerox 3 Wireless (wired mode)"
#define STEELSERIES_AEROX5_WIRELESS_NAME                    "SteelSeries Aerox 5 Wireless (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX5_WIRELESS_WIRED_NAME              "SteelSeries Aerox 5 Wireless (wired mode)"
#define STEELSERIES_AEROX5_DESTINY_WIRELESS_NAME            "SteelSeries Aerox 5 Wireless Destiny 2 Edition (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX5_DESTINY_WIRELESS_WIRED_NAME      "SteelSeries Aerox 5 Wireless Destiny 2 Edition (wired mode)"
#define STEELSERIES_AEROX5_DIABLO_WIRELESS_NAME             "SteelSeries Aerox 5 Wireless Diablo IV Edition (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX5_DIABLO_WIRELESS_WIRED_NAME       "SteelSeries Aerox 5 Wireless Diablo IV Edition (wired mode)"
#define STEELSERIES_AEROX9_WIRELESS_NAME                    "SteelSeries Aerox 9 Wireless (2.4 GHz wireless mode)"
#define STEELSERIES_AEROX9_WIRELESS_WIRED_NAME              "SteelSeries Aerox 9 Wireless (wired mode)"
#define STEELSERIES_AEROX_WIRELESS_PACKET_SIZE              64
#define STEELSERIES_AEROX_WIRELESS_TIMEOUT                  250
#define STEELSERIES_AEROX_WIRELESS_FLAG                     0b01000000

static const steelseries_mouse aerox_3_wireless =
{
    {   0x04, 0x05  },
    {
        {"Front",           0 },
        {"Middle",          1 },
        {"Rear",            2 },
    }
};

static const steelseries_mouse aerox_9 =
{
    {   0x04, 0x05  },
    {
        {"Front",           0 },
        {"Middle",          1 },
        {"Rear",            2 },
        {"Reactive",        3 },
     }
};

class SteelSeriesAeroxWirelessController: public SteelSeriesMouseController
{
public:
    SteelSeriesAeroxWirelessController(hid_device* dev_handle, steelseries_type proto_type, const char* path, std::string dev_name);
    ~SteelSeriesAeroxWirelessController();

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
    bool                IsWireless();
    void                SendInit();
    void                WriteBuffer(uint8_t* buffer);
    void                SetBrightness(uint8_t brightness);
    uint8_t             current_brightness;
    const char*         name;
};
