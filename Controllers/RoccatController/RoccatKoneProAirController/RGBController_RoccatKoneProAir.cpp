/*---------------------------------------------------------*\
| RGBController_RoccatKoneProAir.cpp                        |
|                                                           |
|   RGBController for Roccat Kone Pro Air                   |
|                                                           |
|   Plunti                                      10 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RoccatKoneProAir.h"

/**------------------------------------------------------------------*\
    @name Roccat Kone Pro Air Mouse
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatKoneProAirControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatKoneProAir::RGBController_RoccatKoneProAir(RoccatKoneProAirController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Kone Pro Air Mouse Device";
    serial                      = controller->GetSerialString();
    location                    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_KONE_PRO_AIR_DIRECT_MODE_VALUE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ROCCAT_KONE_PRO_AIR_OFF_MODE_VALUE;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_KONE_PRO_AIR_STATIC_MODE_VALUE;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness           = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Static.brightness_min       = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = ROCCAT_KONE_PRO_AIR_RAINBOW_WAVE_MODE_VALUE;
    RainbowWave.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    RainbowWave.brightness      = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    RainbowWave.brightness_min  = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN;
    RainbowWave.brightness_max  = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    RainbowWave.speed           = ROCCAT_KONE_PRO_AIR_SPEED_MID;
    RainbowWave.speed_min       = ROCCAT_KONE_PRO_AIR_SPEED_MIN;
    RainbowWave.speed_max       = ROCCAT_KONE_PRO_AIR_SPEED_MAX;
    modes.push_back(RainbowWave);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = ROCCAT_KONE_PRO_AIR_HEARTBEAT_MODE_VALUE;
    Heartbeat.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Heartbeat.color_mode        = MODE_COLORS_PER_LED;
    Heartbeat.brightness        = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Heartbeat.brightness_min    = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN;
    Heartbeat.brightness_max    = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Heartbeat.speed             = ROCCAT_KONE_PRO_AIR_SPEED_MID;
    Heartbeat.speed_min         = ROCCAT_KONE_PRO_AIR_SPEED_MIN;
    Heartbeat.speed_max         = ROCCAT_KONE_PRO_AIR_SPEED_MAX;
    modes.push_back(Heartbeat);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_KONE_PRO_AIR_BREATHING_MODE_VALUE;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness        = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Breathing.brightness_min    = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Breathing.speed             = ROCCAT_KONE_PRO_AIR_SPEED_MID;
    Breathing.speed_min         = ROCCAT_KONE_PRO_AIR_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_KONE_PRO_AIR_SPEED_MAX;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = ROCCAT_KONE_PRO_AIR_FLASHING_MODE_VALUE;
    Flashing.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Flashing.color_mode         = MODE_COLORS_PER_LED;
    Flashing.brightness         = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Flashing.brightness_min     = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MIN;
    Flashing.brightness_max     = ROCCAT_KONE_PRO_AIR_BRIGHTNESS_MAX;
    Flashing.speed              = ROCCAT_KONE_PRO_AIR_SPEED_MID;
    Flashing.speed_min          = ROCCAT_KONE_PRO_AIR_SPEED_MIN;
    Flashing.speed_max          = ROCCAT_KONE_PRO_AIR_SPEED_MAX;
    modes.push_back(Flashing);

    mode Battery;
    Battery.name                = "Battery";
    Battery.value               = ROCCAT_KONE_PRO_AIR_BATTERY_MODE_VALUE;
    Battery.flags               = MODE_FLAG_AUTOMATIC_SAVE;
    Battery.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Battery);

    SetupZones();
}

RGBController_RoccatKoneProAir::~RGBController_RoccatKoneProAir()
{
    delete controller;
}

void RGBController_RoccatKoneProAir::SetupZones()
{
    zone new_zone;
    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = ROCCAT_KONE_PRO_AIR_LED_COUNT;
    new_zone.leds_max   = ROCCAT_KONE_PRO_AIR_LED_COUNT;
    new_zone.leds_count = ROCCAT_KONE_PRO_AIR_LED_COUNT;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    std::string led_names[2] =
    {
        "Left Button",
        "Right Button"
    };

    for(unsigned int i = 0; i < ROCCAT_KONE_PRO_AIR_LED_COUNT; i++)
    {
        led new_led;
        new_led.name = led_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatKoneProAir::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatKoneProAir::DeviceUpdateLEDs()
{
    const mode& active = modes[active_mode];

    if(active.value == ROCCAT_KONE_PRO_AIR_DIRECT_MODE_VALUE)
    {
        controller->SendDirect(colors);
    }
    else
    {
        controller->SetMode(colors, active.value, active.speed, active.brightness, active.flags);
    }
}

void RGBController_RoccatKoneProAir::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneProAir::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneProAir::DeviceUpdateMode()
{
    const mode& active = modes[active_mode];

    if(!(active.flags & MODE_FLAG_HAS_PER_LED_COLOR))
    {
        DeviceUpdateLEDs();
    }
}
