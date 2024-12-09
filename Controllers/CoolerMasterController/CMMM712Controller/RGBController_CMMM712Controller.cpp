/*---------------------------------------------------------*\
| RGBController_CMMM712Controller.cpp                       |
|                                                           |
|   RGBController for Cooler Master MM712 mouse             |
|   Derived from RGBController_CMMM712Controller.cpp        |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Frans Meulenbroeks                          08 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CMMM712Controller.h"

#define applyBrightness(c, bright) ((RGBColor) ((RGBGetBValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 16 | (RGBGetGValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 8 | (RGBGetRValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT)))

/**------------------------------------------------------------------*\
    @name Coolermaster Master Mouse
    @category Mouse
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterMouse
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMMM712Controller::RGBController_CMMM712Controller(CMMM712Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Cooler Master";
    type                            = DEVICE_TYPE_MOUSE;
    description                     = controller->GetDeviceName();
    serial                          = controller->GetSerial();
    location                        = controller->GetLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = CM_MM712_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Direct.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Direct.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CM_MM712_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Static.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    Static.speed_min                = CM_MM712_SPEED_SLOWEST;
    Static.speed_max                = CM_MM712_SPEED_FASTEST;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.speed                    = CM_MM712_SPEED_NORMAL;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CM_MM712_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min        = CM_MM_ARGB_BRIGHTNESS_MIN;
    Breathing.brightness_max        = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.brightness            = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.speed_min             = CM_MM712_SPEED_SLOWEST;
    Breathing.speed_max             = CM_MM712_SPEED_FASTEST;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed                 = CM_MM712_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Spectrum_Cycle;
    Spectrum_Cycle.name             = "Spectrum Cycle";
    Spectrum_Cycle.value            = CM_MM712_MODE_SPECTRUM_CYCLE;
    Spectrum_Cycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Spectrum_Cycle.brightness_min   = CM_MM_ARGB_BRIGHTNESS_MIN;
    Spectrum_Cycle.brightness_max   = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.brightness       = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.speed_min        = CM_MM712_SPEED_SLOWEST;
    Spectrum_Cycle.speed_max        = CM_MM712_SPEED_FASTEST;
    Spectrum_Cycle.color_mode       = MODE_COLORS_NONE;
    Spectrum_Cycle.speed            = CM_MM712_SPEED_NORMAL;
    modes.push_back(Spectrum_Cycle);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = CM_MM712_MODE_OFF;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    Init_Controller();         //Only processed on first run
    SetupZones();

    uint8_t temp_mode               = controller->GetMode();

    for(int mode_index = 0; mode_index < (int)modes.size(); mode_index++)
    {
        if(modes[mode_index].value == temp_mode)
        {
            active_mode             = mode_index;
            break;
        }
    }

    colors[0]                        = controller->GetInitialLedColor();
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        modes[active_mode].colors[0] = colors[0];
    }
}

RGBController_CMMM712Controller::~RGBController_CMMM712Controller()
{
    delete controller;
}

void RGBController_CMMM712Controller::Init_Controller()
{
    zone mouse_zone;
    mouse_zone.name         = name;
    mouse_zone.type         = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min     = 1;
    mouse_zone.leds_max     = 1;
    mouse_zone.leds_count   = 1;
    mouse_zone.matrix_map   = NULL;
    zones.push_back(mouse_zone);

    led logo_led;
    logo_led.name           = "Logo LED";
    logo_led.value          = 0;
    leds.push_back(logo_led);
}

void RGBController_CMMM712Controller::SetupZones()
{
    SetupColors();
}

void RGBController_CMMM712Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMMM712Controller::DeviceUpdateLEDs()
{
    modes[active_mode].brightness=255;
    RGBColor logo   = applyBrightness(colors[0], modes[active_mode].brightness);

    controller->SetLedsDirect(logo);
}

void RGBController_CMMM712Controller::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMM712Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMM712Controller::DeviceUpdateMode()
{
    if(modes[active_mode].value==CM_MM712_MODE_DIRECT)
    {
        controller->SetDirectMode(true);
    }
    else
    {
        controller->SetDirectMode(false);
        RGBColor colour = 0;

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC )
        {
            colour = modes[active_mode].colors[0];
        }

        controller->SendUpdate(modes[active_mode].value, modes[active_mode].speed, colour, modes[active_mode].brightness);
    }
}

void RGBController_CMMM712Controller::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveStatus();
}
