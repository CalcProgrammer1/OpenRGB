/*---------------------------------------------------------*\
| RGBController_LianLiUniversalScreen.cpp                   |
|                                                           |
|   RGBController for Lian Li 8.8" Universal Screen LEDs    |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      17 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_LianLiUniversalScreen.h"

/**------------------------------------------------------------------*\
    @name Lian Li Universal Screen
    @category Monitor
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLianLiUniversalScreenControllers
    @comment Only controls the LEDs around the screen, not the screen
        itself.
\*-------------------------------------------------------------------*/

RGBController_LianLiUniversalScreen::RGBController_LianLiUniversalScreen(LianLiUniversalScreenController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "Lian Li Universal Screen";
    type                = DEVICE_TYPE_MONITOR;
    vendor              = "Lian Li";
    description         = "Lian Li Universal Screen Device";
    location            = controller->GetLocation();
    serial              = controller->GetSerial();
    version             = controller->GetVersion();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

void RGBController_LianLiUniversalScreen::SetupZones()
{
    zone Screen;
    Screen.name         = "Screen Lighting";
    Screen.type         = ZONE_TYPE_LINEAR;
    Screen.leds_min     = 60;
    Screen.leds_max     = 60;
    Screen.leds_count   = 60;
    Screen.matrix_map   = NULL;
    zones.push_back(Screen);

    for(std::size_t led_idx = 0; led_idx < Screen.leds_count; led_idx++)
    {
        led ScreenLED;

        ScreenLED.name  = "Screen Lighting LED " + std::to_string(led_idx);
        ScreenLED.value = 0;

        leds.push_back(ScreenLED);
    }

    SetupColors();
}

void RGBController_LianLiUniversalScreen::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_LianLiUniversalScreen::DeviceUpdateLEDs()
{
    controller->SetLedColors(colors.data(), colors.size());
}

void RGBController_LianLiUniversalScreen::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LianLiUniversalScreen::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LianLiUniversalScreen::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
