/*---------------------------------------------------------*\
| RGBController_GenesisXenon200.h                           |
|                                                           |
|   RGBController for Genesis Xenon 200 mouse               |
|                                                           |
|   chrabonszcz                                    Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "GenesisXenon200Controller.h"
#include "RGBController.h"

#define GENESIS_XENON_200_MODE_STATIC         0x18
#define GENESIS_XENON_200_MODE_BREATHING      0x12
#define GENESIS_XENON_200_MODE_SPECTRUM_CYCLE 0x14
#define GENESIS_XENON_200_MODE_OFF            0x11

const unsigned char GENESIS_XENON_200_STATIC_BRIGHTESS_VALUES[]       {0x11, 0x51, 0xA1};
const unsigned char GENESIS_XENON_200_BREATHING_SPEED_VALUES[]        {0x51, 0x31, 0x11};
const unsigned char GENESIS_XENON_200_SPECTRUM_CYCLE_SPEED_VALUES[]   {0xC1, 0x81, 0x41};

class RGBController_GenesisXenon200 : public RGBController
{
public:
    RGBController_GenesisXenon200(GenesisXenon200Controller* controller_ptr);
    ~RGBController_GenesisXenon200();

    void            SetupZones();

    void            ResizeZone(int zone, int new_size);

    void            DeviceUpdateLEDs();
    void            UpdateZoneLEDs(int zone);
    void            UpdateSingleLED(int led);

    void            DeviceUpdateMode();
    void            DeviceSaveMode();

private:
    GenesisXenon200Controller* controller;
};
