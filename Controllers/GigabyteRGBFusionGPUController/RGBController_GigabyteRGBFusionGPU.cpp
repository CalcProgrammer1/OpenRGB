/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusionGPU.cpp                    |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion GPU         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                23 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusionGPU.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Fusion GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusionGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RGBFusionGPU::RGBController_RGBFusionGPU(RGBFusionGPUController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "Gigabyte";
    description                     = "RGB Fusion GPU Device";
    location                        = controller->GetDeviceLocation();
    type                            = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = RGB_FUSION_GPU_MODE_STATIC;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = RGB_FUSION_GPU_BRIGHTNESS_MIN;
    Direct.brightness_max           = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    Direct.brightness               = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = RGB_FUSION_GPU_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min             = RGB_FUSION_GPU_SPEED_SLOWEST;
    Breathing.speed_max             = RGB_FUSION_GPU_SPEED_FASTEST;
    Breathing.speed                 = RGB_FUSION_GPU_SPEED_NORMAL;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = RGB_FUSION_GPU_BRIGHTNESS_MIN;
    Breathing.brightness_max        = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    Breathing.brightness            = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name                   = "Flashing";
    Flashing.value                  = RGB_FUSION_GPU_MODE_FLASHING;
    Flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Flashing.speed_min              = RGB_FUSION_GPU_SPEED_SLOWEST;
    Flashing.speed_max              = RGB_FUSION_GPU_SPEED_FASTEST;
    Flashing.speed                  = RGB_FUSION_GPU_SPEED_NORMAL;
    Flashing.color_mode             = MODE_COLORS_PER_LED;
    Flashing.brightness_min         = RGB_FUSION_GPU_BRIGHTNESS_MIN;
    Flashing.brightness_max         = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    Flashing.brightness             = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    modes.push_back(Flashing);

    mode DualFlashing;
    DualFlashing.name               = "Dual Flashing";
    DualFlashing.value              = RGB_FUSION_GPU_MODE_DUAL_FLASHING;
    DualFlashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    DualFlashing.speed_min          = RGB_FUSION_GPU_SPEED_SLOWEST;
    DualFlashing.speed_max          = RGB_FUSION_GPU_SPEED_FASTEST;
    DualFlashing.speed              = RGB_FUSION_GPU_SPEED_NORMAL;
    DualFlashing.color_mode         = MODE_COLORS_PER_LED;
    DualFlashing.brightness_min     = RGB_FUSION_GPU_BRIGHTNESS_MIN;
    DualFlashing.brightness_max     = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    DualFlashing.brightness         = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    modes.push_back(DualFlashing);

    mode ColorCycle;
    ColorCycle.name                 = "Color Cycle";
    ColorCycle.value                = RGB_FUSION_GPU_MODE_COLOR_CYCLE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.flags                = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min            = RGB_FUSION_GPU_SPEED_SLOWEST;
    ColorCycle.speed_max            = RGB_FUSION_GPU_SPEED_FASTEST;
    ColorCycle.speed                = RGB_FUSION_GPU_SPEED_NORMAL;
    ColorCycle.color_mode           = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = RGB_FUSION_GPU_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.speed_min         = RGB_FUSION_GPU_SPEED_SLOWEST;
    SpectrumCycle.speed_max         = RGB_FUSION_GPU_SPEED_FASTEST;
    SpectrumCycle.speed             = RGB_FUSION_GPU_SPEED_NORMAL;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    SpectrumCycle.brightness_min    = RGB_FUSION_GPU_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = RGB_FUSION_GPU_BRIGHTNESS_MAX;
    modes.push_back(SpectrumCycle);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

RGBController_RGBFusionGPU::~RGBController_RGBFusionGPU()
{
    delete controller;
}

void RGBController_RGBFusionGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone          = new zone();
    led*  new_led           = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_RGBFusionGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusionGPU::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_RGBFusionGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusionGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusionGPU::DeviceUpdateMode()
{
    controller->SetMode((unsigned char)modes[(unsigned int)active_mode].value, (unsigned char)modes[(unsigned int)active_mode].speed, (unsigned char)modes[(unsigned int)active_mode].brightness);
}

void RGBController_RGBFusionGPU::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->Save();
}
