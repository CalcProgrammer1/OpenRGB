/*---------------------------------------------------------*\
| RGBController_HYTEKeyboard.cpp                            |
|                                                           |
|   RGBController for HYTE keyboard                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      30 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "KeyboardLayoutManager.h"
#include "RGBController_HYTEKeyboard.h"

/*---------------------------------------------------------------------*\
|  HYTE Keeb TKL KLM Layout                                             |
\*---------------------------------------------------------------------*/
const std::vector<unsigned int> hyte_keeb_tkl_values =
{
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK */
        0,           2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,    15,   16,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  INS   HOME  PGUP */
        21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,    36,   37,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   DEL    END  PGDN */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,    57,   58,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                   */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
    /* LSFT    \     Z     X     C     V     B     N     M     ,     .     /   RSFT              ARWU       */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,   97,                99,
    /* LCTL  LWIN  LALT               SPC              RALT  RFNC  RMNU  RCTL              ARWL ARWD   ARWR */
        105,  106,  107,              111,              115,  116,  117,  118,              119,  120,   121,
};

keyboard_keymap_overlay_values hyte_keeb_tkl_layout =
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        hyte_keeb_tkl_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys - Add additional LEDs for space bar underglow and media keys                                                                |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      5,      4,          109,        KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      5,      5,          110,        KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      5,      7,          112,        KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      5,      8,          113,        KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      0,          77,         KEY_EN_MEDIA_STOP,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_ROW,         },
        {   0,      0,      1,          78,         KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      2,          79,         KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      3,          98,         KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      4,          100,        KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
    }
};

RGBController_HYTEKeyboard::RGBController_HYTEKeyboard(HYTEKeyboardController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "HYTE";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "HYTE Keyboard Device";
    location            = controller->GetDeviceLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HYTEKeyboard::~RGBController_HYTEKeyboard()
{
    delete controller;
}

void RGBController_HYTEKeyboard::SetupZones()
{
    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_DEFAULT, hyte_keeb_tkl_layout.base_size, hyte_keeb_tkl_layout.key_values);
    new_kb.ChangeKeys(hyte_keeb_tkl_layout.edit_keys);

    zone keyboard_zone;

    keyboard_zone.name                  = "Keyboard";
    keyboard_zone.type                  = ZONE_TYPE_MATRIX;

    matrix_map_type * keyboard_map      = new matrix_map_type;
    keyboard_zone.matrix_map            = keyboard_map;
    keyboard_zone.matrix_map->height    = new_kb.GetRowCount();
    keyboard_zone.matrix_map->width     = new_kb.GetColumnCount();

    keyboard_zone.matrix_map->map       = new unsigned int[keyboard_map->height * keyboard_map->width];
    keyboard_zone.leds_count            = new_kb.GetKeyCount();
    keyboard_zone.leds_min              = keyboard_zone.leds_count;
    keyboard_zone.leds_max              = keyboard_zone.leds_count;

    zones.push_back(keyboard_zone);

    /*---------------------------------------------------------*\
    | Matrix map still uses declared zone rows and columns      |
    |   as the packet structure depends on the matrix map       |
    \*---------------------------------------------------------*/
    new_kb.GetKeyMap(keyboard_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, keyboard_map->height, keyboard_map->width);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < keyboard_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = new_kb.GetKeyNameAt(led_idx);
        new_led.value               = new_kb.GetKeyValueAt(led_idx);

        leds.push_back(new_led);
    }

    zone surround_zone;

    surround_zone.name          = "Underglow";
    surround_zone.type          = ZONE_TYPE_LINEAR;
    surround_zone.leds_min      = 63;
    surround_zone.leds_max      = 63;
    surround_zone.leds_count    = 63;
    surround_zone.matrix_map    = NULL;

    zones.push_back(surround_zone);

    for(unsigned int led_idx = 0; led_idx < surround_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name            = surround_zone.name;

        leds.push_back(new_led);
    }


    SetupColors();
}

void RGBController_HYTEKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HYTEKeyboard::DeviceUpdateLEDs()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_HYTEKeyboard::UpdateZoneLEDs(int zone)
{
    if(zone == HYTE_KEYBOARD_ZONE_KEYBOARD)
    {
        RGBColor color_buf[127];

        for(unsigned int led_idx = 0; led_idx < zones[HYTE_KEYBOARD_ZONE_KEYBOARD].leds_count; led_idx++)
        {
            color_buf[leds[led_idx].value] = colors[led_idx];
        }

        controller->LEDStreaming(zone, &color_buf[0]);
    }
    else
    {
        controller->LEDStreaming(zone, zones[zone].colors);
    }
}

void RGBController_HYTEKeyboard::UpdateSingleLED(int led)
{
    if(led < (int)zones[0].leds_count)
    {
        UpdateZoneLEDs(0);
    }
    else
    {
        UpdateZoneLEDs(1);
    }
}

void RGBController_HYTEKeyboard::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
