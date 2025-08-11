/*---------------------------------------------------------*\
| HyperXPulsefireSurgeController.h                          |
|                                                           |
|   Driver for HyperX Pulsefire Surge                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    HYPERX_PULSEFIRE_SURGE_PACKET_ID_SET_CONFIGURATION  = 0x01,     /* Set profile configuration packet     */
    HYPERX_PULSEFIRE_SURGE_PACKET_ID_SET_BRIGHTNESS     = 0x03,     /* Set profile settings and brightness  */
    HYPERX_PULSEFIRE_SURGE_PACKET_ID_SELECT_PROFILE     = 0x07,     /* Select profile                       */
    HYPERX_PULSEFIRE_SURGE_PACKET_ID_DIRECT             = 0x14,     /* Direct control packet                */
};

enum
{
    HYPERX_PULSEFIRE_SURGE_MODE_SOLID                   = 0x00,     /* Solid color mode                     */
    HYPERX_PULSEFIRE_SURGE_MODE_CYCLE                   = 0x01,     /* Spectrum cycle mode                  */
    HYPERX_PULSEFIRE_SURGE_MODE_BREATHING               = 0x02,     /* Breathing mode                       */
    HYPERX_PULSEFIRE_SURGE_MODE_WAVE                    = 0x03,     /* Wave mode                            */
    HYPERX_PULSEFIRE_SURGE_MODE_TRIGGER                 = 0x04,     /* Trigger mode                         */
};

class HyperXPulsefireSurgeController
{
public:
    HyperXPulsefireSurgeController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HyperXPulsefireSurgeController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SelectProfile
            (
            unsigned char   profile
            );

    void SetProfileBrightness
            (
            unsigned char   profile,
            unsigned char   brightness
            );

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;
};
