/*---------------------------------------------------------*\
| RGBController_AsusROGSpatha.cpp                           |
|                                                           |
|   RGBController for ASUS ROG Spatha                       |
|                                                           |
|   Mola19                                      05 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusROGSpatha.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Spatha
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBSpatha
    @comment This device allows indiviual modes for each zone,
        which currently can't be implemented in OpenRGB.
        Also there seem to be a firmware bug which causes static
        to use a random colorand random to use a static color
        that was previously set. This can be worked around by saving
\*-------------------------------------------------------------------*/

RGBController_AsusROGSpatha::RGBController_AsusROGSpatha(AsusAuraMouseGen1Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "ASUS Aura Mouse Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ASUS_ROG_SPATHA_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASUS_ROG_SPATHA_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min       = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    Static.brightness_max       = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    Static.brightness           = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASUS_ROG_SPATHA_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.brightness_min    = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    Breathing.brightness        = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    Breathing.colors_min        = 2;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(2);
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = ASUS_ROG_SPATHA_MODE_SPECTRUM_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.brightness_min   = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    ColorCycle.brightness       = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    ColorCycle.colors_min       = 12;
    ColorCycle.colors_max       = 12;
    ColorCycle.colors.resize(12);
    ColorCycle.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = ASUS_ROG_SPATHA_MODE_RANDOM;
    Random.flags                = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Random.brightness_min       = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    Random.brightness_max       = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    Random.brightness           = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = ASUS_ROG_SPATHA_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.brightness_min     = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    Reactive.brightness_max     = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    Reactive.brightness         = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    Reactive.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(Reactive);

    mode Battery;
    Battery.name               = "Battery";
    Battery.value              = ASUS_ROG_SPATHA_MODE_BATTERY;
    Battery.flags              = MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Battery.brightness_min     = ASUS_ROG_SPATHA_BRIGHTNESS_MIN;
    Battery.brightness_max     = ASUS_ROG_SPATHA_BRIGHTNESS_MAX;
    Battery.brightness         = ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT;
    Battery.color_mode         = MODE_COLORS_NONE;
    modes.push_back(Battery);

    SetupZones();
}

RGBController_AsusROGSpatha::~RGBController_AsusROGSpatha()
{
    delete controller;
}

void RGBController_AsusROGSpatha::SetupZones()
{

    std::string zones_names[3] = {"Side", "Scroll Wheel", "Logo"};

    for(unsigned char i = 0; i < 3; i++)
    {
        zone spatha_zone;

        spatha_zone.name         = zones_names[i];
        spatha_zone.type         = ZONE_TYPE_SINGLE;
        spatha_zone.leds_min     = 1;
        spatha_zone.leds_max     = 1;
        spatha_zone.leds_count   = 1;
        spatha_zone.matrix_map   = NULL;

        zones.push_back(spatha_zone);

        led spatha_led;

        spatha_led.name          = zones_names[i];
        spatha_led.value         = 1;

        leds.push_back(spatha_led);
    }

    SetupColors();
}

void RGBController_AsusROGSpatha::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AsusROGSpatha::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_DIRECT)
    {
        controller->SendDirectSpatha(colors);
    }
    else
    {
        UpdateSingleLED(0);
        UpdateSingleLED(1);
        UpdateSingleLED(2);
    }
}

void RGBController_AsusROGSpatha::UpdateZoneLEDs(int zone)
{
    if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_DIRECT)
    {
        controller->SendDirectSpatha(colors);
    }
    else
    {
        UpdateSingleLED(zone);
    }
}

void RGBController_AsusROGSpatha::UpdateSingleLED(int led)
{
    if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_DIRECT)
    {
        controller->SendDirectSpatha(colors);
    }
    else
    {
        controller->SendUpdate(0x13 + led * 38, RGBGetRValue(colors[led]));
        controller->SendUpdate(0x14 + led * 38, RGBGetGValue(colors[led]));
        controller->SendUpdate(0x15 + led * 38, RGBGetBValue(colors[led]));
    }
}

void RGBController_AsusROGSpatha::DeviceUpdateMode()
{
    if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_DIRECT)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Needed to overwrite direct                            |
    \*-----------------------------------------------------*/
    controller->ResetToSavedLighting();

    /*-----------------------------------------------------*\
    | Send data to all 3 zones                              |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 3; i++)
    {
        controller->SendUpdate(0x11 + i * 38, modes[active_mode].value);

        /*------------------------------------------------------------------*\
        | This mouse has independent brightness for wired and wireless.      |
        | Each is 4-bit in the same byte (wireless is the first/bigger one). |
        | OpenRGB misses that feature, hence both are the same               |
        \*------------------------------------------------------------------*/
        controller->SendUpdate(0x12 + i * 38, (modes[active_mode].brightness << 4) + modes[active_mode].brightness);

        if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_SPECTRUM_CYCLE || modes[active_mode].value == ASUS_ROG_SPATHA_MODE_BREATHING)
        {
            for(unsigned int j = 0; j < modes[active_mode].colors.size(); j++)
            {
                controller->SendUpdate(0x13 + j * 3 + i * 38, RGBGetRValue(modes[active_mode].colors[j]));
                controller->SendUpdate(0x14 + j * 3 + i * 38, RGBGetGValue(modes[active_mode].colors[j]));
                controller->SendUpdate(0x15 + j * 3 + i * 38, RGBGetBValue(modes[active_mode].colors[j]));
            }
        }
    }
}

void RGBController_AsusROGSpatha::DeviceSaveMode()
{
    if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_DIRECT)
    {
        return;
    }

    unsigned int profile = controller->GetActiveProfile();

    /*-----------------------------------------------------*\
    | Send data to all 3 zones                              |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 3; i++)
    {
        controller->UpdateProfile(0x11 + i * 38, profile, modes[active_mode].value);
        /*------------------------------------------------------------------*\
        | This mouse has independent brightness for wired and wireless.      |
        | Each is 4-bit in the same byte (wireless is the first/bigger one). |
        | OpenRGB misses that feature, hence both are the same               |
        \*------------------------------------------------------------------*/
        controller->UpdateProfile(0x12 + i * 38, profile, (modes[active_mode].brightness << 4) + modes[active_mode].brightness);

        if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_SPECTRUM_CYCLE || modes[active_mode].value == ASUS_ROG_SPATHA_MODE_BREATHING)
        {
            for(unsigned int j = 0; j < modes[active_mode].colors.size(); j++)
            {
                controller->UpdateProfile(0x13 + j * 3 + i * 38, profile, RGBGetRValue(modes[active_mode].colors[j]));
                controller->UpdateProfile(0x14 + j * 3 + i * 38, profile, RGBGetGValue(modes[active_mode].colors[j]));
                controller->UpdateProfile(0x15 + j * 3 + i * 38, profile, RGBGetBValue(modes[active_mode].colors[j]));
            }
        }
        else if(modes[active_mode].value == ASUS_ROG_SPATHA_MODE_STATIC || modes[active_mode].value == ASUS_ROG_SPATHA_MODE_REACTIVE)
        {
            controller->UpdateProfile(0x13 + i * 38, profile, RGBGetRValue(colors[i]));
            controller->UpdateProfile(0x14 + i * 38, profile, RGBGetGValue(colors[i]));
            controller->UpdateProfile(0x15 + i * 38, profile, RGBGetBValue(colors[i]));
        }
    }

    controller->ResetToSavedLighting();
}
