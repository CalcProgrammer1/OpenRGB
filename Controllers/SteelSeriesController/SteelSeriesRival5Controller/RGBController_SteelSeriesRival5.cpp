/*---------------------------------------------------------*\
| RGBController_SteelSeriesRival5.cpp                       |
|                                                           |
|   RGBController for SteelSeries Rival 5                   |
|                                                           |
|   A Ingram (Woodsy900)                        20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesRival5.h"

RGBController_SteelSeriesRival5::RGBController_SteelSeriesRival5(SteelSeriesMouseController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "SteelSeries Mouse Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = STEELSERIES_MOUSE_EFFECT_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0x00;
    Direct.brightness_max   = STEELSERIES_MOUSE_BRIGHTNESS_MAX;
    Direct.brightness       = STEELSERIES_MOUSE_BRIGHTNESS_MAX;

    mode Reactive;
    Reactive.name           = "Reactive";
    Reactive.value          = STEELSERIES_MOUSE_EFFECT_REACTIVE;   // any unique value; define e.g. 0x26
    Reactive.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Reactive.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min     = 1;
    Reactive.colors_max     = 1;
    Reactive.colors.resize(1);
    Reactive.colors[0]      = ToRGBColor(255, 255, 255);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = STEELSERIES_MOUSE_EFFECT_RAINBOW;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    Rainbow.flags           = MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.brightness_min  = 0x00;
    Rainbow.brightness_max  = STEELSERIES_MOUSE_BRIGHTNESS_MAX;        
    Rainbow.brightness      = STEELSERIES_MOUSE_BRIGHTNESS_MAX;

    steelseries_mouse mouse = controller->GetMouse();

    for(const uint8_t i: mouse.modes)
    {
        switch(i)
        {
            case STEELSERIES_MOUSE_EFFECT_DIRECT:
                modes.push_back(Direct);
                break;
        
            case STEELSERIES_MOUSE_EFFECT_REACTIVE:
                modes.push_back(Reactive);
                break;
            
            case STEELSERIES_MOUSE_EFFECT_RAINBOW:     
                modes.push_back(Rainbow);
                break;
        }
    }

    SetupZones();
}

RGBController_SteelSeriesRival5::~RGBController_SteelSeriesRival5()
{
    Shutdown();

    delete controller;
}

void RGBController_SteelSeriesRival5::SetupZones()
{
    steelseries_mouse mouse = controller->GetMouse();

    zone lighting_zone;

    lighting_zone.name       = "Whole Mouse";
    lighting_zone.type       = ZONE_TYPE_LINEAR;
    lighting_zone.leds_min   = static_cast<unsigned int>(mouse.leds.size());
    lighting_zone.leds_max   = static_cast<unsigned int>(mouse.leds.size());
    lighting_zone.leds_count = static_cast<unsigned int>(mouse.leds.size());

    zones.push_back(lighting_zone);

    for(const led_info& info : mouse.leds)
    {
        led mouse_led;

        mouse_led.name  = info.name;
        mouse_led.value = info.value;

        leds.push_back(mouse_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesRival5::DeviceUpdateLEDs()
{
    SteelSeriesRival5Controller* ctrl = static_cast<SteelSeriesRival5Controller*>(controller);
    ctrl->SetBrightness((uint8_t)modes[active_mode].brightness);  // guarded, so ~free under effects
    ctrl->UpdateLEDs(colors);
}

void RGBController_SteelSeriesRival5::DeviceUpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesRival5::DeviceUpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesRival5::DeviceUpdateMode()
{
    SteelSeriesRival5Controller* ctrl = static_cast<SteelSeriesRival5Controller*>(controller);
    if(modes[active_mode].name == "Reactive")
    {
        RGBColor c = modes[active_mode].colors[0];
        ctrl->SetReactiveColor(
            true, 
            RGBGetRValue(c), 
            RGBGetGValue(c), 
            RGBGetBValue(c)
        );
    }
    else if(modes[active_mode].name == "Rainbow")
    {
        ctrl->SetRainbowEffect(true);
    }
    else
    {
        ctrl->SetReactiveColor(false,0,0,0);   // leaving Reactive must disable it (it's persistent in HW)
        DeviceUpdateLEDs();
    }
}

void RGBController_SteelSeriesRival5::DeviceSaveMode()
{
 /*--------------------------------------------------------*\
 | Note: Rival 5 does not have a "save" command             |
 |    The Rival 5 saves the current state automatically     |
 \*--------------------------------------------------------*/
}

