/*---------------------------------------------------------------------*\
|  RGBController_BloodyMouse.cpp                                        |
|                                                                       |
|  Driver for BloodyMouse USB Controller                                |
|                                                                       |
|  Chris M (Dr_No)          30 Jun 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

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

    name                        = "BloodyMouse";
    vendor                      = "Bloody";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "Controller compatible with the Bloody W60 Pro";
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

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < w60_pro.size(); zone_idx++)
    {
        mouse_zone mz           = w60_pro[zone_idx];
        bool bool_single        = mz.zone_leds.size() == 1;

        zone new_zone;
        new_zone.name           = mz.name;
        new_zone.leds_min       = mz.zone_leds.size();
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

void RGBController_BloodyMouse::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_BloodyMouse::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device only supports Direct mode                     |
    \*---------------------------------------------------------*/
}
