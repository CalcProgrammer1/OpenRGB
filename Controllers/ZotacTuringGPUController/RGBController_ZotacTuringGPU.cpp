/*---------------------------------------------------------*\
| RGBController_ZotacTuringGPU.cpp                          |
|                                                           |
|   RGBController for Zotac Turing GPU                      |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ZotacTuringGPU.h"

/**------------------------------------------------------------------*\
    @name ZOTAC Turing GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectZotacTuringGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_ZotacTuringGPU::RGBController_ZotacTuringGPU(ZotacTuringGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "ZOTAC";
    description             = "ZOTAC Turing-based RGB GPU Device";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = ZOTAC_GPU_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Flashing;
    Flashing.name           = "Flashing";
    Flashing.value          = ZOTAC_GPU_MODE_STROBE;
    Flashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min      = ZOTAC_GPU_SPEED_SLOWEST;
    Flashing.speed_max      = ZOTAC_GPU_SPEED_FASTEST;
    Flashing.speed          = ZOTAC_GPU_SPEED_NORMAL;
    Flashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode Wave;
    Wave.name               = "Rainbow Wave";
    Wave.value              = ZOTAC_GPU_MODE_WAVE;
    Wave.flags              = MODE_FLAG_HAS_SPEED;
    Wave.speed_min          = ZOTAC_GPU_SPEED_SLOWEST;
    Wave.speed_max          = ZOTAC_GPU_SPEED_FASTEST;
    Wave.speed              = ZOTAC_GPU_SPEED_NORMAL;
    Wave.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = ZOTAC_GPU_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min     = ZOTAC_GPU_SPEED_SLOWEST;
    Breathing.speed_max     = ZOTAC_GPU_SPEED_FASTEST;
    Breathing.speed         = ZOTAC_GPU_SPEED_NORMAL;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name         = "Spectrum Cycle";
    ColorCycle.value        = ZOTAC_GPU_MODE_COLOR_CYCLE;
    ColorCycle.flags        = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min    = ZOTAC_GPU_SPEED_SLOWEST;
    ColorCycle.speed_max    = ZOTAC_GPU_SPEED_FASTEST;
    ColorCycle.speed        = ZOTAC_GPU_SPEED_NORMAL;
    ColorCycle.color_mode   = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    SetupZones();
}

RGBController_ZotacTuringGPU::~RGBController_ZotacTuringGPU()
{
    delete controller;
}

void RGBController_ZotacTuringGPU::SetupZones()
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
    SetupInitialValues();
}

void RGBController_ZotacTuringGPU::SetupInitialValues()
{
    /*---------------------------------------------------------*\
    | Retrieve current values by reading the device             |
    \*---------------------------------------------------------*/
    unsigned int speed;

    controller->GetMode(colors[0], active_mode, speed);
    modes[active_mode].speed = speed;

    SignalUpdate();
}

void RGBController_ZotacTuringGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ZotacTuringGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ZotacTuringGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateMode();
}

void RGBController_ZotacTuringGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateMode();
}

void RGBController_ZotacTuringGPU::DeviceUpdateMode()
{
    controller->SetMode(colors[0], modes[active_mode].value, modes[active_mode].speed);
}
