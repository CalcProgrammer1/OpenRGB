/*---------------------------------------------------------*\
| RGBController_WinbondGamingKeyboard.cpp                   |
|                                                           |
|   RGBController for Winbond Gaming Keyboard               |
|                                                           |
|   Daniel Gibson                               03 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_WinbondGamingKeyboard.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"
#include "LogManager.h"

/**------------------------------------------------------------------*\
    @name Winbond Gaming Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectWinbondGamingKeyboard
    @comment Also known as Pulsar PCMK, and KT108 (by various manufacturers)
\*-------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*\
| - MSG_NUM is between 0 and (incl) 7, indicating which HID message contains the key's LED        |
| - IDX is between 0 and (incl.) 17 (0 to 5 for message 7),                                       |
|   indicating the index of that LED within the message.                                          |
| - 1 << 16 is set so KV(0,0) doesn't have the same value as 0 (the default "not assigned" value) |
\*-----------------------------------------------------------------------------------------------*/
#define KV(MSG_NUM, IDX)  ((MSG_NUM) << 8 | (IDX) | (1 << 16))

static std::vector<keyboard_led> additional_mm_leds =
{
    {
        0,       // zone
        0,       // row
        17,      // col
        KV(1,0), // value
        KEY_EN_MEDIA_VOLUME_UP, // name
        KEY_EN_UNUSED, // translated name
        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
    },
    {
        0,       // zone
        0,       // row
        18,      // col
        KV(1,1), // value
        KEY_EN_MEDIA_VOLUME_DOWN, // name
        KEY_EN_UNUSED, // translated name
        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
    },
    {
        0,       // zone
        0,       // row
        19,      // col
        KV(1,2), // value
        KEY_EN_MEDIA_MUTE, // name
        KEY_EN_UNUSED, // translated name
        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
    },
    {
        0,       // zone
        0,       // row
        20,      // col
        KV(1,3), // value
        "Key: Cylinder?!",   // name; TODO: no idea what the symbol meant, was a cylinder..
        KEY_EN_UNUSED, // translated name
        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
    }
};

