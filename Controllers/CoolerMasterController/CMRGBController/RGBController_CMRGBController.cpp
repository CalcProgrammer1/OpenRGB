/*---------------------------------------------------------*\
| RGBController_CMRGBController.cpp                         |
|                                                           |
|   RGBController for Cooler Master RGB controller          |
|                                                           |
|   Nic W (midgetspy)                           13 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CMRGBController.h"

/*-----------------------------------------------------------------------------------------------------------------------------------------*\
| This controller has 4 ports, each for a 12v non-addressable LED item.                                                                     |
|                                                                                                                                           |
| It supports the following modes:                                                                                                          |
|   Static: All 4 ports a single color. Has brightness option.                                                                              |
|   Breathing: All ports a single color, fading in and out. Has brightness and speed option.                                                |
|   Star: Some weird effect using all 4 ports and a single color. Has brightness and speed option.                                          |
|   Color Cycle: All ports cycle through the rainbow in unison. Has brightness and speed option.                                            |
|   Off: All 4 ports off                                                                                                                    |
|                                                                                                                                           |
| Plus some "special" modes:                                                                                                                |
|   Multilayer: Each of the 4 ports can have any of the modes above applied individually                                                    |
|   Multiple Color/Customize: Each port can be set to its own static color                                                                  |
|   Mirage: A strobe effect that varies the LED pulse frequency which affects any of the above modes                                        |
|                                                                                                                                           |
| Note:                                                                                                                                     |
|  Multiple Color/Customize is equivalent to Multilayer + Static, but the device supports both separately                                   |
|  Static is equivalent to Multiple Color/Customize with the same color on each port, but the device supports both separately               |
|                                                                                                                                           |
| It can be controlled with 2 different pieces of software: MasterPlus+ or "RGB LED Controller". They appear to use different protocols.    |
|                                                                                                                                           |
| RGB LED Controller:                                                                                                                       |
|   Sets changes temporarily and then applies them or cancels the changes separately                                                        |
|   Supports all modes above                                                                                                                |
|   Has 3 brightness increments                                                                                                             |
|   Has two different colors for the Star effect (Star/Sky)                                                                                 |
|                                                                                                                                           |
| MasterPlus+:                                                                                                                              |
|   Sets changes permanently as soon as you change anything in the UI                                                                       |
|   Doesn't support Multilayer or Mirage                                                                                                    |
|   Has 5 brightness increments                                                                                                             |
|   Single color for Star                                                                                                                   |
\*-----------------------------------------------------------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name Coolermaster RGB
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterRGB
    @comment This is a 12V analogue RGB controller only.
\*-------------------------------------------------------------------*/

RGBController_CMRGBController::RGBController_CMRGBController(CMRGBController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Cooler Master RGB Controller";
    vendor                      = "Cooler Master";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = controller->GetDeviceName();
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Static;
    Static.name                 = "Static";
    Static.value                = CM_RGBC_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min       = 0x00;
    Static.brightness_max       = 0xFF;
    Static.brightness           = 0xFF;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_RGBC_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.speed_min         = CM_RGBC_SPEED_BREATHING_SLOWEST;
    Breathing.speed_max         = CM_RGBC_SPEED_BREATHING_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = CM_RGBC_SPEED_BREATHING_SLOWEST;
    Breathing.brightness_min    = 0x00;
    Breathing.brightness_max    = 0xFF;
    Breathing.brightness        = 0xFF;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = CM_RGBC_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR;
    ColorCycle.speed_min        = CM_RGBC_SPEED_COLOR_CYCLE_SLOWEST;
    ColorCycle.speed_max        = CM_RGBC_SPEED_COLOR_CYCLE_FASTEST;
    ColorCycle.color_mode       = MODE_COLORS_RANDOM;
    ColorCycle.speed            = CM_RGBC_SPEED_COLOR_CYCLE_SLOWEST;
    ColorCycle.brightness_min   = 0x00;
    ColorCycle.brightness_max   = 0xFF;
    ColorCycle.brightness       = 0xFF;
    modes.push_back(ColorCycle);

    mode Star;
    Star.name                   = "Star";
    Star.value                  = CM_RGBC_MODE_STAR;
    Star.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Star.colors_min             = 2;
    Star.colors_max             = 2;
    Star.colors.resize(Star.colors_max);
    Star.speed_min              = CM_RGBC_SPEED_STAR_SLOWEST;
    Star.speed_max              = CM_RGBC_SPEED_STAR_FASTEST;
    Star.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Star.speed                  = CM_RGBC_SPEED_STAR_SLOWEST;
    Star.brightness_min         = 0x00;
    Star.brightness_max         = 0xFF;
    Star.brightness             = 0xFF;
    modes.push_back(Star);

    mode Multiple;
    Multiple.name               = "Custom";
    Multiple.value              = CM_RGBC_MODE_MULTIPLE;
    Multiple.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Multiple.colors_min         = 1;
    Multiple.colors_max         = 1;
    Multiple.colors.resize(Multiple.colors_max);
    Multiple.color_mode         = MODE_COLORS_PER_LED;
    Multiple.speed              = 0;
    Multiple.brightness_min     = 0x00;
    Multiple.brightness_max     = 0xFF;
    Multiple.brightness         = 0xFF;
    modes.push_back(Multiple);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_RGBC_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    Off.flags                   = 0;
    modes.push_back(Off);

    SetupZones();

    ReadAllModeConfigsFromDevice();
}

