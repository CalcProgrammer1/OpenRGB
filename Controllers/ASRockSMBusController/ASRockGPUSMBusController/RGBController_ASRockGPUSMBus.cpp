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
    Breathing.speed_min         = ASROCK_GPU_SPEED_MIN;
    Breathing.speed_max         = ASROCK_GPU_SPEED_MAX;
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
    Strobe.speed_min            = ASROCK_GPU_SPEED_MIN;
    Strobe.speed_max            = ASROCK_GPU_SPEED_MAX;
    Strobe.speed                = ASROCK_GPU_SPEED_DEFAULT;
    Strobe.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Strobe.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Strobe.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = ASROCK_GPU_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.speed_min     = ASROCK_GPU_SPEED_MIN;
    SpectrumCycle.speed_max     = ASROCK_GPU_SPEED_MAX;
    SpectrumCycle.speed         = ASROCK_GPU_SPEED_DEFAULT;
    SpectrumCycle.brightness_min= ASROCK_GPU_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max= ASROCK_GPU_BRIGHTNESS_MAX;
    SpectrumCycle.brightness    = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = ASROCK_GPU_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Random.speed_min            = ASROCK_GPU_SPEED_MIN;
    Random.speed_max            = ASROCK_GPU_SPEED_MAX;
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
    Wave.speed_min              = ASROCK_GPU_SPEED_MIN;
    Wave.speed_max              = ASROCK_GPU_SPEED_MAX;
    Wave.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Wave.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Wave.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Wave.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = ASROCK_GPU_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Spring.speed_min            = ASROCK_GPU_SPEED_MIN;
    Spring.speed_max            = ASROCK_GPU_SPEED_MAX;
    Spring.speed                = ASROCK_GPU_SPEED_DEFAULT;
    Spring.brightness_min       = ASROCK_GPU_BRIGHTNESS_MIN;
    Spring.brightness_max       = ASROCK_GPU_BRIGHTNESS_MAX;
    Spring.brightness           = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Spring.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = ASROCK_GPU_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Stack.speed_min             = ASROCK_GPU_SPEED_MIN;
    Stack.speed_max             = ASROCK_GPU_SPEED_MAX;
    Stack.speed                 = ASROCK_GPU_SPEED_DEFAULT;
    Stack.brightness_min        = ASROCK_GPU_BRIGHTNESS_MIN;
    Stack.brightness_max        = ASROCK_GPU_BRIGHTNESS_MAX;
    Stack.brightness            = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Stack.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Stack);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = ASROCK_GPU_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Scan.speed_min              = ASROCK_GPU_SPEED_MIN;
    Scan.speed_max              = ASROCK_GPU_SPEED_MAX;
    Scan.speed                  = ASROCK_GPU_SPEED_DEFAULT;
    Scan.brightness_min         = ASROCK_GPU_BRIGHTNESS_MIN;
    Scan.brightness_max         = ASROCK_GPU_BRIGHTNESS_MAX;
    Scan.brightness             = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Scan.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Scan);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = ASROCK_GPU_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min           = ASROCK_GPU_SPEED_MIN;
    Rainbow.speed_max           = ASROCK_GPU_SPEED_MAX;
    Rainbow.speed               = ASROCK_GPU_SPEED_DEFAULT;
    Rainbow.brightness_min      = ASROCK_GPU_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ASROCK_GPU_BRIGHTNESS_MAX;
    Rainbow.brightness          = ASROCK_GPU_BRIGHTNESS_DEFAULT;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ASROCK_GPU_MODE_OFF;
    Off.flags                   = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
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

void RGBController_ASRockGPUSMBus::DeviceUpdateLEDs(std::vector<RGBColor>& update_colors)
{
    if(modes[active_mode].value == ASROCK_GPU_MODE_DIRECT)
    {
        for(unsigned int i = 0; i < controller->num_channels; i++)
        {
            uint8_t channel_idx = controller->enabled_channels[i];
            RGBColor color      = colors[i];

            controller->SetChannelColor(channel_idx,
                                        RGBGetRValue(color),
                                        RGBGetGValue(color),
                                        RGBGetBValue(color));
        }
    }
    else
    {
        DeviceUpdateMode();
    }
}

void RGBController_ASRockGPUSMBus::UpdateZoneLEDs(int zone)
{
    if(zone < 0 || (unsigned int)zone >= controller->num_channels)
    {
        return;
    }

    uint8_t channel_idx = controller->enabled_channels[zone];
    RGBColor color      = colors[zone];

    if(modes[active_mode].value == ASROCK_GPU_MODE_DIRECT)
    {
        controller->SetChannelColor(channel_idx,
                                    RGBGetRValue(color),
                                    RGBGetGValue(color),
                                    RGBGetBValue(color));
    }
    else
    {
        uint8_t mode_val   = modes[active_mode].value;
        uint8_t speed      = (modes[active_mode].flags & MODE_FLAG_HAS_SPEED) ? modes[active_mode].speed : ASROCK_GPU_SPEED_DEFAULT;
        uint8_t brightness = (modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS) ? modes[active_mode].brightness : ASROCK_GPU_BRIGHTNESS_DEFAULT;

        controller->SetChannelMode(channel_idx,
                                   mode_val,
                                   speed,
                                   brightness,
                                   RGBGetRValue(color),
                                   RGBGetGValue(color),
                                   RGBGetBValue(color));
    }
}

void RGBController_ASRockGPUSMBus::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_ASRockGPUSMBus::DeviceUpdateMode()
{
    uint8_t mode_val   = modes[active_mode].value;
    uint8_t speed      = (modes[active_mode].flags & MODE_FLAG_HAS_SPEED) ? modes[active_mode].speed : ASROCK_GPU_SPEED_DEFAULT;
    uint8_t brightness = (modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS) ? modes[active_mode].brightness : ASROCK_GPU_BRIGHTNESS_DEFAULT;

    for(unsigned int i = 0; i < controller->num_channels; i++)
    {
        uint8_t channel_idx = controller->enabled_channels[i];
        RGBColor color      = (i < colors.size()) ? colors[i] : 0;

        controller->SetChannelMode(channel_idx,
                                   mode_val,
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
