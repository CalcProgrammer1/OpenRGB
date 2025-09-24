/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion.cpp                       |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion SMBus       |
|   motherboard                                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion.h"

static const char* rgb_fusion_zone_names[] =
{
    "Motherboard",
    "RGB Header"
};

/**------------------------------------------------------------------*\
    @name Gigabyte Fusion SMBus
    @category Motherboard
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusionControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RGBFusion::RGBController_RGBFusion(RGBFusionController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Gigabyte";
    description = "RGB Fusion 1.0";
    location    = controller->GetDeviceLocation();
    type        = DEVICE_TYPE_MOTHERBOARD;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = RGB_FUSION_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = RGB_FUSION_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = RGB_FUSION_SPEED_SLOW;
    Breathing.speed_max  = RGB_FUSION_SPEED_FAST;
    Breathing.speed      = RGB_FUSION_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = RGB_FUSION_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min  = RGB_FUSION_SPEED_SLOW;
    Flashing.speed_max  = RGB_FUSION_SPEED_FAST;
    Flashing.speed      = RGB_FUSION_SPEED_NORMAL;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();

    // Initialize active mode
    active_mode = GetDeviceMode();
}

RGBController_RGBFusion::~RGBController_RGBFusion()
{
    delete controller;
}

void RGBController_RGBFusion::SetupZones()
{
    /*---------------------------------------------------------*\
    | Search through all LEDs and create zones for each channel |
    | type                                                      |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < controller->GetLEDCount(); zone_idx++)
    {
        zone* new_zone          = new zone();

        /*---------------------------------------------------------*\
        | Set zone name to channel name                             |
        \*---------------------------------------------------------*/
        new_zone->name          = rgb_fusion_zone_names[zone_idx];
        new_zone->leds_min      = 1;
        new_zone->leds_max      = 1;
        new_zone->leds_count    = 1;

        /*---------------------------------------------------------*\
        | Push new zone to zones vector                             |
        \*---------------------------------------------------------*/
        zones.push_back(*new_zone);
    }

    for(unsigned int led_idx = 0; led_idx < zones.size(); led_idx++)
    {
        led* new_led            = new led();

        /*---------------------------------------------------------*\
        | Set LED name to channel name                              |
        \*---------------------------------------------------------*/
        new_led->name           = rgb_fusion_zone_names[led_idx];

        /*---------------------------------------------------------*\
        | Push new LED to LEDs vector                               |
        \*---------------------------------------------------------*/
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_RGBFusion::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < (unsigned int)colors.size(); led++)
    {
        RGBColor      color = colors[led];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        controller->SetLEDColor(led, red, grn, blu);
    }
}

void RGBController_RGBFusion::DeviceUpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetLEDColor(zone, red, grn, blu);
}

void RGBController_RGBFusion::DeviceUpdateSingleLED(int led)
{
    DeviceUpdateZoneLEDs(led);
}

int RGBController_RGBFusion::GetDeviceMode()
{
    int dev_mode = controller->GetMode();

    for(int mode = 0; mode < (int)modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            return(mode);
        }
    }

    return(0);
}

void RGBController_RGBFusion::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed);
}
