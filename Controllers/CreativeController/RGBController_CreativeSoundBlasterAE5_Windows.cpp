/*---------------------------------------------------------*\
| RGBController_CreativeSoundBlasterAE5_Windows.cpp         |
|                                                           |
|   RGBController for Creative SoundBlaster AE-5 (Windows)  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_CreativeSoundBlasterAE5_Windows.h"

/**------------------------------------------------------------------*\
    @name Creative Sound Blaster AE-5
    @category Audio
    @type PCI
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectCreativeAE5Device
    @comment
\*-------------------------------------------------------------------*/

RGBController_CreativeSoundBlasterAE5::RGBController_CreativeSoundBlasterAE5(CreativeSoundBlasterAE5ControllerBase* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Creative Labs";
    type                        = DEVICE_TYPE_SPEAKER;
    description                 = controller->GetDeviceName() + " Device";
    location                    = controller->GetDeviceLocation();
    serial                      = "";

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CreativeSoundBlasterAE5::~RGBController_CreativeSoundBlasterAE5()
{
    Shutdown();

    delete controller;
}

void RGBController_CreativeSoundBlasterAE5::SetupZones()
{
    /*-----------------------------------------------------*\
    | Only set LED count on the first run                   |
    \*-----------------------------------------------------*/
    bool first_run = false;

    if(zones.size() == 0)
    {
        first_run = true;
    }

    /*-----------------------------------------------------*\
    | Clear any existing color/LED configuration            |
    \*-----------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(2);

    /*-----------------------------------------------------*\
    | Set up internal zone                                  |
    \*-----------------------------------------------------*/
    zones[0].name                       = "Internal";
    zones[0].type                       = ZONE_TYPE_LINEAR;
    zones[0].leds_min                   = 5;
    zones[0].leds_max                   = 5;
    zones[0].leds_count                 = 5;

    /*-----------------------------------------------------*\
    | Set up internal zone and LEDs                         |
    \*-----------------------------------------------------*/
    zones[1].leds_min                   = 0;
    zones[1].leds_max                   = 100;

    if(first_run)
    {
        zones[1].flags                  = ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE
                                        | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP;
    }

    if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
    {
        zones[1].name                   = "Addressable RGB Header";
    }

    if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE))
    {
        zones[1].leds_count             = controller->GetExternalLEDCount();;
    }

    if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
    {
        zones[1].type                   = ZONE_TYPE_LINEAR;
    }

    if(!(zones[1].flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP))
    {
        zones[1].matrix_map.width       = 0;
        zones[1].matrix_map.height      = 0;
        zones[1].matrix_map.map.resize(0);
    }

    /*-----------------------------------------------------*\
    | Set up LEDs                                           |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int led_idx = 0; led_idx < zones[zone_idx].leds_count; led_idx++)
        {
            led new_led;
            new_led.name = zones[0].name + ", LED " + std::to_string(led_idx + 1);
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_CreativeSoundBlasterAE5::DeviceConfigureZone(int zone_idx)
{
    if((size_t)zone_idx < zones.size())
    {
        controller->SetExternalLEDCount(zones[zone_idx].leds_count);

        SetupZones();
    }
}

void RGBController_CreativeSoundBlasterAE5::UpdateLEDRange(unsigned int start_led, unsigned int led_count)
{
    if(led_count == 0)
    {
        return;
    }

    unsigned char* red_values   = new unsigned char[led_count];
    unsigned char* green_values = new unsigned char[led_count];
    unsigned char* blue_values  = new unsigned char[led_count];

    for(unsigned int i = 0; i < led_count; i++)
    {
        unsigned int led_idx    = start_led + i;
        red_values[i]           = RGBGetRValue(colors[led_idx]);
        green_values[i]         = RGBGetGValue(colors[led_idx]);
        blue_values[i]          = RGBGetBValue(colors[led_idx]);
    }

    controller->SetLEDColors(led_count, red_values, green_values, blue_values);

    delete[] red_values;
    delete[] green_values;
    delete[] blue_values;
}

void RGBController_CreativeSoundBlasterAE5::DeviceUpdateLEDs()
{
    UpdateLEDRange(0, controller->GetLEDCount());
}

void RGBController_CreativeSoundBlasterAE5::DeviceUpdateZoneLEDs(int zone)
{
    if(zone >= 0 && zone < (int)zones.size())
    {
        unsigned int start_led = 0;

        for(int i = 0; i < zone; i++)
        {
            start_led += zones[i].leds_count;
        }

        UpdateLEDRange(start_led, zones[zone].leds_count);
    }
}

void RGBController_CreativeSoundBlasterAE5::DeviceUpdateSingleLED(int led)
{
    /*-------------------------------------------------------------*\
    | Find which zone this LED belongs to and update only that zone |
    \*-------------------------------------------------------------*/

    unsigned int current_led = 0;
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(led >= (int)current_led && led < (int)(current_led + zones[zone_idx].leds_count))
        {
            UpdateLEDRange(current_led, zones[zone_idx].leds_count);
            return;
        }
        current_led += zones[zone_idx].leds_count;
    }
}

void RGBController_CreativeSoundBlasterAE5::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
