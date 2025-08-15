/*---------------------------------------------------------*\
| RGBController_SkyloongGK104Pro.cpp                        |
|                                                           |
|   RGBController for Skyloong GK104 Pro                    |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_SkyloongGK104Pro.h"
#include "KeyboardLayoutManager.h"

using namespace std::chrono_literals;

/*---------------------------------------------------------------------*\
|  Skyloong GK104 Pro Keyboard KLM Layout                               |
\*---------------------------------------------------------------------*/
layout_values keyboard_offset_values =
{
    {
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   PRSC  SCLK  PSBK                         */
        0,           2,    3,    4,    5,    7,    8,    9,   10,   11,   12,   13,   14,    15,   16,   17,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC   INS  HOME  PGUP   NLCK   NP/   NP*   NP-  */
        22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   36,    37,   38,   39,    40,   41,   42,  43,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \    DEL   END  PGDN    NP7   NP8   NP9   NP+ */
        44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,   58,    59,   60,   61,    62,   63,   64,  65,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #  ENTR                       NP4   NP5   NP6       */
        66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,                       84,   85,   86,
    /* LSFT    /     Z     X     C     V     B     N     M     ,     .     /  RSFT               ARWU          NP1   NP2   NP3  NPEN */
        88,   89,   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,  102,               104,         106,  107,  108,  109,
    /* LCTL LWIN  LALT         SPC   SPC   SPC        RALT  RFNC  RMNU  RCTL               ARWL  ARWD  ARWR    NP0         NP.       */
       110,  111,  112,              116,              120,  121,  122,  124,               125,  126,  127,   128,        130
    },
    {
        /* Add more regional layout fixes here */
    }
};

/**------------------------------------------------------------------*\
    @name Skyloong GK104 Pro
    @category Keyboard
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :o:
    @detectors SkyloongControllerDetect
    @comment
\*-------------------------------------------------------------------*/

RGBController_SkyloongGK104Pro::RGBController_SkyloongGK104Pro(SkyloongGK104ProController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Skyloong";
    description                 = "Skyloong GK104 Pro Keyboard";
    location                    = controller->GetDeviceLocation();
    type                        = DEVICE_TYPE_KEYBOARD;

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFFFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    Direct.brightness_min       = BRIGHTNESS_MIN;
    Direct.brightness_max       = BRIGHTNESS_MAX;
    Direct.brightness           = BRIGHTNESS_MAX;

    modes.push_back(Direct);

    SetupZones();
}

RGBController_SkyloongGK104Pro::~RGBController_SkyloongGK104Pro()
{
    delete controller;
}

void RGBController_SkyloongGK104Pro::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create the keyboard zone usiung Keyboard Layout Manager   |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;

    KeyboardLayoutManager new_kb(KEYBOARD_LAYOUT_ANSI_QWERTY, KEYBOARD_SIZE_FULL, keyboard_offset_values);

    new_kb.ChangeKeys(
    {
        {   0,     4,      12,     0,      KEY_EN_RIGHT_SHIFT,     KEY_EN_UNUSED,   KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },
        {   0,     4,      14,     0,      KEY_EN_UNUSED,          KEY_EN_UNUSED,   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },
        {   0,     5,      4,      114,    "Key: Left Space",      KEY_EN_UNUSED,   KEYBOARD_OPCODE_SWAP_ONLY           },
        {   0,     5,      8,      118,    "Key: Right Space",     KEY_EN_UNUSED,   KEYBOARD_OPCODE_SWAP_ONLY           }
    }
    );

    matrix_map_type * new_map   = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = new_kb.GetRowCount();
    new_zone.matrix_map->width  = new_kb.GetColumnCount();

    new_zone.matrix_map->map    = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count         = new_kb.GetKeyCount();
    new_zone.leds_min           = new_zone.leds_count;
    new_zone.leds_max           = new_zone.leds_count;

    /*---------------------------------------------------------*\
    | Matrix map still uses declared zone rows and columns      |
    |   as the packet structure depends on the matrix map       |
    \*---------------------------------------------------------*/
    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name            = new_kb.GetKeyNameAt(led_idx);
        new_led.value           = new_kb.GetKeyValueAt(led_idx);

        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_SkyloongGK104Pro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SkyloongGK104Pro::DeviceUpdateLEDs()
{
    controller->SendColorPacket(colors, &leds, modes[active_mode].brightness);
}

void RGBController_SkyloongGK104Pro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK104Pro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SkyloongGK104Pro::DeviceUpdateMode()
{
}
