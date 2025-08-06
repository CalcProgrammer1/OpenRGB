/*---------------------------------------------------------*\
| RGBController_CougarRevengerST.cpp                        |
|                                                           |
|   RGBController for Cougar Revenger ST                    |
|                                                           |
|   Morgan Guimard (morg)                       17 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <thread>
#include <chrono>
#include "RGBController_CougarRevengerST.h"

/**------------------------------------------------------------------*\
    @name Cougar Revenger ST
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCougarRevengerSTControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_CougarRevengerST::RGBController_CougarRevengerST(CougarRevengerSTController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Cougar";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Cougar Revenger ST Device";
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();
    version                     = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = DIRECT_MODE_VALUE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = MIN_BRIGHTNESS;
    Direct.brightness_max       = MAX_BRIGHTNESS;
    Direct.brightness           = MAX_BRIGHTNESS;
    modes.push_back(Direct);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = OFF_MODE_VALUE;
    Off.flags                   = MODE_FLAG_HAS_BRIGHTNESS;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = BREATHING_MODE_VALUE;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.brightness        = MAX_BRIGHTNESS;
    Breathing.brightness_min    = MIN_BRIGHTNESS;
    Breathing.brightness_max    = MAX_BRIGHTNESS;
    Breathing.speed             = MAX_SPEED;
    Breathing.speed_min         = MIN_SPEED;
    Breathing.speed_max         = MAX_SPEED;
    Breathing.colors_min        = COLORS_SIZE;
    Breathing.colors_max        = COLORS_SIZE;
    Breathing.colors.resize(COLORS_SIZE);
    modes.push_back(Breathing);

    mode Swift;
    Swift.name                  = "Swift";
    Swift.value                 = SWIFT_MODE_VALUE;
    Swift.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Swift.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Swift.brightness            = MAX_BRIGHTNESS;
    Swift.brightness_min        = MIN_BRIGHTNESS;
    Swift.brightness_max        = MAX_BRIGHTNESS;
    Swift.speed                 = MAX_SPEED;
    Swift.speed_min             = MIN_SPEED_SWIFT;
    Swift.speed_max             = MAX_SPEED;
    Swift.colors_min            = COLORS_SIZE;
    Swift.colors_max            = COLORS_SIZE;
    Swift.colors.resize(COLORS_SIZE);
    modes.push_back(Swift);

    mode Flow;
    Flow.name                  = "Flow";
    Flow.value                 = FLOW_MODE_VALUE;
    Flow.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Flow.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Flow.brightness            = MAX_BRIGHTNESS;
    Flow.brightness_min        = MIN_BRIGHTNESS;
    Flow.brightness_max        = MAX_BRIGHTNESS;
    Flow.speed                 = MAX_SPEED;
    Flow.speed_min             = MIN_SPEED;
    Flow.speed_max             = MAX_SPEED;
    Flow.colors_min            = COLORS_SIZE;
    Flow.colors_max            = COLORS_SIZE;
    Flow.direction             = MODE_DIRECTION_LEFT;
    Flow.colors.resize(COLORS_SIZE);
    modes.push_back(Flow);

    SetupZones();
}

RGBController_CougarRevengerST::~RGBController_CougarRevengerST()
{
    delete controller;
}

void RGBController_CougarRevengerST::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Bottom";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;
    zones.push_back(new_zone);

    new_zone.name       = "Mouse wheel";
    zones.push_back(new_zone);

    new_zone.name       = "Logo";
    zones.push_back(new_zone);

    leds.resize(zones.size());

    for(unsigned int i = 0; i < leds.size(); i++)
    {
        leds[i].name = "LED " + std::to_string(i+1);
    }

    SetupColors();
}

void RGBController_CougarRevengerST::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CougarRevengerST::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < colors.size(); i++)
    {
        UpdateZoneLEDs(i);
    }
}

void RGBController_CougarRevengerST::UpdateZoneLEDs(int zone)
{
    controller->SetDirect(zone, colors[zone], modes[active_mode].brightness);
}

void RGBController_CougarRevengerST::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_CougarRevengerST::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case DIRECT_MODE_VALUE:
        case OFF_MODE_VALUE:
            for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                controller->ActivateMode(zone_idx, modes[active_mode].value);
            }
            break;
        case BREATHING_MODE_VALUE:
        case SWIFT_MODE_VALUE:
            for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                controller->ActivateMode(zone_idx, modes[active_mode].value);
                controller->SetModeData(zone_idx, modes[active_mode].value, modes[active_mode].colors, modes[active_mode].brightness, modes[active_mode].speed);
            }
            break;

        case FLOW_MODE_VALUE:

            unsigned char mode_value = modes[active_mode].direction == MODE_DIRECTION_LEFT ? FLOW_LEFT_MODE_VALUE : FLOW_RIGHT_MODE_VALUE;

            controller->ActivateMode(0, mode_value);
            controller->ActivateMode(1, OFF_MODE_VALUE);
            controller->ActivateMode(2, mode_value);

            controller->SetModeData(0, mode_value, modes[active_mode].colors, modes[active_mode].brightness, modes[active_mode].speed);
            controller->SetModeData(2, mode_value, modes[active_mode].colors, modes[active_mode].brightness, modes[active_mode].speed);

            break;
    }
}
