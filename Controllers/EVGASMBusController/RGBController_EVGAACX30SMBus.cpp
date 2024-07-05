/*---------------------------------------------------------*\
| RGBController_EVGAACX30SMBus.cpp                          |
|                                                           |
|   RGBController for SMBus EVGA ACX 30 motherboards        |
|                                                           |
|   Balázs Triszka (balika011)                  21 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_EVGAACX30SMBus.h"

/**------------------------------------------------------------------*\
    @name EVGA ACX 30
    @category Motherboard
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectAcx30SMBusControllers
    @comment EVGA ACX 30 LED controllers will save with each update.
        Per ARGB LED support is not possible with these devices.
\*-------------------------------------------------------------------*/

RGBController_EVGAACX30SMBus::RGBController_EVGAACX30SMBus(EVGAACX30SMBusController *controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "EVGA";
    version                     = controller->GetFirmwareVersion();
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = "EVGA ACX 30 LED Device";
    location                    = controller->GetDeviceLocation();
    active_mode                 = controller->GetMode();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ACX30_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ACX30_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = ACX30_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min     = ACX30_SPEED_MIN;
    SpectrumCycle.speed_max     = ACX30_SPEED_MAX;
    SpectrumCycle.speed         = ACX30_SPEED_DEFAULT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ACX30_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min         = ACX30_SPEED_MIN;
    Breathing.speed_max         = ACX30_SPEED_MAX;
    Breathing.speed             = ACX30_SPEED_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_EVGAACX30SMBus::~RGBController_EVGAACX30SMBus()
{
    delete controller;
}

void RGBController_EVGAACX30SMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Acx30 motherboards only have a single zone/LED            |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();

    /*---------------------------------------------------------*\
    | Set single zone name to "Motherboard"                     |
    \*---------------------------------------------------------*/
    new_zone->name          = "Motherboard";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    /*---------------------------------------------------------*\
    | Push new zone to zones vector                             |
    \*---------------------------------------------------------*/
    zones.push_back(*new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    led* new_led = new led();

    /*---------------------------------------------------------*\
    | Set single LED name to "Motherboard"                      |
    \*---------------------------------------------------------*/
    new_led->name           = "Motherboard";

    /*---------------------------------------------------------*\
    | Push new LED to LEDs vector                               |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);

    SetupColors();
}

void RGBController_EVGAACX30SMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAACX30SMBus::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED(led);
    }
}

void RGBController_EVGAACX30SMBus::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAACX30SMBus::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->Unlock();
    controller->SetColors(red, grn, blu);
    controller->Lock();
}

void RGBController_EVGAACX30SMBus::DeviceUpdateMode()
{
    controller->Unlock();
    controller->SetMode(modes[active_mode].value);
    controller->SetSpeed(modes[active_mode].speed);
    controller->Lock();

    DeviceUpdateLEDs();
}
