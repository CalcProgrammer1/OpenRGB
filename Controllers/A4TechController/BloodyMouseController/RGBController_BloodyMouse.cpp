/*---------------------------------------------------------*\
| RGBController_BloodyMouse.cpp                             |
|                                                           |
|   RGBController for A4Tech Bloody Mouse                   |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_BloodyMouse.h"

static const mouse_layout w60_pro
{
    {
        "Scroll Wheel", { 14 }
    },
    {
        "Mid Line",     {  0,  1,  2,  9,  3,  4,  5,  6 }
    },
    {
        "Logo",         { 10 }
    },
    {
        "Rear",         {  8,  7, 13, 12, 11 }
    }
};

static const mouse_layout w90_max
{
    {
        "Scroll Wheel", { 14 }
    },
    {
        "Logo",         { 7 }
    },
    {
        "Rear",         { 13, 12, 11, 10, 9, 8, 6, 5, 4, 3, 2, 1, 0 }
    }
};

static const mouse_layout mp_50rs
{
    {
        "Mouse Pad",    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 }
    }
};

/**------------------------------------------------------------------*\
    @name BloodyMouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectA4TechMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_BloodyMouse::RGBController_BloodyMouse(BloodyMouseController *controller_ptr)
{
    controller                  = controller_ptr;

    switch(controller->GetPid())
    {
        case BLOODY_MP_50RS_PID:
            type                = DEVICE_TYPE_MOUSEMAT;
            break;
        default:
            type                = DEVICE_TYPE_MOUSE;
    }

    name                        = controller->GetName();
    vendor                      = "Bloody";
    description                 = "Controller compatible with the Bloody W60 Pro and MP 50RS";
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = BLOODYMOUSE_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_BloodyMouse::~RGBController_BloodyMouse()
{
    delete controller;
}

void RGBController_BloodyMouse::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*-------------------------------------------------*\
    | Select layout from PID                            |
    \*-------------------------------------------------*/
    mouse_layout layout;

    switch(controller->GetPid())
    {
        case BLOODY_W60_PRO_PID:
            layout = w60_pro;
            break;
        case BLOODY_W70_MAX_PID:
        case BLOODY_W90_MAX_PID:
        case BLOODY_W90_PRO_PID:
            layout = w90_max;
            break;
        case BLOODY_MP_50RS_PID:
            layout = mp_50rs;
            break;
    }

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < layout.size(); zone_idx++)
    {
        mouse_zone mz           = layout[zone_idx];
        bool bool_single        = mz.zone_leds.size() == 1;

        zone new_zone;
        new_zone.name           = mz.name;
        new_zone.leds_min       = (unsigned int)mz.zone_leds.size();
        new_zone.leds_max       = new_zone.leds_min;
        new_zone.leds_count     = new_zone.leds_min;
        new_zone.type           = bool_single ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
        new_zone.matrix_map     = NULL;
        zones.push_back(new_zone);

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;

            new_led.value       = mz.zone_leds[lp_idx];

            if(bool_single)
            {
                new_led.name    = mz.name + " LED";
            }
            else
            {
                new_led.name    = mz.name;
                new_led.name.append(" LED " + std::to_string(lp_idx));
            }

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_BloodyMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_BloodyMouse::DeviceUpdateLEDs()
{
    std::vector<RGBColor> colour;
    for(size_t i = 0; i < colors.size(); i++)
    {
        RGBColor c = colors[i] | (leds[i].value << 24);
        colour.push_back(c);
    }

    controller->SetLedsDirect(colour);
}

void RGBController_BloodyMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_BloodyMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_BloodyMouse::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device only supports Direct mode                     |
    \*---------------------------------------------------------*/
}
