/*---------------------------------------------------------*\
| RGBController_ASRockPolychromeV1SMBus.cpp                 |
|                                                           |
|   RGBController for SMBus ASRock Polychrome V1            |
|   motherboards                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "RGBController_ASRockPolychromeV1SMBus.h"

static const char* polychrome_v1_zone_names[] =
{
    "RGB LED 1 Header",
    "RGB LED 2 Header",
    "PCH",
    "IO Cover",
    "Audio",
    "Addressable Header"
};

/**------------------------------------------------------------------*\
    @name ASRock Polychrome v1 SMBus
    @category Motherboard
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectASRockSMBusControllers
    @comment ASRock Polychrome v1 controllers will save with each update.
        Per ARGB LED support is not possible with these devices.
        ARGB size  and color order is set using the `ARGB Header Config mode`
        `Right` = GRB mode that is needed for WS2812B ARGB devices and `Left` = RGB used for WS2811 strips
        The modes speed slider will set the size of the header
        Spectrum Cycles uses the RGB values to set the individual color brightness.
\*-------------------------------------------------------------------*/

RGBController_ASRockPolychromeV1SMBus::RGBController_ASRockPolychromeV1SMBus(ASRockPolychromeV1SMBusController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "ASRock";
    version                     = controller->GetFirmwareVersion();
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = "ASRock Polychrome v1 Device";
    location                    = controller->GetDeviceLocation();


    mode Off;
    Off.name                    = "Off";
    Off.value                   = POLYCHROME_V1_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = POLYCHROME_V1_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = POLYCHROME_V1_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min         = POLYCHROME_V1_SPEED_MIN_BREATHING;
    Breathing.speed_max         = POLYCHROME_V1_SPEED_MAX_BREATHING;
    Breathing.speed             = POLYCHROME_V1_SPEED_DEFAULT_BREATHING;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = POLYCHROME_V1_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Strobe.speed_min            = POLYCHROME_V1_SPEED_MIN_STROBE;
    Strobe.speed_max            = POLYCHROME_V1_SPEED_MAX_STROBE;
    Strobe.speed                = POLYCHROME_V1_SPEED_DEFAULT_STROBE;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = POLYCHROME_V1_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min     = POLYCHROME_V1_SPEED_MIN_CYCLE;
    SpectrumCycle.speed_max     = POLYCHROME_V1_SPEED_MAX_CYCLE;
    SpectrumCycle.speed         = POLYCHROME_V1_SPEED_DEFAULT_CYCLE;
    SpectrumCycle.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = POLYCHROME_V1_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Random.speed_min            = POLYCHROME_V1_SPEED_MIN_RANDOM;
    Random.speed_max            = POLYCHROME_V1_SPEED_MAX_RANDOM;
    Random.speed                = POLYCHROME_V1_SPEED_DEFAULT_RANDOM;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Music;
    Music.name                  = "Music";
    Music.value                 = POLYCHROME_V1_MODE_MUSIC;
    Music.flags                 = MODE_FLAG_AUTOMATIC_SAVE;
    Music.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Music);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = POLYCHROME_V1_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min              = POLYCHROME_V1_SPEED_MIN_WAVE;
    Wave.speed_max              = POLYCHROME_V1_SPEED_MAX_WAVE;
    Wave.speed                  = POLYCHROME_V1_SPEED_DEFAULT_WAVE;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    /*---------------------------------------------------------------------*\
    | Comment out until per zone modes are working. These are only for ARGB |
    \*---------------------------------------------------------------------*/
    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = POLYCHROME_V1_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Spring.speed_min            = POLYCHROME_V1_SPEED_MIN_ARGB;
    Spring.speed_max            = POLYCHROME_V1_SPEED_MAX_ARGB;
    Spring.speed                = POLYCHROME_V1_SPEED_DEFAULT_SPRING;
    Spring.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = POLYCHROME_V1_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Stack.speed_min             = POLYCHROME_V1_SPEED_MIN_ARGB;
    Stack.speed_max             = POLYCHROME_V1_SPEED_MAX_ARGB;
    Stack.speed                 = POLYCHROME_V1_SPEED_DEFAULT_STACK;
    Stack.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Cram;
    Cram.name                   = "Cram";
    Cram.value                  = POLYCHROME_V1_MODE_CRAM;
    Cram.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Cram.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Cram.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Cram.speed                  = POLYCHROME_V1_SPEED_DEFAULT_CRAM;
    Cram.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Cram);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = POLYCHROME_V1_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Scan.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Scan.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Scan.speed                  = POLYCHROME_V1_SPEED_DEFAULT_SCAN;
    Scan.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Scan);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = POLYCHROME_V1_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Neon.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Neon.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Neon.speed                  = POLYCHROME_V1_SPEED_DEFAULT_NEON;
    Neon.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Neon);

    mode Water;
    Water.name                  = "Water";
    Water.value                 = POLYCHROME_V1_MODE_WATER;
    Water.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Water.speed_min             = POLYCHROME_V1_SPEED_MIN_ARGB;
    Water.speed_max             = POLYCHROME_V1_SPEED_MAX_ARGB;
    Water.speed                 = POLYCHROME_V1_SPEED_DEFAULT_WATER;
    Water.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = POLYCHROME_V1_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.speed_min           = POLYCHROME_V1_SPEED_MIN_RAINBOW;
    Rainbow.speed_max           = POLYCHROME_V1_SPEED_MAX_RAINBOW;
    Rainbow.speed               = POLYCHROME_V1_SPEED_DEFAULT_RAINBOW;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    /*---------------------------------------------------------------------*\
    | This ARGB_Config section is a hack to allow users to configure the    |
    | RGB vs GRB mode using the direction of the mode as well as set the    |
    | size of the devices connected to the header using the brightness      |
    | value.                                                                |
    |                                                                       |
    | The mode should be removed onece the device settings are avaiable.    |
    \*---------------------------------------------------------------------*/
    mode ARGB_Config;
    ARGB_Config.name            = "ARGB Header Config";
    ARGB_Config.value           = POLYCHROME_V1_REG_ARGB_GRB;
    ARGB_Config.flags           = MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_SPEED  | MODE_FLAG_AUTOMATIC_SAVE;
    ARGB_Config.speed           = controller->zone_led_count[POLYCHROME_V1_ZONE_ADDRESSABLE];
    ARGB_Config.speed_min       = 1;
    ARGB_Config.speed_max       = POLYCHROME_V1_ZONE_ADDRESSABLE_MAX;
    ARGB_Config.direction       = controller -> GetARGBColorOrder();
    ARGB_Config.color_mode      = MODE_COLORS_NONE;
    modes.push_back(ARGB_Config);

    SetupZones();

    controller->LoadZoneConfig();
    active_mode = getModeIndex(controller->zone_config[0].mode); // Hard coding zone 0 until per zone modes are available.

    if(active_mode != POLYCHROME_V1_MODE_OFF)
    {
        for( uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
        {
            zones[zone_idx].colors[0] = controller->GetZoneColor(zoneIndexMap[zone_idx]);
        }
    }
}

