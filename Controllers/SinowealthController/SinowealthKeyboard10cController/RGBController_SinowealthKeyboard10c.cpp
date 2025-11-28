/*---------------------------------------------------------*\
| RGBController_SinowealthKeyboard10c.cpp                   |
|                                                           |
|   RGBController for Sinowealth Keyboards with PID 010C    |
|                                                           |
|   Rodrigo Tavares                             27 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "KeyboardLayoutManager.h"
#include "RGBControllerKeyNames.h"
#include "SinowealthKeyboard10cController.h"
#include "SinowealthKeyboard10cDevices.h"
#include "RGBController_SinowealthKeyboard10c.h"

using namespace kbd10c;
using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Sinowealth Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSinowealthKeyboard10c
    @comment
\*-------------------------------------------------------------------*/

RGBController_SinowealthKeyboard10c::RGBController_SinowealthKeyboard10c(
    SinowealthKeyboard10cController* controller_ptr, unsigned char model_id)
    : model_id(model_id)
{
    controller = controller_ptr;

    name        = controller->GetName();
    type        = DEVICE_TYPE_KEYBOARD;
    vendor      = "Sinowealth";
    description = "Sinowealth Keyboard Device";
    location    = controller->GetLocation();
    serial      = controller->GetSerialString();

    mode Off;
    Off.name       = "Off";
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    Off.value      = MODE_OFF;
    modes.push_back(Off);

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.value      = MODE_DIRECT;
    modes.push_back(Direct);

    active_mode = MODE_DIRECT;

    SetupZones();

    /*---------------------------------------------------------*\
    | The Sinowealth 010C Keyboard requires a steady stream     |
    | of packets in order to not revert out of direct mode.     |
    | Start a thread to continuously refresh the device         |
    \*---------------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread     = new std::thread(&RGBController_SinowealthKeyboard10c::KeepaliveThreadFunction, this);
}

RGBController_SinowealthKeyboard10c::~RGBController_SinowealthKeyboard10c()
{
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;
    delete controller;
}

void RGBController_SinowealthKeyboard10c::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create the keyboard zone usiung Keyboard Layout Manager   |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name = ZONE_EN_KEYBOARD;
    new_zone.type = ZONE_TYPE_MATRIX;

    sinowealth_device device = sinowealth_10c_keyboards.at(model_id);

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, device.keyboard_layout.base_size,
                                 device.keyboard_layout.key_values);
    new_kb.ChangeKeys(device.keyboard_layout.edit_keys);

    matrix_map_type* new_map    = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = new_kb.GetRowCount();
    new_zone.matrix_map->width  = new_kb.GetColumnCount();

    new_zone.matrix_map->map = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count      = new_kb.GetRowCount() * new_kb.GetColumnCount();
    new_zone.leds_min        = new_zone.leds_count;
    new_zone.leds_max        = new_zone.leds_count;

    /*---------------------------------------------------------*\
    | These keyboards use sparse LED indexes — for example, a   |
    | 99-key board might use LED indexes 0–112, leaving some    |
    | numbers unused. Empty positions are marked 0xFFFFFFFF.    |
    |                                                           |
    | We map each key to its actual LED index, filling the      |
    | `leds` vector by those indexes and leaving gaps where no  |
    | LED exists.                                               |
    \*---------------------------------------------------------*/

    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_VALUE, new_map->height, new_map->width);

    leds.resize(new_zone.leds_count);

    for(unsigned int i = 0, j = 0; i < new_zone.leds_count; i++)
    {
        if(new_map->map[i] == 0xFFFFFFFF)
        {
            continue;
        }

        led new_led;

        new_led.name  = new_kb.GetKeyNameAt(j);
        new_led.value = new_kb.GetKeyValueAt(j);

        leds[new_map->map[i]] = new_led;

        j++;
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_SinowealthKeyboard10c::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SinowealthKeyboard10c::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard10c::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard10c::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard10c::DeviceUpdateMode()
{
}

void RGBController_SinowealthKeyboard10c::KeepaliveThreadFunction()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == MODE_DIRECT && (std::chrono::steady_clock::now() - last_update_time) > 1s)
        {
            UpdateLEDs();
        }
        std::this_thread::sleep_for(500ms);
    }
}
