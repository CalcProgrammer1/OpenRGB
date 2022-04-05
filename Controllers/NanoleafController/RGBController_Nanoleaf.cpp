/*-----------------------------------------*\
|  RGBController_Nanoleaf.cpp               |
|                                           |
|  Generic RGB Interface for Nanoleaf       |
|                                           |
|  Nikita Rushmanov 01/13/2022              |
\*-----------------------------------------*/

#include "RGBController_Nanoleaf.h"
#include "ResourceManager.h"
#include "LogManager.h"
#include <curl/curl.h>
#include "json.hpp"
using json = nlohmann::json;

RGBController_Nanoleaf::RGBController_Nanoleaf(std::string a_address, int a_port, std::string a_auth_token) :
    nanoleaf(a_address, a_port, a_auth_token)
{
    location = a_address+":"+std::to_string(a_port);
    name = nanoleaf.GetName();
    serial = nanoleaf.GetSerial();
    vendor = nanoleaf.GetManufacturer();
    version = nanoleaf.GetFirmwareVersion();
    description = nanoleaf.GetModel();
    type = DEVICE_TYPE_LIGHT;

    // Direct mode currently only supported for Nanoleaf Panels.
    if(nanoleaf.GetModel() == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        // Set this effect as current if the name is selected.
        if(nanoleaf.GetSelectedEffect() == NANOLEAF_DIRECT_MODE_EFFECT_NAME)
        {
            // If the direct mode is active, we need to call this method to open the socket.
            nanoleaf.StartExternalControl();
            active_mode = 0;
        }
    }

    for(std::vector<std::string>::const_iterator it = nanoleaf.GetEffects().begin(); it != nanoleaf.GetEffects().end(); ++it)
    {
        mode effect;
        effect.name = *it;
        effect.flags      = MODE_FLAG_HAS_BRIGHTNESS;
        effect.color_mode = MODE_COLORS_NONE;
        effect.brightness_max = 100;
        effect.brightness_min = 0;
        effect.brightness = 100;

        modes.push_back(effect);

        // Set this effect as current if the name is selected.
        if(nanoleaf.GetSelectedEffect() == effect.name)
        {
            active_mode = modes.size() - 1;
        }
    }

    SetupZones();
}

void RGBController_Nanoleaf::SetupZones()
{
    zone led_zone;
    led_zone.name           = "Nanoleaf Layout";
    led_zone.type           = ZONE_TYPE_LINEAR;
    led_zone.leds_count     = nanoleaf.GetPanelIds().size();
    led_zone.leds_min       = led_zone.leds_count;
    led_zone.leds_max       = led_zone.leds_count;
    led_zone.matrix_map     = NULL;

    for(std::vector<int>::const_iterator it = nanoleaf.GetPanelIds().begin(); it != nanoleaf.GetPanelIds().end(); ++it)
    {
        led new_led;
        new_led.name = std::to_string(*it);
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
    if(nanoleaf.GetModel() == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        nanoleaf.UpdateLEDs(colors);
    }
}

void RGBController_Nanoleaf::UpdateZoneLEDs(int /*zone*/)
{
	DeviceUpdateLEDs();
}

void RGBController_Nanoleaf::UpdateSingleLED(int /*led*/)
{
	DeviceUpdateLEDs();
}

void RGBController_Nanoleaf::SetCustomMode()
{
    if(nanoleaf.GetModel() == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        // Put the Nanoleaf into direct mode.
        nanoleaf.StartExternalControl();
    }
}

void RGBController_Nanoleaf::DeviceUpdateMode()
{
    // 0 mode is reserved for Direct mode
    if(active_mode == 0 && nanoleaf.GetModel() == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        nanoleaf.StartExternalControl();
    }
    // Update normal effects.
    else
    {
        // Select effect.
        nanoleaf.SelectEffect(modes[active_mode].name);

        // Update brightness.
        nanoleaf.SetBrightness(modes[active_mode].brightness);
    }
}
