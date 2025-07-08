/*---------------------------------------------------------*\
| RGBController_TForceXtreem.cpp                            |
|                                                           |
|   RGBController for TeamGroup T-Force Xtreem RAM          |
|                                                           |
|   Milan Cermak (krysmanta)                    28 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_TForceXtreem.h"
#include "LogManager.h"
#include "ResourceManager.h"

/**------------------------------------------------------------------*\
    @name T-Force Xtreem
    @category RAM
    @type SMBus
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectTForceXtreemControllers
    @comment
        Verified models:
        TeamGroup T-Force Xtreem ARGB DDR4
\*-------------------------------------------------------------------*/

RGBController_TForceXtreem::RGBController_TForceXtreem(TForceXtreemController * controller_ptr)
{
    controller  = controller_ptr;

    type    = DEVICE_TYPE_DRAM;
    name    = "T-Force Xtreem RGB";
    vendor  = "TeamGroup";

    location    = controller->GetDeviceLocation();
    description = "TeamGroup T-Force Xtreem DRAM";

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = XTREEM_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = XTREEM_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = XTREEM_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.speed_min         = XTREEM_SPEED_SLOWEST;
    Breathing.speed_max         = XTREEM_SPEED_FASTEST;
    Breathing.speed             = XTREEM_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = XTREEM_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Flashing.color_mode         = MODE_COLORS_PER_LED;
    Flashing.speed_min          = XTREEM_SPEED_SLOWEST;
    Flashing.speed_max          = XTREEM_SPEED_FASTEST;
    Flashing.speed              = XTREEM_SPEED_NORMAL;
    modes.push_back(Flashing);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = XTREEM_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    SpectrumCycle.speed_min     = XTREEM_SPEED_SLOWEST;
    SpectrumCycle.speed_max     = XTREEM_SPEED_FASTEST;
    SpectrumCycle.speed         = XTREEM_SPEED_NORMAL;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = XTREEM_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed_min           = XTREEM_SPEED_SLOWEST;
    Rainbow.speed_max           = XTREEM_SPEED_FASTEST;
    Rainbow.speed               = XTREEM_SPEED_NORMAL;
    Rainbow.direction           = MODE_DIRECTION_LEFT;
    modes.push_back(Rainbow);

    mode ChaseFade;
    ChaseFade.name              = "Chase Fade";
    ChaseFade.value             = XTREEM_MODE_CHASE_FADE;
    ChaseFade.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    ChaseFade.color_mode        = MODE_COLORS_PER_LED;
    ChaseFade.speed_min         = XTREEM_SPEED_SLOWEST;
    ChaseFade.speed_max         = XTREEM_SPEED_FASTEST;
    ChaseFade.speed             = XTREEM_SPEED_NORMAL;
    ChaseFade.direction         = MODE_DIRECTION_LEFT;
    modes.push_back(ChaseFade);

    mode Chase;
    Chase.name                  = "Chase";
    Chase.value                 = XTREEM_MODE_CHASE;
    Chase.flags                 = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Chase.color_mode            = MODE_COLORS_PER_LED;
    Chase.speed_min             = XTREEM_SPEED_SLOWEST;
    Chase.speed_max             = XTREEM_SPEED_FASTEST;
    Chase.speed                 = XTREEM_SPEED_NORMAL;
    ChaseFade.direction         = MODE_DIRECTION_LEFT;
    modes.push_back(Chase);

    mode RandomFlicker;
    RandomFlicker.name          = "Random Flicker";
    RandomFlicker.value         = XTREEM_MODE_RANDOM_FLICKER;
    RandomFlicker.flags         = MODE_FLAG_HAS_SPEED;
    RandomFlicker.color_mode    = MODE_COLORS_NONE;
    RandomFlicker.speed_min     = XTREEM_SPEED_SLOWEST;
    RandomFlicker.speed_max     = XTREEM_SPEED_FASTEST;
    RandomFlicker.speed         = XTREEM_SPEED_NORMAL;
    modes.push_back(RandomFlicker);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = XTREEM_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Stack.color_mode            = MODE_COLORS_NONE;
    Stack.speed_min             = XTREEM_SPEED_SLOWEST;
    Stack.speed_max             = XTREEM_SPEED_FASTEST;
    Stack.speed                 = XTREEM_SPEED_NORMAL;
    modes.push_back(Stack);

    mode Pong;
    Pong.name                   = "Pong";
    Pong.value                  = XTREEM_MODE_PONG;
    Pong.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Pong.color_mode             = MODE_COLORS_NONE;
    Pong.speed_min              = XTREEM_SPEED_SLOWEST;
    Pong.speed_max              = XTREEM_SPEED_FASTEST;
    Pong.speed                  = XTREEM_SPEED_NORMAL;
    modes.push_back(Pong);

    mode Fillup;
    Fillup.name                 = "Fill up";
    Fillup.value                = XTREEM_MODE_FILLUP;
    Fillup.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Fillup.color_mode           = MODE_COLORS_NONE;
    Fillup.speed_min            = XTREEM_SPEED_SLOWEST;
    Fillup.speed_max            = XTREEM_SPEED_FASTEST;
    Fillup.speed                = XTREEM_SPEED_NORMAL;
    modes.push_back(Fillup);

    mode Neon;
    Neon.name                   = "Neon Sign";
    Neon.value                  = XTREEM_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_SPEED;
    Neon.color_mode             = MODE_COLORS_NONE;
    Neon.speed_min              = XTREEM_SPEED_SLOWEST;
    Neon.speed_max              = XTREEM_SPEED_FASTEST;
    Neon.speed                  = XTREEM_SPEED_NORMAL;
    modes.push_back(Neon);

    mode ColorWave;
    ColorWave.name              = "Wave";
    ColorWave.value             = XTREEM_MODE_COLOR_WAVE;
    ColorWave.flags             = MODE_FLAG_HAS_SPEED;
    ColorWave.color_mode        = MODE_COLORS_NONE;
    ColorWave.speed_min         = XTREEM_SPEED_SLOWEST;
    ColorWave.speed_max         = XTREEM_SPEED_FASTEST;
    ColorWave.speed             = XTREEM_SPEED_NORMAL;
    modes.push_back(ColorWave);

    mode DoubleWave;
    DoubleWave.name             = "Double Wave";
    DoubleWave.value            = XTREEM_MODE_COLOR_DOUBLE_WAVE;
    DoubleWave.flags            = MODE_FLAG_HAS_SPEED;
    DoubleWave.color_mode       = MODE_COLORS_NONE;
    DoubleWave.speed_min        = XTREEM_SPEED_SLOWEST;
    DoubleWave.speed_max        = XTREEM_SPEED_FASTEST;
    DoubleWave.speed            = XTREEM_SPEED_NORMAL;
    modes.push_back(DoubleWave);

    mode Mixer;
    Mixer.name                  = "Mixer";
    Mixer.value                 = XTREEM_MODE_MIXER;
    Mixer.flags                 = MODE_FLAG_HAS_SPEED;
    Mixer.color_mode            = MODE_COLORS_NONE;
    Mixer.speed_min             = XTREEM_SPEED_SLOWEST;
    Mixer.speed_max             = XTREEM_SPEED_FASTEST;
    Mixer.speed                 = XTREEM_SPEED_NORMAL;
    modes.push_back(Mixer);

    mode Spectrum2;
    Spectrum2.name              = "Spectrum Cycle 2";
    Spectrum2.value             = XTREEM_MODE_SPECTRUM_CYCLE_2;
    Spectrum2.flags             = MODE_FLAG_HAS_SPEED;
    Spectrum2.color_mode        = MODE_COLORS_NONE;
    Spectrum2.speed_min         = XTREEM_SPEED_SLOWEST;
    Spectrum2.speed_max         = XTREEM_SPEED_FASTEST;
    Spectrum2.speed             = XTREEM_SPEED_NORMAL;
    modes.push_back(Spectrum2);

    mode FireBreathing;
    FireBreathing.name          = "Fire Breathing";
    FireBreathing.value         = XTREEM_MODE_FIRE_BREATHING;
    FireBreathing.flags         = MODE_FLAG_HAS_SPEED;
    FireBreathing.color_mode    = MODE_COLORS_NONE;
    FireBreathing.speed_min     = XTREEM_SPEED_SLOWEST;
    FireBreathing.speed_max     = XTREEM_SPEED_FASTEST;
    FireBreathing.speed         = XTREEM_SPEED_NORMAL;
    modes.push_back(FireBreathing);

    mode Spectrum3;
    Spectrum3.name              = "Spectrum Cycle 3";
    Spectrum3.value             = XTREEM_MODE_SPECTRUM_CYCLE_3;
    Spectrum3.flags             = MODE_FLAG_HAS_SPEED;
    Spectrum3.color_mode        = MODE_COLORS_NONE;
    Spectrum3.speed_min         = XTREEM_SPEED_SLOWEST;
    Spectrum3.speed_max         = XTREEM_SPEED_FASTEST;
    Spectrum3.speed             = XTREEM_SPEED_NORMAL;
    modes.push_back(Spectrum3);

    mode Slither;
    Slither.name                = "Slither";
    Slither.value               = XTREEM_MODE_SLITHER;
    Slither.flags               = MODE_FLAG_HAS_SPEED;
    Slither.color_mode          = MODE_COLORS_NONE;
    Slither.speed_min           = XTREEM_SPEED_SLOWEST;
    Slither.speed_max           = XTREEM_SPEED_FASTEST;
    Slither.speed               = XTREEM_SPEED_NORMAL;
    modes.push_back(Slither);

    mode TForceXtreem;
    TForceXtreem.name           = "T-Force Xtreem";
    TForceXtreem.value          = XTREEM_MODE_TFORCE_XTREEM;
    TForceXtreem.flags          = MODE_FLAG_HAS_SPEED;
    TForceXtreem.color_mode     = MODE_COLORS_NONE;
    TForceXtreem.speed_min      = XTREEM_SPEED_SLOWEST;
    TForceXtreem.speed_max      = XTREEM_SPEED_FASTEST;
    TForceXtreem.speed          = XTREEM_SPEED_NORMAL;
    modes.push_back(TForceXtreem);

    SetupZones();

    /*-------------------------------------------------*\
    | Initialize active mode                            |
    \*-------------------------------------------------*/
    active_mode = GetDeviceMode();
}

