/*---------------------------------------------------------*\
| RGBController_GigabyteAorusPCCase.cpp                     |
|                                                           |
|   RGBController for Gigabyte Aorus case                   |
|                                                           |
|   Denis Nazarov (nenderus)                    10 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteAorusPCCase.h"

/**------------------------------------------------------------------*\
    @name Gigabyte AORUS PC Case
    @category Case
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectGigabyteAorusPCCaseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GigabyteAorusPCCase::RGBController_GigabyteAorusPCCase(GigabyteAorusPCCaseController *controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Gigabyte";
    description                     = "Gigabyte AORUS PC Case Device";
    type                            = DEVICE_TYPE_CASE;
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Custom;
    Custom.name                     = "Custom";
    Custom.value                    = AORUS_PC_CASE_MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Custom.colors_min               = 1;
    Custom.colors_max               = 1;
    Custom.colors.resize(1);
    Custom.brightness_min           = AORUS_PC_CASE_BRIGHTNESS_MIN;
    Custom.brightness_max           = AORUS_PC_CASE_BRIGHTNESS_MAX;
    Custom.brightness               = AORUS_PC_CASE_BRIGHTNESS_MAX;
    modes.push_back(Custom);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = AORUS_PC_CASE_MODE_OFF;
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AORUS_PC_CASE_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    Breathing.speed_min             = AORUS_PC_CASE_SPEED_SLOWEST;
    Breathing.speed_max             = AORUS_PC_CASE_SPEED_FASTEST;
    Breathing.speed                 = AORUS_PC_CASE_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AORUS_PC_CASE_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.speed_min         = AORUS_PC_CASE_SPEED_SLOWEST;
    SpectrumCycle.speed_max         = AORUS_PC_CASE_SPEED_FASTEST;
    SpectrumCycle.speed             = AORUS_PC_CASE_SPEED_NORMAL;
    modes.push_back(SpectrumCycle);

    mode Flashing;
    Flashing.name                   = "Flashing";
    Flashing.value                  = AORUS_PC_CASE_MODE_FLASHING;
    Flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Flashing.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors_min             = 1;
    Flashing.colors_max             = 1;
    Flashing.colors.resize(1);
    Flashing.speed_min              = AORUS_PC_CASE_SPEED_SLOWEST;
    Flashing.speed_max              = AORUS_PC_CASE_SPEED_FASTEST;
    Flashing.speed                  = AORUS_PC_CASE_SPEED_NORMAL;
    Flashing.brightness_min         = AORUS_PC_CASE_BRIGHTNESS_MIN;
    Flashing.brightness_max         = AORUS_PC_CASE_BRIGHTNESS_MAX;
    Flashing.brightness             = AORUS_PC_CASE_BRIGHTNESS_MAX;
    modes.push_back(Flashing);

    mode DoubleFlashing;
    DoubleFlashing.name             = "Double Flashing";
    DoubleFlashing.value            = AORUS_PC_CASE_MODE_DOUBLE_FLASHING;
    DoubleFlashing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    DoubleFlashing.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    DoubleFlashing.colors_min       = 1;
    DoubleFlashing.colors_max       = 1;
    DoubleFlashing.colors.resize(1);
    DoubleFlashing.speed_min        = AORUS_PC_CASE_SPEED_SLOWEST;
    DoubleFlashing.speed_max        = AORUS_PC_CASE_SPEED_FASTEST;
    DoubleFlashing.speed            = AORUS_PC_CASE_SPEED_NORMAL;
    DoubleFlashing.brightness_min   = AORUS_PC_CASE_BRIGHTNESS_MIN;
    DoubleFlashing.brightness_max   = AORUS_PC_CASE_BRIGHTNESS_MAX;
    DoubleFlashing.brightness       = AORUS_PC_CASE_BRIGHTNESS_MAX;
    modes.push_back(DoubleFlashing);

    SetupZones();
}

RGBController_GigabyteAorusPCCase::~RGBController_GigabyteAorusPCCase()
{
    delete controller;
}

void RGBController_GigabyteAorusPCCase::SetupZones()
{
    zone case_zone;
    case_zone.name          = "Case";
    case_zone.type          = ZONE_TYPE_SINGLE;
    case_zone.leds_min      = 1;
    case_zone.leds_max      = 1;
    case_zone.leds_count    = 1;
    case_zone.matrix_map    = NULL;
    zones.push_back(case_zone);

    led case_led;
    case_led.name           = "Case";
    leds.push_back(case_led);

    SetupColors();
}

void RGBController_GigabyteAorusPCCase::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusPCCase::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | This device does not need update leds                     |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusPCCase::UpdateZoneLEDs(int /*zone*/)
{
    /*---------------------------------------------------------*\
    | This device does not need update zone leds                |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusPCCase::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | This device does not need update single led               |
    \*---------------------------------------------------------*/
}

void RGBController_GigabyteAorusPCCase::DeviceUpdateMode()
{
    aorus_pc_case_mode_config zone_config;
    zone_config.color       = 0x000000;
    zone_config.speed       = modes[active_mode].speed;
    zone_config.brightness  = modes[active_mode].brightness;

    if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        zone_config.color   = modes[active_mode].colors[0];
    }

    controller->SetMode(modes[active_mode].value, zone_config);
}
