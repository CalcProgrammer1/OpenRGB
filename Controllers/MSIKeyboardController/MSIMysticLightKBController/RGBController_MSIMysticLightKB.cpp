/*---------------------------------------------------------*\
| RGBController_MSIMysticLightKB.cpp                        |
|                                                           |
|   Driver for MSI Mystic Light MS-1565 keyboard leds       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MSIMysticLightKB.h"
#include "MSIMysticLightKBController.h"
#include "RGBController.h"

#include <cstddef>

/**------------------------------------------------------------------*\
    @name MSI MS-1565 Mystic Light Keyboard (64 Byte)
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMSIMysticLight1565Controller
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSIKeyboard::RGBController_MSIKeyboard
(
    MSIKeyboardController *controller_ptr
)
{
    controller  = controller_ptr;
    name        = controller->GetDeviceName();
    vendor      = "MSI";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "MSI Mystic Light MS-1565";
    version     = controller->GetFWVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();
    SetupModes();
    SetupColors();
}

RGBController_MSIKeyboard::~RGBController_MSIKeyboard()
{
    delete controller;
}

void RGBController_MSIKeyboard::ResizeZone
(
    int /*zone*/,
    int /*new_size*/
)
{
}

void RGBController_MSIKeyboard::SetupZones()
{
}

void RGBController_MSIKeyboard::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_MSIKeyboard::DeviceUpdateLEDs()
{
    mode &Mode = modes[active_mode];
    MS_1565_MODE msi_mode = (MS_1565_MODE)Mode.value;
    MS_1565_ZONE zone = controller->mode_zones[active_mode];

    /*----------------------------------*\
    | speed is cycle duration in 1/100s  |
    | Mode.speed = 0 % => speed = 12.00s |
    | Mode.speed = 50 % => speed = 7.50s |
    | Mode.speed = 100 % => speed = 3.00 |
    \*--------------------------------- */

    unsigned int speed = 1200 - 9 * Mode.speed;
    unsigned char speed2 =  (unsigned char)(speed & 0xFF);
    unsigned char speed1 =  (unsigned char)((speed & 0xFF00) >> 8);

    MS_1565_WAVE_DIRECTION wave_direction = (MS_1565_WAVE_DIRECTION)(Mode.direction);

    const size_t colors_size = Mode.colors.size();

    ColorKeyFrame ck[MAX_MS_1565_KEYFRAMES] = {};

    for(size_t idx = 0; idx < colors_size; idx++)
    {
        ck[idx].time_frame = idx * 100 / colors_size;
        ck[idx].color.R = RGBGetRValue(Mode.colors[idx]) * Mode.brightness / 100;
        ck[idx].color.G = RGBGetGValue(Mode.colors[idx]) * Mode.brightness / 100;
        ck[idx].color.B = RGBGetBValue(Mode.colors[idx]) * Mode.brightness / 100;
    }
    ck[colors_size].time_frame = 100;
    ck[colors_size].color.R = RGBGetRValue(Mode.colors[0]) * Mode.brightness / 100;
    ck[colors_size].color.G = RGBGetGValue(Mode.colors[0]) * Mode.brightness / 100;
    ck[colors_size].color.B = RGBGetBValue(Mode.colors[0]) * Mode.brightness / 100;

    controller->SetMode(msi_mode, speed1, speed2, wave_direction, zone, ck);
}

void RGBController_MSIKeyboard::UpdateZoneLEDs(int /*zone*/)
{
}

void RGBController_MSIKeyboard::UpdateSingleLED(int /*led*/)
{
}

void RGBController_MSIKeyboard::SetupModes()
{
    SetupZonesMode("Off",         MS_1565_MODE::MS_1565_OFF,       0);
    SetupZonesMode("Static",      MS_1565_MODE::MS_1565_STEADY,    MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                      MODE_FLAG_HAS_BRIGHTNESS);
    SetupZonesMode("Breathing",   MS_1565_MODE::MS_1565_BREATHING, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                      MODE_FLAG_HAS_SPEED |
                                                                                      MODE_FLAG_HAS_BRIGHTNESS);
    SetupZonesMode("Color Cycle", MS_1565_MODE::MS_1565_CYCLE,     MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                      MODE_FLAG_HAS_SPEED |
                                                                                      MODE_FLAG_HAS_BRIGHTNESS);
    SetupZonesMode("Wave",        MS_1565_MODE::MS_1565_WAVE,      MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                      MODE_FLAG_HAS_SPEED |
                                                                                      MODE_FLAG_HAS_BRIGHTNESS |
                                                                                      MODE_FLAG_HAS_DIRECTION_LR);
}

void RGBController_MSIKeyboard::SetupMode
(
    const std::string name,
    MS_1565_MODE mod,
    unsigned int flags,
    MS_1565_ZONE zone
)
{
    mode Mode;
    Mode.name = name;
    Mode.value  = mod;
    Mode.flags  = flags;

    if(Mode.value == MS_1565_MODE::MS_1565_OFF)
    {
        Mode.color_mode= MODE_COLORS_NONE;
        Mode.colors_min = 0;
        Mode.colors_max = 0;
        Mode.colors.resize(1);
        modes.push_back(Mode);
        controller->mode_zones.push_back(zone);
        return;
    }

    if(flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        Mode.color_mode= MODE_COLORS_MODE_SPECIFIC;
        Mode.colors_min = 1;
        if(Mode.value == MS_1565_MODE::MS_1565_STEADY)
        {
            Mode.colors_max = 1;
        }
        else
        {
            Mode.colors_max = MAX_MS_1565_KEYFRAMES - 1;
        }
        Mode.colors.resize(Mode.colors_max);
    }

    if(flags & MODE_FLAG_HAS_SPEED)
    {
        Mode.speed_min = 0;
        Mode.speed_max = 100;
        Mode.speed     = 50;
    }

    if(flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        Mode.brightness_min = 0;
        Mode.brightness_max = 100;
    }
    Mode.brightness = 100;

    if(flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        Mode.direction     = 0;
    }

    modes.push_back(Mode);
    controller->mode_zones.push_back(zone);
}

void RGBController_MSIKeyboard::SetupZonesMode
(
    const std::string name,
    MS_1565_MODE mod,
    unsigned int flags
)
{
    SetupMode(name, mod, flags, MS_1565_ZONE_DEVICE);
    for(int idx = 0; idx < 4; idx++)
    {
        SetupMode(name + " zone " + std::to_string(idx + 1), mod, flags, MS_1565_ZONE(idx + 1));
    }
}
