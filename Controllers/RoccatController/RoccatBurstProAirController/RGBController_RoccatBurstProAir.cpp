/*---------------------------------------------------------*\
| RGBController_RoccatBurstProAir.cpp                       |
|                                                           |
|   RGBController for Roccat Burst Pro Air                  |
|                                                           |
|   Morgan Guimard (morg)                       16 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RoccatBurstProAir.h"

/**------------------------------------------------------------------*\
    @name Roccat Burst Pro Air
    @category Mouse
    @type USB
    @save :warning:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatBurstProAirCoreControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatBurstProAir::RGBController_RoccatBurstProAir(RoccatBurstProAirController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Burst Pro Air Mouse Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_BURST_PRO_AIR_DIRECT_MODE_VALUE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness           = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Direct.brightness_min       = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MIN;
    Direct.brightness_max       = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Blink;
    Blink.name                 = "Blink";
    Blink.value                = ROCCAT_BURST_PRO_AIR_BLINK_MODE_VALUE;
    Blink.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.color_mode           = MODE_COLORS_PER_LED;
    Blink.brightness           = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Blink.brightness_min       = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MIN;
    Blink.brightness_max       = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Blink.speed                = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Blink.speed_min            = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Blink.speed_max            = ROCCAT_BURST_PRO_AIR_SPEED_MAX;
    modes.push_back(Blink);

    mode Breath;
    Breath.name                = "Breathing";
    Breath.value               = ROCCAT_BURST_PRO_AIR_BREATH_MODE_VALUE;
    Breath.flags               = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breath.color_mode          = MODE_COLORS_PER_LED;
    Breath.brightness          = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Breath.brightness_min      = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MIN;
    Breath.brightness_max      = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Breath.speed               = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Breath.speed_min           = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Breath.speed_max           = ROCCAT_BURST_PRO_AIR_SPEED_MAX;
    modes.push_back(Breath);

    mode Wave;
    Wave.name                  = "Wave";
    Wave.value                 = ROCCAT_BURST_PRO_AIR_WAVE_MODE_VALUE;
    Wave.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Wave.color_mode            = MODE_COLORS_PER_LED;
    Wave.brightness            = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Wave.brightness_min        = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MIN;
    Wave.brightness_max        = ROCCAT_BURST_PRO_AIR_BRIGHTNESS_MAX;
    Wave.speed                 = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Wave.speed_min             = ROCCAT_BURST_PRO_AIR_SPEED_MIN;
    Wave.speed_max             = ROCCAT_BURST_PRO_AIR_SPEED_MAX;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_RoccatBurstProAir::~RGBController_RoccatBurstProAir()
{
    delete controller;
}

void RGBController_RoccatBurstProAir::SetupZones()
{
    zone new_zone;
    new_zone.name          = "Mouse";
    new_zone.type          = ZONE_TYPE_LINEAR;
    new_zone.leds_min      = ROCCAT_BURST_PRO_AIR_PRO_NUMBER_OF_LEDS;
    new_zone.leds_max      = ROCCAT_BURST_PRO_AIR_PRO_NUMBER_OF_LEDS;
    new_zone.leds_count    = ROCCAT_BURST_PRO_AIR_PRO_NUMBER_OF_LEDS;
    zones.push_back(new_zone);

    std::string led_names[ROCCAT_BURST_PRO_AIR_PRO_NUMBER_OF_LEDS] =
    {
        "Scroll Wheel",
        "Logo",
        "Left button",
        "Right button"
    };

    for(unsigned int i = 0; i < ROCCAT_BURST_PRO_AIR_PRO_NUMBER_OF_LEDS; i++)
    {
        led new_led;
        new_led.name = led_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatBurstProAir::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_RoccatBurstProAir::DeviceUpdateZoneLEDs(int /*zone_idx*/)
{
    controller->SetColors(colors);
}

void RGBController_RoccatBurstProAir::DeviceUpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateZoneLEDs(0);
}

void RGBController_RoccatBurstProAir::DeviceUpdateMode()
{
    const mode& active = modes[active_mode];
    controller->SetModeValues(active.value, active.speed, active.brightness);
}
