/*---------------------------------------------------------*\
| RGBController_RazerHanbo.cpp                              |
|                                                           |
|   RGBController for Razer Hanbo devices                   |
|                                                           |
|   Joseph East (dripsnek)                      12 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_RazerHanbo.h"
#include "RazerDevices.h"

/**------------------------------------------------------------------*\
    @name Razer Hanbo Chroma
    @category Cooler
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectRazerHanboControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RazerHanbo::RGBController_RazerHanbo(RazerHanboController* controller_ptr)
{
    controller               = controller_ptr;
    name                     = controller->GetName();
    vendor                   = "Razer";
    type                     = controller->GetDeviceType();
    description              = "Razer Hanbo Device";
    location                 = controller->GetDeviceLocation();
    version                  = controller->GetFirmwareString();
    serial                   = controller->GetSerialString();

    mode Direct;
    Direct.name              = "Direct";
    Direct.value             = RAZER_HANBO_MODE_DIRECT;
    Direct.flags             = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode        = MODE_COLORS_PER_LED;
    Direct.brightness_min    = MIN_BRIGHTNESS;
    Direct.brightness_max    = MAX_BRIGHTNESS;
    Direct.brightness        = MAX_BRIGHTNESS/2;
    modes.push_back(Direct);

    mode Off;
    Off.name                 = "Off";
    Off.value                = RAZER_HANBO_MODE_OFF;
    Off.flags                = 0;
    Off.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = RAZER_HANBO_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    local_mode = RAZER_HANBO_MODE_DIRECT;

    SetupZones();
}

RGBController_RazerHanbo::~RGBController_RazerHanbo()
{
    delete controller;
}

void RGBController_RazerHanbo::SetupZones()
{
    unsigned int device_index = controller->GetDeviceIndex();

    /*---------------------------------------------------------*\
    | Fill in zone information based on device table            |
    \*---------------------------------------------------------*/
    for(unsigned int zone_id = 0; zone_id < RAZER_MAX_ZONES; zone_id++)
    {
        if(device_list[device_index]->zones[zone_id] != NULL)
        {
            zone new_zone;

            new_zone.name       = device_list[device_index]->zones[zone_id]->name;
            new_zone.type       = device_list[device_index]->zones[zone_id]->type;
            new_zone.leds_count = device_list[device_index]->zones[zone_id]->rows * device_list[device_index]->zones[zone_id]->cols;
            new_zone.leds_min   = new_zone.leds_count;
            new_zone.leds_max   = new_zone.leds_count;

            if(new_zone.type == ZONE_TYPE_MATRIX)
            {
                new_zone.matrix_map.height  = device_list[device_index]->zones[zone_id]->rows;
                new_zone.matrix_map.width   = device_list[device_index]->zones[zone_id]->cols;
                new_zone.matrix_map.map.resize(new_zone.matrix_map.height * new_zone.matrix_map.width);

                for(unsigned int y = 0; y < new_zone.matrix_map.height; y++)
                {
                    for(unsigned int x = 0; x < new_zone.matrix_map.width; x++)
                    {
                        new_zone.matrix_map.map[(y * new_zone.matrix_map.width) + x] = (y * new_zone.matrix_map.width) + x;
                    }
                }
            }

            zones.push_back(new_zone);
        }
    }

    for(unsigned int zone_id = 0; zone_id < zones.size(); zone_id++)
    {
        for(unsigned int row_id = 0; row_id < device_list[device_index]->zones[zone_id]->rows; row_id++)
        {
            for(unsigned int col_id = 0; col_id < device_list[device_index]->zones[zone_id]->cols; col_id++)
            {
                led* new_led  = new led();

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

void RGBController_RazerHanbo::DeviceUpdateLEDs()
{
    DeviceUpdateZoneLEDs(PUMP);
    DeviceUpdateZoneLEDs(FAN1);
    DeviceUpdateZoneLEDs(FAN2);
    DeviceUpdateZoneLEDs(FAN3);
}

/*---------------------------------------------------------*\
| The Hanbo command set is arranged in terms of zones.      |
| Transactions are straight forward when grouped this way.  |
\*---------------------------------------------------------*/

void RGBController_RazerHanbo::DeviceUpdateZoneLEDs(int zoneid)
{
    controller->SetZoneLeds(zoneid, this->zones[zoneid]);
}

void RGBController_RazerHanbo::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RazerHanbo::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case RAZER_HANBO_MODE_DIRECT:
            if(local_mode != RAZER_HANBO_MODE_DIRECT)
                controller->SetDirectMode();
            break;

        case RAZER_HANBO_MODE_OFF:
            controller->SetModeOff();
            break;

        case RAZER_HANBO_MODE_SPECTRUM_CYCLE:
            controller->SetModeSpectrumCycle();
            break;
    }

    local_mode = modes[active_mode].value;
}
