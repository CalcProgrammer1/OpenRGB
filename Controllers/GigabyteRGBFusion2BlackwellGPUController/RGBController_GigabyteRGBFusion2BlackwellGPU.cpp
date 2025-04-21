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
    @detectors DetectGigabyteRGBFusion2BlackwellGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2BlackwellGPU::RGBController_RGBFusion2BlackwellGPU(RGBFusion2BlackwellGPUController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Gigabyte GPU";
    vendor      = "Gigabyte";
    description = "Gigabyte RGB Fusion 2 Blackwell GPU";
    location    = controller->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = RGB_FUSION2_BLACKWELL_GPU_MODE_STATIC;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Direct.brightness_max       = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Direct.brightness           = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name              = "Pulse";
    Breathing.value             = RGB_FUSION2_BLACKWELL_GPU_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min         = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    Breathing.speed_max         = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    Breathing.speed             = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.brightness_min    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Breathing.brightness_max    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Breathing.brightness        = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flash";
    Flashing.value              = RGB_FUSION2_BLACKWELL_GPU_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Flashing.speed_min          = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    Flashing.speed_max          = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    Flashing.speed              = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    Flashing.color_mode         = MODE_COLORS_PER_LED;
    Flashing.brightness_min     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    Flashing.brightness_max     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    Flashing.brightness         = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(Flashing);

    mode DualFlashing;
    DualFlashing.name           = "Double Flash";
    DualFlashing.value          = RGB_FUSION2_BLACKWELL_GPU_MODE_DUAL_FLASHING;
    DualFlashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    DualFlashing.speed_min      = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    DualFlashing.speed_max      = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    DualFlashing.speed          = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    DualFlashing.color_mode     = MODE_COLORS_PER_LED;
    DualFlashing.brightness_min = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    DualFlashing.brightness_max = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    DualFlashing.brightness     = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(DualFlashing);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Color Cycle";
    SpectrumCycle.value             = RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.speed_min         = RGB_FUSION2_BLACKWELL_GPU_SPEED_SLOWEST;
    SpectrumCycle.speed_max         = RGB_FUSION2_BLACKWELL_GPU_SPEED_FASTEST;
    SpectrumCycle.speed             = RGB_FUSION2_BLACKWELL_GPU_SPEED_NORMAL;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
    modes.push_back(SpectrumCycle);

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

    for(uint8_t zone_idx = 0; zone_idx < RGB_FUSION_2_BLACKWELL_GPU_NUMBER_OF_ZONES; zone_idx++)
    {
        zone new_zone;
        led  new_led;

        new_zone.name          = "GPU zone " + std::to_string(zone_idx + 1);
        new_zone.type          = ZONE_TYPE_SINGLE;
        new_zone.leds_min      = 1;
        new_zone.leds_max      = 1;
        new_zone.leds_count    = 1;
        new_zone.matrix_map    = NULL;

        new_led.name           = new_zone.name;
        /*---------------------------------------------------------*\
        | Push the zone and LED on to device vectors                |
        \*---------------------------------------------------------*/
        leds.push_back(new_led);
        zones.push_back(new_zone);
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
    zone_config.numberOfColors  = (uint8_t)modes[active_mode].colors.size();

    for(uint8_t zone_idx = 0; zone_idx < RGB_FUSION_2_BLACKWELL_GPU_NUMBER_OF_ZONES; zone_idx++)
    {
        zone_config.colors[0] = colors[zone_idx];

        if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            for (uint8_t i = 0; i < zone_config.numberOfColors; i++)
            {
                zone_config.colors[i] = modes[active_mode].colors[i];
            }
        }

        controller->SetZone(zone_idx, modes[active_mode].value, zone_config);
    }
}

void RGBController_RGBFusion2BlackwellGPU::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusion2BlackwellGPU::UpdateSingleLED(int led)
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
