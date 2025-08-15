/*---------------------------------------------------------*\
| RGBController_PowerColorRedDevilGPU.cpp                   |
|                                                           |
|   Driver for PowerColor Red Devil GPU                     |
|                                                           |
|   Jana Rettig (SapphicKitten)                 14 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PowerColorRedDevilGPU.h"

RGBController_RedDevilGPU::RGBController_RedDevilGPU(RedDevilGPUController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "PowerColor";
    description                 = "PowerColor Red Devil GPU Device";
    location                    = controller->GetDeviceLocation();
    type                        = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                    = "Off";
    Off.value                   = RED_DEVIL_GPU_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = RED_DEVIL_GPU_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness_min       = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Static.brightness_max       = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Static.brightness           = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = RED_DEVIL_GPU_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness_min    = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Breathing.brightness_max    = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Breathing.brightness        = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Breathing.speed_min         = RED_DEVIL_GPU_SPEED_SLOWEST;
    Breathing.speed_max         = RED_DEVIL_GPU_SPEED_FASTEST;
    Breathing.speed             = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(Breathing);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = RED_DEVIL_GPU_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Neon.color_mode             = MODE_COLORS_NONE;
    Neon.brightness_min         = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Neon.brightness_max         = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Neon.brightness             = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Neon.speed_min              = RED_DEVIL_GPU_SPEED_SLOWEST;
    Neon.speed_max              = RED_DEVIL_GPU_SPEED_FASTEST;
    Neon.speed                  = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(Neon);

    mode Blink;
    Blink.name                  = "Blink";
    Blink.value                 = RED_DEVIL_GPU_MODE_BLINK;
    Blink.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Blink.color_mode            = MODE_COLORS_PER_LED;
    Blink.brightness_min        = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Blink.brightness_max        = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Blink.brightness            = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Blink.speed_min             = RED_DEVIL_GPU_SPEED_SLOWEST;
    Blink.speed_max             = RED_DEVIL_GPU_SPEED_FASTEST;
    Blink.speed                 = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(Blink);

    mode DoubleBlink;
    DoubleBlink.name            = "Double Blink";
    DoubleBlink.value           = RED_DEVIL_GPU_MODE_DOUBLE_BLINK;
    DoubleBlink.flags           = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    DoubleBlink.color_mode      = MODE_COLORS_PER_LED;
    DoubleBlink.brightness_min  = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    DoubleBlink.brightness_max  = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    DoubleBlink.brightness      = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    DoubleBlink.speed_min       = RED_DEVIL_GPU_SPEED_SLOWEST;
    DoubleBlink.speed_max       = RED_DEVIL_GPU_SPEED_FASTEST;
    DoubleBlink.speed           = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(DoubleBlink);

    mode ColorShift;
    ColorShift.name             = "Color Shift";
    ColorShift.value            = RED_DEVIL_GPU_MODE_COLOR_SHIFT;
    ColorShift.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.color_mode       = MODE_COLORS_NONE;
    ColorShift.brightness_min   = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    ColorShift.brightness_max   = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    ColorShift.brightness       = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    ColorShift.speed_min        = RED_DEVIL_GPU_SPEED_SLOWEST;
    ColorShift.speed_max        = RED_DEVIL_GPU_SPEED_FASTEST;
    ColorShift.speed            = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(ColorShift);


    mode Meteor;
    Meteor.name                 = "Meteor";
    Meteor.value                = RED_DEVIL_GPU_MODE_METEOR;
    Meteor.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Meteor.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors_min           = 1;
    Meteor.colors_max           = 1;
    Meteor.colors.resize(1);
    Meteor.brightness_min       = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Meteor.brightness_max       = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Meteor.brightness           = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Meteor.speed_min            = RED_DEVIL_GPU_SPEED_SLOWEST;
    Meteor.speed_max            = RED_DEVIL_GPU_SPEED_FASTEST;
    Meteor.speed                = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(Meteor);

    mode Ripple;
    Ripple.name                 = "Ripple";
    Ripple.value                = RED_DEVIL_GPU_MODE_RIPPLE;
    Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Ripple.colors_min           = 1;
    Ripple.colors_max           = 1;
    Ripple.colors.resize(1);
    Ripple.brightness_min       = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    Ripple.brightness_max       = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Ripple.brightness           = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    Ripple.speed_min            = RED_DEVIL_GPU_SPEED_SLOWEST;
    Ripple.speed_max            = RED_DEVIL_GPU_SPEED_FASTEST;
    Ripple.speed                = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(Ripple);

    mode SevenColors;
    SevenColors.name            = "Seven Colors";
    SevenColors.value           = RED_DEVIL_GPU_MODE_SEVEN_COLORS;
    SevenColors.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SevenColors.color_mode      = MODE_COLORS_NONE;
    SevenColors.brightness_min  = RED_DEVIL_GPU_BRIGHTNESS_MIN;
    SevenColors.brightness_max  = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    SevenColors.brightness      = RED_DEVIL_GPU_BRIGHTNESS_MAX;
    SevenColors.speed_min       = RED_DEVIL_GPU_SPEED_SLOWEST;
    SevenColors.speed_max       = RED_DEVIL_GPU_SPEED_FASTEST;
    SevenColors.speed           = RED_DEVIL_GPU_SPEED_DEFAULT;
    modes.push_back(SevenColors);

    if(controller->has_sync_mode)
    {
        mode Sync;
        Sync.name               = "Sync with Motherboard";
        Sync.value              = RED_DEVIL_GPU_MODE_MB_SYNC;
        Sync.flags              = MODE_FLAG_AUTOMATIC_SAVE;
        Sync.color_mode         = MODE_COLORS_NONE;
        modes.push_back(Sync);
    }

    SetupZones();

    red_devil_mode_config config = controller->GetMode();
    active_mode = config.mode;

    if(active_mode != RED_DEVIL_GPU_MODE_OFF)
    {
        modes[active_mode].brightness   = config.brightness;
        modes[active_mode].speed        = config.speed;

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            modes[active_mode].colors[0] = controller->GetModeColor();
        }
        else
        {
            colors[0] = controller->GetLEDColor(0);
        }
    }
}

RGBController_RedDevilGPU::~RGBController_RedDevilGPU()
{
    delete controller;
}

void RGBController_RedDevilGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone* new_zone          = new zone();

    new_zone->name          = "GPU";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;
    zones.push_back(*new_zone);

    /*---------------------------------------------------------*\
    | This device can control up to 12 LEDs                     |
    | For now all LEDs show the same color                      |
    \*---------------------------------------------------------*/
    led* new_led            = new led();

    new_led->name           = "GPU";
    leds.push_back(*new_led);

    SetupColors();
}

void RGBController_RedDevilGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RedDevilGPU::DeviceUpdateLEDs()
{
    controller->SetLEDColorAll(colors[0]);
}

void RGBController_RedDevilGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RedDevilGPU::UpdateSingleLED(int led)
{
    controller->SetLEDColor(led, colors[led]);
}

void RGBController_RedDevilGPU::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        if(modes[active_mode].colors[0] != 0)
        {
            controller->SetModeColor(modes[active_mode].colors[0]);
        }
    }

    red_devil_mode_config config{(unsigned char)modes[active_mode].value, (unsigned char)modes[active_mode].brightness, (unsigned char)modes[active_mode].speed};
    controller->SetMode(config);
}
