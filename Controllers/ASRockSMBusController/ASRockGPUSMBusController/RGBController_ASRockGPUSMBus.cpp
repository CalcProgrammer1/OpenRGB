/*---------------------------------------------------------*\
| RGBController_ASRockGPUSMBus.cpp                          |
|                                                           |
|   RGBController for ASRock GPU RGB controllers            |
|                                                           |
|   Based on SignalRGB ASRock GPU.js plugin                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ASRockGPUSMBus.h"

/**------------------------------------------------------------------*\
    @name ASRock GPU SMBus
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectASRockGPUSMBusControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ASRockGPUSMBus::RGBController_ASRockGPUSMBus(ASRockGPUSMBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "ASRock";
    type        = DEVICE_TYPE_GPU;
    description = "ASRock GPU RGB Controller";
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ASROCK_GPU_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASROCK_GPU_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Static.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Static.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASROCK_GPU_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min         = ASROCK_GPU_SPEED_SLOWEST;
    Breathing.speed_max         = ASROCK_GPU_SPEED_FASTEST;
    Breathing.speed             = ASROCK_GPU_SPEED_DEFAULT;
    Breathing.brightness_min    = ASROCK_GPU_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ASROCK_GPU_BRIGHTNESS_MAX;
    Breathing.brightness        = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = ASROCK_GPU_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Strobe.speed_min            = ASROCK_GPU_SPEED_SLOWEST;
    Strobe.speed_max            = ASROCK_GPU_SPEED_FASTEST;
    Strobe.speed                = ASROCK_GPU_SPEED_DEFAULT;
    Strobe.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Strobe.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Strobe.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode Random;
    Random.name                 = "Random";
    Random.value                = ASROCK_GPU_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Random.speed_min            = ASROCK_GPU_SPEED_SLOWEST;
    Random.speed_max            = ASROCK_GPU_SPEED_FASTEST;
    Random.speed                = ASROCK_GPU_SPEED_DEFAULT;
    Random.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Random.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Random.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = ASROCK_GPU_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Wave.speed_min              = ASROCK_GPU_SPEED_SLOWEST;
    Wave.speed_max              = ASROCK_GPU_SPEED_FASTEST;
    Wave.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Wave.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Wave.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Wave.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = ASROCK_GPU_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Spring.speed_min            = ASROCK_GPU_SPEED_SLOWEST;
    Spring.speed_max            = ASROCK_GPU_SPEED_FASTEST;
    Spring.speed                = ASROCK_GPU_SPEED_DEFAULT;
    Spring.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Spring.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Spring.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Spring.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = ASROCK_GPU_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Stack.speed_min             = ASROCK_GPU_SPEED_SLOWEST;
    Stack.speed_max             = ASROCK_GPU_SPEED_FASTEST;
    Stack.speed                 = ASROCK_GPU_SPEED_DEFAULT;
    Stack.brightness_min        = ASROCK_GPU_BRIGHTNESS_MIN;
    Stack.brightness_max        = ASROCK_GPU_BRIGHTNESS_MAX;
    Stack.brightness            = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Stack.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Cram;
    Cram.name                   = "Cram";
    Cram.value                  = ASROCK_GPU_MODE_CRAM;
    Cram.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Cram.speed_min              = ASROCK_GPU_SPEED_SLOWEST;
    Cram.speed_max              = ASROCK_GPU_SPEED_FASTEST;
    Cram.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Cram.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Cram.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Cram.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Cram.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Cram);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = ASROCK_GPU_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Scan.speed_min              = ASROCK_GPU_SPEED_SLOWEST;
    Scan.speed_max              = ASROCK_GPU_SPEED_FASTEST;
    Scan.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Scan.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Scan.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Scan.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Scan.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Scan);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = ASROCK_GPU_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Neon.speed_min              = ASROCK_GPU_SPEED_SLOWEST;
    Neon.speed_max              = ASROCK_GPU_SPEED_FASTEST;
    Neon.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Neon.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Neon.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Neon.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Neon.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Neon);

    mode Water;
    Water.name                  = "Water";
    Water.value                 = ASROCK_GPU_MODE_WATER;
    Water.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Water.speed_min             = ASROCK_GPU_SPEED_SLOWEST;
    Water.speed_max             = ASROCK_GPU_SPEED_FASTEST;
    Water.speed                 = ASROCK_GPU_SPEED_DEFAULT;
    Water.brightness_min        = ASROCK_GPU_BRIGHTNESS_MIN;
    Water.brightness_max        = ASROCK_GPU_BRIGHTNESS_MAX;
    Water.brightness            = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Water.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ASROCK_GPU_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min           = ASROCK_GPU_SPEED_SLOWEST;
    Rainbow.speed_max           = ASROCK_GPU_SPEED_FASTEST;
    Rainbow.speed               = ASROCK_GPU_SPEED_DEFAULT;
    Rainbow.brightness_min      = ASROCK_GPU_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ASROCK_GPU_BRIGHTNESS_MAX;
    Rainbow.brightness          = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode RainbowWave;
    RainbowWave.name            = "Rainbow Wave";
    RainbowWave.value           = ASROCK_GPU_MODE_RAINBOW_WAVE;
    RainbowWave.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    RainbowWave.speed_min       = ASROCK_GPU_SPEED_SLOWEST;
    RainbowWave.speed_max       = ASROCK_GPU_SPEED_FASTEST;
    RainbowWave.speed           = ASROCK_GPU_SPEED_DEFAULT;
    RainbowWave.brightness_min  = ASROCK_GPU_BRIGHTNESS_MIN;
    RainbowWave.brightness_max  = ASROCK_GPU_BRIGHTNESS_MAX;
    RainbowWave.brightness      = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    RainbowWave.color_mode      = MODE_COLORS_NONE;
    modes.push_back(RainbowWave);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ASROCK_GPU_MODE_OFF;
    Off.flags                   = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    /*-----------------------------------------------------*\
    | Each channel runs its own effect, so every mode       |
    | except Direct is also available per zone              |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t mode_idx = 0; mode_idx < modes.size(); mode_idx++)
        {
            if(modes[mode_idx].value != ASROCK_GPU_MODE_DIRECT)
            {
                zones[zone_idx].modes.push_back(modes[mode_idx]);
            }
        }
    }
}

RGBController_ASRockGPUSMBus::~RGBController_ASRockGPUSMBus()
{
    Shutdown();

    delete controller;
}

void RGBController_ASRockGPUSMBus::SetupZones()
{
    /*-----------------------------------------------------*\
    | Set up zones - one zone per channel                   |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < controller->num_channels; i++)
    {
        uint8_t channel_idx = controller->enabled_channels[i];
        
        zone new_zone;
        new_zone.type = ZONE_TYPE_SINGLE;
        new_zone.leds_min = 1;
        new_zone.leds_max = 1;
        new_zone.leds_count = 1;
        
        /*-----------------------------------------------------*\
        | Channel names from SignalRGB                          |
        \*-----------------------------------------------------*/
        if(channel_idx == ASROCK_GPU_CHANNEL_ARGB_HEADER)
        {
            new_zone.name = "ARGB Header";
        }
        else if(channel_idx == ASROCK_GPU_CHANNEL_TOP_SIDE)
        {
            new_zone.name = "Top Side";
        }
        else if(channel_idx == ASROCK_GPU_CHANNEL_FAN)
        {
            new_zone.name = "Fan";
        }
        else
        {
            new_zone.name = "Channel " + std::to_string(channel_idx);
        }
        
        zones.push_back(new_zone);
        
        /*-----------------------------------------------------*\
        | Set up LED for this zone                              |
        \*-----------------------------------------------------*/
        led new_led;
        new_led.name = new_zone.name;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ASRockGPUSMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-----------------------------------------------------*\
    | This device does not support resizing zones           |
    \*-----------------------------------------------------*/
}

