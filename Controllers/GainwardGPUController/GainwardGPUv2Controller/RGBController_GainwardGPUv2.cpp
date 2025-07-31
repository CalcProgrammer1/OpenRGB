/*---------------------------------------------------------*\
| RGBController_GainwardGPUv2.cpp                           |
|                                                           |
|   RGBController for Gainward v2 GPU                       |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GainwardGPUv2.h"

/**------------------------------------------------------------------*\
    @name Gainward GPU v2
    @category GPU
    @type I2C
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectGainwardGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GainwardGPUv2::RGBController_GainwardGPUv2(GainwardGPUv2Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Gainward GPU";
    vendor                      = "Gainward";
    type                        = DEVICE_TYPE_GPU;
    description                 = "Gainward RTX GPU";
    location                    = controller->GetDeviceLocation();

    mode Static;
    Static.name                 = "Static";
    Static.value                = GAINWARD_V2_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathe;
    Breathe.name                = "Breathing";
    Breathe.value               = GAINWARD_V2_MODE_STATIC;
    Breathe.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathe.speed_max           = 0x000a;
    Breathe.speed_min           = 0x1324;
    Breathe.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Breathe.colors_min          = 2;
    Breathe.colors_max          = 2;
    Breathe.colors.resize(2);
    modes.push_back(Breathe);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = GAINWARD_V2_MODE_CYCLE;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.speed_max       = 0x0;
    RainbowWave.speed_min       = 0xF;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = GAINWARD_V2_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Strobe.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Strobe.colors_min           = 1;
    Strobe.colors_max           = 1;
    Strobe.colors.resize(1);
    Strobe.speed_max            = 0x0;
    Strobe.speed_min            = 0xF;
    modes.push_back(Strobe);

    SetupZones();

    /*-------------------------*\
    | Initialize active mode    |
    \*-------------------------*/
    active_mode = 0;
}

RGBController_GainwardGPUv2::~RGBController_GainwardGPUv2()
{
    delete controller;
}

void RGBController_GainwardGPUv2::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone gainward_gpu_zone;
    gainward_gpu_zone.name              = "GPU";
    gainward_gpu_zone.type              = ZONE_TYPE_SINGLE;
    gainward_gpu_zone.leds_min          = 1;
    gainward_gpu_zone.leds_max          = 1;
    gainward_gpu_zone.leds_count        = 1;
    gainward_gpu_zone.matrix_map        = NULL;
    zones.push_back(gainward_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led gainward_gpu_led;
    gainward_gpu_led.name               = "GPU";
    leds.push_back(gainward_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = controller->GetLEDRed();
    unsigned char grn = controller->GetLEDGreen();
    unsigned char blu = controller->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_GainwardGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GainwardGPUv2::DeviceUpdateLEDs()
{
    for(unsigned int color : colors)
    {
        unsigned char red = RGBGetRValue(color);
        unsigned char grn = RGBGetGValue(color);
        unsigned char blu = RGBGetBValue(color);

        controller->SetLEDColors(red, grn, blu);
        controller->SetMode(GAINWARD_V2_MODE_STATIC, 0x2);
    }
}

void RGBController_GainwardGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GainwardGPUv2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GainwardGPUv2::DeviceUpdateMode()
{
    mode current_mode = modes[(unsigned int)active_mode];
    switch (active_mode)
    {
        case 1:
            {
                controller->SetBreathingSpeed(current_mode.speed);

                unsigned char r1 = RGBGetRValue(current_mode.colors[0]);
                unsigned char g1 = RGBGetGValue(current_mode.colors[0]);
                unsigned char b1 = RGBGetBValue(current_mode.colors[0]);
                controller->SetLEDColors(r1, g1, b1);

                unsigned char r2 = RGBGetRValue(current_mode.colors[1]);
                unsigned char g2 = RGBGetGValue(current_mode.colors[1]);
                unsigned char b2 = RGBGetBValue(current_mode.colors[1]);
                controller->SetLEDColors(r2, g2, b2, GAINWARD_V2_COLOR_REGISTER_SECONDARY);
                controller->SetMode(GAINWARD_V2_MODE_STATIC, 0x2, GAINWARD_V2_STATIC_BREATHING);
            }
            break;

        /*---------------------------------------------*\
        | Case 3 intentionally falls through to case 2  |
        \*---------------------------------------------*/
        case 3:
            {
                unsigned char r = RGBGetRValue(current_mode.colors[0]);
                unsigned char g = RGBGetGValue(current_mode.colors[0]);
                unsigned char b = RGBGetBValue(current_mode.colors[0]);
                controller->SetLEDColors(r, g, b, GAINWARD_V2_COLOR_REGISTER_TERTIARY);
            }

        case 2:
            controller->SetMode((unsigned char)(current_mode.value), (unsigned char)(current_mode.speed));
            controller->SetDirection(current_mode.direction);
            break;

        default:
            controller->SetMode((unsigned char)(current_mode.value), (unsigned char)(current_mode.speed));
            break;
    }
}
