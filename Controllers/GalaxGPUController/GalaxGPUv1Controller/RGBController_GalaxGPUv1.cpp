/*---------------------------------------------------------*\
| RGBController_GalaxGPUv1.cpp                              |
|                                                           |
|   RGBController for Galax/KFA2 GPU                        |
|                                                           |
|   Niels Westphal (crashniels)                 12 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GalaxGPUv1.h"

int RGBController_GalaxGPUv1::GetDeviceMode()
{
    int modereg1 = controller->GalaxGPURegisterRead(GALAX_V1_MODE_REGISTER_1);
    int modereg2 = controller->GalaxGPURegisterRead(GALAX_V1_MODE_REGISTER_2);

    if(modereg1 == GALAX_V1_MODE_STATIC_VALUE_1 && modereg2 == GALAX_V1_MODE_STATIC_VALUE_2)
    {
        active_mode = 1;
        modes[active_mode].color_mode = MODE_COLORS_PER_LED;
    }

    if(modereg1 == GALAX_V1_MODE_BREATHING_VALUE_1 && modereg2 == GALAX_V1_MODE_BREATHING_VALUE_2)
    {
        active_mode = 2;
        modes[active_mode].color_mode = MODE_COLORS_PER_LED;
    }

    if(modereg1 == GALAX_V1_MODE_RAINBOW_VALUE_1 && modereg2 == GALAX_V1_MODE_RAINBOW_VALUE_2)
    {
        active_mode = 3;
        modes[active_mode].color_mode = MODE_COLORS_NONE;
    }

    if(modereg1 == GALAX_V1_MODE_CYCLE_BREATHING_VALUE_1 && modereg2 == GALAX_V1_MODE_CYCLE_BREATHING_VALUE_2)
    {
        active_mode = 4;
        modes[active_mode].color_mode = MODE_COLORS_NONE;
    }

    return(active_mode);
}

/**------------------------------------------------------------------*\
    @name Galax GPU v1
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGalaxGPUv1Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GalaxGPUv1::RGBController_GalaxGPUv1(GalaxGPUv1Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "GALAX";
    type                        = DEVICE_TYPE_GPU;
    description                 = "GALAX / KFA2 RTX GPU";
    location                    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 1;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = 2;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = 3;
    Rainbow.flags               = 0;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Cycle_Breathing;
    Cycle_Breathing.name        = "Cycle Breathing";
    Cycle_Breathing.value       = 4;
    Cycle_Breathing.flags       = 0;
    Cycle_Breathing.color_mode  = MODE_COLORS_NONE;
    modes.push_back(Cycle_Breathing);

    SetupZones();

    active_mode = GetDeviceMode();
}

RGBController_GalaxGPUv1::~RGBController_GalaxGPUv1()
{
    delete controller;
}

void RGBController_GalaxGPUv1::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone galax_gpu_zone;
    galax_gpu_zone.name         = "GPU";
    galax_gpu_zone.type         = ZONE_TYPE_SINGLE;
    galax_gpu_zone.leds_min     = 1;
    galax_gpu_zone.leds_max     = 1;
    galax_gpu_zone.leds_count   = 1;
    galax_gpu_zone.matrix_map   = NULL;
    zones.push_back(galax_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led galax_gpu_led;
    galax_gpu_led.name          = "GPU";
    leds.push_back(galax_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = controller->GetLEDRed();
    unsigned char grn = controller->GetLEDGreen();
    unsigned char blu = controller->GetLEDBlue();

    colors[0] = ToRGBColor(red, grn, blu);
}

void RGBController_GalaxGPUv1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GalaxGPUv1::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        if(GetMode() == 1)
        {
            controller->SetLEDColorsDirect(red, grn, blu);
        }
        else
        {
            controller->SetLEDColorsEffect(red, grn, blu);
        }
    }
}

void RGBController_GalaxGPUv1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPUv1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPUv1::DeviceUpdateMode()
{
    int new_mode = modes[active_mode].value;

    controller->SetMode(new_mode);
}
