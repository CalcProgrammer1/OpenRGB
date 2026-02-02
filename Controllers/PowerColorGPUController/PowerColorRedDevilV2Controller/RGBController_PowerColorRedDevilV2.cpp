/*---------------------------------------------------------*\
| RGBController_PowerColorRedDevilV2.cpp                    |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Nexrem                                      15 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "RGBController_PowerColorRedDevilV2.h"

RGBController_PowerColorRedDevilV2::RGBController_PowerColorRedDevilV2(PowerColorRedDevilV2Controller* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetDeviceName();
    vendor                              = "PowerColor";
    description                         = "PowerColor Red Devil V2 GPU Device";
    location                            = controller->GetDeviceLocation();
    type                                = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                            = "Off";
    Off.value                           = RED_DEVIL_V2_MODE_OFF;
    Off.flags                           = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                         = "Custom";
    Static.value                        = RED_DEVIL_V2_MODE_STATIC;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    Static.brightness_min               = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Static.brightness_max               = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Static.brightness                   = RED_DEVIL_V2_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = RED_DEVIL_V2_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode                = MODE_COLORS_PER_LED;
    Breathing.brightness_min            = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Breathing.brightness_max            = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Breathing.brightness                = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Breathing.speed_min                 = RED_DEVIL_V2_SPEED_MIN;
    Breathing.speed_max                 = RED_DEVIL_V2_SPEED_MAX;
    Breathing.speed                     = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Breathing);

    mode SecretRainbow;
    SecretRainbow.name                  = "Secret Rainbow";
    SecretRainbow.value                 = RED_DEVIL_V2_MODE_SECRET_RAINBOW;
    SecretRainbow.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SecretRainbow.color_mode            = MODE_COLORS_NONE;
    SecretRainbow.brightness_min        = RED_DEVIL_V2_BRIGHTNESS_MIN;
    SecretRainbow.brightness_max        = RED_DEVIL_V2_BRIGHTNESS_MAX;
    SecretRainbow.brightness            = RED_DEVIL_V2_BRIGHTNESS_MAX;
    SecretRainbow.speed_min             = RED_DEVIL_V2_SPEED_MIN;
    SecretRainbow.speed_max             = RED_DEVIL_V2_SPEED_MAX;
    SecretRainbow.speed                 = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(SecretRainbow);

    mode Radiance;
    Radiance.name                       = "Radiance";
    Radiance.value                      = RED_DEVIL_V2_MODE_RADIANCE;
    Radiance.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Radiance.color_mode                 = MODE_COLORS_NONE;
    Radiance.brightness_min             = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Radiance.brightness_max             = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Radiance.brightness                 = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Radiance.speed_min                  = RED_DEVIL_V2_SPEED_MIN;
    Radiance.speed_max                  = RED_DEVIL_V2_SPEED_MAX;
    Radiance.speed                      = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Radiance);

    mode Diffuse;
    Diffuse.name                        = "Diffuse";
    Diffuse.value                       = RED_DEVIL_V2_MODE_DIFFUSE;
    Diffuse.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Diffuse.color_mode                  = MODE_COLORS_NONE;
    Diffuse.brightness_min              = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Diffuse.brightness_max              = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Diffuse.brightness                  = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Diffuse.speed_min                   = RED_DEVIL_V2_SPEED_MIN;
    Diffuse.speed_max                   = RED_DEVIL_V2_SPEED_MAX;
    Diffuse.speed                       = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Diffuse);

    mode ColorShift;
    ColorShift.name                     = "Color Shift";
    ColorShift.value                    = RED_DEVIL_V2_MODE_COLOR_SHIFT;
    ColorShift.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.color_mode               = MODE_COLORS_NONE;
    ColorShift.brightness_min           = RED_DEVIL_V2_BRIGHTNESS_MIN;
    ColorShift.brightness_max           = RED_DEVIL_V2_BRIGHTNESS_MAX;
    ColorShift.brightness               = RED_DEVIL_V2_BRIGHTNESS_MAX;
    ColorShift.speed_min                = RED_DEVIL_V2_SPEED_MIN;
    ColorShift.speed_max                = RED_DEVIL_V2_SPEED_MAX;
    ColorShift.speed                    = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(ColorShift);

    mode Meteor;
    Meteor.name                         = "Meteor";
    Meteor.value                        = RED_DEVIL_V2_MODE_METEOR;
    Meteor.flags                        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Meteor.color_mode                   = MODE_COLORS_PER_LED;
    Meteor.brightness_min               = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Meteor.brightness_max               = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Meteor.brightness                   = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Meteor.speed_min                    = RED_DEVIL_V2_SPEED_MIN;
    Meteor.speed_max                    = RED_DEVIL_V2_SPEED_MAX;
    Meteor.speed                        = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Meteor);

    mode Ripple;
    Ripple.name                         = "Ripple";
    Ripple.value                        = RED_DEVIL_V2_MODE_RIPPLE;
    Ripple.flags                        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode                   = MODE_COLORS_PER_LED;
    Ripple.brightness_min               = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Ripple.brightness_max               = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Ripple.brightness                   = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Ripple.speed_min                    = RED_DEVIL_V2_SPEED_MIN;
    Ripple.speed_max                    = RED_DEVIL_V2_SPEED_MAX;
    Ripple.speed                        = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Ripple);

    mode Rainbow;
    Rainbow.name                        = "Rainbow";
    Rainbow.value                       = RED_DEVIL_V2_MODE_RAINBOW;
    Rainbow.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode                  = MODE_COLORS_NONE;
    Rainbow.brightness_min              = RED_DEVIL_V2_BRIGHTNESS_MIN;
    Rainbow.brightness_max              = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Rainbow.brightness                  = RED_DEVIL_V2_BRIGHTNESS_MAX;
    Rainbow.speed_min                   = RED_DEVIL_V2_SPEED_MIN;
    Rainbow.speed_max                   = RED_DEVIL_V2_SPEED_MAX;
    Rainbow.speed                       = RED_DEVIL_V2_SPEED_DEFAULT;
    modes.push_back(Rainbow);

    mode Sync;
    Sync.name                           = "Sync with motherboard";
    Sync.value                          = RED_DEVIL_V2_MODE_SYNC;
    Sync.flags                          = MODE_FLAG_AUTOMATIC_SAVE;
    Sync.color_mode                     = MODE_COLORS_NONE;
    modes.push_back(Sync);

    SetupZones();

    ReadConfig();

    /*------------------------------------------------------------------*\
    | Copy the read colors for later delta-ing                           |
    \*------------------------------------------------------------------*/
    colors_copy = colors;
}

