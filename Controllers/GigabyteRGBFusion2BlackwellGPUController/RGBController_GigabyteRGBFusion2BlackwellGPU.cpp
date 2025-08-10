/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2BlackwellGPU.cpp          |
|                                                           |
|   RGBController for Gigabyte RGB Fusion 2 Blackwell GPU   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2BlackwellGPU.h"
#include "LogManager.h"
#include "GigabyteRGBFusion2BlackwellGPUDefinitions.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Fusion 2 Blackwell GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusion2BlackwellSingleZoneGPUControllers,DetectGigabyteRGBFusion2BlackwellGamingLayoutGPUControllers,DetectGigabyteRGBFusion2BlackwellWaterforceLayoutGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2BlackwellGPU::RGBController_RGBFusion2BlackwellGPU(RGBFusion2BlackwellGPUController* controller_ptr, uint8_t led_layout)
{
    controller                      = controller_ptr;
    gpu_layout                      = led_layout;

    name                            = controller->GetDeviceName();
    vendor                          = "Gigabyte";
    description                     = "Gigabyte RGB Fusion 2 Blackwell GPU Device";
    location                        = controller->GetDeviceLocation();
    type                            = DEVICE_TYPE_GPU;

    mode Static;
    Static.name                     = "Static";
    Static.value                    = RGB_FUSION2_BLACKWELL_GPU_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode               = MODE_COLORS_PER_LED;
    Static.brightness_min           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Static.brightness_max           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Static.brightness               = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Static);

    // Some GPU models (Gaming) dont maintain the colors but it has faster updates, useful for Effects (and the reason it has to be named Direct)
    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = RGB_FUSION2_BLACKWELL_GPU_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Direct.brightness_max           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Direct.brightness               = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name                  = "Pulse";
    Breathing.value                 = RGB_FUSION2_BLACKWELL_GPU_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min             = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    Breathing.speed_max             = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    Breathing.speed                 = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Breathing.brightness_max        = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Breathing.brightness            = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name                   = "Flash";
    Flashing.value                  = RGB_FUSION2_BLACKWELL_GPU_MODE_FLASHING;
    Flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Flashing.speed_min              = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    Flashing.speed_max              = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    Flashing.speed                  = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    Flashing.color_mode             = MODE_COLORS_PER_LED;
    Flashing.brightness_min         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Flashing.brightness_max         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Flashing.brightness             = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Flashing);

    mode DualFlashing;
    DualFlashing.name               = "Double Flash";
    DualFlashing.value              = RGB_FUSION2_BLACKWELL_GPU_MODE_DUAL_FLASHING;
    DualFlashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    DualFlashing.speed_min          = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    DualFlashing.speed_max          = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    DualFlashing.speed              = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    DualFlashing.color_mode         = MODE_COLORS_PER_LED;
    DualFlashing.brightness_min     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    DualFlashing.brightness_max     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    DualFlashing.brightness         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(DualFlashing);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Color Cycle";
    SpectrumCycle.value             = RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.speed_min         = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    SpectrumCycle.speed_max         = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    SpectrumCycle.speed             = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(SpectrumCycle);

    if(led_layout == RGB_FUSION2_BLACKWELL_GPU_GAMING_LAYOUT || led_layout == RGB_FUSION2_BLACKWELL_GPU_WATERFORCE_LAYOUT)
    {
        mode Wave;
        Wave.name                   = "Wave";
        Wave.value                  = RGB_FUSION2_BLACKWELL_GPU_MODE_WAVE;
        Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
        Wave.speed_min              = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
        Wave.speed_max              = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
        Wave.speed                  = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
        Wave.color_mode             = MODE_COLORS_NONE;
        Wave.brightness_min         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
        Wave.brightness_max         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        Wave.brightness             = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        modes.push_back(Wave);

        mode Gradient;
        Gradient.name               = "Gradient";
        Gradient.value              = RGB_FUSION2_BLACKWELL_GPU_MODE_GRADIENT;
        Gradient.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
        Gradient.speed_min          = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
        Gradient.speed_max          = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
        Gradient.speed              = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
        Gradient.color_mode         = MODE_COLORS_PER_LED;
        Gradient.brightness_min     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
        Gradient.brightness_max     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        Gradient.brightness         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        modes.push_back(Gradient);

        mode ColorShift;
        ColorShift.name             = "Color Shift";
        ColorShift.value            = RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_SHIFT;
        ColorShift.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
        ColorShift.speed_min        = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
        ColorShift.speed_max        = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
        ColorShift.speed            = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
        ColorShift.color_mode       = MODE_COLORS_MODE_SPECIFIC;
        ColorShift.colors_min       = 1;
        ColorShift.colors_max       = 8;
        ColorShift.colors.resize(8);
        ColorShift.brightness_min   = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
        ColorShift.brightness_max   = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        ColorShift.brightness       = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        modes.push_back(ColorShift);

        /* Disabled Dazzle as it seems to only execute once, would need to loop it maybe?
        *  Not for Waterforce
        mode Dazzle;
        Dazzle.name                 = "Dazzle";
        Dazzle.value                = RGB_FUSION2_BLACKWELL_GPU_MODE_DAZZLE;
        Dazzle.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
        Dazzle.speed_min            = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
        Dazzle.speed_max            = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
        Dazzle.speed                = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
        Dazzle.color_mode           = MODE_COLORS_MODE_SPECIFIC;
        Dazzle.colors_min           = 1;
        Dazzle.colors_max           = 8;
        Dazzle.colors.resize(8);
        Dazzle.brightness_min       = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
        Dazzle.brightness_max       = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        Dazzle.brightness           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
        modes.push_back(Dazzle);*/
    }

    SetupZones();
}

