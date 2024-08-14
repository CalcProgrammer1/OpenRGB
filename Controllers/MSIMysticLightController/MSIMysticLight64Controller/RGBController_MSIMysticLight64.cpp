/*---------------------------------------------------------*\
| RGBController_MSIMysticLight64.cpp                        |
|                                                           |
|   RGBController for MSI Mystic Light 64-byte motherboard  |
|                                                           |
|   T-bond                                      03 Apr 2020 |
|   Adam Honse                                  06 Mar 2021 |
|   Elchanan Haas                               23 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MSIMysticLight64.h"

/**------------------------------------------------------------------*\
    @name MSI GL66 Mystic Light Keyboard (64 Byte)
    @category Keyboard
    @type USB
    @save :robot:
    @effects :white_check_mark:
    @detectors DetectMSIMysticLight64Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSIMysticLight64::RGBController_MSIMysticLight64
(
    MSIMysticLight64Controller *controller_ptr
)
{
    controller  = controller_ptr;
    name        = controller->GetDeviceName();
    vendor      = "MSI";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "MSI Mystic Light Device (64-byte)";
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();
    SetupZones();
}

RGBController_MSIMysticLight64::~RGBController_MSIMysticLight64()
{
    delete controller;
}

void RGBController_MSIMysticLight64::ResizeZone
(
    int /*zone*/,
    int /*new_size*/
)
{
}

void RGBController_MSIMysticLight64::SetupZones()
{
    zone msi_zone;
    msi_zone.name       = "MSI Zone";
    msi_zone.type       = ZONE_TYPE_SINGLE;
    msi_zone.leds_min   = 1;
    msi_zone.leds_max   = 1;
    msi_zone.leds_count = 1;
    msi_zone.matrix_map = NULL;
    zones.push_back(msi_zone);

    led msi_led;
    msi_led.name = "MSI LED";
    leds.push_back(msi_led);
    SetupModes();
    SetupColors();
}

void RGBController_MSIMysticLight64::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMysticLight64::DeviceUpdateLEDs()
{
    mode &Mode = modes[active_mode];
    MSI_64_MODE msi_mode = (MSI_64_MODE)Mode.value;
    MSI_SPEED speed = (MSI_SPEED)Mode.speed;
    MSI_BRIGHTNESS brightness = (MSI_BRIGHTNESS)(Mode.brightness);
    Color led_colors[MSI_64_MAX_COLORS] = {};
    unsigned int num_colors = 0;
    if(Mode.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        num_colors = (unsigned int)Mode.colors.size();
        for(unsigned int i = 0; i < num_colors; i++)
        {
            led_colors[i].R = RGBGetRValue(Mode.colors[i]);
            led_colors[i].G = RGBGetGValue(Mode.colors[i]);
            led_colors[i].B = RGBGetBValue(Mode.colors[i]);
        }
    }
    controller->SetMode(msi_mode, speed, brightness, num_colors, led_colors);
}

void RGBController_MSIMysticLight64::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMysticLight64::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIMysticLight64::SetupModes()
{
    unsigned int TRANSITION=MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SetupMode("Off", MSI_64_MODE::MSI_64_OFF, 0);
    SetupMode("Static",                MSI_64_MODE::MSI_64_STEADY,         MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR);
    SetupMode("Breathing",             MSI_64_MODE::MSI_64_BREATHING,      TRANSITION);
    SetupMode("Flashing",              MSI_64_MODE::MSI_64_PULSE,          TRANSITION);
    SetupMode("Double Flashing",       MSI_64_MODE::MSI_64_DOUBLE_PULSE,   TRANSITION);
    SetupMode("Spectrum Cycle",        MSI_64_MODE::MSI_64_CYCLE,          TRANSITION);
    SetupMode("Smooth Spectrum Cycle", MSI_64_MODE::MSI_64_SMOOTH_CYCLE,   TRANSITION);
}
void RGBController_MSIMysticLight64::SetupMode
(
    const char *name,
    MSI_64_MODE mod,
    unsigned int flags
)
{
    mode Mode;
    Mode.name = name;
    Mode.value  = mod;
    Mode.flags  = flags;
    if(flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        Mode.brightness_min = MSI_BRIGHTNESS_LEVEL_10;
        Mode.brightness_max = MSI_BRIGHTNESS_LEVEL_100;
        Mode.brightness     = MSI_BRIGHTNESS_LEVEL_100;
    }
    if(flags & MODE_FLAG_HAS_SPEED)
    {
        Mode.speed_min = MSI_SPEED_LOW;
        Mode.speed_max = MSI_SPEED_HIGH;
        Mode.speed     = MSI_SPEED_LOW;
    }
    if(flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        Mode.color_mode= MODE_COLORS_MODE_SPECIFIC;
        Mode.colors_min = 1;
        Mode.colors_max = 1;
        if (flags & MODE_FLAG_HAS_SPEED)
        {
            Mode.colors_max = MSI_64_MAX_COLORS;
        }
    /*-------------------------------------------------*\
    | Set up colors for rainbow cycle                   |
    \*-------------------------------------------------*/
        Mode.colors.push_back(0x000000FF);
        Mode.colors.push_back(0x000050FF);
        Mode.colors.push_back(0x0000FFFF);
        Mode.colors.push_back(0x0000FF00);
        Mode.colors.push_back(0x00FF0000);
        Mode.colors.push_back(0x00FF0096);
        Mode.colors.push_back(0x00FF00FF);
    }
    modes.push_back(Mode);
}
