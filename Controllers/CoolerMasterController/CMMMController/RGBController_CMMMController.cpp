/*---------------------------------------------------------*\
| RGBController_CMMMController.cpp                          |
|                                                           |
|   RGBController for Cooler Master mouse                   |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Dracrius                                    12 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CMMMController.h"

#define applyBrightness(c, bright) ((RGBColor) ((RGBGetBValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 16 | (RGBGetGValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT) << 8 | (RGBGetRValue(c) * bright / CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT)))

/**------------------------------------------------------------------*\
    @name Coolermaster Master Mouse
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterMouse
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMMMController::RGBController_CMMMController(CMMMController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    vendor                          = controller->GetDeviceVendor();
    type                            = DEVICE_TYPE_MOUSE;
    description                     = "Cooler Master MasterMouse Device";
    serial                          = controller->GetSerial();
    location                        = controller->GetLocation();

    mode Custom;
    Custom.name                     = "Direct";
    Custom.value                    = CM_MM_MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Custom.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Custom.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Custom.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = CM_MM_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min           = CM_MM_ARGB_BRIGHTNESS_MIN;
    Static.brightness_max           = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.brightness               = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    Static.speed_min                = CM_MM_SPEED_1;
    Static.speed_max                = CM_MM_SPEED_5;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.speed                    = CM_MM_SPEED_3;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CM_MM_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.brightness_min        = CM_MM_ARGB_BRIGHTNESS_MIN;
    Breathing.brightness_max        = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.brightness            = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.speed_min             = CM_MM_SPEED_1;
    Breathing.speed_max             = CM_MM_SPEED_5;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed                 = CM_MM_SPEED_3;
    modes.push_back(Breathing);

    mode Spectrum_Cycle;
    Spectrum_Cycle.name             = "Spectrum Cycle";
    Spectrum_Cycle.value            = CM_MM_MODE_SPECTRUM_CYCLE;
    Spectrum_Cycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Spectrum_Cycle.brightness_min   = CM_MM_ARGB_BRIGHTNESS_MIN;
    Spectrum_Cycle.brightness_max   = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.brightness       = CM_MM_ARGB_BRIGHTNESS_MAX_SPECTRUM;
    Spectrum_Cycle.speed_min        = CM_MM_SPEED_1;
    Spectrum_Cycle.speed_max        = CM_MM_SPEED_5;
    Spectrum_Cycle.color_mode       = MODE_COLORS_NONE;
    Spectrum_Cycle.speed            = CM_MM_SPEED_3;
    modes.push_back(Spectrum_Cycle);

    mode Stars;
    Stars.name                      = "Stars";
    Stars.value                     = CM_MM_MODE_STARS;
    Stars.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Stars.brightness_min            = CM_MM_ARGB_BRIGHTNESS_MIN;
    Stars.brightness_max            = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Stars.brightness                = CM_MM_ARGB_BRIGHTNESS_MAX_DEFAULT;
    Stars.colors_min                = 2;
    Stars.colors_max                = 2;
    Stars.colors.resize(Stars.colors_max);
    Stars.speed_min                 = CM_MM_SPEED_1;
    Stars.speed_max                 = CM_MM_SPEED_5;
    Stars.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    Stars.speed                     = CM_MM_SPEED_3;
    modes.push_back(Stars);

    mode Indicator;
    Indicator.name                  = "Indicator";
    Indicator.value                 = CM_MM_MODE_INDICATOR;
    Indicator.flags                 = MODE_FLAG_MANUAL_SAVE;
    Indicator.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Indicator);

    mode Off;
    Off.name                        = "Turn Off";
    Off.value                       = CM_MM_MODE_OFF;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    uint16_t pid = controller->GetProductID();

    if(pid == 0x0065 || pid == 0x0097)
    {
        leds_count           = 3;
    }
    else
    {
        leds_count           = 2;
    }

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

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        modes[active_mode].colors[0] = ToRGBColor(controller->GetLedRed(),controller->GetLedGreen(),controller->GetLedBlue());
    }


    if(pid == 0x0065 || pid == 0x0097)
    {
        colors[0] = controller->GetWheelColour();
        colors[1] = controller->GetButtonsColour();
        colors[2] = controller->GetLogoColour();
    }
    else
    {
        colors[0] = controller->GetWheelColour();
        colors[1] = controller->GetLogoColour();
    }
}

RGBController_CMMMController::~RGBController_CMMMController()
{
    delete controller;
}

void RGBController_CMMMController::Init_Controller()
{
    zone mouse_zone;
    mouse_zone.name         = name;
    mouse_zone.type         = ZONE_TYPE_LINEAR;
    mouse_zone.leds_min     = leds_count;
    mouse_zone.leds_max     = leds_count;
    mouse_zone.leds_count   = leds_count;
    mouse_zone.matrix_map   = NULL;
    zones.push_back(mouse_zone);

    int value               = 0;
    uint16_t pid            = controller->GetProductID();

    led wheel_led;
    wheel_led.name          = "Scroll Wheel";
    wheel_led.value         = value;
    leds.push_back(wheel_led);

    value++;

    if(pid == 0x0065 || pid == 0x0097)
    {
        led buttons_led;
        buttons_led.name        = "Buttons";
        buttons_led.value       = value;
        leds.push_back(buttons_led);
        value++;
    }

    led logo_led;
    logo_led.name           = "Logo";
    logo_led.value          = value;
    leds.push_back(logo_led);
}

void RGBController_CMMMController::SetupZones()
{
    SetupColors();
}

void RGBController_CMMMController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMMMController::DeviceUpdateLEDs()
{
    int value            = 0;
    uint16_t pid         = controller->GetProductID();

    RGBColor wheel       = applyBrightness(colors[value], modes[active_mode].brightness);
    RGBColor buttons     = ToRGBColor(0, 0, 0);
    value++;

    if(pid == 0x0065 || pid == 0x0097)
    {
        buttons          = applyBrightness(colors[value], modes[active_mode].brightness);
        value++;
    }

    RGBColor logo        = applyBrightness(colors[value], modes[active_mode].brightness);

    controller->SetLedsDirect(wheel, buttons, logo);
}

void RGBController_CMMMController::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMMController::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMMMController::DeviceUpdateMode()
{
    RGBColor mode_one = 0;
    RGBColor mode_two = 0;

    if(modes[active_mode].value != CM_MM_MODE_CUSTOM)
    {
        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC )
        {
            mode_one = modes[active_mode].colors[0];

            if(modes[active_mode].colors.size() > 1)
            {
                mode_two = modes[active_mode].colors[1];
            }
        }

    }

    if(modes[active_mode].value == CM_MM_MODE_STARS)
    {
        controller->SendUpdate(modes[active_mode].value, modes[active_mode].speed, mode_one, mode_two, modes[active_mode].brightness);
    }
    else
    {
        controller->SendUpdate(modes[active_mode].value, modes[active_mode].speed, mode_one, modes[active_mode].brightness);
    }
}

void RGBController_CMMMController::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SendSavePacket();
}
