/*---------------------------------------------------------*\
| RGBController_RazerKrakenV4.cpp                           |
|                                                           |
|   RGBController for Razer 64-byte devices                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RazerKrakenV4.h"
#include "RazerDevices.h"

RGBController_RazerKrakenV4::RGBController_RazerKrakenV4(RazerKrakenV4Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "Razer";
    type                    = controller->GetDeviceType();
    description             = "Razer Device";
    location                = controller->GetDeviceLocation();
    version                 = controller->GetFirmwareString();
    serial                  = controller->GetSerialString();
    uint8_t max_brightness  = controller->GetMaxBrightness();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = RAZER_KRAKEN_V4_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = RAZER_KRAKEN_V4_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_BRIGHTNESS;
    Wave.direction          = MODE_DIRECTION_RIGHT;
    Wave.color_mode         = MODE_COLORS_NONE;
    Wave.brightness_min     = 0;
    Wave.brightness_max     = max_brightness;
    Wave.brightness         = max_brightness;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_RazerKrakenV4::~RGBController_RazerKrakenV4()
{
    Shutdown();

    delete controller;
}

void RGBController_RazerKrakenV4::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();

    /*-----------------------------------------------------*\
    | Fill in zone information based on device table        |
    | Kraken V4 devices are assumed to only have one row    |
    \*-----------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone new_zone;

            new_zone.name       = device_list[device_index]->zones[zone_id]->name;
            new_zone.type       = device_list[device_index]->zones[zone_id]->type;

            new_zone.leds_count = device_list[device_index]->zones[zone_id]->cols;
            new_zone.leds_min   = new_zone.leds_count;
            new_zone.leds_max   = new_zone.leds_count;

            zones.push_back(new_zone);

            for(unsigned int col_id = 0; col_id < device_list[device_index]->zones[zone_id]->cols; col_id++)
            {
                led* new_led = new led();

                new_led->name = device_list[device_index]->zones[zone_id]->name;

                if(zones[zone_id].leds_count > 1)
                {
                    new_led->name.append(" LED ");
                    new_led->name.append(std::to_string(col_id + 1));
                }

                leds.push_back(*new_led);
            }
        }
    }

    SetupColors();
}

void RGBController_RazerKrakenV4::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == RAZER_KRAKEN_V4_MODE_DIRECT)
    {
        controller->SetDirect(&colors[0]);
    }
}

void RGBController_RazerKrakenV4::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKrakenV4::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerKrakenV4::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_KRAKEN_V4_MODE_WAVE:
            controller->SetModeWave();
            controller->SetBrightness(modes[active_mode].brightness);
            break;
    }
}
