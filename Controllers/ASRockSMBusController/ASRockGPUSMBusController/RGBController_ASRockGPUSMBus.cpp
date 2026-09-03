/*---------------------------------------------------------*\
| RGBController_ASRockGPUSMBus.cpp                          |
|                                                           |
|   RGBController for ASRock GPU RGB controllers            |
|                                                           |
|   Based on SignalRGB ASRock GPU.js plugin                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ASRockGPUSMBus.h"

/**------------------------------------------------------------------*\
    @name ASRock GPU SMBus
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectASRockGPUSMBusControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ASRockGPUSMBus::RGBController_ASRockGPUSMBus(ASRockGPUSMBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "ASRock";
    type        = DEVICE_TYPE_GPU;
    description = "ASRock GPU RGB Controller";
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ASRockGPUSMBus::~RGBController_ASRockGPUSMBus()
{
    delete controller;
}

void RGBController_ASRockGPUSMBus::SetupZones()
{
    /*-----------------------------------------------------*\
    | Set up zones - one zone per channel                   |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < controller->num_channels; i++)
    {
        uint8_t channel_idx = controller->enabled_channels[i];
        
        zone new_zone;
        new_zone.type = ZONE_TYPE_SINGLE;
        new_zone.leds_min = 1;
        new_zone.leds_max = 1;
        new_zone.leds_count = 1;
        
        /*-----------------------------------------------------*\
        | Channel names from SignalRGB                          |
        \*-----------------------------------------------------*/
        if(channel_idx == ASROCK_GPU_CHANNEL_ARGB_HEADER)
        {
            new_zone.name = "ARGB Header";
        }
        else if(channel_idx == ASROCK_GPU_CHANNEL_TOP_SIDE)
        {
            new_zone.name = "Top Side";
        }
        else if(channel_idx == ASROCK_GPU_CHANNEL_FAN)
        {
            new_zone.name = "Fan";
        }
        else
        {
            new_zone.name = "Channel " + std::to_string(channel_idx);
        }
        
        zones.push_back(new_zone);
        
        /*-----------------------------------------------------*\
        | Set up LED for this zone                              |
        \*-----------------------------------------------------*/
        led new_led;
        new_led.name = new_zone.name;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ASRockGPUSMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | This device does not support resizing zones           |
    \*-----------------------------------------------------*/
}

void RGBController_ASRockGPUSMBus::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < controller->num_channels; i++)
    {
        uint8_t channel_idx = controller->enabled_channels[i];
        RGBColor color = colors[i];
        
        controller->SetChannelColor(channel_idx, 
                                    RGBGetRValue(color),
                                    RGBGetGValue(color),
                                    RGBGetBValue(color));
    }
}

void RGBController_ASRockGPUSMBus::UpdateZoneLEDs(int zone)
{
    uint8_t channel_idx = controller->enabled_channels[zone];
    RGBColor color = colors[zone];
    
    controller->SetChannelColor(channel_idx,
                                RGBGetRValue(color),
                                RGBGetGValue(color),
                                RGBGetBValue(color));
}

void RGBController_ASRockGPUSMBus::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_ASRockGPUSMBus::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
