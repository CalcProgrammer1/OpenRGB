/*---------------------------------------------------------*\
| RGBController_MadCatzCyborg.cpp                           |
|                                                           |
|   RGB Controller for MadCatz Cyborg Gaming Light          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MadCatzCyborg.h"

/**--------------------------------------------------------*\
    @name MadCatz Cyborg Gaming Light
    @category Accessory
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectMadCatzCyborgControllers
    @comment The MadCatz Cyborg Gaming Light is an ambient lighting device.
\*---------------------------------------------------------*/

RGBController_MadCatzCyborg::RGBController_MadCatzCyborg(MadCatzCyborgController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "MadCatz Cyborg Gaming Light";
    vendor      = "MadCatz";
    type        = DEVICE_TYPE_ACCESSORY;
    description = "MadCatz Cyborg Gaming Light";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = 0;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness_max = 100;
    Direct.brightness     = 100;
    modes.push_back(Direct);

    SetupZones();

    controller->SetIntensity(modes[active_mode].brightness);
}

RGBController_MadCatzCyborg::~RGBController_MadCatzCyborg()
{
    delete controller;
}

void RGBController_MadCatzCyborg::SetupZones()
{
    zone cyborg_zone;

    cyborg_zone.name       = "Cyborg";
    cyborg_zone.type       = ZONE_TYPE_SINGLE;
    cyborg_zone.leds_min   = 1;
    cyborg_zone.leds_max   = 1;
    cyborg_zone.leds_count = 1;
    cyborg_zone.matrix_map = NULL;

    zones.push_back(cyborg_zone);

    led cyborg_led;
    cyborg_led.name = "LED";
    leds.push_back(cyborg_led);

    SetupColors();
}

void RGBController_MadCatzCyborg::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Single LED device - nothing to resize
}

void RGBController_MadCatzCyborg::DeviceUpdateLEDs()
{
    if(colors.size() > 0)
    {
        RGBColor color = colors[0];
        controller->SetLEDColor(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
    }
}

void RGBController_MadCatzCyborg::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MadCatzCyborg::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MadCatzCyborg::DeviceUpdateMode()
{
    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetIntensity(modes[active_mode].brightness);
    }
}
