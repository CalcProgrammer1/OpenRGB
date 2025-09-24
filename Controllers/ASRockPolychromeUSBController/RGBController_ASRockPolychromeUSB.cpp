/*---------------------------------------------------------*\
| RGBController_ASRockPolychromeUSB.cpp                     |
|                                                           |
|   RGBController for ASRock Polychrome USB motherboards    |
|                                                           |
|   Ed Kambulow (dredvard)                      20 Dec 2020 |
|   Shady Nawara (ShadyNawara)                  16 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string.h>
#include "RGBController_ASRockPolychromeUSB.h"

#define ASROCK_USB_MAX_ZONES        8
#define ASROCK_ADDRESSABLE_MAX_LEDS 100

/**------------------------------------------------------------------*\
    @name ASrock Polychrome USB
    @category Motherboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectPolychromeUSBControllers
    @comment ASRock Polychrome controllers will save with each update.
\*-------------------------------------------------------------------*/

RGBController_PolychromeUSB::RGBController_PolychromeUSB(PolychromeUSBController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    description                 = "ASRock Polychrome USB Device";
    vendor                      = "ASRock";
    type                        = DEVICE_TYPE_MOTHERBOARD;
    location                    = controller->GetDeviceLocation();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = POLYCHROME_USB_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = POLYCHROME_USB_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = POLYCHROME_USB_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = POLYCHROME_USB_SPEED_MIN;
    Breathing.speed_max         = POLYCHROME_USB_SPEED_MAX;
    Breathing.speed             = POLYCHROME_USB_SPEED_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = POLYCHROME_USB_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Strobe.speed_min            = POLYCHROME_USB_SPEED_MIN;
    Strobe.speed_max            = POLYCHROME_USB_SPEED_MAX;
    Strobe.speed                = POLYCHROME_USB_SPEED_DEFAULT;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = POLYCHROME_USB_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min     = POLYCHROME_USB_SPEED_MIN;
    SpectrumCycle.speed_max     = POLYCHROME_USB_SPEED_MAX;
    SpectrumCycle.speed         = POLYCHROME_USB_SPEED_DEFAULT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = POLYCHROME_USB_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED;
    Random.speed_min            = POLYCHROME_USB_SPEED_MIN;
    Random.speed_max            = POLYCHROME_USB_SPEED_MAX;
    Random.speed                = POLYCHROME_USB_SPEED_DEFAULT;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Music;
    Random.name                 = "Music";
    Random.value                = POLYCHROME_USB_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_BRIGHTNESS;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = POLYCHROME_USB_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED;
    Wave.speed_min              = POLYCHROME_USB_SPEED_MIN;
    Wave.speed_max              = POLYCHROME_USB_SPEED_MAX;
    Wave.speed                  = POLYCHROME_USB_SPEED_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = POLYCHROME_USB_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED;
    Spring.speed_min            = POLYCHROME_USB_SPEED_MIN;
    Spring.speed_max            = POLYCHROME_USB_SPEED_MAX;
    Spring.speed                = POLYCHROME_USB_SPEED_DEFAULT;
    Spring.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = POLYCHROME_USB_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED;
    Stack.speed_min             = POLYCHROME_USB_SPEED_MIN;
    Stack.speed_max             = POLYCHROME_USB_SPEED_MAX;
    Stack.speed                 = POLYCHROME_USB_SPEED_DEFAULT;
    Stack.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Stack);

    mode Cram;
    Cram.name                   = "Cram";
    Cram.value                  = POLYCHROME_USB_MODE_CRAM;
    Cram.flags                  = MODE_FLAG_HAS_SPEED;
    Cram.speed_min              = POLYCHROME_USB_SPEED_MIN;
    Cram.speed_max              = POLYCHROME_USB_SPEED_MAX;
    Cram.speed                  = POLYCHROME_USB_SPEED_DEFAULT;
    Cram.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Cram);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = POLYCHROME_USB_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED;
    Scan.speed_min              = POLYCHROME_USB_SPEED_MIN;
    Scan.speed_max              = POLYCHROME_USB_SPEED_MAX;
    Scan.speed                  = POLYCHROME_USB_SPEED_DEFAULT;
    Scan.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Scan);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = POLYCHROME_USB_MODE_NEON;
    Neon.flags                  = 0;
    Neon.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Neon);

    mode Water;
    Water.name                  = "Water";
    Water.value                 = POLYCHROME_USB_MODE_WATER;
    Water.flags                 = MODE_FLAG_HAS_SPEED;
    Water.speed_min             = POLYCHROME_USB_SPEED_MIN;
    Water.speed_max             = POLYCHROME_USB_SPEED_MAX;
    Water.speed                 = POLYCHROME_USB_SPEED_DEFAULT;
    Water.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = POLYCHROME_USB_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min           = POLYCHROME_USB_SPEED_MIN;
    Rainbow.speed_max           = POLYCHROME_USB_SPEED_MAX;
    Rainbow.speed               = POLYCHROME_USB_SPEED_DEFAULT;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Direct;
    Direct.name                = "Direct";
    Direct.value               = POLYCHROME_USB_MODE_DIRECT;
    Direct.flags               = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode          = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_PolychromeUSB::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();
    zones.resize(controller->GetChannelCount());

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    for(unsigned int channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        PolychromeDeviceInfo device_info = controller->GetPolychromeDevices()[channel_idx];

        zones[channel_idx].type     = ZONE_TYPE_LINEAR;

        if(device_info.device_type== PolychromeDeviceType::ADDRESSABLE)
        {
            zones[channel_idx].name       = polychrome_USB_zone_names[device_info.zone_type];
            zones[channel_idx].leds_min   = 0;
            zones[channel_idx].leds_max   = ASROCK_ADDRESSABLE_MAX_LEDS;
            zones[channel_idx].leds_count = device_info.num_leds;
        }
        else if(device_info.device_type==PolychromeDeviceType::FIXED)
        {
            zones[channel_idx].name       = polychrome_USB_zone_names[device_info.zone_type];
            zones[channel_idx].leds_min   = device_info.num_leds;
            zones[channel_idx].leds_max   = device_info.num_leds;
            zones[channel_idx].leds_count = device_info.num_leds;
        }


        for(unsigned int led_ch_idx = 0; led_ch_idx < zones[channel_idx].leds_count; led_ch_idx++)
        {
            led new_led;
            new_led.name = zones[channel_idx].name;
            new_led.name.append(", LED ");
            new_led.name.append(std::to_string(led_ch_idx + 1));
            new_led.value = channel_idx;

            leds.push_back(new_led);
        }
    }

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize zone info to track zone, speed, mode           |
    | B550 Boards have modes, speed for each zone               |
    \*---------------------------------------------------------*/
     for (unsigned int channel_idx = 0; channel_idx < zones.size(); channel_idx++)
    {
        PolychromeZoneInfo zoneinfo;
        zoneinfo = controller->GetZoneConfig(channel_idx);
        zones_info.push_back(zoneinfo);
    }

     /*---------------------------------------------------------*\
     | Initialize colors for each LED                            |
     | We cannot currently get the individual led color so       |
     | we use the zone color in each zone to set all leds        |
     | !!TODO: in Per-Led mode, zone color is always black       |
     \*---------------------------------------------------------*/
     for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
     {
         unsigned char led = (unsigned char)leds[led_idx].value;

         colors[led_idx] = zones_info[led].color;
     }

     /*-------------------------------------------------*\
     | Initialize active mode                            |
     \*-------------------------------------------------*/
     active_mode = zones_info.size() > 1 ? zones_info[0].mode : 0x0;

     /*-----------------------------------------------------*\
     | If in Direct mode, reset all Leds to match interface  |
     \*-----------------------------------------------------*/
     if(active_mode == 0x0F)
     {
        controller->WriteAllZones(zones_info, zones);
     }
}

