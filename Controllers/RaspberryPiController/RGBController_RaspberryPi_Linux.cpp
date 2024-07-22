/*---------------------------------------------------------*\
| RGBController_RaspberryPi_Linux.cpp                       |
|                                                           |
|   RGBController for Raspberry Pi GPIO ARGB                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_RaspberryPi_Linux.h"

/**------------------------------------------------------------------*\
    @name Raspberry Pi
    @category Motherboard
    @type GPIO
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectRaspberryPiControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RaspberryPi::RGBController_RaspberryPi(RaspberryPiController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = "Raspberry Pi";
    type                    = DEVICE_TYPE_MOTHERBOARD;
    vendor                  = "Raspberry Pi";
    description             = "Raspberry Pi GPIO Device";
    location                = controller->GetDeviceLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_RaspberryPi::SetupZones()
{
    /*-------------------------------------------------*\
    | Only set LED count on the first run               |
    \*-------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(1);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for (unsigned int channel_idx = 0; channel_idx < 1; channel_idx++)
    {
        char ch_idx_string[2];
        snprintf(ch_idx_string, 2, "%d", channel_idx + 1);

        zones[channel_idx].name     = "Channel ";
        zones[channel_idx].name.append(ch_idx_string);
        zones[channel_idx].type     = ZONE_TYPE_LINEAR;
        zones[channel_idx].leds_min = 0;
        zones[channel_idx].leds_max = 1000;

        if(first_run)
        {
            zones[channel_idx].leds_count = 0;
        }

        zones[channel_idx].matrix_map = NULL;

        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            char led_idx_string[4];
            snprintf(led_idx_string, 4, "%d", led_ch_idx + 1);

            led new_led;
            new_led.name = "Channel ";
            new_led.name.append(ch_idx_string);
            new_led.name.append(", LED ");
            new_led.name.append(led_idx_string);

            leds.push_back(new_led);
        }
    }

    controller->SetSize(zones[0].leds_count);

    SetupColors();
}

void RGBController_RaspberryPi::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_RaspberryPi::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_RaspberryPi::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RaspberryPi::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RaspberryPi::DeviceUpdateMode()
{
    controller->SetColors(&colors[0]);
}
