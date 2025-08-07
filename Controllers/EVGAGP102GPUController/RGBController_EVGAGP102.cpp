/*---------------------------------------------------------*\
| RGBController_EVGAGP102.cpp                               |
|                                                           |
|   RGBController for EVGA GP102 GPU                        |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_EVGAGP102.h"

/**------------------------------------------------------------------*\
    @name EVGA GP102 GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectEVGAGPUControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_EVGAGP102::RGBController_EVGAGP102(std::vector<EVGAGP102Controller*> controller_list)
{
    controllers         = controller_list;

    name                = controllers[0]->GetDeviceName();
    vendor              = "EVGA";
    description         = "EVGA GP102-based RGB GPU Device";

    for(unsigned int i = 0; i < zones.size(); i++)
    {
        location       += controllers[i]->GetDeviceLocation() + " ";
    }

    type = DEVICE_TYPE_GPU;

    mode Off;
    Off.name            = "Off";
    Off.value           = EVGA_GP102_MODE_OFF;
    Off.flags           = 0;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = EVGA_GP102_MODE_CUSTOM;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    // Initialize active mode and stored color

    unsigned char raw_active_mode = controllers[0]->GetMode();

    active_mode = 0;
    for(unsigned int i = 0; i < modes.size(); i++)
    {
        if (modes[i].value == raw_active_mode)
        {
            active_mode = i;
            break;
        }
    }
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        std::array<unsigned char, 3> rgb = controllers[i]->GetColor();

        colors[i] = ToRGBColor(rgb[0], rgb[1], rgb[2]);
    }
}

RGBController_EVGAGP102::~RGBController_EVGAGP102()
{
    for(unsigned int i = 0; i < controllers.size(); i++)
    {
        delete controllers[i];
    }
}

void RGBController_EVGAGP102::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device basically has two controllable zones, one at  |
    | the top of the board with GeForce 1080 Ti and another for |
    | the backplate logo (K|NGP|N logo, or EVGA GeForce 1080 Ti |
    | for the FTW3).
    \*---------------------------------------------------------*/
    for(unsigned int i = 0; i < controllers.size(); i++)
    {
        zone new_zone;
        led new_led;

        new_zone.name          = controllers[i]->GetZoneName();
        new_zone.type          = ZONE_TYPE_SINGLE;
        new_zone.leds_min      = 1;
        new_zone.leds_max      = 1;
        new_zone.leds_count    = 1;
        new_zone.matrix_map    = NULL;

        new_led.name           = controllers[i]->GetZoneName();

        leds.push_back(new_led);
        zones.push_back(new_zone);
    }

    SetupColors();
}

void RGBController_EVGAGP102::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAGP102::DeviceUpdateLEDs()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_EVGAGP102::UpdateZoneLEDs(int zone)
{
    RGBColor color    = colors[zone];
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);
    controllers[zone]->SetColor(red, grn, blu);
}

void RGBController_EVGAGP102::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGP102::DeviceUpdateMode()
{
    for(unsigned int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
    }
}

void RGBController_EVGAGP102::DeviceSaveMode()
{
}
