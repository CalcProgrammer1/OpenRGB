/*---------------------------------------------------------*\
| RGBController_LogitechGPowerPlay.cpp                      |
|                                                           |
|   RGBController for Logitech G PowerPlay                  |
|                                                           |
|   TheRogueZeta                                31 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LogitechGPowerPlay.h"

/**------------------------------------------------------------------*\
    @name Logitech Powerplay Mat
    @category Mousemat
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechGPowerPlay::RGBController_LogitechGPowerPlay(LogitechGLightsyncController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Logitech";
    type                    = DEVICE_TYPE_MOUSEMAT;
    description             = "Logitech G PowerPlay Wireless Charging System";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Off;
    Off.name                = "Off";
    Off.value               = LOGITECH_G_LIGHTSYNC_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = LOGITECH_G_LIGHTSYNC_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Cycle;
    Cycle.name              = "Spectrum Cycle";
    Cycle.value             = LOGITECH_G_LIGHTSYNC_MODE_CYCLE;
    Cycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.color_mode        = MODE_COLORS_NONE;
    Cycle.speed_min         = LOGITECH_G_LIGHTSYNC_SPEED_SLOWEST;
    Cycle.speed_max         = LOGITECH_G_LIGHTSYNC_SPEED_FASTEST;
    Cycle.speed             = LOGITECH_G_LIGHTSYNC_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = LOGITECH_G_LIGHTSYNC_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = LOGITECH_G_LIGHTSYNC_SPEED_SLOWEST;
    Breathing.speed_max     = LOGITECH_G_LIGHTSYNC_SPEED_FASTEST;
    Breathing.speed         = LOGITECH_G_LIGHTSYNC_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechGPowerPlay::~RGBController_LogitechGPowerPlay()
{
    delete controller;
}

void RGBController_LogitechGPowerPlay::SetupZones()
{
    zone GPowerPlay_logo_zone;
    GPowerPlay_logo_zone.name           = "Logo";
    GPowerPlay_logo_zone.type           = ZONE_TYPE_SINGLE;
    GPowerPlay_logo_zone.leds_min       = 1;
    GPowerPlay_logo_zone.leds_max       = 1;
    GPowerPlay_logo_zone.leds_count     = 1;
    GPowerPlay_logo_zone.matrix_map     = NULL;
    zones.push_back(GPowerPlay_logo_zone);

    led GPowerPlay_logo_led;
    GPowerPlay_logo_led.name            = "Logo";
    leds.push_back(GPowerPlay_logo_led);

    SetupColors();
}

void RGBController_LogitechGPowerPlay::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechGPowerPlay::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_LogitechGPowerPlay::UpdateZoneLEDs(int zone)
{
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    /*---------------------------------------------------------*\
    | Replace direct mode with static when sending to controller|
    \*---------------------------------------------------------*/
    unsigned char temp_mode = (modes[active_mode].value != 0xFF) ? modes[active_mode].value : LOGITECH_G_LIGHTSYNC_MODE_STATIC;

    controller->UpdateMouseLED(temp_mode, modes[active_mode].speed, zone, red, grn, blu, /* Brightness */ 0x64);
}

void RGBController_LogitechGPowerPlay::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechGPowerPlay::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | If direct mode is true, then sent the packet to put the   |
    | mouse in direct mode.  This code will only be called when |
    | we change modes as to not spam the device.                |
    \*---------------------------------------------------------*/
    controller->SetDirectMode(modes[active_mode].value == 0xFF);
    DeviceUpdateLEDs();
}
