/*---------------------------------------------------------*\
| RGBController_MNTKeyboard.cpp                             |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_MNTKeyboard.h"

void RGBController_MNTKeyboard::CommonInit()
{
    vendor   = "MNT Research";
    type     = DEVICE_TYPE_KEYBOARD;
    location = controller->location;
    modes.resize(1);
    modes[0].name       = "Direct";
    modes[0].flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    modes[0].color_mode = MODE_COLORS_PER_LED;
    SetupZones();
    SetAllLEDs(ToRGBColor(255, 255, 255));
    DeviceUpdateLEDs();
}

RGBController_MNTKeyboard::~RGBController_MNTKeyboard()
{
    delete zones[0].matrix_map;
    delete controller;
}

void RGBController_MNTKeyboard::SetupZones()
{
    zone new_zone;
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_count         = KBD_ROWS * controller->kbd_cols;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = KBD_ROWS;
    new_zone.matrix_map->width  = controller->kbd_cols;
    new_zone.matrix_map->map    = (unsigned int *)matrix_keys;
    zones.push_back(new_zone);
    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names[led_idx];
        leds.push_back(new_led);
    }
    SetupColors();
}

void RGBController_MNTKeyboard::DeviceUpdateLEDs()
{
    unsigned char *color_map = new unsigned char[zones[0].leds_count * KBD_COLOR_SIZE];
    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        RGBColor color        = colors[led_idx];
        int offset            = led_idx * KBD_COLOR_SIZE;
        color_map[offset + 2] = RGBGetRValue(color);
        color_map[offset + 1] = RGBGetGValue(color);
        color_map[offset + 0] = RGBGetBValue(color);
    }
    controller->SendColorMatrix(color_map);
    delete[] color_map;
}

void RGBController_MNTKeyboard::ResizeZone(int, int)
{
}
void RGBController_MNTKeyboard::UpdateZoneLEDs(int)
{
    DeviceUpdateLEDs();
}
void RGBController_MNTKeyboard::UpdateSingleLED(int)
{
    DeviceUpdateLEDs();
}
void RGBController_MNTKeyboard::DeviceUpdateMode()
{
}
