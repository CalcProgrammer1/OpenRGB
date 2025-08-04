/*---------------------------------------------------------*\
| RGBController_ColorfulGPU.cpp                             |
|                                                           |
|   RGBController for Colorful GPU                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_ColorfulGPU.h"

/**------------------------------------------------------------------*\
    @name Colorful GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectColorfulGPUControllers
    @comment This card only supports direct mode
\*-------------------------------------------------------------------*/

RGBController_ColorfulGPU::RGBController_ColorfulGPU(ColorfulGPUController * colorful_gpu_ptr)
{
    controller  = colorful_gpu_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Colorful";
    type        = DEVICE_TYPE_GPU;
    description = name;
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

}

RGBController_ColorfulGPU::~RGBController_ColorfulGPU()
{
    delete controller;
}

void RGBController_ColorfulGPU::SetupZones()
{
    zone new_zone;

    new_zone.name       = "GPU";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);
    leds[0].name = "GPU LED";

    SetupColors();
}

void RGBController_ColorfulGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_ColorfulGPU::DeviceUpdateLEDs()
{
    controller->SetDirect(colors[0]);
}

void RGBController_ColorfulGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulGPU::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
