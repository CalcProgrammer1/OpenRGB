/*---------------------------------------------------------*\
| RGBController_AsusAuraHeadsetStand.cpp                    |
|                                                           |
|   RGBController for ASUS Aura headset stand               |
|                                                           |
|   Mola19                                      06 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusAuraHeadsetStand.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Headset Stand
    @category HeadsetStand
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBHeadsetStand
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraHeadsetStand::RGBController_AuraHeadsetStand(AuraHeadsetStandController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    vendor                          = "ASUS";
    type                            = DEVICE_TYPE_HEADSET_STAND;
    description                     = "ASUS Aura Headset Stand Device";
    version                         = controller->GetVersion();
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = AURA_HEADSET_STAND_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = AURA_HEADSET_STAND_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.brightness_min           = AURA_HEADSETSTAND_BRIGHTNESS_MIN;
    Static.brightness_max           = AURA_HEADSETSTAND_BRIGHTNESS_MAX;
    Static.brightness               = AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AURA_HEADSET_STAND_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min             = AURA_HEADSETSTAND_SPEED_MIN;
    Breathing.speed_max             = AURA_HEADSETSTAND_SPEED_MAX;
    Breathing.speed                 = AURA_HEADSETSTAND_SPEED_DEFAULT;
    Breathing.brightness_min        = AURA_HEADSETSTAND_BRIGHTNESS_MIN;
    Breathing.brightness_max        = AURA_HEADSETSTAND_BRIGHTNESS_MAX;
    Breathing.brightness            = AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Strobing;
    Strobing.name                   = "Flashing";
    Strobing.value                  = AURA_HEADSET_STAND_MODE_STROBING;
    Strobing.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Strobing.brightness_min         = AURA_HEADSETSTAND_BRIGHTNESS_MIN;
    Strobing.brightness_max         = AURA_HEADSETSTAND_BRIGHTNESS_MAX;
    Strobing.brightness             = AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT;
    Strobing.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Strobing.colors_min             = 1;
    Strobing.colors_max             = 1;
    Strobing.colors.resize(1);
    modes.push_back(Strobing);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AURA_HEADSET_STAND_MODE_COLOR_CYCLE;
    SpectrumCycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    SpectrumCycle.speed_min         = AURA_HEADSETSTAND_SPEED_MIN;
    SpectrumCycle.speed_max         = AURA_HEADSETSTAND_SPEED_MAX;
    SpectrumCycle.speed             = AURA_HEADSETSTAND_SPEED_DEFAULT;
    SpectrumCycle.brightness_min    = AURA_HEADSETSTAND_BRIGHTNESS_MIN;
    SpectrumCycle.brightness_max    = AURA_HEADSETSTAND_BRIGHTNESS_MAX;
    SpectrumCycle.brightness        = AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT;
    SpectrumCycle.color_mode        = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name                    = "Rainbow Wave";
    Rainbow.value                   = AURA_HEADSET_STAND_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min               = AURA_HEADSETSTAND_SPEED_MIN;
    Rainbow.speed_max               = AURA_HEADSETSTAND_SPEED_MAX;
    Rainbow.speed                   = AURA_HEADSETSTAND_SPEED_DEFAULT;
    Rainbow.brightness_min          = AURA_HEADSETSTAND_BRIGHTNESS_MIN;
    Rainbow.brightness_max          = AURA_HEADSETSTAND_BRIGHTNESS_MAX;
    Rainbow.brightness              = AURA_HEADSETSTAND_BRIGHTNESS_DEFAULT;
    modes.push_back(Rainbow);
    SetupZones();
}

RGBController_AuraHeadsetStand::~RGBController_AuraHeadsetStand()
{
    delete controller;
}

void RGBController_AuraHeadsetStand::SetupZones()
{
    zone underglow_zone;

    underglow_zone.name         = "Underglow";
    underglow_zone.type         = ZONE_TYPE_LINEAR;
    underglow_zone.leds_min     = 17;
    underglow_zone.leds_max     = 17;
    underglow_zone.leds_count   = 17;
    underglow_zone.matrix_map   = NULL;

    zones.push_back(underglow_zone);

    for(unsigned int i = 0; i < 17; i++)
    {
        led underglow_led;

        underglow_led.name      = "Underglow LED " + std::to_string(i);

        leds.push_back(underglow_led);
    }

    zone logo_zone;

    logo_zone.name              = "Logo";
    logo_zone.type              = ZONE_TYPE_SINGLE;
    logo_zone.leds_min          = 1;
    logo_zone.leds_max          = 1;
    logo_zone.leds_count        = 1;
    logo_zone.matrix_map        = NULL;

    zones.push_back(logo_zone);

    led logo_led;

    logo_led.name               = "Logo LED";

    leds.push_back(logo_led);

    SetupColors();
}

void RGBController_AuraHeadsetStand::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraHeadsetStand::DeviceUpdateLEDs()
{
    controller->UpdateLeds(std::vector<RGBColor>(colors));
}

void RGBController_AuraHeadsetStand::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraHeadsetStand::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraHeadsetStand::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    switch(modes[active_mode].value)
    {
        case 0:
            controller->UpdateLeds(std::vector<RGBColor>(colors));
            break;
        case 1:
        case 2:
        case 3:
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
            controller->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed, modes[active_mode].brightness);
            break;
        case 4:
        case 5:
            controller->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed, modes[active_mode].brightness);
            break;
    }
}

void RGBController_AuraHeadsetStand::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
