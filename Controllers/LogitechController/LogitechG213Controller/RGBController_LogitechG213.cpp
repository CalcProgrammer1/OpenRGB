/*---------------------------------------------------------*\
| RGBController_LogitechG213.cpp                            |
|                                                           |
|   RGBController for Logitech G203L                        |
|                                                           |
|   Eric Samuelson (edbgon)                     06 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LogitechG213.h"

static const char* led_names[] =
{
    "Left Area",
    "Middle Area",
    "Right Area",
    "Arrow and Homekeys",
    "Numpad",
};

static const unsigned char led_values[] =
{
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
};

#define LOGITECH_G213_ZONES (sizeof(led_values) / sizeof(led_values[ 0 ]))

/**------------------------------------------------------------------*\
    @name Logitech G213
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechKeyboardG213
    @comment
\*-------------------------------------------------------------------*/

RGBController_LogitechG213::RGBController_LogitechG213(LogitechG213Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Logitech";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Logitech G213 Keyboard Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 0xFFFF;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G213_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Cycle;
    Cycle.name                      = "Cycle";
    Cycle.value                     = LOGITECH_G213_MODE_CYCLE;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed_min                 = LOGITECH_G213_SPEED_SLOWEST;
    Cycle.speed_max                 = LOGITECH_G213_SPEED_FASTEST;
    Cycle.speed                     = LOGITECH_G213_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = LOGITECH_G213_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.color_mode                 = MODE_COLORS_NONE;
    Wave.speed_min                  = LOGITECH_G213_SPEED_SLOWEST;
    Wave.speed_max                  = LOGITECH_G213_SPEED_FASTEST;
    Wave.speed                      = LOGITECH_G213_SPEED_NORMAL;
    Wave.direction                  = MODE_DIRECTION_LEFT;
    modes.push_back(Wave);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = LOGITECH_G213_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed_min             = LOGITECH_G213_SPEED_SLOWEST;
    Breathing.speed_max             = LOGITECH_G213_SPEED_FASTEST;
    Breathing.speed                 = LOGITECH_G213_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechG213::~RGBController_LogitechG213()
{
    delete controller;
}

void RGBController_LogitechG213::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_LINEAR;
    new_zone.leds_min               = 5;
    new_zone.leds_max               = 5;
    new_zone.leds_count             = 5;

    new_zone.matrix_map             = NULL;

    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < LOGITECH_G213_ZONES; led_idx++)
    {
        led new_led;
        new_led.name                = led_names[led_idx];
        new_led.value               = led_values[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_LogitechG213::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechG213::DeviceUpdateLEDs()
{
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        controller->SetDirect((unsigned char)leds[led_idx].value, RGBGetRValue(colors[led_idx]), RGBGetGValue(colors[led_idx]), RGBGetBValue(colors[led_idx]));
    }
}

void RGBController_LogitechG213::UpdateZoneLEDs(int zone)
{
    controller->SetDirect((unsigned char) zone, RGBGetRValue(zones[zone].colors[0]), RGBGetGValue(zones[zone].colors[0]), RGBGetBValue(zones[zone].colors[0]));
}

void RGBController_LogitechG213::UpdateSingleLED(int led)
{
    controller->SetDirect(leds[led].value, RGBGetRValue(colors[led]), RGBGetGValue(colors[led]), RGBGetBValue(colors[led]));
}

void RGBController_LogitechG213::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | Direct mode does not send a mode packet                   |
    | Call UpdateLEDs to send direct packet                     |
    \*---------------------------------------------------------*/
    if(active_mode == 0xFFFF)
    {
        UpdateLEDs();
        return;
    }

    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    unsigned char direction = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    switch (modes[active_mode].direction)
    {
        case MODE_DIRECTION_LEFT:
            // Right to left
            direction = LOGITECH_G213_WAVE_MODE_LEFT;
            break;
        case MODE_DIRECTION_RIGHT:
            // Left to right
            direction = LOGITECH_G213_WAVE_MODE_RIGHT;
            break;
        case MODE_DIRECTION_UP:
            // Edge to center
            direction = LOGITECH_G213_WAVE_MODE_EDGE_CENTER;
            break;
        case MODE_DIRECTION_DOWN:
            // Center to edge
            direction = LOGITECH_G213_WAVE_MODE_CENTER_EDGE;
            break;
    }

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, direction, red, grn, blu);
}
