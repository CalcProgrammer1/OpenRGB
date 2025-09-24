/*---------------------------------------------------------*\
| RGBController_Seagate.cpp                                 |
|                                                           |
|   RGBController for Seagate                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_Seagate.h"

/**------------------------------------------------------------------*\
    @name Seagate
    @category Storage
    @type SCSI
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSeagateControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Seagate::RGBController_Seagate(SeagateController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Seagate Device";
    vendor      = "Seagate";
    type        = DEVICE_TYPE_STORAGE;
    description = "Seagate Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = SEAGATE_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Blink;
    Blink.name              = "Flashing";
    Blink.value             = SEAGATE_MODE_BLINK;
    Blink.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Blink.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = SEAGATE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Spectrum;
    Spectrum.name           = "Spectrum Cycle";
    Spectrum.value          = SEAGATE_MODE_SPECTRUM;
    Spectrum.flags          = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Spectrum.color_mode     = MODE_COLORS_RANDOM;
    modes.push_back(Spectrum);

    SetupZones();
}

RGBController_Seagate::~RGBController_Seagate()
{
    delete controller;
}

void RGBController_Seagate::SetupZones()
{
    zone led_zone;
    led_zone.name       = "LED Strip";
    led_zone.type       = ZONE_TYPE_LINEAR;
    led_zone.leds_min   = 6;
    led_zone.leds_max   = 6;
    led_zone.leds_count = 6;
    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name        = "LED Strip LED";

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_Seagate::DeviceUpdateLEDs()
{
    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        DeviceUpdateSingleLED(led_idx);
    }
}

void RGBController_Seagate::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Seagate::DeviceUpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    switch(modes[active_mode].value)
    {
        case SEAGATE_MODE_STATIC:
            controller->SetLEDStatic(led, red, grn, blu, false);
            break;

        case SEAGATE_MODE_BLINK:
            controller->SetLEDBlink(led, red, grn, blu, false);
            break;

        case SEAGATE_MODE_BREATHING:
            controller->SetLEDBreathing(led, red, grn, blu, false);
            break;

        case SEAGATE_MODE_SPECTRUM:
            controller->SetLEDsSpectrum(led, false);
            break;
    }
}

void RGBController_Seagate::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_Seagate::DeviceSaveMode()
{
    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        switch(modes[active_mode].value)
        {
            case SEAGATE_MODE_STATIC:
                controller->SetLEDStatic(led_idx, red, grn, blu, true);
                break;

            case SEAGATE_MODE_BLINK:
                controller->SetLEDBlink(led_idx, red, grn, blu, true);
                break;

            case SEAGATE_MODE_BREATHING:
                controller->SetLEDBreathing(led_idx, red, grn, blu, true);
                break;

            case SEAGATE_MODE_SPECTRUM:
                controller->SetLEDsSpectrum(led_idx, true);
                break;
        }
    }
}