RGBController_PowerColorRedDevilV2::~RGBController_PowerColorRedDevilV2()
{
    Shutdown();

    delete controller;
}

void RGBController_PowerColorRedDevilV2::SetupZones()
{
    zone stripe1;
    stripe1.name                    = "Stripe 1";
    stripe1.type                    = ZONE_TYPE_LINEAR;
    stripe1.leds_min                = 3;
    stripe1.leds_max                = 3;
    stripe1.leds_count              = 3;
    zones.push_back(stripe1);

    zone stripe2;
    stripe2.name                    = "Stripe 2";
    stripe2.type                    = ZONE_TYPE_LINEAR;
    stripe2.leds_min                = 3;
    stripe2.leds_max                = 3;
    stripe2.leds_count              = 3;
    zones.push_back(stripe2);

    static unsigned int hellstone_map[2][7] =
    {
        {  0,  1,  2,  3,  4,  5,  6 },
        { 13, 12, 11, 10,  9,  8,  7 }
    };

    zone hellstone;
    hellstone.name                  = "Hellstone";
    hellstone.type                  = ZONE_TYPE_MATRIX;
    hellstone.leds_min              = 14;
    hellstone.leds_max              = 14;
    hellstone.leds_count            = 14;
    hellstone.matrix_map.Set(2, 7, (unsigned int *)hellstone_map);
    zones.push_back(hellstone);

    zone devil;
    devil.name                      = "Devil";
    devil.type                      = ZONE_TYPE_LINEAR;
    devil.leds_min                  = 4;
    devil.leds_max                  = 4;
    devil.leds_count                = 4;
    zones.push_back(devil);

    /*------------------------------------------------------------------*\
    | Create the LEDs for each zone                                      |
    \*------------------------------------------------------------------*/
    for(unsigned int i = 0; i < stripe1.leds_count; i++)
    {
        led new_led;
        new_led.name = stripe1.name + " " + std::to_string(i+1);
        leds.push_back(new_led);
    }

    for(unsigned int i = 0; i < stripe2.leds_count; i++)
    {
        led new_led;
        new_led.name = stripe2.name + " " + std::to_string(i+1);
        leds.push_back(new_led);
    }

    for(unsigned int i = 0; i < hellstone.leds_count; i++)
    {
        led new_led;
        new_led.name = hellstone.name + " " + std::to_string(i+1);
        leds.push_back(new_led);
    }

    for(unsigned int i = 0; i < devil.leds_count; i++)
    {
        led new_led;
        new_led.name = devil.name + " " + std::to_string(i+1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_PowerColorRedDevilV2::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | Check if all colors are identical. If they are do a       |
    | single register write instead of writing to each LED      |
    \*---------------------------------------------------------*/
    bool all_same = true;
    for(std::size_t i = 1; i < colors.size(); i++)
    {
        if(colors[i-1] != colors[i])
        {
            all_same = false;
            break;
        }
    }

    /*---------------------------------------------------------*\
    | Do single register write to set all                       |
    \*---------------------------------------------------------*/
    if(all_same)
    {
        RGBColor color = colors[0];
        controller->SetLedColorAll(color);
    }
    else
    {
        /*---------------------------------------------------------*\
        | Since writing to each LED is slow check which colors have |
        | changed and only write those instead                      |
        \*---------------------------------------------------------*/
        for(std::size_t i = 0; i < colors.size(); i++)
        {
            if(colors[i] != colors_copy[i])
            {
                controller->SetLedColor((int)i, colors[i]);
            }
        }
    }

    /*---------------------------------------------------------*\
    | Store changed colors                                      |
    \*---------------------------------------------------------*/
    colors_copy = colors;
}

void RGBController_PowerColorRedDevilV2::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_PowerColorRedDevilV2::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_PowerColorRedDevilV2::DeviceUpdateMode()
{
    red_devil_v2_mode mode;
    mode.mode       = (unsigned char)modes[active_mode].value;
    mode.brightness = (unsigned char)modes[active_mode].brightness;
    mode.speed      = (unsigned char)modes[active_mode].speed;

    controller->SetMode(mode);
}

void RGBController_PowerColorRedDevilV2::ReadConfig()
{
    red_devil_v2_mode mode  = controller->GetMode();
    bool sync               = controller->GetSync();

    for(std::size_t i = 0; i < colors.size(); i++)
    {
        colors[i] = controller->GetLedColor((int)i);
    }

    /*---------------------------------------------------------*\
    | Since Sync is not actually "a mode" it needs special      |
    | handling                                                  |
    \*---------------------------------------------------------*/
    if(sync)
    {
        active_mode = 10;
    }
    else if(mode.mode < modes.size() - 1)
    {
        /*---------------------------------------------------------*\
        | Mode ordering is important, keep them in order            |
        \*---------------------------------------------------------*/
        active_mode = mode.mode;
    }

    modes[active_mode].brightness   = mode.brightness;
    modes[active_mode].speed        = mode.speed;
}