static layout_values winbond_gaming_keyboard_full_layouts =
{
    {}, // "std::vector<unsigned int> default_values"  is set in InitLayouts()
    { // std::map<KEYBOARD_LAYOUT, std::vector<keyboard_led> > regional_overlay;
      { KEYBOARD_LAYOUT_ISO_QWERTY,
        {
            /*---------------------------------------------------------*\
            | just setting the values for some keys that should already |
            | be defined by KeyboardLayoutManager's keyboard_zone_main  |
            \*---------------------------------------------------------*/
            {
                0,            // zone
                3,            // row
                12,           // col
                KV(4,7),      // value
                KEY_EN_POUND, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
            {
                0,            // zone
                4,            // row
                1,            // col
                KV(4,17),     // value
                KEY_EN_ISO_BACK_SLASH, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
        }
      },
      { KEYBOARD_LAYOUT_JIS,
        {
            /*------------------------------------------------*\
            | that extra key before backspace on JIS keyboards |
            \*------------------------------------------------*/
            {
                0,            // zone
                1,            // row
                13,           // col
                KV(1,17),     // value
                KEY_JP_YEN,   // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
            }
        }
      }
      /*----------------------------------------------------------------------------------*\
      | TODO: KV(5,10) could be that extra key left of right shift on ABNT keyboards (/ ?) |
      \*----------------------------------------------------------------------------------*/
    }
};

static layout_values winbond_gaming_keyboard_tkl_layouts =
{
    {}, // "std::vector<unsigned int> default_values"  is set in InitLayouts()
    { // std::map<KEYBOARD_LAYOUT, std::vector<keyboard_led> > regional_overlay;
      { KEYBOARD_LAYOUT_ISO_QWERTY,
        {
            /*---------------------------------------------------------*\
            | just setting the values for some keys that should already |
            | be defined by KeyboardLayoutManager's keyboard_zone_main  |
            \*---------------------------------------------------------*/
            {
                0,            // zone
                3,            // row
                12,           // col
                KV(4,7),      // value
                KEY_EN_POUND, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
            {
                0,            // zone
                4,            // row
                1,            // col
                KV(4,17),     // value
                KEY_EN_ISO_BACK_SLASH, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
        }
      },
      { KEYBOARD_LAYOUT_JIS,
        {
            /*------------------------------------------------*\
            | that extra key before backspace on JIS keyboards |
            \*------------------------------------------------*/
            {
                0,            // zone
                1,            // row
                13,           // col
                KV(1,17),     // value
                KEY_JP_YEN,   // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
            }
        }
      }
      /*----------------------------------------------------------------------------------*\
      | TODO: KV(5,10) could be that extra key left of right shift on ABNT keyboards (/ ?) |
      \*----------------------------------------------------------------------------------*/
    }
};

static layout_values winbond_gaming_keyboard_60_layouts =
{
    {}, // "std::vector<unsigned int> default_values"  is set in InitLayouts()
    { // std::map<KEYBOARD_LAYOUT, std::vector<keyboard_led> > regional_overlay;
      { KEYBOARD_LAYOUT_ISO_QWERTY,
        {
            /*---------------------------------------------------------*\
            | just setting the values for some keys that should already |
            | be defined by KeyboardLayoutManager's keyboard_zone_main  |
            \*---------------------------------------------------------*/
            {
                0,            // zone
                3,            // row
                12,           // col
                KV(4,7),      // value
                KEY_EN_POUND, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
            {
                0,            // zone
                4,            // row
                1,            // col
                KV(4,17),     // value
                KEY_EN_ISO_BACK_SLASH, // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_SWAP_ONLY // opcode
            },
        }
      },
      { KEYBOARD_LAYOUT_JIS,
        {
            /*------------------------------------------------*\
            | that extra key before backspace on JIS keyboards |
            \*------------------------------------------------*/
            {
                0,            // zone
                1,            // row
                13,           // col
                KV(1,17),     // value
                KEY_JP_YEN,   // name
                KEY_EN_UNUSED, // translated name
                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT // opcode
            }
        }
      }
      /*----------------------------------------------------------------------------------*\
      | TODO: KV(5,10) could be that extra key left of right shift on ABNT keyboards (/ ?) |
      \*----------------------------------------------------------------------------------*/
    }
};

static void InitLayouts(layout_values& keyboard_layouts, KEYBOARD_SIZE kb_size, std::string vendor)
{
    /*-------------------------------------------------------------------*\
    | using kvs ("keyvals" or sth like that) as an alias for              |
    | keyboard_layouts.default_values, to make the code below             |
    | shorter/more readable                                               |
    \*-------------------------------------------------------------------*/
    std::vector<unsigned int>& kvs = keyboard_layouts.default_values;

    /*------------------------------------------------------------------------------------*\
    | Message X: what indices the keys have within the USB HID messages to set their color |
    | "??" means "that index exists, but doesn't seem to be used on my TKL ISO keyboard"   |
    |                                                                                      |
    | A Note regarding kb_size: I wrote this with my Pulsar PCMK TKL keyboard, there the   |
    | LED positions ( KV(MSGNUM, IDX) ) were like on a fullsize keyboard,                  |
    | i.e. there USB messages had space for NumPad keys (or their LEDs).                   |
    | No idea if that's also true for e.g. 60% keyboards, and it's also possible that      |
    | some positions of NumPad LEDs are incorrect, I only guessed them.                    |
    \*------------------------------------------------------------------------------------*/

    /* Message 0: ---------------------------------------------------------------*\
    | 0 = Esc, ??, 2 = F1, F2, F3, F4, ??, 7 = F5, F6, F7, F8, F9, F10, F11, F12, |
    |   15 = Print, ScrollLock, 17 = Pause                                        |
    \*---------------------------------------------------------------------------*/

    if(kb_size & KEYBOARD_ZONE_FN_ROW)
    {
        kvs.push_back( KV(0,0) ); // Esc
        // F1 to F4
        for(int i=2; i <= 5; ++i)
        {
            kvs.push_back( KV(0,i) );
        }

        // F5 to F12
        for(int i=7; i <= 14; ++i)
        {
            kvs.push_back( KV(0,i) );
        }
    }

    if(kb_size & KEYBOARD_ZONE_EXTRA)
    {
        // PrintScreen, ScrollLock, Pause
        for(int i=15; i <= 17; ++i)
        {
            kvs.push_back( KV(0,i) );
        }
    }

    /* Message 1: ------------------------------------------------------------------------*\
    |  0-3 = (could be multimedia keys above numblock?), 4 = BKTK, 1, 2, 3, 4, 5,          |
    |    10 = 6, 7, 8, 9, 0, '-', 16 = '=', 17 = (that extra key before backspace on JIS?) |
    \*------------------------------------------------------------------------------------*/

    // KV(1,0) to KV(1,3) are probably 4 extra multimedia keys above the numblock

    // BKTK ("Quake console key"), 1, 2, ... 0, -, =
    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        for(int i=4; i <= 16; ++i)
        {
            kvs.push_back( KV(1,i) );
        }

        // KV(1,17) might be that extra Yen key before backspace on JIS keyboards
    }

    /* Message 2: ------------------------------------------------------------*\
    | 0 = Backspace, Ins, Pos1, 3 = PgUp, 4-7 = (NumLock, Num/, Num*, Num- ?), |
    |   8 = Tab, Q, W, E, R, T, ..., 16 = I, 17 = O                            |
    \*------------------------------------------------------------------------*/

    // Backspace
    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        kvs.push_back( KV(2,0) );
    }

    // Ins, Home, PgUp
    if(kb_size & KEYBOARD_ZONE_EXTRA)
    {
        for(int i=1; i <= 3; ++i)
        {
            kvs.push_back( KV(2,i) );
        }
    }

    // NumLock, NP/, NP*,  NP-
    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        for(int i=4; i <= 7; ++i)
        {
            kvs.push_back( KV(2,i) );
        }
    }

    // Tab, Q, ..., I, O
    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        for(int i=8; i <= 17; ++i)
        {
            kvs.push_back( KV(2,i) );
        }
    }

    /* Message 3: -----------------------------------------------------------*\
    | 0 = P, [ (Ü), ] (+), 3 = (US-\ ?), ??, 5 = Del, End, 7 = PgDown,        |
    |    8-1 = (Num7, Num8, Num9, Num+ ?), 12 = CapsLock, ??, 14 = A, S, D, F |
    \*-----------------------------------------------------------------------*/

    // P, Ü/[, +/]
    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        for(int i=0; i <= 2; ++i)
        {
            kvs.push_back( KV(3,i) );
        }
        // Backslash
        if(vendor != "Hator")
        {
            kvs.push_back( KV(3,3) );
        }
        else
        {
            kvs.push_back( KV(3,4) );
        }
    }

    // Del, End, PgDown
    if(kb_size & KEYBOARD_ZONE_EXTRA)
    {
        for(int i=5; i <= 7; ++i)
        {
            kvs.push_back( KV(3,i) );
        }
    }

    // NP7-NP9, NP+
    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        for(int i=8; i <= 11; ++i)
        {
            kvs.push_back( KV(3,i) );
        }
    }


    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        // CapsLock
        kvs.push_back( KV(3,12) );
        // A, S, D, F
        for(int i=14; i <= 17; ++i)
        {
            kvs.push_back( KV(3,i) );
        }
    }

    /* Message 4: -------------------------------------------------------------*\
    | 0 = G, H, J, K, L, ; (Ö), " (Ä), 7 = #, 8 = Enter, 9 - 15 = ??            |
    |   (maybe 12-14 are Num4-6, maybe 15 = Num+ ?), 16 = Shift, 17 = ISO-\ (<) |
    \*-------------------------------------------------------------------------*/

    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        // G, H, ..., L, Ö/;, Ä/"
        for(int i=0; i <= 6; ++i)
        {
            kvs.push_back( KV(4,i) );
        }

        // KV(4,7) is the ISO # key that doesn't exist on ANSI (set in overlay)
        // even though # is no ANSI key, that default_values array expects it to be there..
        kvs.push_back( KV(4,7) );

        // Enter - assuming that on ANSI it uses the same LED index
        kvs.push_back( KV(4,8) );
    }


    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        // NP4-6 - not sure if those are really at 12-14, would fit though
        for(int i=12; i <= 14; ++i)
        {
            kvs.push_back( KV(4,i) );
        }
    }

    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        // Left Shift
        kvs.push_back( KV(4,16) );

        // KV(4,17) is the ISO |\ (or <) key that doesn't exist on ANSI (set in overlay)
        // apparently it must be listed anyway..
        kvs.push_back( KV(4,17) );
    }

    /* Message 5: --------------------------------------------------------*\
    | 0 = Z/Y, X, C, V, B, N, M, ',', '.' , 9 = / or -, ??, ??,            |
    |   12 = Shift, ??, 14 = Up, 15-17 ?? (maybe 16 and 17 are Num1, Num2, |
    |    and  Num3, NumEnter are in next msg?)                             |
    \*--------------------------------------------------------------------*/

    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        // Z (Y), X, .., M, ',', '.', / (- on DE)
        for(int i=0; i <= 9; ++i)
        {
            kvs.push_back( KV(5,i) );
        }

        // KV(5,10) could be that extra key left of right shift on ABNT keyboards (/ ?)

        kvs.push_back( KV(5,12) ); // Right Shift
    }

    if(kb_size & KEYBOARD_ZONE_EXTRA)
    {
        kvs.push_back( KV(5,14) ); // Up
    }

    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        kvs.push_back( KV(5,16) ); // Num1
        kvs.push_back( KV(5,17) ); // Num2
    }

    /* Message 6: -------------------------------------------------------------*\
    | 0 = (Num3 ?), (Num Enter ?), 2 = Ctrl, Win, 4 = Alt, .. ?? .., 8 = Space, |
    | .. ?? .., 12 = AltGr, Fn, Menu, 15 = Ctrl, ??, 17 = Left                  |
    \*-------------------------------------------------------------------------*/

    // Num3, Num Enter
    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        for(int i=0; i <= 1; ++i)
        {
            kvs.push_back( KV(6,i) );
        }
    }

    if(kb_size & KEYBOARD_ZONE_MAIN)
    {
        // LeftCtrl, Win, LeftAlt
        for(int i=2; i <= 4; ++i)
        {
            kvs.push_back( KV(6,i) );
        }
        // Space
        kvs.push_back( KV(6,8) );
        // RightAlt, Fn, RMenu, RightCtrl
        for(int i=12; i <= 15; ++i)
        {
            kvs.push_back( KV(6,i) );
        }
    }

    if(kb_size & KEYBOARD_ZONE_EXTRA)
        kvs.push_back( KV(6,17) ); // Left

    /* Message 7: -------------------------------------------------------*\
    | 0 = Down, 1 = Right, 2-5 = ?? (maybe 2 or 3 = Num0, 4 = NumDecimal, |
    |   5 = NumEnter, more likely: 3 = Num0, 5 = NumDec)                  |
    \*-------------------------------------------------------------------*/

    if(kb_size & KEYBOARD_ZONE_EXTRA)
    {
        kvs.push_back( KV(7,0) ); // Down
        kvs.push_back( KV(7,1) ); // Right
    }

    if(kb_size & KEYBOARD_ZONE_NUMPAD)
    {
        kvs.push_back( KV(7,3) ); // Num0 - TODO: or 7,2 or 7,4 ?
        kvs.push_back( KV(7,5) ); // NumDec - TODO: or 7,4 ?
    }

    /*-------------------------------------------------------------------*\
    | for fullsize keyboards, add multimedia keys to all layouts/overlays |
    \*-------------------------------------------------------------------*/
    if(kb_size == KEYBOARD_SIZE_FULL)
    {
        for(std::pair<const KEYBOARD_LAYOUT, std::vector<keyboard_led>>& overlay: keyboard_layouts.regional_overlay)
        {
            for(const keyboard_led& mm_key_led : additional_mm_leds)
            {
                overlay.second.push_back(mm_key_led);
            }
        }

        /*---------------------------------------------------------------------------------*\
        | there is no overlay for the ANSI layout defined yet, add one just for the MM keys |
        \*---------------------------------------------------------------------------------*/
        keyboard_layouts.regional_overlay.insert({ KEYBOARD_LAYOUT_ANSI_QWERTY, additional_mm_leds });
    }
}



