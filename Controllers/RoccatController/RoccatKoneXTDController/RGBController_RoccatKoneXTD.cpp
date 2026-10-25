/*---------------------------------------------------------*\
| RGBController_RoccatKoneXTD.cpp                           |
|                                                           |
|   RGBController for Roccat Kone XTD                       |
|                                                           |
|   Julian                                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RoccatKoneXTD.h"

enum
{
    ROCCAT_KONE_XTD_MODE_DIRECT,
    ROCCAT_KONE_XTD_MODE_STATIC,
    ROCCAT_KONE_XTD_MODE_BLINKING,
    ROCCAT_KONE_XTD_MODE_BREATHING,
    ROCCAT_KONE_XTD_MODE_HEARTBEAT,
    ROCCAT_KONE_XTD_MODE_SPECTRUM_CYCLE,
    ROCCAT_KONE_XTD_MODE_RAINBOW_WAVE,
    ROCCAT_KONE_XTD_MODE_OFF,
};

/**------------------------------------------------------------------*\
    @name Roccat Kone XTD
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRoccatKoneXTDControllers
    @comment Hardware modes are written to the active profile on the
    mouse. Direct mode is not saved; leaving it hands the LEDs back to
    the profile's lighting.
\*-------------------------------------------------------------------*/

RGBController_RoccatKoneXTD::RGBController_RoccatKoneXTD(RoccatKoneXTDController* controller_ptr)
{
    controller                  = controller_ptr;
    direct_active               = false;

    name                        = controller->GetName();
    vendor                      = "Roccat";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Roccat Kone XTD Mouse Device";
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();
    version                     = controller->GetVersion();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROCCAT_KONE_XTD_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROCCAT_KONE_XTD_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Blinking;
    Blinking.name               = "Flashing";
    Blinking.value              = ROCCAT_KONE_XTD_MODE_BLINKING;
    Blinking.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Blinking.color_mode         = MODE_COLORS_PER_LED;
    Blinking.speed_min          = ROCCAT_KONE_XTD_SPEED_MIN;
    Blinking.speed_max          = ROCCAT_KONE_XTD_SPEED_MAX;
    Blinking.speed              = 2;
    modes.push_back(Blinking);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROCCAT_KONE_XTD_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.speed_min         = ROCCAT_KONE_XTD_SPEED_MIN;
    Breathing.speed_max         = ROCCAT_KONE_XTD_SPEED_MAX;
    Breathing.speed             = 2;
    modes.push_back(Breathing);

    mode Heartbeat;
    Heartbeat.name              = "Heartbeat";
    Heartbeat.value             = ROCCAT_KONE_XTD_MODE_HEARTBEAT;
    Heartbeat.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Heartbeat.color_mode        = MODE_COLORS_PER_LED;
    Heartbeat.speed_min         = ROCCAT_KONE_XTD_SPEED_MIN;
    Heartbeat.speed_max         = ROCCAT_KONE_XTD_SPEED_MAX;
    Heartbeat.speed             = 2;
    modes.push_back(Heartbeat);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = ROCCAT_KONE_XTD_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    SpectrumCycle.speed_min     = ROCCAT_KONE_XTD_SPEED_MIN;
    SpectrumCycle.speed_max     = ROCCAT_KONE_XTD_SPEED_MAX;
    SpectrumCycle.speed         = 2;
    modes.push_back(SpectrumCycle);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = ROCCAT_KONE_XTD_MODE_RAINBOW_WAVE;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    RainbowWave.speed_min       = ROCCAT_KONE_XTD_SPEED_MIN;
    RainbowWave.speed_max       = ROCCAT_KONE_XTD_SPEED_MAX;
    RainbowWave.speed           = 2;
    RainbowWave.direction       = MODE_DIRECTION_UP;
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ROCCAT_KONE_XTD_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_RoccatKoneXTD::~RGBController_RoccatKoneXTD()
{
    Shutdown();

    if(direct_active)
    {
        controller->ReleaseDirect();
    }

    delete controller;
}

void RGBController_RoccatKoneXTD::SetupZones()
{
    zone mouse_zone;
    mouse_zone.name             = "Mouse";
    mouse_zone.type             = ZONE_TYPE_LINEAR;
    mouse_zone.leds_min         = ROCCAT_KONE_XTD_NUMBER_OF_LEDS;
    mouse_zone.leds_max         = ROCCAT_KONE_XTD_NUMBER_OF_LEDS;
    mouse_zone.leds_count       = ROCCAT_KONE_XTD_NUMBER_OF_LEDS;
    zones.push_back(mouse_zone);

    for(unsigned int led_idx = 0; led_idx < ROCCAT_KONE_XTD_NUMBER_OF_LEDS; led_idx++)
    {
        led new_led;
        new_led.name            = "LED " + std::to_string(led_idx + 1);
        new_led.value           = led_idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RoccatKoneXTD::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROCCAT_KONE_XTD_MODE_DIRECT)
    {
        controller->SendDirectColors(colors);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        DeviceUpdateMode();
    }
}

void RGBController_RoccatKoneXTD::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneXTD::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatKoneXTD::DeviceUpdateMode()
{
    mode& active = modes[active_mode];

    if(active.value == ROCCAT_KONE_XTD_MODE_DIRECT)
    {
        direct_active = true;
        controller->SendDirectColors(colors);
        return;
    }

    if(direct_active)
    {
        controller->ReleaseDirect();
        direct_active = false;
    }

    bool            lights_on   = true;
    unsigned char   effect_type = ROCCAT_KONE_XTD_EFFECT_FULLY_LIT;
    unsigned char   color_flow  = ROCCAT_KONE_XTD_FLOW_OFF;
    unsigned char   color_mode  = ROCCAT_KONE_XTD_COLOR_SELECTED;
    unsigned char   speed       = 2;

    switch(active.value)
    {
        case ROCCAT_KONE_XTD_MODE_BLINKING:
            effect_type = ROCCAT_KONE_XTD_EFFECT_BLINKING;
            break;

        case ROCCAT_KONE_XTD_MODE_BREATHING:
            effect_type = ROCCAT_KONE_XTD_EFFECT_BREATHING;
            break;

        case ROCCAT_KONE_XTD_MODE_HEARTBEAT:
            effect_type = ROCCAT_KONE_XTD_EFFECT_HEARTBEAT;
            break;

        case ROCCAT_KONE_XTD_MODE_SPECTRUM_CYCLE:
            color_flow  = ROCCAT_KONE_XTD_FLOW_SIMULTANEOUS;
            break;

        case ROCCAT_KONE_XTD_MODE_RAINBOW_WAVE:
            switch(active.direction)
            {
                case MODE_DIRECTION_LEFT:   color_flow = ROCCAT_KONE_XTD_FLOW_LEFT;  break;
                case MODE_DIRECTION_RIGHT:  color_flow = ROCCAT_KONE_XTD_FLOW_RIGHT; break;
                case MODE_DIRECTION_DOWN:   color_flow = ROCCAT_KONE_XTD_FLOW_DOWN;  break;
                default:                    color_flow = ROCCAT_KONE_XTD_FLOW_UP;    break;
            }
            break;

        case ROCCAT_KONE_XTD_MODE_OFF:
            lights_on   = false;
            effect_type = ROCCAT_KONE_XTD_EFFECT_OFF;
            break;
    }

    if(active.color_mode == MODE_COLORS_RANDOM)
    {
        color_mode = ROCCAT_KONE_XTD_COLOR_RANDOM_MULTI;
    }

    if(active.flags & MODE_FLAG_HAS_SPEED)
    {
        speed = active.speed;
    }

    controller->SetProfileLighting(lights_on, effect_type, color_flow, color_mode, speed, colors);
}