void RGBController_ASRockGPUSMBus::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        DeviceUpdateZoneLEDs((int)zone_idx);
    }
}

void RGBController_ASRockGPUSMBus::DeviceUpdateZoneLEDs(int zone)
{
    /*-----------------------------------------------------*\
    | In Direct mode only the zones that follow the device  |
    | mode stream colors, resending a zone's own effect     |
    | would restart it                                      |
    \*-----------------------------------------------------*/
    if((modes[active_mode].value == ASROCK_GPU_MODE_DIRECT) && (GetZoneMode(zone) != &modes[active_mode]))
    {
        return;
    }

    DeviceUpdateZoneMode(zone);
}

void RGBController_ASRockGPUSMBus::DeviceUpdateSingleLED(int led)
{
    /*-----------------------------------------------------*\
    | Each zone has exactly one LED                         |
    \*-----------------------------------------------------*/
    DeviceUpdateZoneLEDs(led);
}

void RGBController_ASRockGPUSMBus::DeviceUpdateMode()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        DeviceUpdateZoneMode((int)zone_idx);
    }
}

void RGBController_ASRockGPUSMBus::DeviceUpdateZoneMode(int zone)
{
    if((zone < 0) || ((unsigned int)zone >= controller->num_channels))
    {
        return;
    }

    uint8_t     channel_idx = controller->enabled_channels[zone];
    mode*       zone_mode   = GetZoneMode(zone);
    RGBColor    color       = 0;

    if(zone_mode->color_mode == MODE_COLORS_PER_LED)
    {
        color = colors[zone];
    }

    if(zone_mode->value == ASROCK_GPU_MODE_DIRECT)
    {
        controller->SetChannelColor(channel_idx,
                                    RGBGetRValue(color),
                                    RGBGetGValue(color),
                                    RGBGetBValue(color));
    }
    else
    {
        uint8_t speed       = (zone_mode->flags & MODE_FLAG_HAS_SPEED) ? zone_mode->speed : ASROCK_GPU_SPEED_DEFAULT;
        uint8_t brightness  = (zone_mode->flags & MODE_FLAG_HAS_BRIGHTNESS) ? zone_mode->brightness : ASROCK_GPU_BRIGHTNESS_DEFAULT;

        controller->SetChannelMode(channel_idx,
                                   zone_mode->value,
                                   speed,
                                   brightness,
                                   RGBGetRValue(color),
                                   RGBGetGValue(color),
                                   RGBGetBValue(color));
    }
}

void RGBController_ASRockGPUSMBus::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveConfig();
}

mode* RGBController_ASRockGPUSMBus::GetZoneMode(int zone)
{
    /*-----------------------------------------------------*\
    | A zone uses its own mode if it has one selected,      |
    | otherwise the device mode                             |
    \*-----------------------------------------------------*/
    if((zones[zone].active_mode >= 0) && (zones[zone].active_mode < (int)zones[zone].modes.size()))
    {
        return(&zones[zone].modes[zones[zone].active_mode]);
    }

    return(&modes[active_mode]);
}
