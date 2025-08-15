/*---------------------------------------------------------*\
| RGBController_CMMonitorController.cpp                     |
|                                                           |
|   RGBController for Cooler Master monitor                 |
|                                                           |
|   Morgan Guimard (morg)                       18 Sep 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_CMMonitorController.h"

/**------------------------------------------------------------------*\
    @name Coolermaster Gaming Monitor
    @category Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterMonitor
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMMonitorController::RGBController_CMMonitorController(CMMonitorController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "CoolerMaster";
    type                        = DEVICE_TYPE_MONITOR;
    description                 = "CoolerMaster Monitor Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CM_MONITOR_DIRECT_MODE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Spectrum;
    Spectrum.name               = "Spectrum cycle";
    Spectrum.value              = CM_MONITOR_SPECTRUM_MODE;
    Spectrum.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed_min          = CM_MONITOR_SPEED_MIN;
    Spectrum.speed_max          = CM_MONITOR_SPEED_MAX;
    Spectrum.speed              = CM_MONITOR_SPEED_MAX/2;
    Spectrum.brightness_min     = CM_MONITOR_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = CM_MONITOR_BRIGHTNESS_MAX;
    Spectrum.brightness         = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Spectrum);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_MONITOR_RELOAD_MODE;
    Reload.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Reload.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Reload.colors_min           = 1;
    Reload.colors_max           = 1;
    Reload.colors.resize(1);
    Reload.speed_min            = CM_MONITOR_SPEED_MIN;
    Reload.speed_max            = CM_MONITOR_SPEED_MAX;
    Reload.speed                = CM_MONITOR_SPEED_MAX/2;
    Reload.brightness_min       = CM_MONITOR_BRIGHTNESS_MIN;
    Reload.brightness_max       = CM_MONITOR_BRIGHTNESS_MAX;
    Reload.brightness           = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_MONITOR_RECOIL_MODE;
    Recoil.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Recoil.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Recoil.colors_min           = 1;
    Recoil.colors_max           = 1;
    Recoil.colors.resize(1);
    Recoil.speed_min            = CM_MONITOR_SPEED_MIN;
    Recoil.speed_max            = CM_MONITOR_SPEED_MAX;
    Recoil.speed                = CM_MONITOR_SPEED_MAX/2;
    Recoil.brightness_min       = CM_MONITOR_BRIGHTNESS_MIN;
    Recoil.brightness_max       = CM_MONITOR_BRIGHTNESS_MAX;
    Recoil.brightness           = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_MONITOR_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    Breathing.speed_min         = CM_MONITOR_SPEED_MIN;
    Breathing.speed_max         = CM_MONITOR_SPEED_MAX;
    Breathing.speed             = CM_MONITOR_SPEED_MAX/2;
    Breathing.brightness_min    = CM_MONITOR_BRIGHTNESS_MIN;
    Breathing.brightness_max    = CM_MONITOR_BRIGHTNESS_MAX;
    Breathing.brightness        = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_MONITOR_REFILL_MODE;
    Refill.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Refill.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Refill.colors_min           = 1;
    Refill.colors_max           = 1;
    Refill.colors.resize(1);
    Refill.speed_min            = CM_MONITOR_SPEED_MIN;
    Refill.speed_max            = CM_MONITOR_SPEED_MAX;
    Refill.speed                = CM_MONITOR_SPEED_MAX/2;
    Refill.brightness_min       = CM_MONITOR_BRIGHTNESS_MIN;
    Refill.brightness_max       = CM_MONITOR_BRIGHTNESS_MAX;
    Refill.brightness           = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Refill);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CM_MONITOR_CUSTOM_MODE;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    Custom.brightness_min       = CM_MONITOR_BRIGHTNESS_MIN;
    Custom.brightness_max       = CM_MONITOR_BRIGHTNESS_MAX;
    Custom.brightness           = CM_MONITOR_BRIGHTNESS_MAX;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_MONITOR_SPECTRUM_MODE;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_CMMonitorController::~RGBController_CMMonitorController()
{
    delete controller;
}

void RGBController_CMMonitorController::SetupZones()
{
    zone z;

    z.name          = "Monitor";
    z.type          = ZONE_TYPE_LINEAR;
    z.leds_min      = 47;
    z.leds_max      = 47;
    z.leds_count    = 47;
    z.matrix_map    = NULL;

    zones.push_back(z);

    for(unsigned int i = 0; i < 47; i++)
    {
        led l;
        l.name          = std::to_string(i + 1);
        l.value         = i;
        leds.push_back(l);
    }

    SetupColors();
}

void RGBController_CMMonitorController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMMonitorController::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == CM_MONITOR_DIRECT_MODE)
    {
        controller->SendDirect(colors);
    }
    else if(modes[active_mode].value == CM_MONITOR_CUSTOM_MODE)
    {
        controller->SetCustomMode(colors, modes[active_mode].brightness);
    }
}

void RGBController_CMMonitorController::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMonitorController::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMonitorController::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case CM_MONITOR_OFF_MODE:
        case CM_MONITOR_SPECTRUM_MODE:
            controller->SetMode(modes[active_mode].value, 0, modes[active_mode].speed, modes[active_mode].brightness);
            break;

        case CM_MONITOR_RELOAD_MODE:
        case CM_MONITOR_RECOIL_MODE:
        case CM_MONITOR_BREATHING_MODE:
        case CM_MONITOR_REFILL_MODE:
            controller->SetMode(modes[active_mode].value, modes[active_mode].colors[0], modes[active_mode].speed, modes[active_mode].brightness);
            break;

        case CM_MONITOR_CUSTOM_MODE:
            DeviceUpdateLEDs();
            break;
        default: break;
    }
}
