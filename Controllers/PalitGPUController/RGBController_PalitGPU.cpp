/*---------------------------------------------------------*\
| RGBController_PalitGPU.cpp                                |
|                                                           |
|   RGBController for Palit GPU                             |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PalitGPU.h"

/**------------------------------------------------------------------*\
    @name Palit GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPalitGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PalitGPU::RGBController_PalitGPU(PalitGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "Palit";
    description             = "Legacy Palit RGB GPU Device";
    location                = controller->GetDeviceLocation();

    type                    = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = PALIT_GPU_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

void RGBController_PalitGPU::SetupZones()
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

void RGBController_PalitGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PalitGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_PalitGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PalitGPU::DeviceUpdateMode()
{
    RGBColor      color = colors[0];
    unsigned char r     = RGBGetRValue(color);
    unsigned char g     = RGBGetGValue(color);
    unsigned char b     = RGBGetBValue(color);

    switch(modes[active_mode].value)
    {
        case PALIT_GPU_MODE_DIRECT:
            controller->SetDirect(r, g, b);
            break;

        default:
            break;
    }
}
