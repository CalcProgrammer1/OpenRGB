/*---------------------------------------------------------*\
| RGBController_ManliGPU.cpp                                |
|                                                           |
|   RGBController for Manli GPU                             |
|                                                           |
|   Based on RGBController_ZotacV2GPU                       |
|   Adapted for Manli RTX 4090 Gallardo                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <map>
#include "RGBController_ManliGPU.h"

std::map<std::string, ManliGPUConfig> MANLI_GPU_CONFIG =
{
    { "N675M-1018", { 1, false } },  // MANLI GeForce RTX 4090 Gallardo
};

/**------------------------------------------------------------------*\
    @name Manli GPU
    @category GPU
    @type I2C
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectManliGPUControllers
    @comment
        Manli GPU RGB controllers use I2C communication at address 0x49.
        Supported modes: Static, Breathing, Wave, Strobing, Rainbow, Color Cycle.
\*-------------------------------------------------------------------*/

RGBController_ManliGPU::RGBController_ManliGPU(ManliGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "MANLI";
    description             = "MANLI RGB GPU Device (" + controller->GetVersion() + ")";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    if(MANLI_GPU_CONFIG.count(controller->GetVersion()) > 0)
    {
        config = MANLI_GPU_CONFIG.at(controller->GetVersion());
    }
    else
    {
        config = { 0, false };
    }

    version += std::to_string(config.numberOfZones) + " zones";

    mode STATIC;
    STATIC.name             = "Static";
    STATIC.value            = MANLI_GPU_MODE_STATIC;
    STATIC.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    STATIC.brightness_min   = 0;
    STATIC.brightness_max   = 100;
    STATIC.brightness       = 100;
    STATIC.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    STATIC.colors_min       = 1;
    STATIC.colors_max       = 1;
    STATIC.colors.resize(1);
    STATIC.colors[0]        = ToRGBColor(0, 0, 255);
    modes.push_back(STATIC);

    mode BREATHING;
    BREATHING.name          = "Breathing";
    BREATHING.value         = MANLI_GPU_MODE_BREATHING;
    BREATHING.flags         = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BREATHING.brightness_min = 0;
    BREATHING.brightness_max = 100;
    BREATHING.brightness    = 100;
    BREATHING.speed_min     = 0;
    BREATHING.speed_max     = 100;
    BREATHING.speed         = 50;
    BREATHING.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    BREATHING.colors_min    = 1;
    BREATHING.colors_max    = 1;
    BREATHING.colors.resize(1);
    BREATHING.colors[0]     = ToRGBColor(0, 0, 255);
    modes.push_back(BREATHING);

    mode WAVE;
    WAVE.name               = "Wave";
    WAVE.value              = MANLI_GPU_MODE_WAVE;
    WAVE.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;  // No brightness - HW limitation
    WAVE.speed_min          = 0;
    WAVE.speed_max          = 100;
    WAVE.speed              = 50;
    WAVE.color_mode         = MODE_COLORS_NONE;
    modes.push_back(WAVE);

    mode STROBING;
    STROBING.name           = "Strobing";
    STROBING.value          = MANLI_GPU_MODE_STROBING;
    STROBING.flags          = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    STROBING.brightness_min = 0;
    STROBING.brightness_max = 100;
    STROBING.brightness     = 100;
    STROBING.speed_min      = 0;
    STROBING.speed_max      = 100;
    STROBING.speed          = 50;
    STROBING.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    STROBING.colors_min     = 1;
    STROBING.colors_max     = 1;
    STROBING.colors.resize(1);
    STROBING.colors[0]      = ToRGBColor(0, 0, 255);
    modes.push_back(STROBING);

    mode RAINBOW;
    RAINBOW.name            = "Rainbow";
    RAINBOW.value           = MANLI_GPU_MODE_RAINBOW;
    RAINBOW.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    RAINBOW.speed_min       = 0;
    RAINBOW.speed_max       = 100;
    RAINBOW.speed           = 50;
    RAINBOW.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RAINBOW);

    mode COLOR_CYCLE;
    COLOR_CYCLE.name            = "Color Cycle";
    COLOR_CYCLE.value           = MANLI_GPU_MODE_COLOR_CYCLE;
    COLOR_CYCLE.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    COLOR_CYCLE.brightness_min  = 0;
    COLOR_CYCLE.brightness_max  = 100;
    COLOR_CYCLE.brightness      = 100;
    COLOR_CYCLE.speed_min       = 0;
    COLOR_CYCLE.speed_max       = 100;
    COLOR_CYCLE.speed           = 50;
    COLOR_CYCLE.color_mode      = MODE_COLORS_NONE;
    modes.push_back(COLOR_CYCLE);

    SetupZones();
}

RGBController_ManliGPU::~RGBController_ManliGPU()
{
    delete controller;
}

void RGBController_ManliGPU::SetupZones()
{
    led new_led;
    new_led.name           = "GPU LED";
    leds.push_back(new_led);

    zone new_zone;
    new_zone.name          = "GPU Zone";
    new_zone.type          = ZONE_TYPE_SINGLE;
    new_zone.leds_min      = 1;
    new_zone.leds_max      = 1;
    new_zone.leds_count    = 1;
    new_zone.matrix_map    = NULL;
    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_ManliGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ManliGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ManliGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_ManliGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_ManliGPU::DeviceUpdateMode()
{
    ManliGPUZone zoneConfig;
    zoneConfig.mode = modes[active_mode].value;
    zoneConfig.speed = modes[active_mode].speed;
    zoneConfig.brightness = modes[active_mode].brightness;

    if(modes[active_mode].colors.size() >= 1)
    {
        zoneConfig.color1 = modes[active_mode].colors[0];
    }
    else
    {
        zoneConfig.color1 = ToRGBColor(0, 0, 0);
    }

    controller->SetMode(zoneConfig);
}
