/*---------------------------------------------------------*\
| RGBController_EVGAGPUv3.cpp                               |
|                                                           |
|   RGBController for EVGA V3 (Ampere) GPU                  |
|                                                           |
|   TheRogueZeta                                15 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_EVGAGPUv3.h"
#include "LogManager.h"

static const char* evga_v3_zone_names[] =
{
    "Front Logo",
    "End plate Logo",
    "Back Logo",
    "Addressable Header"
};

/**------------------------------------------------------------------*\
    @name EVGA RGB v3 GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectEVGAAmpereGPUControllers
    @comment EVGA has not exposed a per LED control method yet so OpenRGB
        is only able to set all LED's to a single color.
\*-------------------------------------------------------------------*/

RGBController_EVGAGPUv3::RGBController_EVGAGPUv3(EVGAGPUv3Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "EVGA";
    description                 = "EVGA Ampere RGB GPU Device";
    location                    = controller->GetDeviceLocation();
    version                     = controller->GetFWVersion();
    type                        = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                    = "Off";
    Off.value                   = EVGA_GPU_V3_MODE_OFF;
    Off.flags                   = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Direct";
    Static.value                = EVGA_GPU_V3_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.brightness_min       = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Static.brightness           = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Static.brightness_max       = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = EVGA_GPU_V3_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min         = EVGA_GPU_V3_SPEED_GENERIC_SLOWEST;
    Breathing.speed             = EVGA_GPU_V3_SPEED_GENERIC_NORMAL;
    Breathing.speed_max         = EVGA_GPU_V3_SPEED_GENERIC_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(1);
    Breathing.brightness_min    = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Breathing.brightness        = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Breathing.brightness_max    = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name                = "Spectrum Cycle";
    Rainbow.value               = EVGA_GPU_V3_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min           = EVGA_GPU_V3_SPEED_GENERIC_SLOWEST;
    Rainbow.speed               = EVGA_GPU_V3_SPEED_GENERIC_NORMAL;
    Rainbow.speed_max           = EVGA_GPU_V3_SPEED_GENERIC_FASTEST;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.brightness_min      = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Rainbow.brightness          = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Rainbow.brightness_max      = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Rainbow);

    mode Color_Cycle;
    Color_Cycle.name            = "Color Cycle";
    Color_Cycle.value           = EVGA_GPU_V3_MODE_COLOR_CYCLE;
    Color_Cycle.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Color_Cycle.speed_min       = EVGA_GPU_V3_SPEED_GENERIC_SLOWEST;
    Color_Cycle.speed           = EVGA_GPU_V3_SPEED_GENERIC_NORMAL;
    Color_Cycle.speed_max       = EVGA_GPU_V3_SPEED_GENERIC_FASTEST;
    Color_Cycle.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Color_Cycle.colors_min      = 2;
    Color_Cycle.colors_max      = 7;
    Color_Cycle.colors.resize(2);
    Color_Cycle.brightness_min  = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Color_Cycle.brightness      = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Color_Cycle.brightness_max  = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Color_Cycle);

    mode Rainbow_Wave;
    Rainbow_Wave.name           = "Rainbow Wave";
    Rainbow_Wave.value          = EVGA_GPU_V3_MODE_RAINBOW_WAVE;
    Rainbow_Wave.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Rainbow_Wave.speed_min      = EVGA_GPU_V3_SPEED_GENERIC_SLOWEST;
    Rainbow_Wave.speed          = EVGA_GPU_V3_SPEED_GENERIC_NORMAL;
    Rainbow_Wave.speed_max      = EVGA_GPU_V3_SPEED_GENERIC_FASTEST;
    Rainbow_Wave.color_mode     = MODE_COLORS_NONE;
    Rainbow_Wave.brightness_min = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Rainbow_Wave.brightness     = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Rainbow_Wave.brightness_max = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Rainbow_Wave);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = EVGA_GPU_V3_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Wave.speed_min              = EVGA_GPU_V3_SPEED_WAVE_SLOWEST;
    Wave.speed                  = EVGA_GPU_V3_SPEED_WAVE_NORMAL;
    Wave.speed_max              = EVGA_GPU_V3_SPEED_WAVE_FASTEST;
    Wave.color_mode             = MODE_COLORS_PER_LED;
    Wave.brightness_min         = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Wave.brightness             = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Wave.brightness_max         = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Wave);

    mode Star;
    Star.name                   = "Star";
    Star.value                  = EVGA_GPU_V3_MODE_STAR;
    Star.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Star.speed_min              = EVGA_GPU_V3_SPEED_STAR_SLOWEST;
    Star.speed                  = EVGA_GPU_V3_SPEED_STAR_NORMAL;
    Star.speed_max              = EVGA_GPU_V3_SPEED_STAR_FASTEST;
    Star.color_mode             = MODE_COLORS_NONE;
    Star.brightness_min         = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Star.brightness             = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Star.brightness_max         = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Star);

    mode Color_Stack;
    Color_Stack.name            = "Color Stack";
    Color_Stack.value           = EVGA_GPU_V3_MODE_COLOR_STACK;
    Color_Stack.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Color_Stack.speed_min       = EVGA_GPU_V3_SPEED_WAVE_SLOWEST;
    Color_Stack.speed           = EVGA_GPU_V3_SPEED_WAVE_NORMAL;
    Color_Stack.speed_max       = EVGA_GPU_V3_SPEED_WAVE_FASTEST;
    Color_Stack.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    Color_Stack.colors_min      = 2;
    Color_Stack.colors_max      = 7;
    Color_Stack.colors.resize(2);
    Color_Stack.brightness_min  = EVGA_GPU_V3_BRIGHTNESS_MIN;
    Color_Stack.brightness      = EVGA_GPU_V3_BRIGHTNESS_DEFAULT;
    Color_Stack.brightness_max  = EVGA_GPU_V3_BRIGHTNESS_MAX;
    modes.push_back(Color_Stack);


    SetupZones();

    // Initialize active mode
    for( uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
    {
        active_mode = controller->GetZoneMode(0); // Hard coding zone 0 until per zone modes are available.

        if(active_mode != EVGA_GPU_V3_MODE_OFF)
        {
            EVGAv3_config hw_config = controller->GetZoneConfig(zoneIndexMap[zone_idx], active_mode);

            /*---------------------------------------------------------*\
            | The LED color (color[0]) will always be set. Mode colors  |
            | are only set for the MODE_COLORS_MODE_SPECIFIC modes      |
            \*---------------------------------------------------------*/

            zones[zone_idx].colors[0]           = hw_config.colors[0];

            if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && zone_idx == 0) // Hard coding zone 0 until per zone modes are available.
            {
                for( uint8_t j = 0 ; j < hw_config.numberOfColors; j ++)
                {
                    if(modes[active_mode].colors.size() > j)
                    {
                        modes[active_mode].colors[j] = hw_config.colors[j];
                    }
                    else
                    {
                        modes[active_mode].colors.push_back(hw_config.colors[j]);
                    }
                }
            }

            modes[active_mode].speed        = hw_config.speed;
            modes[active_mode].brightness   = hw_config.brightness;
            modes[active_mode].direction    = hw_config.direction;
        }
    }
}