RGBController_CMRGBController::~RGBController_CMRGBController()
{
    delete controller;
}

void RGBController_CMRGBController::ReadAllModeConfigsFromDevice()
{
    int device_mode = controller->GetMode();

    for(int mode_idx = 0; mode_idx < (int)modes.size(); mode_idx++)
    {
        if(device_mode == modes[mode_idx].value)
        {
            active_mode = mode_idx;
            continue;
        }

        if(!modes[mode_idx].flags)
        {
            continue;
        }

        controller->ReadModeConfig(modes[mode_idx].value);
        LoadConfigFromDeviceController(mode_idx);
    }

    /*---------------------------------------------------------*\
    | Do the active mode last so the device controller state    |
    | is left with the active mode's config                     |
    \*---------------------------------------------------------*/
    if(active_mode != -1)
    {
        controller->ReadModeConfig(modes[active_mode].value);
        LoadConfigFromDeviceController(active_mode);
    }
}

void RGBController_CMRGBController::LoadConfigFromDeviceController(int mode_idx)
{
    for(int color_idx = 0; color_idx < (int)modes[mode_idx].colors.size(); color_idx++)
    {
        modes[mode_idx].colors[0] = controller->GetModeColor(color_idx);
    }

    if(modes[mode_idx].flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        for(int led_idx = 0; led_idx < (int)leds.size(); led_idx++)
        {
            colors[led_idx] = controller->GetPortColor(led_idx);
        }
    }

    if(modes[mode_idx].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[mode_idx].speed = controller->GetSpeed();
    }

    if(modes[mode_idx].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        modes[active_mode].brightness = controller->GetBrightness();
    }
}


void RGBController_CMRGBController::SetupZones()
{
    leds.clear();
    zones.clear();
    colors.clear();

    /*-----------------------------------------------------*\
    | One zone, 4 leds. This might not actually work with   |
    | the Multilayer mode, but we'll deal with that later   |
    \*-----------------------------------------------------*/
    zone* new_zone          = new zone();
    new_zone->name          = "Controller";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 4;
    new_zone->leds_count    = 4;

    for(int i = 1; i <= CM_RGBC_NUM_LEDS; i++)
    {
        led*  new_led       = new led();
        new_led->name       = "LED " + std::to_string(i);
        leds.push_back(*new_led);
    }

    zones.push_back(*new_zone);
    SetupColors();
}

void RGBController_CMRGBController::DeviceUpdateLEDs()
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}

void RGBController_CMRGBController::DeviceUpdateZoneLEDs(int zone)
{
    controller->SetLedsDirect(zones[zone].colors[0], zones[zone].colors[1], zones[zone].colors[2], zones[zone].colors[3]);
}

void RGBController_CMRGBController::DeviceUpdateSingleLED(int /*led*/)
{
}

void RGBController_CMRGBController::DeviceUpdateMode()
{
    RGBColor color_1 = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC) ? modes[active_mode].colors[0] : 0;
    RGBColor color_2 = (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && modes[active_mode].colors.size() > 1) ? modes[active_mode].colors[1] : 0;

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, color_1, color_2);
}
