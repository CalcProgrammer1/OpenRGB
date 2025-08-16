/*---------------------------------------------------------*\
| RGBController_RoccatKoneXP.cpp                            |
|                                                           |
|   RGBController for Roccat Kone XP                        |
|                                                           |
|   Mola19                                      12 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RoccatKoneXP.h"

/**------------------------------------------------------------------*\
    @name Roccat Kone XP Mouse
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatKoneXPControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatKoneXP::RGBController_RoccatKoneXP(RoccatKoneXPController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Kone XP Mouse Device";
    version                     = controller->GetVersion();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_KONE_XP_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ROCCAT_KONE_XP_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_KONE_XP_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness           = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Static.brightness_min       = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Static.brightness_max       = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ROCCAT_KONE_XP_MODE_WAVE;
    Rainbow.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness          = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_min      = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    Rainbow.speed               = ROCCAT_KONE_XP_SPEED_DEFAULT;
    Rainbow.speed_min           = ROCCAT_KONE_XP_SPEED_MIN;
    Rainbow.speed_max           = ROCCAT_KONE_XP_SPEED_MAX;
    modes.push_back(Rainbow);

    mode Blinking;
    Blinking.name              = "Blinking";
    Blinking.value             = ROCCAT_KONE_XP_MODE_BLINKING;
    Blinking.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blinking.color_mode        = MODE_COLORS_PER_LED;
    Blinking.brightness        = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Blinking.brightness_min    = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Blinking.brightness_max    = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    Blinking.speed             = ROCCAT_KONE_XP_SPEED_DEFAULT;
    Blinking.speed_min         = ROCCAT_KONE_XP_SPEED_MIN;
    Blinking.speed_max         = ROCCAT_KONE_XP_SPEED_MAX;
    modes.push_back(Blinking);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_KONE_XP_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness        = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Breathing.brightness_min    = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    Breathing.speed             = ROCCAT_KONE_XP_SPEED_DEFAULT;
    Breathing.speed_min         = ROCCAT_KONE_XP_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_KONE_XP_SPEED_MAX;
    modes.push_back(Breathing);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = ROCCAT_KONE_XP_MODE_HEARTBEAT;
    Heartbeat.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Heartbeat.color_mode        = MODE_COLORS_PER_LED;
    Heartbeat.brightness        = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Heartbeat.brightness_min    = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Heartbeat.brightness_max    = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    Heartbeat.speed             = ROCCAT_KONE_XP_SPEED_DEFAULT;
    Heartbeat.speed_min         = ROCCAT_KONE_XP_SPEED_MIN;
    Heartbeat.speed_max         = ROCCAT_KONE_XP_SPEED_MAX;
    modes.push_back(Heartbeat);

    mode Photon;
    Photon.name                 = "Photon FX";
    Photon.value                = ROCCAT_KONE_XP_MODE_PHOTON_FX;
    Photon.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Photon.color_mode           = MODE_COLORS_NONE;
    Photon.brightness           = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Photon.brightness_min       = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Photon.brightness_max       = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    modes.push_back(Photon);

    /*---------------------------------------------------------------------*\
    | This is the default mode for software modes, while swarm isn't active |
    \*---------------------------------------------------------------------*/
    mode Default;
    Default.name                = "Default";
    Default.value               = ROCCAT_KONE_XP_MODE_DEFAULT;
    Default.flags               = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Default.color_mode          = MODE_COLORS_NONE;
    Default.brightness          = ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT;
    Default.brightness_min      = ROCCAT_KONE_XP_BRIGHTNESS_MIN;
    Default.brightness_max      = ROCCAT_KONE_XP_BRIGHTNESS_MAX;
    modes.push_back(Default);

    SetupZones();

    uint8_t active_profile = controller->GetActiveProfile();
    controller->SetReadProfile(active_profile);
    controller->WaitUntilReady();

    roccat_kone_xp_mode_struct active = controller->GetMode();

    for(uint32_t i = 0; i < modes.size(); i++)
    {
        if(modes[i].value == active.mode)
        {
            active_mode = i;
            break;
        }

        /*----------------------------------------------*\
        | If no mode was found, select 0th mode (direct) |
        \*----------------------------------------------*/
        if(i == modes.size() - 1)
        {
            active_mode = 0;
        }
    }

    modes[active_mode].speed      = active.speed;
    modes[active_mode].brightness = active.brightness;

    for(uint8_t i = 0; i < 20; i++)
    {
        colors[i] = active.colors[i].color;
    }
}

RGBController_RoccatKoneXP::~RGBController_RoccatKoneXP()
{
    delete controller;
}

void RGBController_RoccatKoneXP::SetupZones()
{
    zone left;
    left.name          = "Left";
    left.type          = ZONE_TYPE_LINEAR;
    left.leds_min      = 9;
    left.leds_max      = 9;
    left.leds_count    = 9;
    left.matrix_map    = NULL;
    zones.push_back(left);

    for (uint8_t i = 1; i <= 9; i++) {
        led left_led;
        left_led.name = "Left LED " + std::to_string(i);
        leds.push_back(left_led);
    }

    zone right;
    right.name          = "Right";
    right.type          = ZONE_TYPE_LINEAR;
    right.leds_min      = 9;
    right.leds_max      = 9;
    right.leds_count    = 9;
    right.matrix_map    = NULL;
    zones.push_back(right);

    for (uint8_t i = 1; i <= 9; i++) {
        led left_led;
        left_led.name = "Right LED " + std::to_string(i);
        leds.push_back(left_led);
    }

    zone wheel;
    wheel.name          = "Scrollwheel";
    wheel.type          = ZONE_TYPE_SINGLE;
    wheel.leds_min      = 1;
    wheel.leds_max      = 1;
    wheel.leds_count    = 1;
    wheel.matrix_map    = NULL;
    zones.push_back(wheel);

    led wheel_led;
    wheel_led.name = "Scrollwheel LED";
    leds.push_back(wheel_led);

    zone dpi;
    dpi.name          = "DPI button";
    dpi.type          = ZONE_TYPE_SINGLE;
    dpi.leds_min      = 1;
    dpi.leds_max      = 1;
    dpi.leds_count    = 1;
    dpi.matrix_map    = NULL;
    zones.push_back(dpi);

    led dpi_led;
    dpi_led.name = "DPI button LED";
    leds.push_back(dpi_led);

    SetupColors();
}

void RGBController_RoccatKoneXP::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatKoneXP::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROCCAT_KONE_XP_MODE_DIRECT)
    {
        controller->SendDirect(colors);
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_RoccatKoneXP::UpdateZoneLEDs(int /*zone_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneXP::UpdateSingleLED(int /*led_idx*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneXP::DeviceUpdateMode()
{
    mode selected = modes[active_mode];

    roccat_kone_xp_mode_struct active = controller->GetMode();

    active.mode         = selected.value;
    active.speed        = selected.speed;
    active.brightness   = (modes[active_mode].value == ROCCAT_KONE_XP_MODE_DIRECT) ? 0xFF : selected.brightness;

    for(uint8_t i = 0; i < 20; i++)
    {
        active.colors[i].color = colors[i];
    }

    controller->SetMode(active);
    controller->WaitUntilReady();

    controller->EnableDirect(selected.value == ROCCAT_KONE_XP_MODE_DIRECT);
    controller->WaitUntilReady();
}
