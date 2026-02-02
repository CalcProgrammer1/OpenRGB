/*---------------------------------------------------------*\
| RGBController_RoccatBurst.cpp                             |
|                                                           |
|   RGBController for Roccat Burst                          |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RoccatBurst.h"

/**------------------------------------------------------------------*\
    @name Roccat Burst Mouse
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatBurstCoreControllers,DetectRoccatBurstProControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatBurst::RGBController_RoccatBurst(RoccatBurstController* controller_ptr, unsigned int leds_count):
    leds_count(leds_count)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Burst Mouse Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_BURST_DIRECT_MODE_VALUE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_BURST_STATIC_MODE_VALUE;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness           = ROCCAT_BURST_BRIGHTNESS_MAX;
    Static.brightness_min       = ROCCAT_BURST_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_BURST_BRIGHTNESS_MAX;
    Static.colors.resize(leds_count);
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ROCCAT_BURST_WAVE_MODE_VALUE;
    Rainbow.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = ROCCAT_BURST_BRIGHTNESS_MAX;
    Rainbow.brightness_min      = ROCCAT_BURST_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ROCCAT_BURST_BRIGHTNESS_MAX;
    Rainbow.speed               = ROCCAT_BURST_SPEED_MIN;
    Rainbow.speed_min           = ROCCAT_BURST_SPEED_MIN;
    Rainbow.speed_max           = ROCCAT_BURST_SPEED_MAX;
    modes.push_back(Rainbow);

    mode HeartBeat;
    HeartBeat.name              = "HeartBeat";
    HeartBeat.value             = ROCCAT_BURST_HEARTBEAT_MODE_VALUE;
    HeartBeat.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    HeartBeat.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    HeartBeat.brightness        = ROCCAT_BURST_BRIGHTNESS_MAX;
    HeartBeat.brightness_min    = ROCCAT_BURST_BRIGHTNESS_MIN;
    HeartBeat.brightness_max    = ROCCAT_BURST_BRIGHTNESS_MAX;
    HeartBeat.speed             = ROCCAT_BURST_SPEED_MIN;
    HeartBeat.speed_min         = ROCCAT_BURST_SPEED_MIN;
    HeartBeat.speed_max         = ROCCAT_BURST_SPEED_MAX;
    HeartBeat.colors.resize(leds_count);
    modes.push_back(HeartBeat);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_BURST_BREATHING_MODE_VALUE;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness        = ROCCAT_BURST_BRIGHTNESS_MAX;
    Breathing.brightness_min    = ROCCAT_BURST_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROCCAT_BURST_BRIGHTNESS_MAX;
    Breathing.speed             = ROCCAT_BURST_SPEED_MIN;
    Breathing.speed_min         = ROCCAT_BURST_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_BURST_SPEED_MAX;
    Breathing.colors.resize(leds_count);
    modes.push_back(Breathing);

    mode Blinking;
    Blinking.name               = "Blinking";
    Blinking.value              = ROCCAT_BURST_BLINKING_MODE_VALUE;
    Blinking.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blinking.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Blinking.brightness         = ROCCAT_BURST_BRIGHTNESS_MAX;
    Blinking.brightness_min     = ROCCAT_BURST_BRIGHTNESS_MIN;
    Blinking.brightness_max     = ROCCAT_BURST_BRIGHTNESS_MAX;
    Blinking.speed              = ROCCAT_BURST_SPEED_MIN;
    Blinking.speed_min          = ROCCAT_BURST_SPEED_MIN;
    Blinking.speed_max          = ROCCAT_BURST_SPEED_MAX;
    Blinking.colors.resize(leds_count);
    modes.push_back(Blinking);

    SetupZones();
}

RGBController_RoccatBurst::~RGBController_RoccatBurst()
{
    Shutdown();

    delete controller;
}

void RGBController_RoccatBurst::SetupZones()
{
    zone new_zone;
    new_zone.name          = "Mouse";
    new_zone.type          = ZONE_TYPE_LINEAR;
    new_zone.leds_min      = leds_count;
    new_zone.leds_max      = leds_count;
    new_zone.leds_count    = leds_count;
    zones.push_back(new_zone);

    std::string led_names[2] =
    {
        "Scroll Wheel",
        "Logo"
    };

    for(unsigned int i = 0; i < leds_count; i++)
    {
        led new_led;
        new_led.name = led_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatBurst::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_RoccatBurst::DeviceUpdateZoneLEDs(int /*zone_idx*/)
{
    const mode& active = modes[active_mode];

    if(active.value == ROCCAT_BURST_DIRECT_MODE_VALUE)
    {
        controller->SendDirect(colors);
    }
    else
    {
        controller->SetMode(active.colors, active.value, active.speed, active.brightness, active.color_mode, active.flags);
    }

}

void RGBController_RoccatBurst::DeviceUpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_RoccatBurst::DeviceUpdateMode()
{
    if(modes[active_mode].value == ROCCAT_BURST_DIRECT_MODE_VALUE)
    {
        controller->SetupDirectMode();
    }
    else
    {
        DeviceUpdateLEDs();
    }
}
