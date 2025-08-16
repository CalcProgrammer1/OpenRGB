/*---------------------------------------------------------*\
| SteelSeriesAerox5Controller.h                             |
|                                                           |
|   Driver for the Steelseries Aerox 5                      |
|                                                           |
|   Bobby Quantum (BobbyQuantum)                19 May 2024 |
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

#define STEELSERIES_AEROX5_NAME "SteelSeries Aerox 5"
#define STEELSERIES_AEROX5_PACKET_SIZE 65
#define STEELSERIES_AEROX5_TIMEOUT 250

static const steelseries_mouse aerox_5 =
{
    {   0x04  },
    {
        {"Front",           0 },
        {"Middle",          1 },
        {"Rear",            2 },
        {"Reactive",        3 },
    }
};


class SteelSeriesAerox5Controller : public SteelSeriesMouseController
{
public:
    SteelSeriesAerox5Controller(hid_device *dev_handle, steelseries_type proto_type, const char *path, std::string dev_name);
    ~SteelSeriesAerox5Controller();

    std::string         GetFirmwareVersion() override;
    steelseries_mouse   GetMouse() override;

    void                Save() override;
    void                SetLightEffectAll(uint8_t effect) override;
    void                SetColor
                            (
                                unsigned char zone_id,
                                unsigned char red,
                                unsigned char green,
                                unsigned char blue,
                                unsigned char brightness
                            ) override;

private:
    void                SendInit();
    void                SetBrightness(uint8_t brightness);
    uint8_t             current_brightness;
};
