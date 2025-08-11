/*---------------------------------------------------------*\
| RGBController_Ionico.cpp                                  |
|                                                           |
|   RGBController for Ionico-II-17                          |
|                                                           |
|   Lucas Strafe                                31 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Ionico.h"

/**------------------------------------------------------------------*\
    @name Ionico-II 17
    @category Keyboard,LEDStrip
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectIonicoControllers
    @comment
\*-------------------------------------------------------------------*/


RGBController_Ionico::RGBController_Ionico(IonicoController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetDeviceName();
    vendor                              = "Pcspecialist";
    description                         = name;
    location                            = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.value                        = IONICO_MODE_DIRECT;
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    Direct.brightness_min               = IONICO_DIRECT_BRIGHTNESS_MIN;
    Direct.brightness_max               = IONICO_DIRECT_BRIGHTNESS_MAX;
    Direct.brightness                   = IONICO_DIRECT_BRIGHTNESS_MAX;
    Direct.colors.resize(4);
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name                      = "Breathing";
    Breathing.value                     = IONICO_MODE_BREATHING;
    Breathing.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min                 = IONICO_DIRECT_SPEED_MIN;
    Breathing.speed_max                 = IONICO_DIRECT_SPEED_MAX;
    Breathing.speed                     = IONICO_DIRECT_SPEED_DEFAULT;
    Breathing.brightness_min            = IONICO_DIRECT_BRIGHTNESS_MIN;
    Breathing.brightness_max            = IONICO_DIRECT_BRIGHTNESS_MAX;
    Breathing.brightness                = IONICO_DIRECT_BRIGHTNESS_MAX;
    Breathing.colors.resize(7);
    modes.push_back(Breathing);

    mode Wave;
    Wave.name                           = "Wave";
    if(controller->GetUSBPID() == IONICO_KB_PID)
    {
        Wave.value    = IONICO_MODE_WAVE;
    }
    else if(controller->GetUSBPID() == IONICO_FB_PID)
    {
        Wave.value    = IONICO_FB_MODE_WAVE;
    }
    Wave.flags                          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Wave.color_mode                     = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min                      = IONICO_DIRECT_SPEED_MIN;
    Wave.speed_max                      = IONICO_DIRECT_SPEED_MAX;
    Wave.speed                          = IONICO_DIRECT_SPEED_DEFAULT;
    Wave.brightness_min                 = IONICO_DIRECT_BRIGHTNESS_MIN;
    Wave.brightness_max                 = IONICO_DIRECT_BRIGHTNESS_MAX;
    Wave.brightness                     = IONICO_DIRECT_BRIGHTNESS_MAX;
    Wave.colors.resize(7);
    modes.push_back(Wave);

    if(controller->GetUSBPID() == IONICO_KB_PID)
    {
        mode Flash;
        Flash.name                      = "Flashing";
        Flash.value                     = IONICO_MODE_FLASH;
        Flash.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
        Flash.color_mode                = MODE_COLORS_MODE_SPECIFIC;
        Flash.speed_min                 = IONICO_DIRECT_SPEED_MIN;
        Flash.speed_max                 = IONICO_DIRECT_SPEED_MAX;
        Flash.speed                     = IONICO_DIRECT_SPEED_DEFAULT;
        Flash.brightness_min            = IONICO_DIRECT_BRIGHTNESS_MIN;
        Flash.brightness_max            = IONICO_DIRECT_BRIGHTNESS_MAX;
        Flash.brightness                = IONICO_DIRECT_BRIGHTNESS_MAX;
        Flash.colors.resize(7);
        modes.push_back(Flash);
    }

    if(controller->GetUSBPID() == IONICO_FB_PID)
    {
        mode Raindrops;
        Raindrops.name                  = "Raindrops";
        Raindrops.value                 = IONICO_MODE_RAIN;
        Raindrops.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
        Raindrops.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Raindrops.speed_min             = IONICO_DIRECT_SPEED_MIN;
        Raindrops.speed_max             = IONICO_DIRECT_SPEED_MAX;
        Raindrops.speed                 = IONICO_DIRECT_SPEED_DEFAULT;
        Raindrops.brightness_min        = IONICO_DIRECT_BRIGHTNESS_MIN;
        Raindrops.brightness_max        = IONICO_DIRECT_BRIGHTNESS_MAX;
        Raindrops.brightness            = IONICO_DIRECT_BRIGHTNESS_MAX;
        Raindrops.colors.resize(7);
        modes.push_back(Raindrops);
    }

    mode Off;
    Off.name                            = "Off";
    modes.push_back(Off);

    SetupZones();
}

RGBController_Ionico::~RGBController_Ionico()
{
    delete controller;
}

void RGBController_Ionico::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    if(controller->GetUSBPID() == IONICO_KB_PID)
    {
        leds.resize(IONICO_KEYBOARD_LED_COUNT);
        zone zone_keyboard;
        zone_keyboard.name       = "Keyboard";
        zone_keyboard.type       = ZONE_TYPE_LINEAR;
        zone_keyboard.leds_min   = (unsigned int)leds.size();
        zone_keyboard.leds_max   = (unsigned int)leds.size();
        zone_keyboard.leds_count = (unsigned int)leds.size();
        zone_keyboard.matrix_map = nullptr;
        zones.emplace_back(zone_keyboard);
        for(size_t i = 0; i < leds.size(); ++i)
        {
            leds[i].name = "Keyboard Zone " + std::to_string(i+1);
        }
    }
    else if(controller->GetUSBPID() == IONICO_FB_PID)
    {
        leds.resize(IONICO_BAR_LED_COUNT);
        zone zone_bar;
        zone_bar.name       = "Front Bar";
        zone_bar.type       = ZONE_TYPE_LINEAR;
        zone_bar.leds_min   = (unsigned int)leds.size();
        zone_bar.leds_max   = (unsigned int)leds.size();
        zone_bar.leds_count = (unsigned int)leds.size();
        zone_bar.matrix_map = nullptr;
        zones.emplace_back(zone_bar);
        for(size_t i = 0; i < leds.size(); ++i)
        {
            leds[i].name = "Bar Led " + std::to_string(i+1);
        }
    }
    SetupColors();
}

void RGBController_Ionico::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Ionico::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    |                   MODE_COLORS_PER_LED                     |
    \*---------------------------------------------------------*/
    controller->SetColors(type, colors, false);
}

void RGBController_Ionico::DeviceSaveMode()
{
    controller->SaveBios();
}

void RGBController_Ionico::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Ionico::UpdateSingleLED(int /*led*/)
{
    //
}

void RGBController_Ionico::DeviceUpdateMode()
{
    switch (modes[active_mode].value)
    {
    case IONICO_MODE_OFF:
        controller->TurnOff();
        break;
    case IONICO_MODE_DIRECT:
        if(type == DEVICE_TYPE_LEDSTRIP)
        {
            controller->SetMode(0x33, modes[active_mode].brightness, 0);
        }
        else
        {
            controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, 0);
        }
        break;
    default:
        controller->SetMode(modes[active_mode].value, modes[active_mode].brightness, modes[active_mode].speed);
        controller->SetColors(type, modes[active_mode].colors, true);
        break;
    }
}
