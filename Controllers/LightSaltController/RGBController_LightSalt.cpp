/*---------------------------------------------------------*\
| RGBController_LightSalt.cpp                               |
|                                                           |
|   RGBController for LightSalt Peripherals                 |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "RGBControllerKeyNames.h"
#include "RGBController_LightSalt.h"

static RGBColor DeflectColor(bool deflection, RGBColor color)
{
    if(deflection)
    {
        int old_red = RGBGetRValue(color);
        int old_grn = RGBGetGValue(color);
        int old_blu = RGBGetBValue(color);
        int new_red = old_red + ((255 - old_red) * 50 / 100);
        int new_grn = old_grn + ((255 - old_grn) * 50 / 100);
        int new_blu = old_blu + ((255 - old_blu) * 50 / 100);
        return ToRGBColor(new_red, new_grn, new_blu);
    }
    else
    {
        return color;
    }
}

RGBController_LightSalt::~RGBController_LightSalt()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != nullptr)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_LightSalt::SetupData(LightSaltController* controller_ptr)
{
    controller  = controller_ptr;
    name        = "LightSalt " + table.device.name;
    vendor      = "LightSalt";
    type        = table.device.type;
    description = "LightSalt " + table.device.name + " (" + controller->GetDeviceModel() + ")";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerial();
    version     = controller->GetFirmwareVersion();
}

void RGBController_LightSalt::SetupModes()
{
    {
        mode mode;
        mode.name           = "Custom";
        mode.value          = LIGHTSALT_MODE_CUSTOM;
        mode.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_PER_LED;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Point";
        mode.value          = LIGHTSALT_MODE_POINT;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Area";
        mode.value          = LIGHTSALT_MODE_AREA;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Collapse";
        mode.value          = LIGHTSALT_MODE_COLLAPSE;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Expand";
        mode.value          = LIGHTSALT_MODE_EXPAND;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Explode";
        mode.value          = LIGHTSALT_MODE_EXPLODE;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Dart";
        mode.value          = LIGHTSALT_MODE_DART;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Flame";
        mode.value          = LIGHTSALT_MODE_FLAME;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Reactive Laser";
        mode.value          = LIGHTSALT_MODE_LASER;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Breathing";
        mode.value          = LIGHTSALT_MODE_BREATHING;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_HORIZONTAL;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Tricolor Radar";
        mode.value          = LIGHTSALT_MODE_TRICOLOR_RADAR;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_LEFT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Wheel 1";
        mode.value          = LIGHTSALT_MODE_WHEEL_1;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_LEFT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Wheel 2";
        mode.value          = LIGHTSALT_MODE_WHEEL_2;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_LEFT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Wave 1";
        mode.value          = LIGHTSALT_MODE_WAVE_1;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_LEFT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Wave 2";
        mode.value          = LIGHTSALT_MODE_WAVE_2;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_UP;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Wave 3";
        mode.value          = LIGHTSALT_MODE_WAVE_3;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_UP;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Rainbow 1";
        mode.value          = LIGHTSALT_MODE_RAINBOW_1;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_HORIZONTAL;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }

    {
        mode mode;
        mode.name           = "Rainbow 2";
        mode.value          = LIGHTSALT_MODE_RAINBOW_2;
        mode.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        mode.speed_min      = LIGHTSALT_SPEED_MINIMUM;
        mode.speed_max      = LIGHTSALT_SPEED_MAXIMUM;
        mode.speed          = LIGHTSALT_SPEED_DEFAULT;
        mode.direction      = MODE_DIRECTION_LEFT;
        mode.brightness_min = LIGHTSALT_BRIGHTNESS_MINIMUM;
        mode.brightness_max = LIGHTSALT_BRIGHTNESS_MAXIMUM;
        mode.brightness     = LIGHTSALT_BRIGHTNESS_DEFAULT;
        mode.color_mode     = MODE_COLORS_NONE;
        modes.push_back(mode);
    }
}

void RGBController_LightSalt::SetupZones()
{
    {
        zone zone;
        zone.name               = table.device.name;
        zone.type               = ZONE_TYPE_MATRIX;
        zone.leds_min           = table.led.count;
        zone.leds_max           = table.led.count;
        zone.leds_count         = table.led.count;
        zone.matrix_map         = new matrix_map_type;
        zone.matrix_map->height = table.map.height;
        zone.matrix_map->width  = table.map.width;
        zone.matrix_map->map    = table.map.matrix;
        zones.push_back(zone);
    }

    for(int led_idx = 0; led_idx < table.led.count; led_idx++)
    {
        led led;
        led.name = table.led.names[led_idx];
        leds.push_back(led);
    }

    {
        zone zone;
        zone.name       = "Color Filter";
        zone.type       = ZONE_TYPE_SINGLE;
        zone.leds_min   = 1;
        zone.leds_max   = 1;
        zone.leds_count = 1;
        zone.matrix_map = NULL;
        zones.push_back(zone);
    }

    {
        led led;
        led.name = "Color Filter";
        leds.push_back(led);
    }

    SetupColors();

    colors[colors.size() - 1] = ToRGBColor(0xFF, 0xFF, 0xFF);
}

void RGBController_LightSalt::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LightSalt::DeviceUpdateColors(bool save)
{
    int sets    = table.led.sets;
    int rows    = table.led.rows;
    int columns = table.led.columns;

    RGBColor colors_data[LIGHTSALT_SETS_MAX][LIGHTSALT_ROWS_MAX][LIGHTSALT_COLUMNS_MAX];
    memset(colors_data, 0x00, sizeof(colors_data));

    for(int led_idx = 0; led_idx < table.led.count; led_idx++)
    {
        int      index      = table.led.indices[led_idx];
        int      deflection = table.led.deflections[led_idx];
        int      row        = index / columns;
        int      column     = index % columns;
        RGBColor color      = colors[led_idx];

        for(int set_idx = 0; set_idx < sets; set_idx++)
        {
            colors_data[set_idx][row][column] = DeflectColor(deflection & (1 << set_idx), color);
        }
    }

    controller->SetFilter(colors[colors.size() - 1]);
    controller->SetColors((RGBColor*)colors_data, sets, rows, columns);
    controller->ApplyColors(sets);
    if(save)
    {
        controller->SaveColors(sets);
    }
}

void RGBController_LightSalt::DeviceUpdateLEDs()
{
    DeviceUpdateColors(false);
}

void RGBController_LightSalt::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LightSalt::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LightSalt::DeviceUpdateMode()
{
    const mode& mode = modes[active_mode];

    controller->SetBrightness(mode.brightness);
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    controller->SetMode(mode.value, mode.direction, mode.speed);
}

void RGBController_LightSalt::DeviceSaveMode()
{
    DeviceUpdateColors(true);
}
