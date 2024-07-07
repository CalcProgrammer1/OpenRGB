/*---------------------------------------------------------*\
| RGBController_ASRockPolychromeV2SMBus.cpp                 |
|                                                           |
|   RGBController for SMBus ASRock Polychrome V2            |
|   motherboards                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ASRockPolychromeV2SMBus.h"

static const char* polychrome_v2_zone_names[] =
{
    "RGB LED 1 Header",
    "RGB LED 2 Header",
    "Audio",
    "PCH",
    "IO Cover",
    "Addressable Header"
};

/**------------------------------------------------------------------*\
    @name ASRock Polychrome v2 SMBus
    @category Motherboard
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectASRockSMBusControllers
    @comment ASRock Polychrome v2 controllers will save with each update.
        Per ARGB LED support is not possible with these devices.
\*-------------------------------------------------------------------*/

RGBController_ASRockPolychromeV2SMBus::RGBController_ASRockPolychromeV2SMBus(ASRockPolychromeV2SMBusController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "ASRock";
    version                     = controller->GetFirmwareVersion();
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = "ASRock Polychrome v2 Device";
    location                    = controller->GetDeviceLocation();


    mode Off;
    Off.name                    = "Off";
    Off.value                   = POLYCHROME_V2_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = POLYCHROME_V2_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = POLYCHROME_V2_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min         = POLYCHROME_V2_BREATHING_SPEED_MIN;
    Breathing.speed_max         = POLYCHROME_V2_BREATHING_SPEED_MAX;
    Breathing.speed             = POLYCHROME_V2_BREATHING_SPEED_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = POLYCHROME_V2_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Strobe.speed_min            = POLYCHROME_V2_STROBE_SPEED_MIN;
    Strobe.speed_max            = POLYCHROME_V2_STROBE_SPEED_MAX;
    Strobe.speed                = POLYCHROME_V2_STROBE_SPEED_DEFAULT;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = POLYCHROME_V2_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min     = POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_MIN;
    SpectrumCycle.speed_max     = POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_MAX;
    SpectrumCycle.speed         = POLYCHROME_V2_SPECTRUM_CYCLE_SPEED_DEFAULT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = POLYCHROME_V2_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Random.speed_min            = POLYCHROME_V2_RANDOM_SPEED_MIN;
    Random.speed_max            = POLYCHROME_V2_RANDOM_SPEED_MAX;
    Random.speed                = POLYCHROME_V2_RANDOM_SPEED_DEFAULT;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = POLYCHROME_V2_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min              = POLYCHROME_V2_WAVE_SPEED_MIN;
    Wave.speed_max              = POLYCHROME_V2_WAVE_SPEED_MAX;
    Wave.speed                  = POLYCHROME_V2_WAVE_SPEED_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = POLYCHROME_V2_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Spring.speed_min            = POLYCHROME_V2_SPRING_SPEED_MIN;
    Spring.speed_max            = POLYCHROME_V2_SPRING_SPEED_MAX;
    Spring.speed                = POLYCHROME_V2_SPRING_SPEED_DEFAULT;
    Spring.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = POLYCHROME_V2_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Stack.speed_min             = POLYCHROME_V2_STACK_SPEED_MIN;
    Stack.speed_max             = POLYCHROME_V2_STACK_SPEED_MAX;
    Stack.speed                 = POLYCHROME_V2_STACK_SPEED_DEFAULT;
    Stack.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Stack);

    mode Cram;
    Cram.name                   = "Cram";
    Cram.value                  = POLYCHROME_V2_MODE_CRAM;
    Cram.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Cram.speed_min              = POLYCHROME_V2_CRAM_SPEED_MIN;
    Cram.speed_max              = POLYCHROME_V2_CRAM_SPEED_MAX;
    Cram.speed                  = POLYCHROME_V2_CRAM_SPEED_DEFAULT;
    Cram.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Cram);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = POLYCHROME_V2_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Scan.speed_min              = POLYCHROME_V2_SCAN_SPEED_MIN;
    Scan.speed_max              = POLYCHROME_V2_SCAN_SPEED_MAX;
    Scan.speed                  = POLYCHROME_V2_SCAN_SPEED_DEFAULT;
    Scan.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Scan);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = POLYCHROME_V2_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Neon.speed_min              = POLYCHROME_V2_NEON_SPEED_MIN;
    Neon.speed_max              = POLYCHROME_V2_NEON_SPEED_MAX;
    Neon.speed                  = POLYCHROME_V2_NEON_SPEED_DEFAULT;
    Neon.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Neon);

    mode Water;
    Water.name                  = "Water";
    Water.value                 = POLYCHROME_V2_MODE_WATER;
    Water.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Water.speed_min             = POLYCHROME_V2_WATER_SPEED_MIN;
    Water.speed_max             = POLYCHROME_V2_WATER_SPEED_MAX;
    Water.speed                 = POLYCHROME_V2_WATER_SPEED_DEFAULT;
    Water.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = POLYCHROME_V2_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.speed_min           = POLYCHROME_V2_RAINBOW_SPEED_MIN;
    Rainbow.speed_max           = POLYCHROME_V2_RAINBOW_SPEED_MAX;
    Rainbow.speed               = POLYCHROME_V2_RAINBOW_SPEED_DEFAULT;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    SetupZones();
}