RGBController_EVGAGPUv3::~RGBController_EVGAGPUv3()
{
    delete controller;
}

uint8_t RGBController_EVGAGPUv3::getModeIndex(uint8_t mode_value)
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

void RGBController_EVGAGPUv3::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only allows setting the entire zone for all   |
    | LED's in the zone and does not allow per LED control.     |
    | Resizing is only possible on zone 4, addressable header   |
    \*---------------------------------------------------------*/

    controller->GetDeviceModes();

    for(uint8_t zone_idx = 0; zone_idx < 4; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            zone* new_zone = new zone();
            led*  new_led  = new led();

            new_zone->name          = evga_v3_zone_names[zone_idx];
            new_zone->type          = ZONE_TYPE_SINGLE;
            new_zone->leds_min      = 1;
            new_zone->leds_max      = 1;
            new_zone->leds_count    = 1;
            new_zone->matrix_map    = NULL;

            new_led->name           = evga_v3_zone_names[zone_idx];
            /*---------------------------------------------------------*\
            | Push the zone and LED on to device vectors                |
            \*---------------------------------------------------------*/
            leds.push_back(*new_led);
            zones.push_back(*new_zone);
            zoneIndexMap.push_back(zone_idx);
        }
    }
    SetupColors();
}

void RGBController_EVGAGPUv3::ResizeZone(int /*zone*/, int newSize)
{
    controller->ResizeARGB(newSize);
}

void RGBController_EVGAGPUv3::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | DeviceUpdateLEDs() is only used in MODE_COLORS_PER_LED    |
    | modes and as such colorB will always be black (0x000000)  |
    \*---------------------------------------------------------*/
    EVGAv3_config zone_config;

    zone_config.brightness      = modes[active_mode].brightness;
    zone_config.speed           = modes[active_mode].speed;
    zone_config.direction       = modes[active_mode].direction;
    zone_config.numberOfColors  = (uint8_t) modes[active_mode].colors.size();

    for(uint8_t zone_idx = 0; zone_idx < zoneIndexMap.size(); zone_idx++)
    {
        zone_config.colors[0] = colors[zone_idx];

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            for( uint8_t i = 0 ; i < zone_config.numberOfColors; i ++)
            {
                zone_config.colors[i] = modes[active_mode].colors[i];
            }
        }
        //LOG_TRACE("[%s] Updating LED %1d", controller->evgaGPUName, zone_idx);
        controller->SetZone(zoneIndexMap[zone_idx], modes[active_mode].value, zone_config);
    }
}

void RGBController_EVGAGPUv3::UpdateZoneLEDs(int /*zone*/)
{
    //LOG_TRACE("[%s] Updating zone %1d", controller->evgaGPUName, zone);
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv3::UpdateSingleLED(int /*led*/)
{
    //LOG_TRACE("[%s] Updating single LED %1d", controller->evgaGPUName, led);
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv3::DeviceUpdateMode()
{
    /* Update all zone modes in a loop, each one with a packet to be use with per zone control
    for(uint8_t zone = 0; zone < 4; zone++)
    {
        controller->SetZoneMode(zone, modes[active_mode].value);
    }
    */
    //LOG_TRACE("[%s] Updating to mode %1d", controller->evgaGPUName, modes[active_mode].value);
    DeviceUpdateLEDs();
    controller->SetAllModes(modes[active_mode].value, modes[active_mode].value, modes[active_mode].value,modes[active_mode].value, true); //Set all zones to the same mode
}

void RGBController_EVGAGPUv3::DeviceSaveMode()
{
    controller->SaveConfig();
}