RGBController_TForceXtreem::~RGBController_TForceXtreem()
{
    delete controller;
}

int RGBController_TForceXtreem::GetDeviceMode()
{
    /*-----------------------------------------------------------------*\
    | Determine starting mode by reading the mode and direct registers  |
    \*-----------------------------------------------------------------*/
    int dev_mode    = controller->ENERegisterRead(XTREEM_REG_MODE);
    int color_mode  = MODE_COLORS_PER_LED;
    int speed       = controller->ENERegisterRead(XTREEM_REG_SPEED);
    int direction   = controller->ENERegisterRead(XTREEM_REG_DIRECTION);

    LOG_TRACE("[%s] Retrieved ENE mode from module: %02d", name.c_str(), dev_mode);

    if(controller->ENERegisterRead(XTREEM_REG_DIRECT))
    {
        dev_mode = 0xFFFF;
    }

    switch(dev_mode)
    {
        case XTREEM_MODE_OFF:
        case XTREEM_MODE_RAINBOW:
        case XTREEM_MODE_SPECTRUM_CYCLE:
        case XTREEM_MODE_RANDOM_FLICKER:
            color_mode = MODE_COLORS_NONE;
            break;

        case XTREEM_MODE_SPECTRUM_CYCLE_CHASE:
            dev_mode   = XTREEM_MODE_CHASE;
            color_mode = MODE_COLORS_RANDOM;
            break;

        case XTREEM_MODE_SPECTRUM_CYCLE_BREATHING:
            dev_mode   = XTREEM_MODE_BREATHING;
            color_mode = MODE_COLORS_RANDOM;
            break;

        case XTREEM_MODE_SPECTRUM_CYCLE_CHASE_FADE:
            dev_mode   = XTREEM_MODE_CHASE_FADE;
            color_mode = MODE_COLORS_RANDOM;
            break;
    }

    for(int mode = 0; mode < (int)modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            active_mode                 = mode;
            modes[mode].color_mode      = color_mode;

            if(modes[mode].flags & MODE_FLAG_HAS_SPEED)
            {
                modes[mode].speed       = speed;
            }

            if(modes[mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
            {
                if(direction == XTREEM_DIRECTION_FORWARD)
                {
                    modes[mode].direction   = MODE_DIRECTION_RIGHT;
                }
                else
                {
                    modes[mode].direction   = MODE_DIRECTION_LEFT;
                }
            }

            break;
        }
    }

    /*---------------------------------------------------------*\
    | Initialize colors for each LED                            |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned int  led = leds[led_idx].value;
        unsigned char red;
        unsigned char grn;
        unsigned char blu;

        if(active_mode == 0)
        {
            red = controller->GetLEDRed(led);
            grn = controller->GetLEDGreen(led);
            blu = controller->GetLEDBlue(led);
        }
        else
        {
            red = controller->GetLEDRedEffect(led);
            grn = controller->GetLEDGreenEffect(led);
            blu = controller->GetLEDBlueEffect(led);
        }

        colors[led_idx] = ToRGBColor(red, grn, blu);
    }

    return(active_mode);
}

void RGBController_TForceXtreem::DeviceUpdateLEDs()
{
    if(GetMode() == 0)
    {
        controller->SetAllColorsDirect(&colors[0]);
    }
    else
    {
        controller->SetAllColorsEffect(&colors[0]);
    }

}

void RGBController_TForceXtreem::UpdateZoneLEDs(int zone)
{
    for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
    {
        int           led   = zones[zone].leds[led_idx].value;
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        if(GetMode() == 0)
        {
            controller->SetLEDColorDirect(led, red, grn, blu);
        }
        else
        {
            controller->SetLEDColorEffect(led, red, grn, blu);
        }
    }
}

void RGBController_TForceXtreem::UpdateSingleLED(int led)
{
    RGBColor color    = colors[led];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(GetMode() == 0)
    {
        controller->SetLEDColorDirect(led, red, grn, blu);
    }
    else
    {
        controller->SetLEDColorEffect(led, red, grn, blu);
    }
}

void RGBController_TForceXtreem::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name           = "DRAM";
    new_zone.type           = ZONE_TYPE_LINEAR;
    new_zone.leds_min       = XTREEM_LED_COUNT;
    new_zone.leds_max       = XTREEM_LED_COUNT;
    new_zone.leds_count     = XTREEM_LED_COUNT;
    new_zone.matrix_map     = NULL;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name        = "DRAM LED ";
        new_led.name.append(std::to_string(led_idx));
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_TForceXtreem::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_TForceXtreem::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        controller->SetDirect(true);
    }
    else
    {
        int new_mode        = modes[active_mode].value;
        int new_speed       = 0;
        int new_direction   = 0;

        if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
        {
            switch(new_mode)
            {
            case XTREEM_MODE_CHASE:
                new_mode = XTREEM_MODE_SPECTRUM_CYCLE_CHASE;
                break;
            case XTREEM_MODE_BREATHING:
                new_mode = XTREEM_MODE_SPECTRUM_CYCLE_BREATHING;
                break;
            case XTREEM_MODE_CHASE_FADE:
                new_mode = XTREEM_MODE_SPECTRUM_CYCLE_CHASE_FADE;
                break;
            }
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            new_speed = modes[active_mode].speed;
        }

        if(modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR)
        {
            switch(modes[active_mode].direction)
            {
                case MODE_DIRECTION_LEFT:
                    new_direction = XTREEM_DIRECTION_REVERSE;
                    break;

                case MODE_DIRECTION_RIGHT:
                    new_direction = XTREEM_DIRECTION_FORWARD;
                    break;
            }
        }

        controller->SetMode(new_mode, new_speed, new_direction);
        controller->SetDirect(false);
    }
}