RGBController_ASRockPolychromeV1SMBus::~RGBController_ASRockPolychromeV1SMBus()
{
    delete controller;
}

uint8_t RGBController_ASRockPolychromeV1SMBus::getModeIndex(uint8_t mode_value)
{
    for(uint8_t mode_index = 0; mode_index < modes.size(); mode_index++)
    {
        if (modes[mode_index].value == mode_value)
        {
            return mode_index;
        }
    }
    return 0;
}

void RGBController_ASRockPolychromeV1SMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Polychrome motherboards should set up zones based on LED  |
    | configuration register read from device                   |
    \*---------------------------------------------------------*/
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < POLYCHROME_V1_ZONE_COUNT; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            zone*   new_zone        = new zone();
            /*---------------------------------------------------------*\
            | Set zone name to channel name                             |
            \*---------------------------------------------------------*/
            new_zone->name          = polychrome_v1_zone_names[zone_idx];
            new_zone->type          = ZONE_TYPE_SINGLE;
            new_zone->leds_min      = 1;
            new_zone->leds_max      = 1;
            new_zone->leds_count    = 1;
            new_zone->matrix_map    = NULL;

            if(zone_idx == POLYCHROME_V1_ZONE_ADDRESSABLE)
            {
                new_zone->leds_max  = POLYCHROME_V1_ZONE_ADDRESSABLE_MAX;
            }
            /*---------------------------------------------------------*\
            | Push new zone to zones vector                             |
            \*---------------------------------------------------------*/
            zones.push_back(*new_zone);

            /*---------------------------------------------------------*\
            | Each zone only has one LED                                |
            \*---------------------------------------------------------*/
            led*    new_led         = new led();

            new_led->name           = polychrome_v1_zone_names[zone_idx];
            new_led->value          = zone_idx;

            /*---------------------------------------------------------*\
            | Push new LED to LEDs vector                               |
            \*---------------------------------------------------------*/
            leds.push_back(*new_led);
            zoneIndexMap.push_back(zone_idx);
        }
    }

    SetupColors();
}

void RGBController_ASRockPolychromeV1SMBus::ResizeZone(int zone, int new_size)
{
    LOG_TRACE("[%s] ResizeZone(%02X, %02X)", name.c_str(), zone, new_size);
    controller-> SetARGBSize(new_size & 0xFF);
    zones[POLYCHROME_V1_ZONE_ADDRESSABLE].leds_count = 1;
}

void RGBController_ASRockPolychromeV1SMBus::DeviceUpdateLEDs()
{
    LOG_TRACE("[%s] DeviceUpdateLEDs()", name.c_str());
    for (uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
    {
        UpdateSingleLED(zone_idx);
    }
}

void RGBController_ASRockPolychromeV1SMBus::UpdateZoneLEDs(int /*zone*/)
{
    LOG_TRACE("[%s] UpdateZoneLEDs()", name.c_str());
    DeviceUpdateLEDs();
}

void RGBController_ASRockPolychromeV1SMBus::UpdateSingleLED(int zone)
{
    LOG_TRACE("[%s] UpdateSingleLED(%02X)", name.c_str(), zone);

    uint8_t red = RGBGetRValue(colors[zone]);
    uint8_t grn = RGBGetGValue(colors[zone]);
    uint8_t blu = RGBGetBValue(colors[zone]);

    controller->SetColorsAndSpeed(zoneIndexMap[zone], red, grn, blu);
}

void RGBController_ASRockPolychromeV1SMBus::DeviceUpdateMode()
{
    LOG_TRACE("[%s] DeviceUpdateMode()", name.c_str());
    if(modes[active_mode].value != POLYCHROME_V1_REG_ARGB_GRB)
    {
        for(uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
        {
            controller->SetMode(zoneIndexMap[zone_idx], modes[active_mode].value, modes[active_mode].speed);
            UpdateSingleLED(zone_idx);
        }
    }
    else
    {
        controller-> SetARGBColorOrder(modes[active_mode].direction);
        controller-> SetARGBSize(modes[active_mode].speed);
    }
}
