/*---------------------------------------------------------*\
| RGBController_CreativeSoundBlasterAE5.cpp                 |
|                                                           |
|   RGBController for Creative SoundBlaster AE-5            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CreativeSoundBlasterAE5.h"

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
    delete controller;
}

void RGBController_CreativeSoundBlasterAE5::SetupZones()
{
    zone internal_zone;
    internal_zone.name                  = "Internal";
    internal_zone.type                  = ZONE_TYPE_LINEAR;
    internal_zone.leds_min              = 5;
    internal_zone.leds_max              = 5;
    internal_zone.leds_count            = 5;
    internal_zone.matrix_map            = NULL;
    zones.push_back(internal_zone);

    for(unsigned int led_idx = 0; led_idx < 5; led_idx++)
    {
        led new_led;
        new_led.name = "Internal LED " + std::to_string(led_idx + 1);
        leds.push_back(new_led);
    }

    zone external_zone;
    external_zone.name                  = "External";
    external_zone.type                  = ZONE_TYPE_LINEAR;
    external_zone.leds_min              = 0;
    external_zone.leds_max              = 100;
    external_zone.leds_count            = controller->GetExternalLEDCount();
    external_zone.matrix_map            = NULL;
    zones.push_back(external_zone);

    for(unsigned int led_idx = 0; led_idx < controller->GetExternalLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name = "External LED " + std::to_string(led_idx + 1);
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CreativeSoundBlasterAE5::ResizeZone(int zone, int new_size)
{
    if(zone == 1) // External zone
    {
        zones[zone].leds_count = new_size;

        leds.resize(5);

        for(unsigned int led_idx = 0; led_idx < (unsigned int)new_size; led_idx++)
        {
            led new_led;
            new_led.name = "External LED " + std::to_string(led_idx + 1);
            leds.push_back(new_led);
        }

        controller->SetExternalLEDCount(new_size);
        SetupColors();
    }
}

void RGBController_CreativeSoundBlasterAE5::UpdateLEDRange(unsigned int start_led, unsigned int led_count)
{
    if(led_count == 0)
    {
        return;
    }

    unsigned char* red_values = new unsigned char[led_count];
    unsigned char* green_values = new unsigned char[led_count];
    unsigned char* blue_values = new unsigned char[led_count];

    for(unsigned int i = 0; i < led_count; i++)
    {
        unsigned int led_idx = start_led + i;
        red_values[i]   = RGBGetRValue(colors[led_idx]);
        green_values[i] = RGBGetGValue(colors[led_idx]);
        blue_values[i]  = RGBGetBValue(colors[led_idx]);
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

void RGBController_CreativeSoundBlasterAE5::UpdateZoneLEDs(int zone)
{
    if(zone >= 0 && zone < (int)zones.size())
    {
        unsigned int start_led = 0;

        // Calculate starting LED index for this zone
        for(int i = 0; i < zone; i++)
        {
            start_led += zones[i].leds_count;
        }

        UpdateLEDRange(start_led, zones[zone].leds_count);
    }
}

void RGBController_CreativeSoundBlasterAE5::UpdateSingleLED(int led)
{
    // Find which zone this LED belongs to and update only that zone
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
