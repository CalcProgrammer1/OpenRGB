/*---------------------------------------------------------*\
| RGBController_Sinowealth.cpp                              |
|                                                           |
|   RGBController for Sinowealth mice, including Glorious   |
|                                                           |
|   Niels Westphal (crashniels)                 20 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Sinowealth.h"

/**------------------------------------------------------------------*\
    @name Sinowealth Mice
    @category Mouse
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthMouse
    @comment
\*-------------------------------------------------------------------*/

RGBController_Sinowealth::RGBController_Sinowealth(SinowealthController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Sinowealth Device";
    location                        = controller->GetLocation();
    serial                          = controller->GetSerialString();
    version                         = controller->GetFirmwareVersion();

    mode Static;
    Static.name                     = "Static";
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode               = MODE_COLORS_PER_LED;
    Static.value                    = GLORIOUS_MODE_STATIC;
    modes.push_back(Static);

    mode Off;
    Off.name                        = "Off";
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    Off.value                       = GLORIOUS_MODE_OFF;
    modes.push_back(Off);

    mode Rainbow;
    Rainbow.name                    = "Glorious Mode";
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.speed_min               = GLORIOUS_SPEED_SLOW;
    Rainbow.speed                   = GLORIOUS_SPEED_NORMAL;
    Rainbow.speed_max               = GLORIOUS_SPEED_FAST;
    Rainbow.direction               = MODE_DIRECTION_UP;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    Rainbow.value                   = GLORIOUS_MODE_RAINBOW;
    modes.push_back(Rainbow);

    mode SpectrumBreathing;
    SpectrumBreathing.name          = "Seemless Breathing";
    SpectrumBreathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumBreathing.speed_min     = GLORIOUS_SPEED_SLOW;
    SpectrumBreathing.speed         = GLORIOUS_SPEED_NORMAL;
    SpectrumBreathing.speed_max     = GLORIOUS_SPEED_FAST;
    SpectrumBreathing.colors_min    = 7;
    SpectrumBreathing.colors_max    = 7;
    SpectrumBreathing.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    SpectrumBreathing.value         = GLORIOUS_MODE_SPECTRUM_BREATING;
    SpectrumBreathing.colors.resize(7);
    modes.push_back(SpectrumBreathing);

    mode Chase;
    Chase.name                      = "Tail";
    Chase.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Chase.speed_min                 = GLORIOUS_SPEED_SLOW;
    Chase.speed                     = GLORIOUS_SPEED_NORMAL;
    Chase.speed_max                 = GLORIOUS_SPEED_FAST;
    Chase.color_mode                = MODE_COLORS_NONE;
    Chase.value                     = GLORIOUS_MODE_TAIL;
    modes.push_back(Chase);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min         = GLORIOUS_SPEED_SLOW;
    SpectrumCycle.speed             = GLORIOUS_SPEED_NORMAL;
    SpectrumCycle.speed_max         = GLORIOUS_SPEED_FAST;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.value             = GLORIOUS_MODE_SPECTRUM_CYCLE;
    modes.push_back(SpectrumCycle);

    mode Flashing;
    Flashing.name                   = "Rave";
    Flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Flashing.speed_min              = GLORIOUS_SPEED_SLOW;
    Flashing.speed                  = GLORIOUS_SPEED_NORMAL;
    Flashing.speed_max              = GLORIOUS_SPEED_FAST;
    Flashing.colors_min             = 2;
    Flashing.colors_max             = 2;
    Flashing.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Flashing.value                  = GLORIOUS_MODE_RAVE;
    Flashing.colors.resize(2);
    modes.push_back(Flashing);

    mode Epilepsy;
    Epilepsy.name                   = "Epilepsy";
    Epilepsy.flags                  = MODE_FLAG_AUTOMATIC_SAVE;
    Epilepsy.color_mode             = MODE_COLORS_NONE;
    Epilepsy.value                  = GLORIOUS_MODE_EPILEPSY;
    modes.push_back(Epilepsy);

    mode RainbowSlow;
    RainbowSlow.name                = "Wave";
    RainbowSlow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowSlow.speed_min           = GLORIOUS_SPEED_SLOW;
    RainbowSlow.speed               = GLORIOUS_SPEED_NORMAL;
    RainbowSlow.speed_max           = GLORIOUS_SPEED_FAST;
    RainbowSlow.color_mode          = MODE_COLORS_NONE;
    RainbowSlow.value               = GLORIOUS_MODE_WAVE;
    modes.push_back(RainbowSlow);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min             = GLORIOUS_MODE_BREATING_SLOW;
    Breathing.speed                 = GLORIOUS_MODE_BREATING_NORMAL;
    Breathing.speed_max             = GLORIOUS_MODE_BREATING_FAST;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.value                 = GLORIOUS_MODE_BREATHING;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_Sinowealth::~RGBController_Sinowealth()
{
    delete controller;
}

void RGBController_Sinowealth::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create a single zone                                      |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = controller->GetLEDCount();
    new_zone.leds_max   = controller->GetLEDCount();
    new_zone.leds_count = controller->GetLEDCount();
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led* new_led = new led();
        new_led->name = "Mouse LED";
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_Sinowealth::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Sinowealth::DeviceUpdateLEDs()
{
    controller->SetLEDColor(&colors[0]);
}

void RGBController_Sinowealth::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Sinowealth::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Sinowealth::DeviceUpdateMode()
{
    unsigned int direction = 0;
    unsigned int speed = GLORIOUS_SPEED_NORMAL;

    if (modes[active_mode].value == GLORIOUS_MODE_STATIC)
    {

    }
    else
    {
        if (modes[active_mode].direction == MODE_DIRECTION_UP)
        {
            direction = GLORIOUS_DIRECTION_DOWN;
        }
        else
        {
            direction = GLORIOUS_DIRECTION_UP;
        }

        if ((modes[active_mode].speed == GLORIOUS_SPEED_FAST)   ||
            (modes[active_mode].speed == GLORIOUS_SPEED_SLOW)   ||
            (modes[active_mode].speed == GLORIOUS_SPEED_NORMAL))
        {
            speed = modes[active_mode].speed;
        }
        else
        {
            if ((modes[active_mode].speed == GLORIOUS_MODE_BREATING_FAST) ||
                (modes[active_mode].speed == GLORIOUS_MODE_BREATING_SLOW) ||
                (modes[active_mode].speed == GLORIOUS_MODE_BREATING_NORMAL))
            {
                speed = modes[active_mode].speed;
            }
            else
            {
                speed = GLORIOUS_SPEED_NORMAL;
            }
        }

        if (modes[active_mode].color_mode == MODE_COLORS_NONE)
        {
            controller->SetMode(modes[active_mode].value, speed, direction, 0);
        }
        else
        {
            controller->SetMode(modes[active_mode].value, speed, direction, &modes[active_mode].colors[0]);
        }
    }
}

