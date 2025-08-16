/*---------------------------------------------------------*\
| SteelSeriesOldApexController.h                            |
|                                                           |
|   Driver for older SteelSeries Apex keyboards             |
|   (Apex/Apex Fnatic/Apex 350)                             |
|                                                           |
|   Based on findings in ApexCtl by Audrius/tuxmark5, et.   |
|   al, https://github.com/tuxmark5/ApexCtl                 |
|                                                           |
|   David Lee (RAMChYLD)                        15 Nov 2020 |
|   Based on work by B Horn (bahorn)            13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "color32.h"
#include "SteelSeriesGeneric.h"

/* Mode, we then use these to set actual effect based on speed. */
enum
{
    STEELSERIES_OLDAPEX_DIRECT = 0x08,
};

/* Effects */
enum
{
    STEELSERIES_OLDAPEX_EFFECT_DIRECT = 0x08,
};

class SteelSeriesOldApexController
{
public:
    SteelSeriesOldApexController
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path,
        std::string         dev_name
        );

    ~SteelSeriesOldApexController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    steelseries_type GetKeyboardType();

    void SetColorDetailed
        (
            color32 qwerty,
            color32 tenkey,
            color32 functionkey,
            color32 mxkey,
            color32 logo
        );

    void DoUpdateLEDs();

private:
    hid_device*         dev;
    std::string         location;
    std::string         name;
    steelseries_type    proto;
};
