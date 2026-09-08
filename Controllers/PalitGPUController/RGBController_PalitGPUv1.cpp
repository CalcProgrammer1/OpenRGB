/*---------------------------------------------------------*\
| RGBController_PalitGPUv1.cpp                              |
|                                                           |
|   RGBController for Palit v1 GPU                          |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_PalitGPUv1.h"

/**------------------------------------------------------------------*\
    @name Palit GPU v1
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPalitGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PalitGPUv1::RGBController_PalitGPUv1(PalitGPUv1Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Palit";
    type                    = DEVICE_TYPE_GPU;
    description             = "Palit GPU V1 Device";
    location                = controller->GetDeviceLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 1;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_PalitGPUv1::~RGBController_PalitGPUv1()
{
    Shutdown();

    delete controller;
}

void RGBController_PalitGPUv1::SetupZones()
{
    /*-----------------------------------------------------*\
    | Set up zone                                           |
    \*-----------------------------------------------------*/
    zone palit_gpu_zone;
    palit_gpu_zone.name              = "GPU";
    palit_gpu_zone.type              = ZONE_TYPE_SINGLE;
    palit_gpu_zone.leds_min          = 1;
    palit_gpu_zone.leds_max          = 1;
    palit_gpu_zone.leds_count        = 1;
    zones.push_back(palit_gpu_zone);

    /*-----------------------------------------------------*\
    | Set up LED                                            |
    \*-----------------------------------------------------*/
    led palit_gpu_led;
    palit_gpu_led.name               = "GPU";
    leds.push_back(palit_gpu_led);

    SetupColors();

    /*-----------------------------------------------------*\
    | Initialize color                                      |
    \*-----------------------------------------------------*/
    unsigned char red = controller->GetLEDRed();
    unsigned char grn = controller->GetLEDGreen();
    unsigned char blu = controller->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_PalitGPUv1::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        controller->SetLEDColors(red, grn, blu);
    }
}

void RGBController_PalitGPUv1::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPUv1::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPUv1::DeviceUpdateMode()
{

}
