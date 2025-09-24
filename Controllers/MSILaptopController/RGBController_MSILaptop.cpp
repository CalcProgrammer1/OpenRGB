/*---------------------------------------------------------*\
| RGBController_MSILaptop.cpp                       |
|                                                           |
|   RGBController for MSI laptop SteelSeries RGB devices    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_MSILaptop.h"

#define NA                                      0xFFFFFFFF
/**------------------------------------------------------------------*\
    @name MSI Laptop SteelSeries RGB
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectMSILaptop
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSILaptop::RGBController_MSILaptop(MSILaptopController* controller_ptr, const MSILaptopModel* model_ptr)
{
    controller          = controller_ptr;
    model               = model_ptr;

    name                = controller->GetDeviceName();
    vendor              = "SteelSeries";
    description         = std::string(model->sys_vendor) + " " + std::string(model->product_name) + " RGB Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();
    type                = (controller->GetDeviceType() == MSI_LAPTOP_KLC) ? DEVICE_TYPE_KEYBOARD : DEVICE_TYPE_LEDSTRIP;

    mode Direct;
    Direct.name         = "Direct";
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_MSILaptop::~RGBController_MSILaptop()
{
    delete controller;
}

void RGBController_MSILaptop::SetupZones()
{
    if(controller->GetDeviceType() == MSI_LAPTOP_KLC)
    {
        zone keyboard_zone;

        keyboard_zone.name                   = ZONE_EN_KEYBOARD;
        keyboard_zone.type                   = ZONE_TYPE_MATRIX;
        keyboard_zone.leds_min               = model->klc_leds_count;
        keyboard_zone.leds_max               = model->klc_leds_count;
        keyboard_zone.leds_count             = model->klc_leds_count;
        keyboard_zone.matrix_map.Set(model->klc_matrix_height, model->klc_matrix_width, (unsigned int *)model->klc_matrix_map);

        zones.push_back(keyboard_zone);

        for(unsigned int led_idx = 0; led_idx < model->klc_leds_count; led_idx++)
        {
            led new_led;
            new_led.name  = model->klc_leds[led_idx].name;
            new_led.value = model->klc_leds[led_idx].id;
            leds.push_back(new_led);
        }
    }
    else
    {
        zone lightbar_zone;
        lightbar_zone.name       = "Lightbar";
        lightbar_zone.type       = ZONE_TYPE_LINEAR;
        lightbar_zone.start_idx  = 0;
        lightbar_zone.leds_min   = model->alc_lightbar_leds;
        lightbar_zone.leds_max   = model->alc_lightbar_leds;
        lightbar_zone.leds_count = model->alc_lightbar_leds;
        zones.push_back(lightbar_zone);

        zone logo_zone;
        logo_zone.name           = "Logo";
        logo_zone.type           = ZONE_TYPE_SINGLE;
        logo_zone.start_idx      = model->alc_lightbar_leds;
        logo_zone.leds_min       = 1;
        logo_zone.leds_max       = 1;
        logo_zone.leds_count     = 1;
        zones.push_back(logo_zone);

        for(unsigned int led_idx = 0; led_idx < model->alc_leds_count; led_idx++)
        {
            led new_led;
            new_led.name  = model->alc_leds[led_idx].name;
            new_led.value = model->alc_leds[led_idx].id;
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_MSILaptop::DeviceUpdateLEDs()
{
    controller->SetLEDs(leds, colors);
}

void RGBController_MSILaptop::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSILaptop::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSILaptop::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
