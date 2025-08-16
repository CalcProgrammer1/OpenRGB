/*---------------------------------------------------------*\
| RGBController_SteelSeriesQCKMat.cpp                       |
|                                                           |
|   RGBController for SteelSeries Mouse                     |
|                                                           |
|   Edbgon                                      22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesQCKMat.h"

/**------------------------------------------------------------------*\
    @name Steel Series QCK Mat
    @category Mousemat
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSteelSeriesMousemat
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesQCKMat::RGBController_SteelSeriesQCKMat(SteelSeriesQCKMatController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "SteelSeries";
    type                = DEVICE_TYPE_MOUSEMAT;
    description         = "SteelSeries QCK Mat Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    mode Direct;
    Direct.name         = "Direct";
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_SteelSeriesQCKMat::~RGBController_SteelSeriesQCKMat()
{
    delete controller;
}

void RGBController_SteelSeriesQCKMat::SetupZones()
{
    /*---------------------------------------------------------*\
    | QCK has two zones                                         |
    \*---------------------------------------------------------*/
    zone mousemat_zone;
    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_SINGLE;
    mousemat_zone.leds_min      = 2;
    mousemat_zone.leds_max      = 2;
    mousemat_zone.leds_count    = 2;
    mousemat_zone.matrix_map    = NULL;
    zones.push_back(mousemat_zone);

    led bot_led;
    bot_led.name = "Mat Bottom LED";
    leds.push_back(bot_led);

    led top_led;
    top_led.name = "Mat Top LED";
    leds.push_back(top_led);

    SetupColors();
}

void RGBController_SteelSeriesQCKMat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesQCKMat::DeviceUpdateLEDs()
{
    controller->SetColors(colors);
}

void RGBController_SteelSeriesQCKMat::UpdateZoneLEDs(int /*zone*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesQCKMat::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesQCKMat::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
