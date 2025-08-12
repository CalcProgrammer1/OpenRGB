/*---------------------------------------------------------*\
| RGBController_LegoDimensionsToypadBase.cpp                |
|                                                           |
|   RGBController for Lego Dimensions Toypad Base           |
|                                                           |
|   Morgan Guimard (morg)                       02 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "RGBController_LegoDimensionsToypadBase.h"

/**------------------------------------------------------------------*\
    @name Lego Dimensions Toypad Base
    @category Case
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLegoDimensionsToypadBaseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LegoDimensionsToypadBase::RGBController_LegoDimensionsToypadBase(LegoDimensionsToypadBaseController* controller_ptr)
{
    controller                          = controller_ptr;

    name                                = controller->GetNameString();
    vendor                              = "Logic3";
    type                                = DEVICE_TYPE_LEDSTRIP;
    description                         = "Lego Dimensions Toypad Base";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();

    mode Direct;
    Direct.name                         = "Direct";
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Flash;
    Flash.name                          = "Flash";
    Flash.value                         = LEGO_DIMENSIONS_TOYPAD_BASE_FLASH_MODE_VALUE;
    Flash.flags                         = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Flash.color_mode                    = MODE_COLORS_MODE_SPECIFIC;
    Flash.colors.resize(1);
    Flash.colors_max                    = 1;
    Flash.colors_min                    = 1;
    Flash.speed                         = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX / 2;
    Flash.speed_max                     = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX;
    Flash.speed_min                     = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MIN;
    modes.push_back(Flash);

    mode Fade;
    Fade.name                           = "Fade";
    Fade.value                          = LEGO_DIMENSIONS_TOYPAD_BASE_FADE_MODE_VALUE;
    Fade.flags                          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Fade.color_mode                     = MODE_COLORS_MODE_SPECIFIC;
    Fade.colors.resize(1);
    Fade.colors_max                     = 1;
    Fade.colors_min                     = 1;
    Fade.speed                          = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX / 2;
    Fade.speed_max                      = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX;
    Fade.speed_min                      = LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MIN;
    modes.push_back(Fade);

    SetupZones();
}

RGBController_LegoDimensionsToypadBase::~RGBController_LegoDimensionsToypadBase()
{
    delete controller;
}

void RGBController_LegoDimensionsToypadBase::SetupZones()
{
    std::vector<std::string> zone_names =
    {
        "Center",
        "Left",
        "Right"
    };

    for(const std::string& zone_name: zone_names)
    {
        zone new_zone;

        new_zone.name       = zone_name;
        new_zone.type       = ZONE_TYPE_LINEAR;
        new_zone.leds_min   = 1;
        new_zone.leds_max   = 1;
        new_zone.leds_count = 1;
        new_zone.matrix_map = nullptr;

        zones.emplace_back(new_zone);

        led new_led;
        new_led.name = "LED";
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LegoDimensionsToypadBase::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LegoDimensionsToypadBase::DeviceUpdateLEDs()
{
    for(unsigned int zone = 0; zone < zones.size(); zone++)
    {
        UpdateZoneLEDs(zone);
    }
}

void RGBController_LegoDimensionsToypadBase::UpdateZoneLEDs(int zone)
{
    controller->SetDirect(zone + 1, zones[zone].colors[0]);
}

void RGBController_LegoDimensionsToypadBase::UpdateSingleLED(int /*led*/)
{
    UpdateZoneLEDs(0);
}

void RGBController_LegoDimensionsToypadBase::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        controller->SetMode(0, modes[active_mode].value, modes[active_mode].speed, modes[active_mode].colors[0]);
    }
}
