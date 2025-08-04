/*---------------------------------------------------------*\
| RGBController_ColorfulTuringGPU.cpp                       |
|                                                           |
|   RGBController for Colorful Turing GPU                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_ColorfulTuringGPU.h"

/**------------------------------------------------------------------*\
    @name Colorful GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectColorfulTuringGPUControllers
    @comment This card supports off, direct, rainbow and pulse mode.
\*-------------------------------------------------------------------*/

RGBController_ColorfulTuringGPU::RGBController_ColorfulTuringGPU(ColorfulTuringGPUController * colorful_gpu_ptr)
{
    controller              = colorful_gpu_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "Colorful";
    type                    = DEVICE_TYPE_GPU;
    description             = name;
    location                = controller->GetDeviceLocation();

    mode Off;
    Off.name                = "Off";
    Off.value               = COLORFUL_TURING_GPU_RGB_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = COLORFUL_TURING_GPU_RGB_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode StateDisplay;
    StateDisplay.name       = "State Display";
    StateDisplay.value      = COLORFUL_TURING_GPU_RGB_MODE_STATE_DISPLAY;
    StateDisplay.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    StateDisplay.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(StateDisplay);

    mode Rainbow;
    Rainbow.name            = "Spectrum Cycle";
    Rainbow.value           = COLORFUL_TURING_GPU_RGB_MODE_RAINBOW;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = COLORFUL_TURING_GPU_RGB_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    SetupZones();

    // Initialize active mode
    active_mode             = getModeIndex(controller->GetMode());
    colors[0]               = controller->GetColor();

}

RGBController_ColorfulTuringGPU::~RGBController_ColorfulTuringGPU()
{
    delete controller;
}

int RGBController_ColorfulTuringGPU::getModeIndex(int mode_value)
{
    for(unsigned int mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if(modes[mode_index].value == mode_value)
        {
            return(mode_index);
        }
    }

    return(0);
}

void RGBController_ColorfulTuringGPU::SetupZones()
{
    zone new_zone;

    new_zone.name       = "GPU";
    new_zone.type       = ZONE_TYPE_SINGLE;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);
    leds[0].name = "GPU LED";

    SetupColors();
}

void RGBController_ColorfulTuringGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_ColorfulTuringGPU::DeviceUpdateLEDs()
{
    switch(modes[active_mode].value)
    {
        case COLORFUL_TURING_GPU_RGB_MODE_BREATHING:
            controller->SetBreathing(colors[0]);
            break;
        case COLORFUL_TURING_GPU_RGB_MODE_OFF:
            controller->SetOff();
            break;
        case COLORFUL_TURING_GPU_RGB_MODE_RAINBOW:
            controller->SetRainbow();
            break;
        case COLORFUL_TURING_GPU_RGB_MODE_STATE_DISPLAY:
            controller->SetStateDisplay(colors[0]);
            break;
        case COLORFUL_TURING_GPU_RGB_MODE_STATIC:
            controller->SetDirect(colors[0], false);
            break;
        default:
            controller->SetDirect(colors[0], false);
    }
}

void RGBController_ColorfulTuringGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulTuringGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulTuringGPU::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulTuringGPU::DeviceSaveMode()
{
    switch(modes[active_mode].value)
    {
        case COLORFUL_TURING_GPU_RGB_MODE_STATIC:
            controller->SetDirect(colors[0], true);
            break;
        default:
            DeviceUpdateLEDs();
    }
}
