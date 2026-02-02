/*---------------------------------------------------------*\
| RGBController_AsusAuraUSB.cpp                             |
|                                                           |
|   RGBController for ASUS Aura USB device                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraUSB.h"

/**------------------------------------------------------------------*\
    @name Asus Aura USB
    @category Motherboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBTerminal,DetectAsusAuraUSBAddressable
    @comment The Asus Aura USB controller applies to most AMD and
        Intel mainboards from the x470 and z390 chipset generations.
\*-------------------------------------------------------------------*/

RGBController_AuraUSB::RGBController_AuraUSB(AuraUSBController* controller_ptr) :
    initializedMode(false)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = "ASUS Aura USB Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();
    version                     = controller->GetDeviceVersion();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = AURA_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = AURA_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = AURA_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = AURA_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = AURA_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors.resize(1);
    modes.push_back(Flashing);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = AURA_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = 0;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = AURA_MODE_RAINBOW;
    Rainbow.flags               = 0;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode ChaseFade;
    ChaseFade.name              = "Chase Fade";
    ChaseFade.value             = AURA_MODE_CHASE_FADE;
    ChaseFade.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    ChaseFade.colors_min        = 1;
    ChaseFade.colors_max        = 1;
    ChaseFade.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    ChaseFade.colors.resize(1);
    modes.push_back(ChaseFade);

    mode Chase;
    Chase.name                  = "Chase";
    Chase.value                 = AURA_MODE_CHASE;
    Chase.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Chase.colors_min            = 1;
    Chase.colors_max            = 1;
    Chase.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Chase.colors.resize(1);
    modes.push_back(Chase);

    SetupZones();
}

RGBController_AuraUSB::~RGBController_AuraUSB()
{
    Shutdown();

    delete controller;
}

void RGBController_AuraUSB::SetupZones()
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
    zones.resize(controller->GetChannelCount());

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    int addressableCounter = 1;
    for (unsigned int channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        AuraDeviceInfo device_info  = controller->GetAuraDevices()[channel_idx];

        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        if(device_info.device_type == AuraDeviceType::FIXED)
        {
            zones[channel_idx].name       = "Aura Mainboard";
            zones[channel_idx].leds_min   = device_info.num_leds;
            zones[channel_idx].leds_max   = device_info.num_leds;
            zones[channel_idx].leds_count = device_info.num_leds;
        }
        else
        {
            zones[channel_idx].name       = "Aura Addressable ";
            zones[channel_idx].name.append(std::to_string(addressableCounter));
            zones[channel_idx].leds_min   = 0;
            zones[channel_idx].leds_max   = AURA_ADDRESSABLE_MAX_LEDS;

            addressableCounter++;

            if(first_run)
            {
                zones[channel_idx].leds_count = 0;
            }
        }

        unsigned int num_mainboard_leds = device_info.num_leds - device_info.num_headers;
        for (unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            unsigned led_idx = led_ch_idx + 1;
            led new_led;

            new_led.name = zones[channel_idx].name;
            if(device_info.device_type == AuraDeviceType::FIXED && led_ch_idx >= num_mainboard_leds)
            {
                new_led.name.append(", RGB Header ");
                led_idx -= num_mainboard_leds;
            }
            else
            {
                new_led.name.append(", LED ");
            }
            new_led.name.append(std::to_string(led_idx));

            new_led.value = channel_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_AuraUSB::DeviceResizeZone(int zone, int new_size)
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

void RGBController_AuraUSB::DeviceUpdateLEDs()
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        controller->SetChannelLEDs((unsigned char)zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_AuraUSB::DeviceUpdateZoneLEDs(int zone)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }

    controller->SetChannelLEDs(zone, zones[zone].colors, zones[zone].leds_count);
}

void RGBController_AuraUSB::DeviceUpdateSingleLED(int led)
{
    if(!initializedMode)
    {
        DeviceUpdateMode();
    }

    unsigned int channel = leds[led].value;

    controller->SetChannelLEDs(channel, zones[channel].colors, zones[channel].leds_count);
}

void RGBController_AuraUSB::DeviceUpdateMode()
{
    initializedMode   = true;
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count > 0)
        {
            controller->SetMode(zone_idx, modes[active_mode].value, red, grn, blu);
        }
    }
}
