/*---------------------------------------------------------*\
| RGBController_RoccatKonePro.cpp                           |
|                                                           |
|   RGBController for Roccat Kone Pro                       |
|                                                           |
|   Garrett Denham (GardenOfWyers)              12 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RoccatKonePro.h"

/**------------------------------------------------------------------*\
    @name Roccat Kone Pro Mouse
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatKoneProControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatKonePro::RGBController_RoccatKonePro(RoccatKoneProController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Kone Pro Mouse Device";
    serial                      = controller->GetSerialString();
    location                    = controller->GetDeviceLocation();

    // Also known as "Intelligent Lighting System" mode in Roccat Swarm
    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_KONE_PRO_DIRECT_MODE_VALUE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    // Also known as "Fully Lit" mode in Roccat Swarm
    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_KONE_PRO_STATIC_MODE_VALUE;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness           = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Static.brightness_min       = ROCCAT_KONE_PRO_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Static.colors.resize(ROCCAT_KONE_PRO_LED_COUNT);
    modes.push_back(Static);

    // Also known as "Wave" mode in Roccat Swarm
    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ROCCAT_KONE_PRO_WAVE_MODE_VALUE;
    Rainbow.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Rainbow.brightness_min      = ROCCAT_KONE_PRO_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Rainbow.speed               = ROCCAT_KONE_PRO_SPEED_MID;
    Rainbow.speed_min           = ROCCAT_KONE_PRO_SPEED_MIN;
    Rainbow.speed_max           = ROCCAT_KONE_PRO_SPEED_MAX;
    modes.push_back(Rainbow);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = ROCCAT_KONE_PRO_HEARTBEAT_MODE_VALUE;
    Heartbeat.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Heartbeat.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Heartbeat.brightness        = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Heartbeat.brightness_min    = ROCCAT_KONE_PRO_BRIGHTNESS_MIN;
    Heartbeat.brightness_max    = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Heartbeat.speed             = ROCCAT_KONE_PRO_SPEED_MID;
    Heartbeat.speed_min         = ROCCAT_KONE_PRO_SPEED_MIN;
    Heartbeat.speed_max         = ROCCAT_KONE_PRO_SPEED_MAX;
    Heartbeat.colors.resize(ROCCAT_KONE_PRO_LED_COUNT);
    modes.push_back(Heartbeat);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_KONE_PRO_BREATHING_MODE_VALUE;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness        = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Breathing.brightness_min    = ROCCAT_KONE_PRO_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Breathing.speed             = ROCCAT_KONE_PRO_SPEED_MID;
    Breathing.speed_min         = ROCCAT_KONE_PRO_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_KONE_PRO_SPEED_MAX;
    Breathing.colors.resize(ROCCAT_KONE_PRO_LED_COUNT);
    modes.push_back(Breathing);

    mode Blinking;
    Blinking.name               = "Blinking";
    Blinking.value              = ROCCAT_KONE_PRO_BLINKING_MODE_VALUE;
    Blinking.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blinking.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Blinking.brightness         = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Blinking.brightness_min     = ROCCAT_KONE_PRO_BRIGHTNESS_MIN;
    Blinking.brightness_max     = ROCCAT_KONE_PRO_BRIGHTNESS_MAX;
    Blinking.speed              = ROCCAT_KONE_PRO_SPEED_MID;
    Blinking.speed_min          = ROCCAT_KONE_PRO_SPEED_MIN;
    Blinking.speed_max          = ROCCAT_KONE_PRO_SPEED_MAX;
    Blinking.colors.resize(ROCCAT_KONE_PRO_LED_COUNT);
    modes.push_back(Blinking);

    SetupZones();
}

RGBController_RoccatKonePro::~RGBController_RoccatKonePro()
{
    delete controller;
}

void RGBController_RoccatKonePro::SetupZones()
{
    zone new_zone;
    new_zone.name          = "Mouse";
    new_zone.type          = ZONE_TYPE_LINEAR;
    new_zone.leds_min      = ROCCAT_KONE_PRO_LED_COUNT;
    new_zone.leds_max      = ROCCAT_KONE_PRO_LED_COUNT;
    new_zone.leds_count    = ROCCAT_KONE_PRO_LED_COUNT;
    new_zone.matrix_map    = NULL;
    zones.push_back(new_zone);

    std::string led_names[2] =
    {
        "Left Button",
        "Right Button"
    };

    for(unsigned int i = 0; i < ROCCAT_KONE_PRO_LED_COUNT; i++)
    {
        led new_led;
        new_led.name = led_names[i];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatKonePro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatKonePro::DeviceUpdateLEDs()
{
    const mode& active = modes[active_mode];

    if(active.value == ROCCAT_KONE_PRO_DIRECT_MODE_VALUE)
    {
        controller->SendDirect(colors);
    }
    else
    {
        controller->SetMode(active.colors, active.value, active.speed, active.brightness, active.color_mode, active.flags);
    }
}

void RGBController_RoccatKonePro::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKonePro::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKonePro::DeviceUpdateMode()
{
    if(modes[active_mode].value == ROCCAT_KONE_PRO_DIRECT_MODE_VALUE)
    {
        controller->SetupDirectMode();
    }
    else
    {
        DeviceUpdateLEDs();
    }
}
