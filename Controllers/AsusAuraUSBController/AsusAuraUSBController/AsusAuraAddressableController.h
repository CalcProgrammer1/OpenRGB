/*---------------------------------------------------------*\
| AsusAuraAddressableController.h                           |
|                                                           |
|   Driver for ASUS Aura addressable controller             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "AsusAuraUSBController.h"

enum
{
    AURA_ADDRESSABLE_CONTROL_MODE_EFFECT            = 0x3B,     /* Effect control mode                  */
};

class AuraAddressableController : public AuraUSBController
{
public:
    AuraAddressableController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AuraAddressableController();

    void SetChannelLEDs
        (
        unsigned char   channel,
        RGBColor *      colors,
        unsigned int    num_colors
        );

    void SetMode
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );

private:

    void SendEffect
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );
};
