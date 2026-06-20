/*---------------------------------------------------------*\
| RGBController_EVGACLC.cpp                                 |
|                                                           |
|   RGBController for EVGA CLC liquid coolers               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_EVGACLC.h"
#include "SettingsManager.h"
#include "ResourceManager.h"

/**------------------------------------------------------------------*\
    @name EVGA CLC
    @category Cooler
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectEVGACLCControllers
    @comment EVGA CLC liquid coolers using the Asetek 690LC platform.
        Supports fixed color, fading, blinking, and rainbow modes.
        Color calibration can be configured in Settings > EVGA CLC.
        Set R/G/B scale values (0-100) to match other devices.
\*-------------------------------------------------------------------*/

RGBController_EVGACLC::RGBController_EVGACLC(EVGACLCController* controller_ptr, const std::string& detector_name)
{
    controller  = controller_ptr;
    det_name    = detector_name;

    if(!controller)
    {
        name        = "Unknown";
        vendor      = "EVGA";
        description = "EVGA CLC Liquid Cooler";
        version     = "Unknown";
        type        = DEVICE_TYPE_COOLER;
        location    = "Unknown";
        return;
    }

    if(detector_name.empty())
    {
        det_name = "EVGA CLC";
    }

    name        = controller->GetNameString();
    vendor      = "EVGA";
    description = "EVGA CLC Liquid Cooler";
    version     = controller->GetFirmwareString();
    type        = DEVICE_TYPE_COOLER;
    location    = controller->GetLocation();

    /*---------------------------------------------------------*\
    | Load color calibration from settings                      |
    \*---------------------------------------------------------*/
    LoadCalibration();

    InitModes();
    SetupZones();
}

void RGBController_EVGACLC::InitModes()
{
    if(!modes.empty())
    {
        return;
    }

    if(!controller)
    {
        return;
    }

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = EVGA_CLC_MODE_FIXED;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Fading;
    Fading.name       = "Fading";
    Fading.value      = EVGA_CLC_MODE_FADING;
    Fading.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Fading.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Fading.speed_min  = 1;
    Fading.speed_max  = 255;
    Fading.speed      = 5;
    Fading.colors_min = 2;
    Fading.colors_max = 2;
    Fading.colors.resize(2);
    modes.push_back(Fading);

    mode Blinking;
    Blinking.name       = "Blinking";
    Blinking.value      = EVGA_CLC_MODE_BLINKING;
    Blinking.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Blinking.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Blinking.speed_min  = 1;
    Blinking.speed_max  = 255;
    Blinking.speed      = 5;
    Blinking.colors_min = 1;
    Blinking.colors_max = 1;
    Blinking.colors.resize(1);
    modes.push_back(Blinking);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = EVGA_CLC_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED;
    Rainbow.color_mode = MODE_COLORS_NONE;
    Rainbow.speed_min  = 1;
    Rainbow.speed_max  = 6;
    Rainbow.speed      = 3;
    modes.push_back(Rainbow);
}

RGBController_EVGACLC::~RGBController_EVGACLC()
{
    delete controller;
}

void RGBController_EVGACLC::LoadCalibration()
{
    /*---------------------------------------------------------*\
    | Default calibration: compensates for the warm phosphor    |
    | bias typical of EVGA CLC pump LEDs. R and G channels      |
    | are scaled to 20% to produce a neutral white that matches |
    | most GPU and motherboard LEDs. Adjust in OpenRGB.json     |
    | under "EVGA CLC" > "Calibration" if needed.               |
    \*---------------------------------------------------------*/
    cal_r = 20;
    cal_g = 20;
    cal_b = 100;

    SettingsManager* settings_manager = ResourceManager::get()->GetSettingsManager();

    if(settings_manager != nullptr)
    {
        json device_settings = settings_manager->GetSettings(det_name);

        if(device_settings.contains("Calibration"))
        {
            json cal = device_settings["Calibration"];

            if(cal.contains("R_Scale"))
            {
                cal_r = cal["R_Scale"].get<unsigned char>();
            }

            if(cal.contains("G_Scale"))
            {
                cal_g = cal["G_Scale"].get<unsigned char>();
            }

            if(cal.contains("B_Scale"))
            {
                cal_b = cal["B_Scale"].get<unsigned char>();
            }
        }
        else
        {
            /*-------------------------------------------------*\
            | Write default calibration so users can find and    |
            | edit it in OpenRGB.json                            |
            \*-------------------------------------------------*/
            device_settings["Calibration"]["R_Scale"] = cal_r;
            device_settings["Calibration"]["G_Scale"] = cal_g;
            device_settings["Calibration"]["B_Scale"] = cal_b;
            settings_manager->SetSettings(det_name, device_settings);
            settings_manager->SaveSettings();
        }
    }
}

unsigned char RGBController_EVGACLC::CalibrateChannel(unsigned char value, unsigned char scale)
{
    if(scale >= 100)
    {
        return value;
    }

    unsigned int result = (value * scale) / 100;

    if(result > 255)
    {
        result = 255;
    }

    return (unsigned char)(result);
}

void RGBController_EVGACLC::SetupZones()
{
    if(!zones.empty())
    {
        return;
    }

    if(!controller)
    {
        return;
    }

    zone new_zone;

    new_zone.name       = "Logo";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = NULL;
    zones.push_back(new_zone);

    led new_led;

    new_led.name        = "Logo LED";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_EVGACLC::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGACLC::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_EVGACLC::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGACLC::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGACLC::DeviceUpdateMode()
{
    if(!controller)
    {
        return;
    }

    if(active_mode < 0 || active_mode >= (int)modes.size())
    {
        return;
    }

    switch(modes[active_mode].value)
    {
        case EVGA_CLC_MODE_FIXED:
            controller->SetFixed(
                CalibrateChannel(RGBGetRValue(colors[0]), cal_r),
                CalibrateChannel(RGBGetGValue(colors[0]), cal_g),
                CalibrateChannel(RGBGetBValue(colors[0]), cal_b)
            );
            break;

        case EVGA_CLC_MODE_FADING:
            controller->SetFading(
                CalibrateChannel(RGBGetRValue(modes[active_mode].colors[0]), cal_r),
                CalibrateChannel(RGBGetGValue(modes[active_mode].colors[0]), cal_g),
                CalibrateChannel(RGBGetBValue(modes[active_mode].colors[0]), cal_b),
                CalibrateChannel(RGBGetRValue(modes[active_mode].colors[1]), cal_r),
                CalibrateChannel(RGBGetGValue(modes[active_mode].colors[1]), cal_g),
                CalibrateChannel(RGBGetBValue(modes[active_mode].colors[1]), cal_b),
                modes[active_mode].speed
            );
            break;

        case EVGA_CLC_MODE_BLINKING:
            controller->SetBlinking(
                CalibrateChannel(RGBGetRValue(modes[active_mode].colors[0]), cal_r),
                CalibrateChannel(RGBGetGValue(modes[active_mode].colors[0]), cal_g),
                CalibrateChannel(RGBGetBValue(modes[active_mode].colors[0]), cal_b),
                modes[active_mode].speed
            );
            break;

        case EVGA_CLC_MODE_RAINBOW:
            controller->SetRainbow(modes[active_mode].speed);
            break;
    }
}
