/*---------------------------------------------------------*\
| RGBController_AorusATC800.cpp                             |
|                                                           |
|   RGBController for Aorus ATC800 cooler                   |
|                                                           |
|   Felipe Cavalcanti                           13 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AorusATC800.h"

/**------------------------------------------------------------------*\
    @name Aorus ATC800
    @category Cooler
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectGigabyteAorusCPUCoolerControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AorusATC800::RGBController_AorusATC800(ATC800Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Gigabyte";
    type                            = DEVICE_TYPE_COOLER;
    description                     = "Aorus ATC800 CPU Cooler Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Custom;
    Custom.name                     = "Custom";
    Custom.value                    = AORUS_ATC800_MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    Custom.brightness_min           = AORUS_ATC800_BRIGHTNESS_MIN;
    Custom.brightness_max           = AORUS_ATC800_BRIGHTNESS_MAX;
    Custom.brightness               = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(Custom);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = AORUS_ATC800_MODE_OFF;
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Flashing;
    Flashing.name                   = "Flashing";
    Flashing.value                  = AORUS_ATC800_MODE_FLASHING;
    Flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Flashing.color_mode             = MODE_COLORS_PER_LED;
    Flashing.speed_min              = AORUS_ATC800_SPEED_SLOWEST;
    Flashing.speed_max              = AORUS_ATC800_SPEED_FASTEST;
    Flashing.speed                  = AORUS_ATC800_SPEED_NORMAL;
    Flashing.brightness_min         = AORUS_ATC800_BRIGHTNESS_MIN;
    Flashing.brightness_max         = AORUS_ATC800_BRIGHTNESS_MAX;
    Flashing.brightness             = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(Flashing);

    mode DoubleFlashing;
    DoubleFlashing.name             = "Double Flashing";
    DoubleFlashing.value            = AORUS_ATC800_MODE_DOUBLE_FLASHING;
    DoubleFlashing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    DoubleFlashing.color_mode       = MODE_COLORS_PER_LED;
    DoubleFlashing.speed_min        = AORUS_ATC800_SPEED_SLOWEST;
    DoubleFlashing.speed_max        = AORUS_ATC800_SPEED_FASTEST;
    DoubleFlashing.speed            = AORUS_ATC800_SPEED_NORMAL;
    DoubleFlashing.brightness_min   = AORUS_ATC800_BRIGHTNESS_MIN;
    DoubleFlashing.brightness_max   = AORUS_ATC800_BRIGHTNESS_MAX;
    DoubleFlashing.brightness       = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(DoubleFlashing);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AORUS_ATC800_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed_min             = AORUS_ATC800_SPEED_SLOWEST;
    Breathing.speed_max             = AORUS_ATC800_SPEED_FASTEST;
    Breathing.speed                 = AORUS_ATC800_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Gradient;
    Gradient.name                   = "Gradient";
    Gradient.value                  = AORUS_ATC800_MODE_GRADIENT;
    Gradient.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Gradient.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Gradient.colors_min             = 1;
    Gradient.colors_max             = 1;
    Gradient.colors                 = { 0x0000FF };
    Gradient.speed_min              = AORUS_ATC800_SPEED_SLOWEST;
    Gradient.speed_max              = AORUS_ATC800_SPEED_FASTEST;
    Gradient.speed                  = AORUS_ATC800_SPEED_NORMAL;
    Gradient.brightness_min         = AORUS_ATC800_BRIGHTNESS_MIN;
    Gradient.brightness_max         = AORUS_ATC800_BRIGHTNESS_MAX;
    Gradient.brightness             = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(Gradient);

    mode ColorShift;
    ColorShift.name                 = "Color Shift";
    ColorShift.value                = AORUS_ATC800_MODE_COLOR_SHIFT;
    ColorShift.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    ColorShift.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.colors_min           = 2;
    ColorShift.colors_max           = 8;
    ColorShift.colors               = { 0x0000FF, 0x0072FF, 0x00FFFF, 0x00FF00, 0xFFFF00, 0xFF0000, 0xFF00FF, 0x8080FF };
    ColorShift.speed_min            = AORUS_ATC800_SPEED_SLOWEST;
    ColorShift.speed_max            = AORUS_ATC800_SPEED_FASTEST;
    ColorShift.speed                = AORUS_ATC800_SPEED_NORMAL;
    ColorShift.brightness_min       = AORUS_ATC800_BRIGHTNESS_MIN;
    ColorShift.brightness_max       = AORUS_ATC800_BRIGHTNESS_MAX;
    ColorShift.brightness           = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(ColorShift);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.value               = AORUS_ATC800_MODE_RAINBOW_WAVE;
    RainbowWave.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowWave.color_mode          = MODE_COLORS_NONE;
    RainbowWave.speed_min           = AORUS_ATC800_SPEED_SLOWEST;
    RainbowWave.speed_max           = AORUS_ATC800_SPEED_FASTEST;
    RainbowWave.speed               = AORUS_ATC800_SPEED_NORMAL;
    RainbowWave.brightness_min      = AORUS_ATC800_BRIGHTNESS_MIN;
    RainbowWave.brightness_max      = AORUS_ATC800_BRIGHTNESS_MAX;
    RainbowWave.brightness          = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(RainbowWave);

    mode Radiate;
    Radiate.name                    = "Radiate";
    Radiate.value                   = AORUS_ATC800_MODE_RADIATE;
    Radiate.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Radiate.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Radiate.colors_min              = 8;
    Radiate.colors_max              = 8;
    Radiate.colors                  = { 0x0000FF, 0x0072FF, 0x00FFFF, 0x00FF00, 0xFFFF00, 0xFFFF00, 0xFF00FF, 0x8080FF };
    Radiate.speed_min               = AORUS_ATC800_SPEED_SLOWEST;
    Radiate.speed_max               = AORUS_ATC800_SPEED_FASTEST;
    Radiate.speed                   = AORUS_ATC800_SPEED_NORMAL;
    Radiate.brightness_min          = AORUS_ATC800_BRIGHTNESS_MIN;
    Radiate.brightness_max          = AORUS_ATC800_BRIGHTNESS_MAX;
    Radiate.brightness              = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(Radiate);

    mode RainbowLoop;
    RainbowLoop.name                = "Rainbow Loop";
    RainbowLoop.value               = AORUS_ATC800_MODE_RAINBOW_LOOP;
    RainbowLoop.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RainbowLoop.color_mode          = MODE_COLORS_NONE;
    RainbowLoop.speed_min           = AORUS_ATC800_SPEED_SLOWEST;
    RainbowLoop.speed_max           = AORUS_ATC800_SPEED_FASTEST;
    RainbowLoop.speed               = AORUS_ATC800_SPEED_NORMAL;
    RainbowLoop.brightness_min      = AORUS_ATC800_BRIGHTNESS_MIN;
    RainbowLoop.brightness_max      = AORUS_ATC800_BRIGHTNESS_MAX;
    RainbowLoop.brightness          = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(RainbowLoop);

    mode Tricolor;
    Tricolor.name                   = "Tricolor";
    Tricolor.value                  = AORUS_ATC800_MODE_TRICOLOR;
    Tricolor.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Tricolor.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Tricolor.colors_min             = 3;
    Tricolor.colors_max             = 3;
    Tricolor.colors                 = { 0xFF0000, 0xFF00FF, 0xFFFF00 };
    Tricolor.speed_min              = AORUS_ATC800_SPEED_SLOWEST;
    Tricolor.speed_max              = AORUS_ATC800_SPEED_FASTEST;
    Tricolor.speed                  = AORUS_ATC800_SPEED_NORMAL;
    Tricolor.brightness_min         = AORUS_ATC800_BRIGHTNESS_MIN;
    Tricolor.brightness_max         = AORUS_ATC800_BRIGHTNESS_MAX;
    Tricolor.brightness             = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(Tricolor);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AORUS_ATC800_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.speed_min         = AORUS_ATC800_SPEED_SLOWEST;
    SpectrumCycle.speed_max         = AORUS_ATC800_SPEED_FASTEST;
    SpectrumCycle.speed             = AORUS_ATC800_SPEED_NORMAL;
    SpectrumCycle.brightness_min    = AORUS_ATC800_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = AORUS_ATC800_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = AORUS_ATC800_BRIGHTNESS_MAX;
    modes.push_back(SpectrumCycle);

    SetupZones();
}

RGBController_AorusATC800::~RGBController_AorusATC800()
{
    delete controller;
}

void RGBController_AorusATC800::SetupZones()
{
    zone atc800_cpu_fans_zone;
    atc800_cpu_fans_zone.name           = "Fan";
    atc800_cpu_fans_zone.type           = ZONE_TYPE_SINGLE;
    atc800_cpu_fans_zone.leds_min       = 1;
    atc800_cpu_fans_zone.leds_max       = 1;
    atc800_cpu_fans_zone.leds_count     = 1;
    zones.push_back(atc800_cpu_fans_zone);

    led atc800_fan_led;
    atc800_fan_led.name                 = "Fan";
    leds.push_back(atc800_fan_led);

    zone atc800_top_zone;
    atc800_top_zone.name                = "Top";
    atc800_top_zone.type                = ZONE_TYPE_SINGLE;
    atc800_top_zone.leds_min            = 1;
    atc800_top_zone.leds_max            = 1;
    atc800_top_zone.leds_count          = 1;
    zones.push_back(atc800_top_zone);

    led atc800_top_led;
    atc800_top_led.name                 = "Top";
    leds.push_back(atc800_top_led);

    SetupColors();
}

void RGBController_AorusATC800::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(0);
    DeviceUpdateZoneLEDs(1);
}

void RGBController_AorusATC800::DeviceUpdateZoneLEDs(int zone)
{
    aorus_atc800_mode_config zone_config;

    zone_config.colors[0] = colors[zone];
    zone_config.numberOfColors = (uint8_t)modes[active_mode].colors.size();
    zone_config.speed = modes[active_mode].speed;
    zone_config.brightness = modes[active_mode].brightness;

    if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        for (uint8_t i = 0; i < zone_config.numberOfColors; i++)
        {
            zone_config.colors[i] = modes[active_mode].colors[i];
        }
    }

    controller->SendCoolerMode(zone, modes[active_mode].value, zone_config);
}

void RGBController_AorusATC800::DeviceUpdateSingleLED(int led)
{
    DeviceUpdateZoneLEDs(led);
}

void RGBController_AorusATC800::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
