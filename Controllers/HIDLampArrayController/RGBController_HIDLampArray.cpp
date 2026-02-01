/*---------------------------------------------------------*\
| RGBController_HIDLampArray.cpp                            |
|                                                           |
|   RGBController for HID LampArray Devices                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <set>
#include "RGBController_HIDLampArray.h"
#include "RGBControllerKeyNames.h"

/**------------------------------------------------------------------*\
    @name HID LampArray Controllers
    @category Keyboard
    @type USB
    @detectors DetectHIDLampArrayControllers
\*-------------------------------------------------------------------*/

RGBController_HIDLampArray::RGBController_HIDLampArray(HIDLampArrayController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "Generic";
    description         = "HID LampArray Device";
    location            = controller->GetDeviceLocation();
    serial              = controller->GetSerialString();

    /*-----------------------------------------------------*\
    | Determine device type from LampArray kind             |
    \*-----------------------------------------------------*/
    switch(controller->GetLampArrayKind())
    {
        case HID_LAMPARRAY_KIND_UNDEFINED:
        default:
            type        = DEVICE_TYPE_UNKNOWN;
            break;

        case HID_LAMPARRAY_KIND_KEYBOARD:
            type        = DEVICE_TYPE_KEYBOARD;
            break;

        case HID_LAMPARRAY_KIND_MOUSE:
            type        = DEVICE_TYPE_MOUSE;
            break;

        case HID_LAMPARRAY_KIND_GAME_CONTROLLER:
            type        = DEVICE_TYPE_GAMEPAD;
            break;

        case HID_LAMPARRAY_KIND_PERIPHERAL:
        case HID_LAMPARRAY_KIND_SCENE:
        case HID_LAMPARRAY_KIND_NOTIFICATION:
        case HID_LAMPARRAY_KIND_CHASSIS:
        case HID_LAMPARRAY_KIND_WEARABLE:
        case HID_LAMPARRAY_KIND_FURNITURE:
        case HID_LAMPARRAY_KIND_ART:
            type        = DEVICE_TYPE_ACCESSORY;
            break;
    }

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HIDLampArray::~RGBController_HIDLampArray()
{
    delete controller;
}

/*---------------------------------------------------------*\
| HID Usage Tables 1.6 -> 10 Keyboard/Keypad Page (0x07)    |
\*---------------------------------------------------------*/
#define HID_KEYBOARD_USAGE_IDS_COUNT 0xE9

static const char *HIDUsageIDsToKeyNames[HID_KEYBOARD_USAGE_IDS_COUNT] =
{
    KEY_EN_UNUSED,              // Reserved
    KEY_EN_UNUSED,              // Keyboard ErrorRollOver
    KEY_EN_UNUSED,              // Keyboard POSTFail
    KEY_EN_UNUSED,              // Keyboard ErrorUndefined
    KEY_EN_A,                   // Keyboard a and A
    KEY_EN_B,                   // Keyboard b and B
    KEY_EN_C,                   // Keyboard c and C
    KEY_EN_D,                   // Keyboard d and D
    KEY_EN_E,                   // Keyboard e and E
    KEY_EN_F,                   // Keyboard f and F
    KEY_EN_G,                   // Keyboard g and G
    KEY_EN_H,                   // Keyboard h and H
    KEY_EN_I,                   // Keyboard i and I
    KEY_EN_J,                   // Keyboard j and J
    KEY_EN_K,                   // Keyboard k and K
    KEY_EN_L,                   // Keyboard l and L
    KEY_EN_M,                   // Keyboard m and M
    KEY_EN_N,                   // Keyboard n and N
    KEY_EN_O,                   // Keyboard o and O
    KEY_EN_P,                   // Keyboard p and P
    KEY_EN_Q,                   // Keyboard q and Q
    KEY_EN_R,                   // Keyboard r and R
    KEY_EN_S,                   // Keyboard s and S
    KEY_EN_T,                   // Keyboard t and T
    KEY_EN_U,                   // Keyboard u and U
    KEY_EN_V,                   // Keyboard v and V
    KEY_EN_W,                   // Keyboard w and W
    KEY_EN_X,                   // Keyboard x and X
    KEY_EN_Y,                   // Keyboard y and Y
    KEY_EN_Z,                   // Keyboard z and Z
    KEY_EN_1,                   // Keyboard 1 and !
    KEY_EN_2,                   // Keyboard 2 and @
    KEY_EN_3,                   // Keyboard 3 and #
    KEY_EN_4,                   // Keyboard 4 and $
    KEY_EN_5,                   // Keyboard 5 and %
    KEY_EN_6,                   // Keyboard 6 and ∧
    KEY_EN_7,                   // Keyboard 7 and &
    KEY_EN_8,                   // Keyboard 8 and *
    KEY_EN_9,                   // Keyboard 9 and (
    KEY_EN_0,                   // Keyboard 0 and )
    KEY_EN_ANSI_ENTER,          // Keyboard Return (ENTER)
    KEY_EN_ESCAPE,              // Keyboard ESCAPE
    KEY_EN_BACKSPACE,           // Keyboard DELETE (Backspace)
    KEY_EN_TAB,                 // Keyboard Tab
    KEY_EN_SPACE,               // Keyboard Spacebar
    KEY_EN_MINUS,               // Keyboard - and (underscore)
    KEY_EN_EQUALS,              // Keyboard = and +
    KEY_EN_LEFT_BRACKET,        // Keyboard [ and {
    KEY_EN_RIGHT_BRACKET,       // Keyboard ] and }
    KEY_EN_BACK_SLASH,          // Keyboard \ and |
    KEY_EN_POUND,               // Keyboard Non-US # and ˜
    KEY_EN_SEMICOLON,           // Keyboard ; and :
    KEY_EN_QUOTE,               // Keyboard ‘ and “
    KEY_EN_BACK_TICK,           // Keyboard Grave Accent and Tilde
    KEY_EN_COMMA,               // Keyboard , and <
    KEY_EN_PERIOD,              // Keyboard . and >
    KEY_EN_FORWARD_SLASH,       // Keyboard / and ?
    KEY_EN_CAPS_LOCK,           // Keyboard Caps Lock
    KEY_EN_F1,                  // Keyboard F1
    KEY_EN_F2,                  // Keyboard F2
    KEY_EN_F3,                  // Keyboard F3
    KEY_EN_F4,                  // Keyboard F4
    KEY_EN_F5,                  // Keyboard F5
    KEY_EN_F6,                  // Keyboard F6
    KEY_EN_F7,                  // Keyboard F7
    KEY_EN_F8,                  // Keyboard F8
    KEY_EN_F9,                  // Keyboard F9
    KEY_EN_F10,                 // Keyboard F10
    KEY_EN_F11,                 // Keyboard F11
    KEY_EN_F12,                 // Keyboard F12
    KEY_EN_PRINT_SCREEN,        // Keyboard PrintScreen
    KEY_EN_SCROLL_LOCK,         // Keyboard Scroll Lock
    KEY_EN_PAUSE_BREAK,         // Keyboard Pause
    KEY_EN_INSERT,              // Keyboard Insert
    KEY_EN_HOME,                // Keyboard Home
    KEY_EN_PAGE_UP,             // Keyboard PageUp
    KEY_EN_DELETE,              // Keyboard Delete Forward
    KEY_EN_END,                 // Keyboard End
    KEY_EN_PAGE_DOWN,           // Keyboard PageDown
    KEY_EN_RIGHT_ARROW,         // Keyboard RightArrow
    KEY_EN_LEFT_ARROW,          // Keyboard LeftArrow
    KEY_EN_DOWN_ARROW,          // Keyboard DownArrow
    KEY_EN_UP_ARROW,            // Keyboard UpArrow
    KEY_EN_NUMPAD_LOCK,         // Keypad Num Lock and Clear
    KEY_EN_NUMPAD_DIVIDE,       // Keypad /
    KEY_EN_NUMPAD_TIMES,        // Keypad *
    KEY_EN_NUMPAD_MINUS,        // Keypad -
    KEY_EN_NUMPAD_PLUS,         // Keypad +
    KEY_EN_NUMPAD_ENTER,        // Keypad ENTER
    KEY_EN_NUMPAD_1,            // Keypad 1 and End
    KEY_EN_NUMPAD_2,            // Keypad 2 and Down Arrow
    KEY_EN_NUMPAD_3,            // Keypad 3 and PageDn
    KEY_EN_NUMPAD_4,            // Keypad 4 and Left Arrow
    KEY_EN_NUMPAD_5,            // Keypad 5
    KEY_EN_NUMPAD_6,            // Keypad 6 and Right Arrow
    KEY_EN_NUMPAD_7,            // Keypad 7 and Home
    KEY_EN_NUMPAD_8,            // Keypad 8 and Up Arrow
    KEY_EN_NUMPAD_9,            // Keypad 9 and PageUp
    KEY_EN_NUMPAD_0,            // Keypad 0 and Insert
    KEY_EN_NUMPAD_PERIOD,       // Keypad . and Delete
    KEY_EN_BACK_SLASH,          // Keyboard Non-US \ and |
    KEY_EN_UNUSED,              // Keyboard Application
    KEY_EN_POWER,               // Keyboard Power
    KEY_EN_NUMPAD_EQUAL,        // Keypad =
    KEY_EN_F13,                 // Keyboard F13
    KEY_EN_F14,                 // Keyboard F14
    KEY_EN_F15,                 // Keyboard F15
    KEY_EN_F16,                 // Keyboard F16
    KEY_EN_F17,                 // Keyboard F17
    KEY_EN_F18,                 // Keyboard F18
    KEY_EN_F19,                 // Keyboard F19
    KEY_EN_F20,                 // Keyboard F20
    KEY_EN_F21,                 // Keyboard F21
    KEY_EN_F22,                 // Keyboard F22
    KEY_EN_F23,                 // Keyboard F23
    KEY_EN_F24,                 // Keyboard F24
    KEY_EN_UNUSED,              // Keyboard Execute
    KEY_EN_UNUSED,              // Keyboard Help
    KEY_EN_MENU,                // Keyboard Menu
    KEY_EN_UNUSED,              // Keyboard Select
    KEY_EN_UNUSED,              // Keyboard Stop
    KEY_EN_UNUSED,              // Keyboard Again
    KEY_EN_UNUSED,              // Keyboard Undo
    KEY_EN_UNUSED,              // Keyboard Cut
    KEY_EN_UNUSED,              // Keyboard Copy
    KEY_EN_UNUSED,              // Keyboard Paste
    KEY_EN_UNUSED,              // Keyboard Find
    KEY_EN_MEDIA_MUTE,          // Keyboard Mute
    KEY_EN_MEDIA_VOLUME_UP,     // Keyboard Volume Up
    KEY_EN_MEDIA_VOLUME_DOWN,   // Keyboard Volume Down
    KEY_EN_UNUSED,              // Keyboard Locking Caps Lock
    KEY_EN_UNUSED,              // Keyboard Locking Num Lock
    KEY_EN_UNUSED,              // Keyboard Locking Scroll Lock
    KEY_EN_NUMPAD_PERIOD,       // Keypad Comma
    KEY_EN_NUMPAD_EQUAL,        // Keypad Equal Sign
    KEY_JP_RO,                  // Keyboard International1
    KEY_JP_KANA,                // Keyboard International2
    KEY_JP_YEN,                 // Keyboard International3
    KEY_JP_HENKAN,              // Keyboard International4
    KEY_JP_MUHENKAN,            // Keyboard International5
    KEY_EN_COMMA,               // Keyboard International6
    KEY_JP_ZENKAKU,             // Keyboard International7
    KEY_EN_UNUSED,              // Keyboard International8
    KEY_EN_UNUSED,              // Keyboard International9
    KEY_KR_HAN,                 // Keyboard LANG1
    KEY_KR_HANJA,               // Keyboard LANG2
    KEY_JP_KATAKANA,            // Keyboard LANG3
    KEY_JP_HIRAGANA,            // Keyboard LANG4
    KEY_JP_ZENKAKU,             // Keyboard LANG5
    KEY_EN_UNUSED,              // Keyboard LANG6
    KEY_EN_UNUSED,              // Keyboard LANG7
    KEY_EN_UNUSED,              // Keyboard LANG8
    KEY_EN_UNUSED,              // Keyboard LANG9
    KEY_EN_UNUSED,              // Keyboard Alternate Erase
    KEY_EN_UNUSED,              // Keyboard SysReq/Attention
    KEY_EN_UNUSED,              // Keyboard Cancel
    KEY_EN_UNUSED,              // Keyboard Clear
    KEY_EN_UNUSED,              // Keyboard Prior
    KEY_EN_UNUSED,              // Keyboard Return
    KEY_EN_UNUSED,              // Keyboard Separator
    KEY_EN_UNUSED,              // Keyboard Out
    KEY_EN_UNUSED,              // Keyboard Oper
    KEY_EN_UNUSED,              // Keyboard Clear/Again
    KEY_EN_UNUSED,              // Keyboard CrSel/Props
    KEY_EN_UNUSED,              // Keyboard ExSel
    KEY_EN_UNUSED,              // AF Reserved
    KEY_EN_UNUSED,              // Keypad 00
    KEY_EN_UNUSED,              // Keypad 000
    KEY_EN_UNUSED,              // Thousands Separator
    KEY_EN_UNUSED,              // Decimal Separator
    KEY_EN_UNUSED,              // Currency Unit
    KEY_EN_UNUSED,              // Currency Sub-unit
    KEY_EN_UNUSED,              // Keypad (
    KEY_EN_UNUSED,              // Keypad )
    KEY_EN_UNUSED,              // Keypad {
    KEY_EN_UNUSED,              // Keypad }
    KEY_EN_UNUSED,              // Keypad Tab
    KEY_EN_UNUSED,              // Keypad Backspace
    KEY_EN_UNUSED,              // Keypad A
    KEY_EN_UNUSED,              // Keypad B
    KEY_EN_UNUSED,              // Keypad C
    KEY_EN_UNUSED,              // Keypad D
    KEY_EN_UNUSED,              // Keypad E
    KEY_EN_UNUSED,              // Keypad F
    KEY_EN_UNUSED,              // Keypad XOR
    KEY_EN_UNUSED,              // Keypad ∧
    KEY_EN_UNUSED,              // Keypad %
    KEY_EN_UNUSED,              // Keypad <
    KEY_EN_UNUSED,              // Keypad >
    KEY_EN_UNUSED,              // Keypad &
    KEY_EN_UNUSED,              // Keypad &&
    KEY_EN_UNUSED,              // Keypad |
    KEY_EN_UNUSED,              // Keypad ||
    KEY_EN_UNUSED,              // Keypad :
    KEY_EN_UNUSED,              // Keypad #
    KEY_EN_UNUSED,              // Keypad Space
    KEY_EN_UNUSED,              // Keypad @
    KEY_EN_UNUSED,              // Keypad !
    KEY_EN_UNUSED,              // Keypad Memory Store
    KEY_EN_UNUSED,              // Keypad Memory Recall
    KEY_EN_UNUSED,              // Keypad Memory Clear
    KEY_EN_UNUSED,              // Keypad Memory Add
    KEY_EN_UNUSED,              // Keypad Memory Subtract
    KEY_EN_UNUSED,              // Keypad Memory Multiply
    KEY_EN_UNUSED,              // Keypad Memory Divide
    KEY_EN_UNUSED,              // Keypad +/-
    KEY_EN_UNUSED,              // Keypad Clear
    KEY_EN_UNUSED,              // Keypad Clear Entry
    KEY_EN_UNUSED,              // Keypad Binary
    KEY_EN_UNUSED,              // Keypad Octal
    KEY_EN_UNUSED,              // Keypad Decimal
    KEY_EN_UNUSED,              // Keypad Hexadecimal
    KEY_EN_UNUSED,              // DF Reserved
    KEY_EN_LEFT_CONTROL,        // Keyboard LeftControl
    KEY_EN_LEFT_SHIFT,          // Keyboard LeftShift
    KEY_EN_LEFT_ALT,            // Keyboard LeftAlt
    KEY_EN_LEFT_WINDOWS,        // Keyboard Left GUI
    KEY_EN_RIGHT_CONTROL,       // Keyboard RightControl
    KEY_EN_RIGHT_SHIFT,         // Keyboard RightShift
    KEY_EN_RIGHT_ALT,           // Keyboard RightAlt
    KEY_EN_RIGHT_WINDOWS,       // Keyboard Right GUI
};

void RGBController_HIDLampArray::SetupZones()
{
    zone new_zone;
    new_zone.name       = ZONE_EN_KEYBOARD;
    new_zone.type       = ZONE_TYPE_MATRIX;
    new_zone.leds_count = controller->GetLampCount();
    new_zone.leds_min   = new_zone.leds_count;
    new_zone.leds_max   = new_zone.leds_count;

    std::set<unsigned int>  columns;
    std::set<unsigned int>  rows;

    for(std::size_t lamp_idx = 0; lamp_idx < controller->GetLamps().size(); lamp_idx++)
    {
        rows.insert(controller->GetLamps()[lamp_idx].PositionYInMillimeters);
        columns.insert(controller->GetLamps()[lamp_idx].PositionXInMillimeters);
    }

    new_zone.matrix_map.height  = rows.size();
    new_zone.matrix_map.width   = columns.size();
    new_zone.matrix_map.map.resize(rows.size() * columns.size());

    memset(new_zone.matrix_map.map.data(), 0xFF, (new_zone.matrix_map.map.size() * sizeof(unsigned int)));

    for(std::size_t lamp_idx = 0; lamp_idx < controller->GetLamps().size(); lamp_idx++)
    {
        //FIXME this assumes that matrix_size is big enough which is only guaranteed when no key possition is doubled
        size_t idx      = std::distance(columns.begin(), columns.find(controller->GetLamps()[lamp_idx].PositionXInMillimeters));
        size_t idy      = std::distance(rows.begin(), rows.find(controller->GetLamps()[lamp_idx].PositionYInMillimeters));

        new_zone.matrix_map.map[idx + idy * new_zone.matrix_map.width] = lamp_idx;
    }

    zones.push_back(new_zone);

    for(std::size_t lamp_idx = 0; lamp_idx < controller->GetLamps().size(); lamp_idx++)
    {
        led new_led;

        if(controller->GetLamps()[lamp_idx].LampKey < HID_KEYBOARD_USAGE_IDS_COUNT)
        {
            new_led.name = HIDUsageIDsToKeyNames[controller->GetLamps()[lamp_idx].LampKey];
        }
        else
        {
            new_led.name = KEY_EN_UNUSED;
        }
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HIDLampArray::DeviceUpdateLEDs()
{
    unsigned int leds_to_go = leds.size();

    while(leds_to_go > 0)
    {
        unsigned int leds_this_frame = LAMP_MULTI_UPDATE_LAMP_COUNT;
        unsigned short LampIds[LAMP_MULTI_UPDATE_LAMP_COUNT];
        LampArrayColor UpdateColors[LAMP_MULTI_UPDATE_LAMP_COUNT];
        unsigned char LampUpdateFlags = 0;

        if(leds_to_go < leds_this_frame)
        {
            leds_to_go = leds_this_frame;
        }

        for(unsigned int led_frame_idx = 0; led_frame_idx < leds_this_frame; led_frame_idx++)
        {
            unsigned short led_idx = (leds.size() - leds_to_go) + led_frame_idx;
            LampIds[led_frame_idx] = led_idx;
            UpdateColors[led_frame_idx].RedChannel = RGBGetRValue(colors[led_idx]);
            UpdateColors[led_frame_idx].GreenChannel = RGBGetGValue(colors[led_idx]);
            UpdateColors[led_frame_idx].BlueChannel = RGBGetBValue(colors[led_idx]);
            UpdateColors[led_frame_idx].IntensityChannel = 255;
        }

        leds_to_go -= leds_this_frame;

        if(leds_to_go <= 0)
        {
            LampUpdateFlags = LAMP_UPDATE_FLAG_UPDATE_COMPLETE;
        }

        controller->SetLampMultiUpdateReport(leds_this_frame, LampUpdateFlags, LampIds, UpdateColors);
    }
}

void RGBController_HIDLampArray::DeviceUpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateSingleLED(int /*led*/)
{

}

void RGBController_HIDLampArray::DeviceUpdateMode()
{

}
