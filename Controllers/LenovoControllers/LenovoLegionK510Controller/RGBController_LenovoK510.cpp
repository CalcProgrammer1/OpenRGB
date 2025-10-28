/*---------------------------------------------------------*\
| RGBController_LenovoM300.cpp                              |
|                                                           |
|   RGBController for Lenovo Legion K510 keyboard           |
|                                                           |
|   Bnyro                                       27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LenovoK510.h"

/**------------------------------------------------------------------*\
    @name Lenovo Legion K510
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectLenovoLegionK510Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LenovoK510::RGBController_LenovoK510(LenovoK510Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Lenovo";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Lenovo Legion K510 Mini Pro";
    location                        = controller->GetDeviceLocation();

    mode Corrugated;
    Corrugated.name                 = "Corrugated";
    Corrugated.value                = K510_MODE_CORRUGATED;
    Corrugated.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Corrugated.color_mode           = MODE_COLORS_RANDOM;
    Corrugated.brightness           = K510_BRIGHTNESS_DEFAULT;
    Corrugated.brightness_min       = K510_BRIGHTNESS_MIN;
    Corrugated.brightness_max       = K510_BRIGHTNESS_MAX;
    Corrugated.speed                = K510_SPEED_DEFAULT;
    Corrugated.speed_min            = K510_SPEED_MIN;
    Corrugated.speed_max            = K510_SPEED_MAX;
    Corrugated.colors.resize(1);
    modes.push_back(Corrugated);

    mode Cloud;
    Cloud.name                      = "Cloud";
    Cloud.value                     = K510_MODE_CLOUD;
    Cloud.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Cloud.color_mode                = MODE_COLORS_RANDOM;
    Cloud.brightness                = K510_BRIGHTNESS_DEFAULT;
    Cloud.brightness_min            = K510_BRIGHTNESS_MIN;
    Cloud.brightness_max            = K510_BRIGHTNESS_MAX;
    Cloud.speed                     = K510_SPEED_DEFAULT;
    Cloud.speed_min                 = K510_SPEED_MIN;
    Cloud.speed_max                 = K510_SPEED_MAX;
    Cloud.colors.resize(1);
    modes.push_back(Cloud);

    mode Serpentine;
    Serpentine.name                 = "Serpentine";
    Serpentine.value                = K510_MODE_SERPENTINE;
    Serpentine.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Serpentine.color_mode           = MODE_COLORS_RANDOM;
    Serpentine.brightness           = K510_BRIGHTNESS_DEFAULT;
    Serpentine.brightness_min       = K510_BRIGHTNESS_MIN;
    Serpentine.brightness_max       = K510_BRIGHTNESS_MAX;
    Serpentine.speed                = K510_SPEED_DEFAULT;
    Serpentine.speed_min            = K510_SPEED_MIN;
    Serpentine.speed_max            = K510_SPEED_MAX;
    Serpentine.colors.resize(1);
    modes.push_back(Serpentine);

    mode Spectrum;
    Spectrum.name                  = "Spectrum Cycle";
    Spectrum.value                 = K510_MODE_SPECTRUM;
    Spectrum.flags                 = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.color_mode            = MODE_COLORS_NONE;
    Spectrum.brightness            = K510_BRIGHTNESS_DEFAULT;
    Spectrum.brightness_min        = K510_BRIGHTNESS_MIN;
    Spectrum.brightness_max        = K510_BRIGHTNESS_MAX;
    Spectrum.speed                 = K510_SPEED_DEFAULT;
    Spectrum.speed_min             = K510_SPEED_MIN;
    Spectrum.speed_max             = K510_SPEED_MAX;
    modes.push_back(Spectrum);

    mode Breath;
    Breath.name                    = "Breathing";
    Breath.value                   = K510_MODE_BREATH;
    Breath.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breath.color_mode              = MODE_COLORS_RANDOM;
    Breath.brightness              = K510_BRIGHTNESS_DEFAULT;
    Breath.brightness_min          = K510_BRIGHTNESS_MIN;
    Breath.brightness_max          = K510_BRIGHTNESS_MAX;
    Breath.speed                   = K510_SPEED_DEFAULT;
    Breath.speed_min               = K510_SPEED_MIN;
    Breath.speed_max               = K510_SPEED_MAX;
    Breath.colors.resize(1);
    modes.push_back(Breath);

    mode Normal;
    Normal.name                    = "Static";
    Normal.value                   = K510_MODE_NORMAL;
    Normal.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Normal.color_mode              = MODE_COLORS_RANDOM;
    Normal.brightness              = K510_BRIGHTNESS_DEFAULT;
    Normal.brightness_min          = K510_BRIGHTNESS_MIN;
    Normal.brightness_max          = K510_BRIGHTNESS_MAX;
    Normal.colors.resize(1);
    modes.push_back(Normal);

    mode Reaction;
    Reaction.name                  = "Reaction";
    Reaction.value                 = K510_MODE_REACTION;
    Reaction.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Reaction.color_mode            = MODE_COLORS_RANDOM;
    Reaction.brightness            = K510_BRIGHTNESS_DEFAULT;
    Reaction.brightness_min        = K510_BRIGHTNESS_MIN;
    Reaction.brightness_max        = K510_BRIGHTNESS_MAX;
    Reaction.speed                 = K510_SPEED_DEFAULT;
    Reaction.speed_min             = K510_SPEED_MIN;
    Reaction.speed_max             = K510_SPEED_MAX;
    Reaction.colors.resize(1);
    modes.push_back(Reaction);

    mode Ripples;
    Ripples.name                    = "Ripples";
    Ripples.value                   = K510_MODE_RIPPLES;
    Ripples.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Ripples.color_mode              = MODE_COLORS_RANDOM;
    Ripples.brightness              = K510_BRIGHTNESS_DEFAULT;
    Ripples.brightness_min          = K510_BRIGHTNESS_MIN;
    Ripples.brightness_max          = K510_BRIGHTNESS_MAX;
    Ripples.speed                   = K510_SPEED_DEFAULT;
    Ripples.speed_min               = K510_SPEED_MIN;
    Ripples.speed_max               = K510_SPEED_MAX;
    Ripples.colors.resize(1);
    modes.push_back(Ripples);

    mode Traverse;
    Traverse.name                   = "Traverse";
    Traverse.value                  = K510_MODE_TRAVERSE;
    Traverse.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Traverse.color_mode             = MODE_COLORS_RANDOM;
    Traverse.brightness             = K510_BRIGHTNESS_DEFAULT;
    Traverse.brightness_min         = K510_BRIGHTNESS_MIN;
    Traverse.brightness_max         = K510_BRIGHTNESS_MAX;
    Traverse.speed                  = K510_SPEED_DEFAULT;
    Traverse.speed_min              = K510_SPEED_MIN;
    Traverse.speed_max              = K510_SPEED_MAX;
    Traverse.colors.resize(1);
    modes.push_back(Traverse);

    mode Stars;
    Stars.name                      = "Stars";
    Stars.value                     = K510_MODE_STARS;
    Stars.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Stars.color_mode                = MODE_COLORS_RANDOM;
    Stars.brightness                = K510_BRIGHTNESS_DEFAULT;
    Stars.brightness_min            = K510_BRIGHTNESS_MIN;
    Stars.brightness_max            = K510_BRIGHTNESS_MAX;
    Stars.speed                     = K510_SPEED_DEFAULT;
    Stars.speed_min                 = K510_SPEED_MIN;
    Stars.speed_max                 = K510_SPEED_MAX;
    Stars.colors.resize(1);
    modes.push_back(Stars);

    mode Flowers;
    Flowers.name                    = "Flowers";
    Flowers.value                   = K510_MODE_FLOWERS;
    Flowers.flags                   = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Flowers.color_mode              = MODE_COLORS_RANDOM;
    Flowers.brightness              = K510_BRIGHTNESS_DEFAULT;
    Flowers.brightness_min          = K510_BRIGHTNESS_MIN;
    Flowers.brightness_max          = K510_BRIGHTNESS_MAX;
    Flowers.speed                   = K510_SPEED_DEFAULT;
    Flowers.speed_min               = K510_SPEED_MIN;
    Flowers.speed_max               = K510_SPEED_MAX;
    modes.push_back(Flowers);

    mode Roll;
    Roll.name                       = "Rainbow Wave";
    Roll.value                      = K510_MODE_ROLL;
    Roll.flags                      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_UD;
    Roll.color_mode                 = MODE_COLORS_NONE;
    Roll.brightness                 = K510_BRIGHTNESS_DEFAULT;
    Roll.brightness_min             = K510_BRIGHTNESS_MIN;
    Roll.brightness_max             = K510_BRIGHTNESS_MAX;
    Roll.speed                      = K510_SPEED_DEFAULT;
    Roll.speed_min                  = K510_SPEED_MIN;
    Roll.speed_max                  = K510_SPEED_MAX;
    modes.push_back(Roll);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = K510_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.color_mode                 = MODE_COLORS_RANDOM;
    Wave.brightness                 = K510_BRIGHTNESS_DEFAULT;
    Wave.brightness_min             = K510_BRIGHTNESS_MIN;
    Wave.brightness_max             = K510_BRIGHTNESS_MAX;
    Wave.speed                      = K510_SPEED_DEFAULT;
    Wave.speed_min                  = K510_SPEED_MIN;
    Wave.speed_max                  = K510_SPEED_MAX;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode Cartoon;
    Cartoon.name                    = "Cartoon";
    Cartoon.value                   = K510_MODE_CARTOON;
    Cartoon.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Cartoon.color_mode              = MODE_COLORS_RANDOM;
    Cartoon.brightness              = K510_BRIGHTNESS_DEFAULT;
    Cartoon.brightness_min          = K510_BRIGHTNESS_MIN;
    Cartoon.brightness_max          = K510_BRIGHTNESS_MAX;
    Cartoon.speed                   = K510_SPEED_DEFAULT;
    Cartoon.speed_min               = K510_SPEED_MIN;
    Cartoon.speed_max               = K510_SPEED_MAX;
    Cartoon.colors.resize(1);
    modes.push_back(Cartoon);

    mode Rain;
    Rain.name                       = "Rain";
    Rain.value                      = K510_MODE_RAIN;
    Rain.flags                      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.color_mode                 = MODE_COLORS_RANDOM;
    Rain.brightness                 = K510_BRIGHTNESS_DEFAULT;
    Rain.brightness_min             = K510_BRIGHTNESS_MIN;
    Rain.brightness_max             = K510_BRIGHTNESS_MAX;
    Rain.speed                      = K510_SPEED_DEFAULT;
    Rain.speed_min                  = K510_SPEED_MIN;
    Rain.speed_max                  = K510_SPEED_MAX;
    Rain.colors.resize(1);
    modes.push_back(Rain);

    mode Scan;
    Scan.name                       = "Scan";
    Scan.value                      = K510_MODE_SCAN;
    Scan.flags                      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Scan.color_mode                 = MODE_COLORS_NONE;
    Scan.brightness                 = K510_BRIGHTNESS_DEFAULT;
    Scan.brightness_min             = K510_BRIGHTNESS_MIN;
    Scan.brightness_max             = K510_BRIGHTNESS_MAX;
    modes.push_back(Scan);

    mode Surmount;
    Surmount.name                    = "Surmount";
    Surmount.value                   = K510_MODE_SURMOUNT;
    Surmount.flags                   = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Surmount.color_mode              = MODE_COLORS_NONE;
    Surmount.brightness              = K510_BRIGHTNESS_DEFAULT;
    Surmount.brightness_min          = K510_BRIGHTNESS_MIN;
    Surmount.brightness_max          = K510_BRIGHTNESS_MAX;
    modes.push_back(Surmount);

    mode Speed;
    Speed.name                       = "Speed";
    Speed.value                      = K510_MODE_SPEED;
    Speed.flags                      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Speed.color_mode                 = MODE_COLORS_NONE;
    Speed.brightness                 = K510_BRIGHTNESS_DEFAULT;
    Speed.brightness_min             = K510_BRIGHTNESS_MIN;
    Speed.brightness_max             = K510_BRIGHTNESS_MAX;
    Speed.speed                      = K510_SPEED_DEFAULT;
    Speed.speed_min                  = K510_SPEED_MIN;
    Speed.speed_max                  = K510_SPEED_MAX;
    modes.push_back(Speed);

    SetupZones();
    ReadAndUpdateCurrentDeviceState();
}

RGBController_LenovoK510::~RGBController_LenovoK510()
{
    delete controller;
}

void RGBController_LenovoK510::SetupZones()
{
    zone default_zone;
    default_zone.name           = "Keyboard";
    default_zone.type           = ZONE_TYPE_SINGLE;
    default_zone.leds_min       = 1;
    default_zone.leds_max       = 1;
    default_zone.leds_count     = 1;
    default_zone.matrix_map     = nullptr;
    zones.emplace_back(default_zone);

    leds.resize(1);
    leds[0].name = "Keyboard";

    SetupColors();
}

void RGBController_LenovoK510::ReadAndUpdateCurrentDeviceState()
{
    mode current_active_mode = controller->GetCurrentState();

    for(std::vector<mode>::size_type i = 0; i < modes.size(); ++i)
    {
        if(modes[i].value == current_active_mode.value)
        {
            // override the default config of the mode with the current one
            modes[i].brightness = current_active_mode.brightness;
            modes[i].speed = current_active_mode.speed;
            modes[i].color_mode = current_active_mode.color_mode;
            zones[0].colors[0] = current_active_mode.colors[0];

            if(modes[i].flags & MODE_FLAG_HAS_DIRECTION_LR)
            {
                current_active_mode.direction = current_active_mode.direction ? MODE_DIRECTION_LEFT : MODE_DIRECTION_RIGHT;
            }
            else if(modes[i].flags & MODE_FLAG_HAS_DIRECTION_UD)
            {
                current_active_mode.direction = current_active_mode.direction ? MODE_DIRECTION_UP : MODE_DIRECTION_DOWN;
            }

            active_mode = i;
            break;
        }
    }
}

void RGBController_LenovoK510::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Not Supported
}

void RGBController_LenovoK510::DeviceUpdateLEDs()
{
    // Not Supported
}

void RGBController_LenovoK510::UpdateZoneLEDs(int /*zone*/)
{
    // Not Supported
}

void RGBController_LenovoK510::UpdateSingleLED(int /*led*/)
{
    // Not Supported
}

void RGBController_LenovoK510::DeviceUpdateMode()
{
    const mode& active = modes[active_mode];
    RGBColor color = active.colors.size() > 0 ? active.colors[0] : 0x00;
    controller->SetMode(active.color_mode, color, active.value, active.brightness, active.speed, active.direction);
}