RGBController_WinbondGamingKeyboard::RGBController_WinbondGamingKeyboard(WinbondGamingKeyboardController* ctrl)
    : controller(ctrl)
{
    type        = DEVICE_TYPE_KEYBOARD;
    name        = ctrl->GetName();
    vendor      = ctrl->GetVendor();
    description = ctrl->GetDescription();
    location    = ctrl->GetDeviceLocation();
    serial      = ctrl->GetSerialString();
    version     = ctrl->GetVersion();

    {
        mode m;
        m.name       = "Custom";
        m.value      = WINBOND_GK_MODE_CUSTOM;
        m.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.color_mode = MODE_COLORS_PER_LED;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name       = "Static";
        m.value      = WINBOND_GK_MODE_STATIC;
        m.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.color_mode = MODE_COLORS_MODE_SPECIFIC;
        m.colors_min = m.colors_max = 1; // one color for all keys
        m.colors.push_back(ToRGBColor(255,255,255)); // default to white
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Neon";
        m.value  = WINBOND_GK_MODE_NEON;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0));
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Breathing";
        m.value  = WINBOND_GK_MODE_BREATHE;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0));
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Wave";
        m.value  = WINBOND_GK_MODE_WAVE;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE
                   | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD
                   | MODE_FLAG_HAS_DIRECTION_HV; // NOTE: this is really outer to inner or inner to outer
        m.direction = MODE_DIRECTION_RIGHT;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0));
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Snake";
        m.value  = WINBOND_GK_MODE_SNAKE;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE
                   | MODE_FLAG_HAS_DIRECTION_UD; // NOTE: more like CW/CCW
        m.direction = MODE_DIRECTION_DOWN;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0));
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    /*------------------------------------------------------------------------*\
    |the following modes are ones that only show effects when a key is pressed |
    | and otherwise only a background color                                    |
    \*------------------------------------------------------------------------*/
    {
        mode m;
        m.name   = "Aurora (on keypress)";
        m.value  = WINBOND_GK_MODE_AURORA;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE
                   | MODE_FLAG_HAS_DIRECTION_HV; // NOTE: more like to inner/to outer
        m.direction = MODE_DIRECTION_HORIZONTAL;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0)); // TODO: set background to white so keyboard isn't dark?
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Ripple (on keypress)";
        m.value  = WINBOND_GK_MODE_RIPPLE;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0)); // TODO: set background to white so keyboard isn't dark?
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    {
        mode m;
        m.name   = "Reactive (on keypress)";
        m.value  = WINBOND_GK_MODE_REACTIVE;
        m.flags  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        m.colors.push_back(ToRGBColor(255,0,0));
        m.colors.push_back(ToRGBColor(0,0,0)); // TODO: set background to white so keyboard isn't dark?
        m.colors_min = 0;
        m.colors_max = 2;
        m.color_mode = MODE_COLORS_RANDOM;
        m.brightness_min = 0;
        m.brightness_max = 4;
        m.brightness = 1;
        m.speed_min = 0;
        m.speed_max = 4;
        m.speed = 2;
        modes.push_back(m);
    }

    /*--------------------------------------------------------------------------------*\
    |  NOTE: logo light has static, neon (WINBOND_GK_MODE_LOGO_NEON), breathe, wave    |
    |  all logo modes have brightness, all but static have speed and 2 colors + random |
    \*--------------------------------------------------------------------------------*/

    SetupZones();
}


