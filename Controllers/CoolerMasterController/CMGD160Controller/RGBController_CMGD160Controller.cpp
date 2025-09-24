/*---------------------------------------------------------*\
| RGBController_CMGD160Controller.cpp                       |
|                                                           |
|   RGBController for Cooler Master GD160 ARGB Gaming Desk  |
|                                                           |
|   Logan Phillips (Eclipse)                    16 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   Adapted from CMMonitor controller code                  |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CMGD160Controller.h"

/**------------------------------------------------------------------*\
    @name Cooler Master GD160 ARGB Gaming Desk
    @category Accessory
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterGD160
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMGD160Controller::RGBController_CMGD160Controller(CMGD160Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Cooler Master";
    type                        = DEVICE_TYPE_ACCESSORY;
    description                 = "Cooler Master GD160 Gaming Desk Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CM_GD160_DIRECT_MODE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Spectrum;
    Spectrum.name               = "Spectrum Cycle";
    Spectrum.value              = CM_GD160_SPECTRUM_MODE;
    Spectrum.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed_min          = CM_GD160_SPEED_MIN;
    Spectrum.speed_max          = CM_GD160_SPEED_MAX;
    Spectrum.speed              = CM_GD160_SPEED_MAX/2;
    Spectrum.brightness_min     = CM_GD160_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = CM_GD160_BRIGHTNESS_MAX;
    Spectrum.brightness         = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Spectrum);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_GD160_RELOAD_MODE;
    Reload.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Reload.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Reload.colors_min           = 1;
    Reload.colors_max           = 1;
    Reload.colors.resize(1);
    Reload.speed_min            = CM_GD160_SPEED_MIN;
    Reload.speed_max            = CM_GD160_SPEED_MAX;
    Reload.speed                = CM_GD160_SPEED_MAX/2;
    Reload.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Reload.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Reload.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_GD160_RECOIL_MODE;
    Recoil.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Recoil.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Recoil.colors_min           = 1;
    Recoil.colors_max           = 1;
    Recoil.colors.resize(1);
    Recoil.speed_min            = CM_GD160_SPEED_MIN;
    Recoil.speed_max            = CM_GD160_SPEED_MAX;
    Recoil.speed                = CM_GD160_SPEED_MAX/2;
    Recoil.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Recoil.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Recoil.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_GD160_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    Breathing.speed_min         = CM_GD160_SPEED_MIN;
    Breathing.speed_max         = CM_GD160_SPEED_MAX;
    Breathing.speed             = CM_GD160_SPEED_MAX/2;
    Breathing.brightness_min    = CM_GD160_BRIGHTNESS_MIN;
    Breathing.brightness_max    = CM_GD160_BRIGHTNESS_MAX;
    Breathing.brightness        = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_GD160_REFILL_MODE;
    Refill.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Refill.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Refill.colors_min           = 1;
    Refill.colors_max           = 1;
    Refill.colors.resize(1);
    Refill.speed_min            = CM_GD160_SPEED_MIN;
    Refill.speed_max            = CM_GD160_SPEED_MAX;
    Refill.speed                = CM_GD160_SPEED_MAX/2;
    Refill.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Refill.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Refill.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Refill);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CM_GD160_CUSTOM_MODE;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    Custom.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Custom.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Custom.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_GD160_OFF_MODE;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_CMGD160Controller::~RGBController_CMGD160Controller()
{
    delete controller;
}

void RGBController_CMGD160Controller::SetupZones()
{
    zone front;
    front.name          = "Front Desk";
    front.type          = ZONE_TYPE_LINEAR;
    front.leds_min      = CM_GD160_LEDS_PER_SIDE;
    front.leds_max      = CM_GD160_LEDS_PER_SIDE;
    front.leds_count    = CM_GD160_LEDS_PER_SIDE;
    zones.push_back(front);

    for(unsigned int i = 0; i < CM_GD160_LEDS_PER_SIDE; i++)
    {
        led l;
        l.name          = "Front LED " + std::to_string(i + 1);
        l.value         = i;
        leds.push_back(l);
    }

    zone back;
    back.name          = "Back Desk";
    back.type          = ZONE_TYPE_LINEAR;
    back.leds_min      = CM_GD160_LEDS_PER_SIDE;
    back.leds_max      = CM_GD160_LEDS_PER_SIDE;
    back.leds_count    = CM_GD160_LEDS_PER_SIDE;
    zones.push_back(back);

    for(unsigned int i = 0; i < CM_GD160_LEDS_PER_SIDE; i++)
    {
        led l;
        l.name          = "Back LED " + std::to_string(i + 1);
        l.value         = i;
        leds.push_back(l);
    }

    SetupColors();
}

void RGBController_CMGD160Controller::DeviceUpdateLEDs()
{
    switch(modes[active_mode].value)
    {
        case CM_GD160_DIRECT_MODE:
            controller->SendColorData(colors, 0x07, 0x01, 0xFF, true);
            break;

        case CM_GD160_CUSTOM_MODE:
            controller->SendColorData(colors, 0x10, 0x80, modes[active_mode].brightness, false);
            break;

        default:
            break;
    }
}

void RGBController_CMGD160Controller::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMGD160Controller::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMGD160Controller::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case CM_GD160_OFF_MODE:
        case CM_GD160_SPECTRUM_MODE:
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0);
            break;

        case CM_GD160_RELOAD_MODE:
        case CM_GD160_RECOIL_MODE:
        case CM_GD160_BREATHING_MODE:
        case CM_GD160_REFILL_MODE:
            controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].colors[0]);
            break;

        default:
            break;
    }
}
