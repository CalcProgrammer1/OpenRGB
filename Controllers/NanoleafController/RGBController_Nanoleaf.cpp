/*---------------------------------------------------------*\
| RGBController_Nanoleaf.cpp                                |
|                                                           |
|   RGBController for Nanoleaf                              |
|                                                           |
|   Nikita Rushmanov                            13 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Nanoleaf.h"
#include "ResourceManager.h"
#include "LogManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**------------------------------------------------------------------*\
    @name Nanoleaf
    @category Light
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectNanoleafControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Nanoleaf::RGBController_Nanoleaf(std::string a_address, int a_port, std::string a_auth_token) :
    controller(a_address, a_port, a_auth_token)
{
    location    = a_address+":"+std::to_string(a_port);
    name        = controller.GetName();
    serial      = controller.GetSerial();
    vendor      = controller.GetManufacturer();
    version     = controller.GetFirmwareVersion();
    description = controller.GetModel();
    type        = DEVICE_TYPE_LIGHT;

    /*-------------------------------------------------------------*\
    | Direct mode uses external control protocol.                   |
    \*-------------------------------------------------------------*/
    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*---------------------------------------------------------*\
    | Set this effect as current if the name is selected.       |
    \*---------------------------------------------------------*/
    if(controller.GetSelectedEffect() == NANOLEAF_DIRECT_MODE_EFFECT_NAME)
    {
        /*-----------------------------------------------------*\
        | If the direct mode is active, we need to call this    |
        | method to open the socket.                            |
        \*-----------------------------------------------------*/
        controller.StartExternalControl();
        active_mode = 0;
    }

    /*-------------------------------------------------------------*\
    | Create additional modes from device effects list              |
    \*-------------------------------------------------------------*/
    for(std::vector<std::string>::const_iterator it = controller.GetEffects().begin(); it != controller.GetEffects().end(); ++it)
    {
        mode effect;
        effect.name             = *it;
        effect.flags            = MODE_FLAG_HAS_BRIGHTNESS;
        effect.color_mode       = MODE_COLORS_NONE;
        effect.brightness_max   = 100;
        effect.brightness_min   = 0;
        effect.brightness       = 100;

        modes.push_back(effect);

        /*---------------------------------------------------------*\
        | Set this effect as current if the name is selected.       |
        \*---------------------------------------------------------*/
        if(controller.GetSelectedEffect() == effect.name)
        {
            active_mode         = (int)modes.size() - 1;
        }
    }

    SetupZones();
}

void RGBController_Nanoleaf::SetupZones()
{
    zone led_zone;
    led_zone.name           = "Nanoleaf Layout";
    led_zone.type           = ZONE_TYPE_LINEAR;
    led_zone.leds_count     = (unsigned int)controller.GetPanelIds().size();
    led_zone.leds_min       = led_zone.leds_count;
    led_zone.leds_max       = led_zone.leds_count;
    led_zone.matrix_map     = NULL;

    for(std::vector<int>::const_iterator it = controller.GetPanelIds().begin(); it != controller.GetPanelIds().end(); ++it)
    {
        led new_led;
        new_led.name        = std::to_string(*it);
        leds.push_back(new_led);
    }

    zones.push_back(led_zone);

    SetupColors();
}

void RGBController_Nanoleaf::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Nanoleaf::DeviceUpdateLEDs()
{
    controller.UpdateLEDs(colors);
}

void RGBController_Nanoleaf::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Nanoleaf::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Nanoleaf::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Mode 0 is reserved for Direct mode                        |
    \*---------------------------------------------------------*/
    if(active_mode == 0)
    {
        controller.StartExternalControl();
    }
    /*---------------------------------------------------------*\
    | Update normal effects.                                    |
    \*---------------------------------------------------------*/
    else
    {
        controller.SelectEffect(modes[active_mode].name);
        controller.SetBrightness(modes[active_mode].brightness);
    }
}
