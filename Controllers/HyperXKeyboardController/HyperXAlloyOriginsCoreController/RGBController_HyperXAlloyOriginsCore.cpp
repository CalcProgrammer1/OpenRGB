/*---------------------------------------------------------*\
| RGBController_HyperXAlloyOriginsCore.cpp                  |
|                                                           |
|   RGBController for HyperX Alloy Origins Core keyboard    |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|   carlos jordao                               15 Mar 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_HyperXAlloyOriginsCore.h"
#include "KeyboardLayoutManager.h"

using namespace std::chrono_literals;

#define HYPERX_MIN_BRIGHTNESS   0
#define HYPERX_MAX_BRIGHTNESS 255

#define NA  0xFFFFFFFF

/*----------------------------------*\
|  Maps LED position number to keys  |
|  * based on ANSI QWERTY            |
\*----------------------------------*/
static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};


/*--------------------------------------------------------------------------------*\
| This keyboard (TKL) always receives 94 led colors.                               |
| * Some indexes are just blank (unused).                                          |
| * Regional layouts have a few different enabled or disabled led indexes.         |
| * Below there is the association of the led indexes and the keyboard keys for    |
|   DEFAULT layout.                                                                |
\*--------------------------------------------------------------------------------*/

std::vector<unsigned int> hyperx_core_default_values
{
     0,        1,   2,   3,   4,   5,   6,   7,  48,       49,  50,  51,  52,       53,  54,  55,
     8,   9,  10,  11,  12,  13,  14,  15,  16,  56,  57,  58,  59,  60,            61,  62,  63,
    17,       18,  19,  20,  21,  22,  23,  24,  64,  65,  66,  67,  68,  69,       70,  71,  72,
    25,       26,  27,  28,  29,  30,  31,  32,  73,  74,  75,  76,  77,  78,
    33,  34,  35,  36,  37,  38,  39,  40,  79,  80,  81,  82,       84,                 85,
    41,  42,  43,                 45,                      86,  87,  88,  89,       90,  91,  92,
};

layout_values hyperx_core_layout
{
    hyperx_core_default_values,
    {
    },
};

/*--------------------------------------------*\
| Provide values to keys that has been changed |
| in DEFAULT layout.                           |
\*--------------------------------------------*/
std::map<KEYBOARD_LAYOUT, key_set> regional_overlay_abnt2
{
    {
        KEYBOARD_LAYOUT_ABNT2,
        {
            /*-------------------------------------------------------------------------------------------------------------------------------------*\
            | Edit Keys                                                                                                                             |
            |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
            \*-------------------------------------------------------------------------------------------------------------------------------------*/
            {   0,      4,      11,        82,          KEY_EN_SEMICOLON,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
            {   0,      4,      12,        83,          KEY_EN_FORWARD_SLASH,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        },
    }
};

/**------------------------------------------------------------------*\
    @name HyperX Alloy Origins Core
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXAlloyOriginsCore
    @comment
\*-------------------------------------------------------------------*/

RGBController_HyperXAlloyOriginsCore::RGBController_HyperXAlloyOriginsCore(HyperXAlloyOriginsCoreController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "HyperX Alloy Origins Core Keyboard Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();
    variant                 = controller->GetVariant();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFFFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min   = HYPERX_MIN_BRIGHTNESS;
    Direct.brightness_max   = HYPERX_MAX_BRIGHTNESS;
    Direct.brightness       = HYPERX_MAX_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    /*-----------------------------------------------------*\
    | The HyperX Origins Core requires a packet within few  |
    | seconds of sending the lighting change in order to    |
    | not revert back into current profile.  Start a thread |
    | to continuously send color values each 10ms           |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&RGBController_HyperXAlloyOriginsCore::KeepaliveThread, this);
}

RGBController_HyperXAlloyOriginsCore::~RGBController_HyperXAlloyOriginsCore()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_HyperXAlloyOriginsCore::SetupZones()
{
    unsigned int total_leds = 0;
    zone new_zone;
    KEYBOARD_LAYOUT layout_name;

    for(unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        new_zone.name  = zone_names[zone_idx];
        new_zone.type  = zone_types[zone_idx];

        /*-----------------------------------------------------*\
        |   Regional configuration                              |
        |   * variant is extracted from keyboard info           |
        \*-----------------------------------------------------*/
        switch(variant)
        {
            case HYPERX_ALLOY_ORIGINS_CORE_ABNT2:
                layout_name                         = KEYBOARD_LAYOUT_ABNT2;
                hyperx_core_layout.regional_overlay = regional_overlay_abnt2;
                break;

            case HYPERX_ALLOY_ORIGINS_CORE_ANSI:
            default:
                layout_name                         = KEYBOARD_LAYOUT_ANSI_QWERTY;
                break;
        }
        KeyboardLayoutManager new_kb(layout_name, KEYBOARD_SIZE_TKL, hyperx_core_layout);

        total_leds = new_kb.GetKeyCount();

        matrix_map_type * keyboard_map  = new matrix_map_type;
        new_zone.leds_count             = total_leds;
        new_zone.leds_min               = new_zone.leds_count;
        new_zone.leds_max               = new_zone.leds_count;

        if(zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = keyboard_map;
            keyboard_map->height        = new_kb.GetRowCount();
            keyboard_map->width         = new_kb.GetColumnCount();
            keyboard_map->map           = new unsigned int[keyboard_map->height * keyboard_map->width];

            new_kb.GetKeyMap(keyboard_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT);
       }
        else
        {
            new_zone.matrix_map         = NULL;
        }
        zones.push_back(new_zone);

        for(unsigned int led_idx = 0; led_idx < total_leds; led_idx++)
        {
            led new_led;
            new_led.name  = new_kb.GetKeyAltNameAt(led_idx);
            if(new_led.name == KEY_EN_UNUSED)
                new_led.name  = new_kb.GetKeyNameAt(led_idx);
            new_led.value = new_kb.GetKeyValueAt(led_idx);
            leds.push_back(new_led);
        }
    }
    SetupColors();
}

void RGBController_HyperXAlloyOriginsCore::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXAlloyOriginsCore::DeviceUpdateLEDs()
{
    controller->SetLEDsDirect(leds, colors);
}

void RGBController_HyperXAlloyOriginsCore::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOriginsCore::DeviceUpdateMode()
{
    controller->SetBrightness(modes[active_mode].brightness);
}

void RGBController_HyperXAlloyOriginsCore::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                controller->SendRGBToDevice();
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}