RGBController_WinbondGamingKeyboard::~RGBController_WinbondGamingKeyboard()
{
    delete controller;
}

void RGBController_WinbondGamingKeyboard::SetupZones()
{
    zone new_zone;
    new_zone.name = ZONE_EN_KEYBOARD;
    new_zone.type = ZONE_TYPE_MATRIX;

    layout_values* layouts = &winbond_gaming_keyboard_full_layouts;
    KEYBOARD_SIZE kb_size = controller->GetSize();
    std::string vendor = controller->GetVendor();

    if(kb_size == KEYBOARD_SIZE_TKL)
    {
        layouts = &winbond_gaming_keyboard_tkl_layouts;
    }
    else if(kb_size == KEYBOARD_SIZE_SIXTY)
    {
        layouts = &winbond_gaming_keyboard_60_layouts;
    }
    else // size is full or something not supported directly - default to full
    {
        kb_size = KEYBOARD_SIZE_FULL;
    }

    if(layouts->default_values.empty())
    {
        InitLayouts(*layouts, kb_size, vendor);
    }

    KeyboardLayoutManager new_kb(controller->GetLayout(), kb_size, *layouts);

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
    new_kb.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = new_kb.GetKeyNameAt((unsigned int)led_idx);
        new_led.value               = new_kb.GetKeyValueAt((unsigned int)led_idx);

        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    /*---------------------------------------------------------------------------*\
    | special case: logo LEDs                                                     |
    | it's 3 LEDs, but the colors can't be set separately, so represent it as one |
    \*---------------------------------------------------------------------------*/

    if(controller->HasLogoLight())
    {
        zone logo_zone;
        logo_zone.name = "Logo Light";
        logo_zone.type = ZONE_TYPE_SINGLE;
        logo_zone.leds_min      = 1;
        logo_zone.leds_max      = 1;
        logo_zone.leds_count    = 1;
        logo_zone.matrix_map    = NULL;

        led zone_led;
        zone_led.name  = "Logo LEDs";
        zone_led.value = KV(255, 0); // using message num 255 as special case for logo LED
        leds.push_back(zone_led);

        zones.push_back(logo_zone);
    }

    SetupColors();
}

#undef KV

void RGBController_WinbondGamingKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WinbondGamingKeyboard::DeviceUpdateLEDs()
{
    controller->SetLEDsData(colors, leds, modes[active_mode].brightness);
}

void RGBController_WinbondGamingKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WinbondGamingKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WinbondGamingKeyboard::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode]);
}
