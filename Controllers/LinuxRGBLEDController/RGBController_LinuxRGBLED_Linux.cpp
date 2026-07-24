/*---------------------------------------------------------*\
| RGBController_LinuxRGBLED_Linux.cpp                       |
|                                                           |
|   RGBController for Linux RGB sysfs LEDs (multi_intensity)|
|                                                           |
|   This driver supports the Valve-style sysfs LED interface|
|   found at /sys/class/leds/valve-leds* where each LED     |
|   directory contains a multi_intensity file for RGB       |
|   control and an effect file for hardware effect          |
|   selection.                                              |
|                                                           |
|   All LEDs on a device are grouped into a single linear   |
|   zone.  Hardware effects are exposed as modes.           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LinuxRGBLED_Linux.h"

/**------------------------------------------------------------------*\
    @name Linux RGB LED (Valve sysfs)
    @category LEDStrip
    @type File Stream (multi_intensity)
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLinuxRGBLEDControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LinuxRGBLED::RGBController_LinuxRGBLED(LinuxRGBLEDController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    type                = DEVICE_TYPE_LEDSTRIP;
    description         = "Linux RGB Sysfs LED Device";

    location            = controller->GetLEDPath(0);

    /*----------------------------------------------------*\
    | Build mode list                                      |
    |                                                      |
    | Mode index 0 is always the Direct (manual) mode.     |
    | Additional modes are created from available hardware |
    | effects if the device supports them.                 |
    \*----------------------------------------------------*/
    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*-----------------------------------------------------*\
    | If the device supports hardware effects, add them as  |
    | modes.  Effects are stored as strings; the mode value |
    | is the index into the available effects list.         |
    \*-----------------------------------------------------*/
    if(controller->SupportsEffects())
    {
        std::vector<std::string> effects = controller->GetAvailableEffects();

        for(unsigned int effect_idx = 0; effect_idx < effects.size(); effect_idx++)
        {
            mode eff_mode;
            eff_mode.name       = effects[effect_idx];
            eff_mode.value      = effect_idx + 1;   /* +1 so Direct (0) is separate */
            eff_mode.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            eff_mode.color_mode = MODE_COLORS_PER_LED;
            modes.push_back(eff_mode);
        }
    }

    SetupZones();
}

RGBController_LinuxRGBLED::~RGBController_LinuxRGBLED()
{
    Shutdown();

    delete controller;
}

void RGBController_LinuxRGBLED::SetupZones()
{
    /*-----------------------------------------------------*\
    | Create a single linear zone containing all LEDs       |
    \*-----------------------------------------------------*/
    zone led_zone;
    led_zone.name       = "RGB LEDs";
    led_zone.type       = ZONE_TYPE_LINEAR;
    led_zone.leds_min   = (unsigned int)controller->GetLEDCount();
    led_zone.leds_max   = (unsigned int)controller->GetLEDCount();
    led_zone.leds_count = (unsigned int)controller->GetLEDCount();

    if(led_zone.leds_count == 0)
    {
        led_zone.leds_min   = 1;
        led_zone.leds_max   = 1;
        led_zone.leds_count = 1;
    }

    zones.push_back(led_zone);

    /*-----------------------------------------------------*\
    | Create LED entries for each LED in the controller     |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name    = "RGB LED " + std::to_string(led_idx);
        new_led.value   = led_idx;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LinuxRGBLED::DeviceUpdateLEDs()
{
    /*-----------------------------------------------------*\
    | If the device supports effects, always set the effect |
    | first.  For Direct mode (active_mode == 0), set       |
    | "manual" to disable hardware effects and allow manual |
    |  control.                                             |
    | For other modes, set the selected hardware effect.    |
    |                                                       |
    | Setting the effect on one LED applies to all LEDs on  |
    | the device, so we only need to set it once.           |
    \*-----------------------------------------------------*/
    if(controller->SupportsEffects())
    {
        if(active_mode == 0)
        {
            /*---------------------------------------------*\
            | Direct mode - disable hardware effects so     |
            | that multi_intensity values control the LEDs  |
            | directly                                      |
            \*---------------------------------------------*/
            controller->SetEffect("manual");
        }
        else
        {
            std::vector<std::string> effects = controller->GetAvailableEffects();
            unsigned int effect_idx = active_mode - 1;

            if(effect_idx < effects.size())
            {
                controller->SetEffect(effects[effect_idx]);
                return;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Update all LEDs with colors from the color buffer     |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        controller->SetLEDColor(led_idx, red, grn, blu);
    }
}

void RGBController_LinuxRGBLED::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LinuxRGBLED::DeviceUpdateSingleLED(int led)
{
    /*-----------------------------------------------------*\
    | When effects are supported, defer to                  |
    | DeviceUpdateLEDs() which handles both effect          |
    | selection and updating colors.                        |
    | This ensures Direct mode gets set correctly.          |
    \*-----------------------------------------------------*/
    if(controller->SupportsEffects())
    {
        DeviceUpdateLEDs();
        return;
    }

    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->SetLEDColor(led, red, grn, blu);
}

void RGBController_LinuxRGBLED::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}