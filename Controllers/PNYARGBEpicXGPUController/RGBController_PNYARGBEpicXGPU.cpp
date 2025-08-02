/*---------------------------------------------------------*\
| RGBController_PNYARGBEpicXGPU.cpp                         |
|                                                           |
|   RGBController for PNY ARGB Epic-X GPU                   |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "pci_ids.h"
#include "RGBController_PNYARGBEpicXGPU.h"

#define __ 0xFFFFFFFF

/**------------------------------------------------------------------*\
    @name PNY ARGB Epic-X GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectPNYARGBEpicXGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PNYARGBEpicXGPU::RGBController_PNYARGBEpicXGPU(PNYARGBEpicXGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "PNY";
    description             = "PNY ARGB Epic-X GPU Device";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = PNY_GPU_MODE_ARGB_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness       = 0xFF;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 0xFF;
    modes.push_back(Direct);

    SetupZones();

    active_mode = 0;
}

RGBController_PNYARGBEpicXGPU::~RGBController_PNYARGBEpicXGPU()
{
    delete controller;
}

void RGBController_PNYARGBEpicXGPU::SetupZones()
{
    /*-----------------------------------------------------*\
    | The logo zone has 4 LEDs, but they are part of the    |
    | FRONT register zone                                   |
    \*-----------------------------------------------------*/
    zone logo;
    logo.name                   = "Logo";
    logo.type                   = ZONE_TYPE_LINEAR;
    logo.leds_min               = 4;
    logo.leds_max               = 4;
    logo.leds_count             = 4;
    logo.matrix_map             = NULL;
    zones.push_back(logo);

    for(std::size_t led_idx = 0; led_idx < logo.leds_count; led_idx++)
    {
        led logo_led;

        logo_led.name           = "Logo LED " + std::to_string(led_idx);
        logo_led.value          = PNY_GPU_REG_ZONE_FRONT;

        leds.push_back(logo_led);
        zone_led_idx.push_back((unsigned char)(led_idx + 20));
    }

    /*-----------------------------------------------------*\
    | The front zone has 20 LEDs in a figure 8 pattern, the |
    | FRONT register has these 20 and then the 4 log LEDs   |
    \*-----------------------------------------------------*/
    zone front;
    front.name                  = "Front";
    front.type                  = ZONE_TYPE_LINEAR;
    front.leds_min              = 20;
    front.leds_max              = 20;
    front.leds_count            = 20;
    front.matrix_map            = NULL;
    zones.push_back(front);

    for(std::size_t led_idx = 0; led_idx < front.leds_count; led_idx++)
    {
        led front_led;

        front_led.name          = "Front LED " + std::to_string(led_idx);
        front_led.value         = PNY_GPU_REG_ZONE_FRONT;

        leds.push_back(front_led);
        zone_led_idx.push_back((unsigned char)led_idx);
    }

    /*-----------------------------------------------------*\
    | The arrow zone has 17 LEDs                            |
    \*-----------------------------------------------------*/
    zone arrow;
    arrow.name                  = "Arrow";
    arrow.type                  = ZONE_TYPE_LINEAR;
    arrow.leds_min              = 17;
    arrow.leds_max              = 17;
    arrow.leds_count            = 17;
    arrow.matrix_map            = NULL;
    zones.push_back(arrow);

    for(std::size_t led_idx = 0; led_idx < arrow.leds_count; led_idx++)
    {
        led arrow_led;

        arrow_led.name          = "Arrow LED " + std::to_string(led_idx);
        arrow_led.value         = PNY_GPU_REG_ZONE_ARROW;

        leds.push_back(arrow_led);
        zone_led_idx.push_back((unsigned char)led_idx);
    }

    SetupColors();
}

void RGBController_PNYARGBEpicXGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PNYARGBEpicXGPU::DeviceUpdateLEDs()
{
    for(std::size_t i = 0; i < leds.size(); i++)
    {
        UpdateSingleLED(i);
    }
}

void RGBController_PNYARGBEpicXGPU::UpdateZoneLEDs(int zone)
{
    for(unsigned int i = 0; i < zones[zone].leds_count; i++)
    {
        UpdateSingleLED(zones[zone].start_idx + i);
    }
}

void RGBController_PNYARGBEpicXGPU::UpdateSingleLED(int led)
{
    controller->SetLEDDirect(leds[led].value, zone_led_idx[led], PNY_GPU_MODE_ARGB_DIRECT, colors[led]);
}

void RGBController_PNYARGBEpicXGPU::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetZoneMode(PNY_GPU_REG_ZONE_FRONT, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, modes[active_mode].colors[0]);
        controller->SetZoneMode(PNY_GPU_REG_ZONE_ARROW, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, 0, modes[active_mode].colors[0]);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SetZoneMode(PNY_GPU_REG_ZONE_FRONT, modes[active_mode].value, 0, 0xFF, 0, 0);
        controller->SetZoneMode(PNY_GPU_REG_ZONE_ARROW, modes[active_mode].value, 0, 0xFF, 0, 0);
        DeviceUpdateLEDs();
    }
}