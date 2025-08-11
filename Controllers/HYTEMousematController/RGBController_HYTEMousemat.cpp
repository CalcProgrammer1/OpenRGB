/*---------------------------------------------------------*\
| RGBController_HYTEMousemat.cpp                            |
|                                                           |
|   RGBController for HYTE mousemat                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_HYTEMousemat.h"

/**------------------------------------------------------------------*\
    @name HYTE Mousemat
    @category Mousemat
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHYTEMousematControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_HYTEMousemat::RGBController_HYTEMousemat(HYTEMousematController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    vendor              = "HYTE";
    description         = "HYTE Mousemat Device";
    type                = DEVICE_TYPE_MOUSEMAT;
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = HYTE_CNVS_MODE_DIRECT;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    // HYTE CNVS does not seem to be able to transfer back into firmware animation
    // after streaming command has been used
    //mode Rainbow;
    //Rainbow.name        = "Rainbow Wave";
    //Rainbow.value       = HYTE_CNVS_MODE_RAINBOW;
    //Rainbow.flags       = MODE_FLAG_HAS_RANDOM_COLOR;
    //Rainbow.color_mode  = MODE_COLORS_RANDOM;
    //modes.push_back(Rainbow);

    SetupZones();
}

RGBController_HYTEMousemat::~RGBController_HYTEMousemat()
{
    delete controller;
}

void RGBController_HYTEMousemat::SetupZones()
{
    zone mousemat_zone;

    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_LINEAR;
    mousemat_zone.leds_min      = 50;
    mousemat_zone.leds_max      = 50;
    mousemat_zone.leds_count    = 50;
    mousemat_zone.matrix_map    = NULL;

    zones.push_back(mousemat_zone);

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led mousemat_led;

        mousemat_led.name       = "Mousemat LED ";
        mousemat_led.name.append(std::to_string(led_idx));

        leds.push_back(mousemat_led);
    }

    SetupColors();
}

void RGBController_HYTEMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HYTEMousemat::DeviceUpdateLEDs()
{
    controller->StreamingCommand(&colors[0]);
}

void RGBController_HYTEMousemat::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HYTEMousemat::UpdateSingleLED(int /*led*/)
{

}

void RGBController_HYTEMousemat::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case HYTE_CNVS_MODE_DIRECT:
            controller->FirmwareAnimationControl(false);
            break;

        case HYTE_CNVS_MODE_RAINBOW:
            controller->FirmwareAnimationControl(true);
            break;
    }
}