void RGBController_PolychromeUSB::DeviceResizeZone(int zone, int new_size)
{
    zones[zone].leds_count = (unsigned char) new_size;
    controller->DeviceResizeZone(zones_info[zone].zone, new_size);
}

void RGBController_PolychromeUSB::DeviceUpdateLEDs()
{
    if(POLYCHROME_USB_MODE_DIRECT == zones_info[0].mode )
    {
        controller->WriteAllZones(zones_info,zones);
        return;
    }
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        unsigned char set_mode = zones_info[zone_idx].mode;

        if (set_mode>modes.size())
        {
            set_mode = active_mode;
        }

        controller->WriteZone((unsigned char)zone_idx, set_mode, zones_info[zone_idx].speed, zones[zone_idx].colors[0], false);
    }
}

void RGBController_PolychromeUSB::DeviceUpdateZoneLEDs(int zone)
{
    unsigned char set_mode=zones_info[zone].mode;

    if(set_mode > modes.size())
    {
        set_mode = active_mode;
    }

    controller->WriteZone(zone, set_mode, zones_info[zone].speed, zones[zone].colors[0], false);
}

void RGBController_PolychromeUSB::DeviceUpdateSingleLED(int led)
{
    unsigned int  channel  = leds[led].value;
    unsigned char set_mode = zones_info[channel].mode;

    if(set_mode > modes.size())
    {
        set_mode = active_mode;
    }

    controller->WriteZone(channel, set_mode, zones_info[channel].speed, zones[channel].colors[0], false);
}

unsigned char RGBController_PolychromeUSB::GetDeviceMode(unsigned char zone)
{
    int dev_mode;

    dev_mode    = controller->GetZoneConfig(zone).mode;
    active_mode = dev_mode;

    return(active_mode);
}

void RGBController_PolychromeUSB::DeviceUpdateMode()
{
    /*-----------------------------------------------------------------*\
    | Disable RGSwap as it causes flashing on each update in direct mode|
    | Otherwise, reset to values specified in settings.json             |
    \*-----------------------------------------------------------------*/
    controller->SetRGSwap(modes[active_mode].name != "Direct");

    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        if(zones[zone_idx].leds_count > 0)
        {
            unsigned char set_mode      =(unsigned char) modes[active_mode].value;
            zones_info[zone_idx].mode   =(unsigned char) modes[active_mode].value;
            zones_info[zone_idx].speed  =(unsigned char) modes[active_mode].speed;

            if(set_mode > modes.size())
            {
                set_mode = active_mode;
            }

            controller->WriteZone(zone_idx, set_mode, zones_info[zone_idx].speed, zones[zone_idx].colors[0], false);
        }
    }
}