RGBController_RGBFusion2BlackwellGPU::~RGBController_RGBFusion2BlackwellGPU()
{
    delete controller;
}

void RGBController_RGBFusion2BlackwellGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only allows setting the entire zone for all   |
    | LED's in the zone and does not allow per LED control.     |
    \*---------------------------------------------------------*/
    if(gpu_layout == RGB_FUSION2_BLACKWELL_GPU_SINGLE_ZONE)
    {
        zone new_zone;
        led  new_led;

        new_zone.name       = "Side";
        new_zone.type       = ZONE_TYPE_SINGLE;
        new_zone.leds_min   = 1;
        new_zone.leds_max   = 1;
        new_zone.leds_count = 1;
        new_zone.matrix_map = NULL;

        new_led.name        = new_zone.name;

        /*---------------------------------------------------------*\
        | Push the zone and LED on to device vectors                |
        \*---------------------------------------------------------*/
        leds.push_back(new_led);
        zones.push_back(new_zone);
    }
    else if(gpu_layout == RGB_FUSION2_BLACKWELL_GPU_GAMING_LAYOUT)
    {
        for(uint8_t zone_idx = 0; zone_idx < 4; zone_idx++)
        {
            zone new_zone;
            led  new_led;

            switch(zone_idx)
            {
                case 0:
                    new_zone.name = "Right fan";
                    break;

                case 1:
                    new_zone.name = "Left fan";
                    break;

                case 2:
                    new_zone.name = "Center fan";
                    break;

                case 3:
                    new_zone.name = "Side";
                    break;
            }

            new_zone.type       = ZONE_TYPE_SINGLE;
            new_zone.leds_min   = 1;
            new_zone.leds_max   = 1;
            new_zone.leds_count = 1;
            new_zone.matrix_map = NULL;

            new_led.name        = new_zone.name;

            /*---------------------------------------------------------*\
            | Push the zone and LED on to device vectors                |
            \*---------------------------------------------------------*/
            leds.push_back(new_led);
            zones.push_back(new_zone);
        }
    }
    else if(gpu_layout == RGB_FUSION2_BLACKWELL_GPU_WATERFORCE_LAYOUT)
    {
        for(uint8_t zone_idx = 0; zone_idx < 2; zone_idx++)
        {
            zone new_zone;
            led  new_led;

            switch(zone_idx)
            {
                case 0:
                    new_zone.name = "Waterblock";
                    break;

                case 1:
                    new_zone.name = "Backplate";
                    break;
            }

            new_zone.type       = ZONE_TYPE_SINGLE;
            new_zone.leds_min   = 1;
            new_zone.leds_max   = 1;
            new_zone.leds_count = 1;
            new_zone.matrix_map = NULL;

            new_led.name        = new_zone.name;

            /*---------------------------------------------------------*\
            | Push the zone and LED on to device vectors                |
            \*---------------------------------------------------------*/
            leds.push_back(new_led);
            zones.push_back(new_zone);
        }
    }

    SetupColors();
}

void RGBController_RGBFusion2BlackwellGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusion2BlackwellGPU::DeviceUpdateLEDs()
{
    fusion2_config zone_config;

    zone_config.brightness      = modes[active_mode].brightness;
    zone_config.speed           = modes[active_mode].speed;
    zone_config.direction       = modes[active_mode].direction;
    zone_config.numberOfColors  = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        zone_config.numberOfColors = (uint8_t)modes[active_mode].colors.size();
    }

    uint8_t gpu_zones;
    switch(gpu_layout) // replicating GCC that sends more packets even when there is less zones
    {
        case RGB_FUSION2_BLACKWELL_GPU_SINGLE_ZONE:
            gpu_zones = 1;
            break;

        case RGB_FUSION2_BLACKWELL_GPU_GAMING_LAYOUT:
            gpu_zones = 6;
            break;

        case RGB_FUSION2_BLACKWELL_GPU_WATERFORCE_LAYOUT:
            gpu_zones = 3;
            break;

        default:
            LOG_TRACE("[%s] Invalid GPU layout (%d) when updating LEDs.", name.c_str(), gpu_layout);
            return; // should not happen
    }

    for(uint8_t zone_idx = 0; zone_idx < gpu_zones; zone_idx++)
    {
        if(zone_idx >= zones.size())
        {
            zone_config.colors[zone_idx] = colors.back();
        }
        else
        {
            zone_config.colors[zone_idx] = colors[zone_idx];
        }

        for(uint8_t i = 0; i < zone_config.numberOfColors; i++) // specific for MODE_COLORS_MODE_SPECIFIC
        {
            zone_config.colors[i] = modes[active_mode].colors[i];
        }

        controller->SetZone(zone_idx, modes[active_mode].value, zone_config);
    }
}

void RGBController_RGBFusion2BlackwellGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2BlackwellGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2BlackwellGPU::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2BlackwellGPU::DeviceSaveMode()
{
    controller->SaveConfig();
}
