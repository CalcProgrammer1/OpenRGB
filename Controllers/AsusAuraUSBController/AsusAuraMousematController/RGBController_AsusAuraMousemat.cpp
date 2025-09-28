/*---------------------------------------------------------*\
| RGBController_AsusAuraMousemat.cpp                        |
|                                                           |
|   RGBController for ASUS Aura mousemat                    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraMousemat.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Mousemat
    @category Mousemat
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBMousemats
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraMousemat::RGBController_AuraMousemat(AuraMousematController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_MOUSEMAT;
    description                 = "ASUS Aura Mousemat Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = AURA_MOUSEMAT_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = AURA_MOUSEMAT_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min       = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Static.brightness_max       = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Static.brightness           = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = AURA_MOUSEMAT_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC | MODE_COLORS_RANDOM;
    Breathing.speed_min         = AURA_MOUSEMAT_SPEED_MIN;
    Breathing.speed_max         = AURA_MOUSEMAT_SPEED_MAX;
    Breathing.speed             = AURA_MOUSEMAT_SPEED_DEFAULT_BREATHING;
    Breathing.brightness_min    = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Breathing.brightness_max    = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Breathing.brightness        = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = AURA_MOUSEMAT_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    ColorCycle.speed_min        = AURA_MOUSEMAT_SPEED_MIN;
    ColorCycle.speed_max        = AURA_MOUSEMAT_SPEED_MAX;
    ColorCycle.speed            = AURA_MOUSEMAT_SPEED_DEFAULT_COLOR_CYCLE;
    ColorCycle.brightness_min   = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    ColorCycle.brightness       = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    modes.push_back(ColorCycle);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = AURA_MOUSEMAT_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    Wave.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min              = AURA_MOUSEMAT_SPEED_MIN;
    Wave.speed_max              = AURA_MOUSEMAT_SPEED_MAX;
    Wave.speed                  = AURA_MOUSEMAT_SPEED_DEFAULT_WAVE;
    Wave.brightness_min         = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Wave.brightness_max         = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Wave.brightness             = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Wave.direction              = MODE_DIRECTION_LEFT;
    Wave.colors_min             = 7;
    Wave.colors_max             = 7;
    Wave.colors.resize(7);
    modes.push_back(Wave);

    mode WavePlane;
    WavePlane.name              = "Wave Plane";
    WavePlane.value             = AURA_MOUSEMAT_MODE_WAVE_PLANE;
    WavePlane.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE;
    WavePlane.color_mode        = MODE_COLORS_NONE;
    WavePlane.speed_min         = AURA_MOUSEMAT_SPEED_MIN;
    WavePlane.speed_max         = AURA_MOUSEMAT_SPEED_MAX;
    WavePlane.speed             = AURA_MOUSEMAT_SPEED_DEFAULT_WAVE_PLANE;
    WavePlane.brightness_min    = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    WavePlane.brightness_max    = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    WavePlane.brightness        = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    WavePlane.direction         = MODE_DIRECTION_LEFT;
    modes.push_back(WavePlane);

    mode Comet;
    Comet.name                  = "Comet";
    Comet.value                 = AURA_MOUSEMAT_MODE_COMET;
    Comet.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_MANUAL_SAVE;
    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Comet.speed_min             = AURA_MOUSEMAT_SPEED_MIN;
    Comet.speed_max             = AURA_MOUSEMAT_SPEED_MAX;
    Comet.speed                 = AURA_MOUSEMAT_SPEED_DEFAULT_COMET;
    Comet.brightness_min        = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Comet.brightness_max        = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Comet.brightness            = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Comet.direction             = MODE_DIRECTION_LEFT;
    Comet.colors_min            = 1;
    Comet.colors_max            = 1;
    Comet.colors.resize(1);
    modes.push_back(Comet);

    mode GlowingYoyo;
    GlowingYoyo.name            = "Glowing Yoyo";
    GlowingYoyo.value           = AURA_MOUSEMAT_MODE_GLOWING_YOYO;
    GlowingYoyo.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    GlowingYoyo.color_mode      = MODE_COLORS_NONE;
    GlowingYoyo.speed_min       = AURA_MOUSEMAT_SPEED_MIN;
    GlowingYoyo.speed_max       = AURA_MOUSEMAT_SPEED_MAX;
    GlowingYoyo.speed           = AURA_MOUSEMAT_SPEED_DEFAULT_GLOWING_YOYO;
    GlowingYoyo.brightness_min  = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    GlowingYoyo.brightness_max  = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    GlowingYoyo.brightness      = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    GlowingYoyo.direction       = MODE_DIRECTION_LEFT;
    modes.push_back(GlowingYoyo);

    mode Cross;
    Cross.name                  = "Cross";
    Cross.value                 = AURA_MOUSEMAT_MODE_CROSS;
    Cross.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Cross.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Cross.speed_min             = AURA_MOUSEMAT_SPEED_MIN;
    Cross.speed_max             = AURA_MOUSEMAT_SPEED_MAX;
    Cross.speed                 = AURA_MOUSEMAT_SPEED_DEFAULT_CROSS;
    Cross.brightness_min        = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Cross.brightness_max        = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Cross.brightness            = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Cross.colors_min            = 2;
    Cross.colors_max            = 2;
    Cross.colors.resize(2);
    modes.push_back(Cross);

    mode StarryNight;
    StarryNight.name            = "Starry Night";
    StarryNight.value           = AURA_MOUSEMAT_MODE_STARRY_NIGHT;
    StarryNight.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    StarryNight.color_mode      = MODE_COLORS_MODE_SPECIFIC | MODE_COLORS_RANDOM;
    StarryNight.speed_min       = AURA_MOUSEMAT_SPEED_MIN;
    StarryNight.speed_max       = AURA_MOUSEMAT_SPEED_MAX;
    StarryNight.speed           = AURA_MOUSEMAT_SPEED_DEFAULT_STARRY_NIGHT;
    StarryNight.brightness_min  = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    StarryNight.brightness_max  = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    StarryNight.brightness      = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    StarryNight.colors_min      = 2;
    StarryNight.colors_max      = 2;
    StarryNight.colors.resize(2);
    modes.push_back(StarryNight);

    SetupZones();
}

RGBController_AuraMousemat::~RGBController_AuraMousemat()
{
    delete controller;
}

void RGBController_AuraMousemat::SetupZones()
{
    zone mousemat_zone;

    mousemat_zone.name         = "Mousemat";
    mousemat_zone.type         = ZONE_TYPE_LINEAR;
    mousemat_zone.leds_min     = 15;
    mousemat_zone.leds_max     = 15;
    mousemat_zone.leds_count   = 15;
    mousemat_zone.matrix_map   = NULL;

    zones.push_back(mousemat_zone);

    for(unsigned int i = 0; i < 15; i++)
    {
        led mousemat_led;

        mousemat_led.name      = "Mousemat LED " + std::to_string(i);

        leds.push_back(mousemat_led);
    }

    SetupColors();
}

void RGBController_AuraMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraMousemat::DeviceUpdateLEDs()
{
    controller->UpdateLeds(std::vector<RGBColor>(colors));
}

void RGBController_AuraMousemat::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMousemat::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMousemat::DeviceUpdateMode()
{
    if(modes[active_mode].value == AURA_MOUSEMAT_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        int pattern = 0;

        switch(modes[active_mode].value)
        {
            case AURA_MOUSEMAT_MODE_BREATHING:
                if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
                {
                    pattern = 2;
                }
                else
                {
                    pattern = (int)modes[active_mode].colors.size() - 1;
                }
                break;
            case AURA_MOUSEMAT_MODE_WAVE:
                pattern = (int)modes[active_mode].colors.size() * 16 + modes[active_mode].direction;
                break;
            case AURA_MOUSEMAT_MODE_WAVE_PLANE:
                switch(modes[active_mode].direction)
                {
                    case MODE_DIRECTION_LEFT:
                        pattern = 2;
                        break;
                    case MODE_DIRECTION_RIGHT:
                        pattern = 3;
                        break;
                    case MODE_DIRECTION_UP:
                        pattern = 0;
                        break;
                    case MODE_DIRECTION_DOWN:
                        pattern = 1;
                        break;
                }
                break;
            case AURA_MOUSEMAT_MODE_COMET:
                pattern = modes[active_mode].direction;
                if(pattern == MODE_DIRECTION_HORIZONTAL || pattern == MODE_DIRECTION_VERTICAL) pattern = 2;
                break;
            case AURA_MOUSEMAT_MODE_GLOWING_YOYO:
                pattern = modes[active_mode].direction;
                break;
            case AURA_MOUSEMAT_MODE_STARRY_NIGHT:
                pattern = 16 + (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
                break;
            default:
                pattern = 255;
                break;
        }

        controller->UpdateDevice(modes[active_mode].value, std::vector<RGBColor>(modes[active_mode].colors), modes[active_mode].speed, modes[active_mode].brightness, pattern);
    }
}

void RGBController_AuraMousemat::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