RGBController_ASRockPolychromeV2SMBus::~RGBController_ASRockPolychromeV2SMBus()
{
    delete controller;
}

void RGBController_ASRockPolychromeV2SMBus::SetupZones()
{

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < POLYCHROME_V2_ZONE_COUNT; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            zone* new_zone = new zone();

            /*---------------------------------------------------------*\
            | Set zone name to channel name                             |
            \*---------------------------------------------------------*/

            new_zone->name          = polychrome_v2_zone_names[zone_idx];


            if(zone_idx == POLYCHROME_V2_ZONE_ADDRESSABLE)
            {
                new_zone->leds_min      = 1;
                new_zone->leds_max      = 1;
                new_zone->leds_count    = 1;
            }
            else
            {
                new_zone->leds_min      = controller->zone_led_count[zone_idx];
                new_zone->leds_max      = controller->zone_led_count[zone_idx];
                new_zone->leds_count    = controller->zone_led_count[zone_idx];
            }

            if(new_zone->leds_count > 1)
            {
                new_zone->type          = ZONE_TYPE_LINEAR;
            }
            else
            {
                new_zone->type          = ZONE_TYPE_SINGLE;
            }

            new_zone->matrix_map        = NULL;

            /*---------------------------------------------------------*\
            | Push new zone to zones vector                             |
            \*---------------------------------------------------------*/
            zones.push_back(*new_zone);
        }
    }

    unsigned int led_count = 0;
    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < POLYCHROME_V2_ZONE_COUNT; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            for(unsigned int led_idx = 0; led_idx < controller->zone_led_count[zone_idx]; led_idx++)
            {
                /*---------------------------------------------------------*\
                | Each zone only has one LED                                |
                \*---------------------------------------------------------*/
                led* new_led = new led();

                new_led->name           = polychrome_v2_zone_names[zone_idx];

                new_led->name.append(" " + std::to_string(led_idx + 1));
                new_led->value = 0;

                if(zone_idx == POLYCHROME_V2_ZONE_ADDRESSABLE)
                {
                    new_led->value              = 0x19;
                }

                /*---------------------------------------------------------*\
                | Push new LED to LEDs vector                               |
                \*---------------------------------------------------------*/
                leds.push_back(*new_led);

                led_count++;

                if(zone_idx == POLYCHROME_V2_ZONE_ADDRESSABLE)
                {
                    break;
                }
            }
        }
    }

    SetupColors();
}

void RGBController_ASRockPolychromeV2SMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ASRockPolychromeV2SMBus::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED(led);
    }
}

void RGBController_ASRockPolychromeV2SMBus::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ASRockPolychromeV2SMBus::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    /*---------------------------------------------------------*\
    | If the LED value is non-zero, this LED overrides the LED  |
    | index                                                     |
    \*---------------------------------------------------------*/
    if(leds[led].value != 0)
    {
        led = leds[led].value;
    }

    controller->SetColorsAndSpeed(led, red, grn, blu);
}

void RGBController_ASRockPolychromeV2SMBus::DeviceUpdateMode()
{

    controller->SetMode(0, modes[active_mode].value, modes[active_mode].speed);

    DeviceUpdateLEDs();
}
