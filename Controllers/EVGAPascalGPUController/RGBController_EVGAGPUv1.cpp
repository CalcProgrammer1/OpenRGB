/*---------------------------------------------------------*\
| RGBController_EVGAGPUv1.cpp                               |
|                                                           |
|   RGBController for EVGA V1 (Pascal) GPU                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_EVGAGPUv1.h"

/**------------------------------------------------------------------*\
    @name EVGA RGB v1 GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectEVGAGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_EVGAGPUv1::RGBController_EVGAGPUv1(EVGAGPUv1Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "EVGA";
    description             = "EVGA RGB v1 GPU Device";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                = "Off";
    Off.value               = EVGA_GPU_V1_MODE_OFF;
    Off.flags               = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = EVGA_GPU_V1_MODE_CUSTOM;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = EVGA_GPU_V1_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_MANUAL_SAVE;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = EVGA_GPU_V1_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    SetupZones();

    // Initialize active mode and stored color

    unsigned char raw_active_mode = controller->GetMode();

    active_mode = 0;
    for(unsigned int i = 0; i < modes.size(); i++)
    {
        if (modes[i].value == raw_active_mode)
        {
            active_mode = i;
            break;
        }
    }

    unsigned char r = controller->GetRed();
    unsigned char g = controller->GetGreen();
    unsigned char b = controller->GetBlue();

    RGBColor color = ToRGBColor(r, g, b);
    colors[0] = color;
}

RGBController_EVGAGPUv1::~RGBController_EVGAGPUv1()
{
    delete controller;
}

void RGBController_EVGAGPUv1::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_EVGAGPUv1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAGPUv1::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_EVGAGPUv1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv1::DeviceUpdateMode()
{
    controller->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
}

void RGBController_EVGAGPUv1::DeviceSaveMode()
{
    controller->SaveSettings();
}
