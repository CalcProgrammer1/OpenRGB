/*---------------------------------------------------------*\
| RGBController_WootingKeyboard.cpp                         |
|                                                           |
|   RGBController for Wooting keyboard                      |
|                                                           |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"

//TODO: These matrix maps have indices to the 6x21 full layout.
// This is incorrect. The values in the matrix map should be indices to the `leds` vector.
// the Value property in each LED is what should map to the index in the full 6x21 layout.

//0xFFFFFFFF indicates an unused entry in matrix
#define NA 0xFFFFFFFF

static unsigned int matrix_map_full[6][21] = {
    {   0,  NA,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20 },
    {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41 },
    {  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62 },
    {  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  NA,  NA,  NA,  80,  81,  82,  NA },
    {  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  NA,  97,  NA,  99,  NA, 101, 102, 103, 104 },
    { 105, 106, 107,  NA,  NA,  NA, 111,  NA,  NA,  NA, 115, 116, 117, 118, 119, 120, 121,  NA, 123, 124,  NA }
};

static const char *led_names_full[6][21] {
    //Row 0
    {
        KEY_EN_ESCAPE,
        KEY_EN_UNUSED,
        KEY_EN_F1,
        KEY_EN_F2,
        KEY_EN_F3,
        KEY_EN_F4,
        KEY_EN_F5,
        KEY_EN_F6,
        KEY_EN_F7,
        KEY_EN_F8,
        KEY_EN_F9,
        KEY_EN_F10,
        KEY_EN_F11,
        KEY_EN_F12,
        KEY_EN_PRINT_SCREEN,
        KEY_EN_PAUSE_BREAK,
        KEY_EN_SCROLL_LOCK,
        "Key: Custom1",
        "Key: Custom2",
        "Key: Custom3",
        "Key: Mode",
    },
    //row 1
    {
        KEY_EN_BACK_TICK,
        KEY_EN_1,
        KEY_EN_2,
        KEY_EN_3,
        KEY_EN_4,
        KEY_EN_5,
        KEY_EN_6,
        KEY_EN_7,
        KEY_EN_8,
        KEY_EN_9,
        KEY_EN_0,
        KEY_EN_MINUS,
        KEY_EN_PLUS,
        KEY_EN_BACKSPACE,
        KEY_EN_INSERT,
        KEY_EN_HOME,
        KEY_EN_PAGE_UP,
        KEY_EN_NUMPAD_LOCK,
        KEY_EN_NUMPAD_DIVIDE,
        KEY_EN_NUMPAD_TIMES,
        KEY_EN_NUMPAD_MINUS,
    },
    //row 2
    {
        KEY_EN_TAB,
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_BACK_SLASH,
        KEY_EN_DELETE,
        KEY_EN_END,
        KEY_EN_PAGE_DOWN,
        KEY_EN_NUMPAD_7,
        KEY_EN_NUMPAD_8,
        KEY_EN_NUMPAD_9,
        KEY_EN_NUMPAD_PLUS,
    },
    //row 3
    {
        KEY_EN_CAPS_LOCK,
        KEY_EN_A,
        KEY_EN_S,
        KEY_EN_D,
        KEY_EN_F,
        KEY_EN_G,
        KEY_EN_H,
        KEY_EN_J,
        KEY_EN_K,
        KEY_EN_L,
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_POUND,
        KEY_EN_ANSI_ENTER,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_4,
        KEY_EN_NUMPAD_5,
        KEY_EN_NUMPAD_6,
        KEY_EN_UNUSED,
    },
    //row 4
    {
        KEY_EN_LEFT_SHIFT,
        KEY_EN_ISO_BACK_SLASH,
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_UNUSED,
        KEY_EN_UP_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_1,
        KEY_EN_NUMPAD_2,
        KEY_EN_NUMPAD_3,
        KEY_EN_NUMPAD_ENTER,
    },
    //row 5
    {
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_SPACE,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_WINDOWS,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
        KEY_EN_UNUSED,
        KEY_EN_NUMPAD_0,
        KEY_EN_NUMPAD_PERIOD,
        KEY_EN_UNUSED,
    }
};

static unsigned int matrix_map_tkl[6][17] = {
    {   0,  NA,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16 },
    {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37 },
    {  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58 },
    {  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  NA,  NA,  NA },
    {  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  NA,  97,  NA,  99,  NA },
    { 105, 106, 107,  NA,  NA,  NA, 111,  NA,  NA,  NA, 115, 116, 117, 118, 119, 120, 121 }
};

static const char *led_names_tkl[6][17] {
    //Row 0
    {
        KEY_EN_ESCAPE,
        KEY_EN_UNUSED,
        KEY_EN_F1,
        KEY_EN_F2,
        KEY_EN_F3,
        KEY_EN_F4,
        KEY_EN_F5,
        KEY_EN_F6,
        KEY_EN_F7,
        KEY_EN_F8,
        KEY_EN_F9,
        KEY_EN_F10,
        KEY_EN_F11,
        KEY_EN_F12,
        KEY_EN_PRINT_SCREEN,
        KEY_EN_PAUSE_BREAK,
        "Key: Custom1",
    },
    //row 1
    {
        KEY_EN_BACK_TICK,
        KEY_EN_1,
        KEY_EN_2,
        KEY_EN_3,
        KEY_EN_4,
        KEY_EN_5,
        KEY_EN_6,
        KEY_EN_7,
        KEY_EN_8,
        KEY_EN_9,
        KEY_EN_0,
        KEY_EN_MINUS,
        KEY_EN_PLUS,
        KEY_EN_BACKSPACE,
        KEY_EN_INSERT,
        KEY_EN_HOME,
        KEY_EN_PAGE_UP,
    },
    //row 2
    {
        KEY_EN_TAB,
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_BACK_SLASH,
        KEY_EN_DELETE,
        KEY_EN_END,
        KEY_EN_PAGE_DOWN,
    },
    //row 3
    {
        KEY_EN_CAPS_LOCK,
        KEY_EN_A,
        KEY_EN_S,
        KEY_EN_D,
        KEY_EN_F,
        KEY_EN_G,
        KEY_EN_H,
        KEY_EN_J,
        KEY_EN_K,
        KEY_EN_L,
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_POUND,
        KEY_EN_ANSI_ENTER,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
    },
    //row 4
    {
        KEY_EN_LEFT_SHIFT,
        KEY_EN_ISO_BACK_SLASH,
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_UNUSED,
        KEY_EN_UP_ARROW,
        KEY_EN_UNUSED,
    },
    //row 5
    {
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_SPACE,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_WINDOWS,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
    }
};

static unsigned int matrix_map_80HE[6][17] = {
    {   0,   1,   2,   3,   4,   5,   6,   7,   8,  NA,  10,  11,  12,  13,  14,  15,  16 },
    {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37 },
    {  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  NA,  56,  57,  58 },
    {  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  NA,  77,  NA,  NA },
    {  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  NA,  97,  98,  99,  NA },
    { 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,  NA, 119, 120, 121 }
};

static const char *led_names_80HE[6][17] {
    //Row 0
    {
        KEY_EN_ESCAPE,
        KEY_EN_F1,
        KEY_EN_F2,
        KEY_EN_F3,
        KEY_EN_F4,
        KEY_EN_F5,
        KEY_EN_F6,
        KEY_EN_F7,
        KEY_EN_F8,
        KEY_EN_UNUSED,
        KEY_EN_F9,
        KEY_EN_F10,
        KEY_EN_F11,
        KEY_EN_F12,
        "Key: Mode",
        KEY_EN_PRINT_SCREEN,
        KEY_EN_PAUSE_BREAK,
    },
    //row 1
    {
        KEY_EN_BACK_TICK,
        KEY_EN_1,
        KEY_EN_2,
        KEY_EN_3,
        KEY_EN_4,
        KEY_EN_5,
        KEY_EN_6,
        KEY_EN_7,
        KEY_EN_8,
        KEY_EN_9,
        KEY_EN_0,
        KEY_EN_MINUS,
        KEY_EN_PLUS,
        "Key: JIS Specific TODO",
        KEY_EN_BACKSPACE,
        KEY_EN_INSERT,
        KEY_EN_HOME,
    },
    //row 2
    {
        KEY_EN_TAB,
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_UNUSED,
        KEY_EN_BACK_SLASH,
        KEY_EN_DELETE,
        KEY_EN_END,
    },
    //row 3
    {
        KEY_EN_CAPS_LOCK,
        KEY_EN_A,
        KEY_EN_S,
        KEY_EN_D,
        KEY_EN_F,
        KEY_EN_G,
        KEY_EN_H,
        KEY_EN_J,
        KEY_EN_K,
        KEY_EN_L,
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_POUND,
        KEY_EN_UNUSED,
        KEY_EN_ANSI_ENTER,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
    },
    //row 4
    {
        KEY_EN_LEFT_SHIFT,
        KEY_EN_ISO_BACK_SLASH,
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_UNUSED,
        "Key: JIS Specific TODO",
        KEY_EN_RIGHT_SHIFT,
        KEY_EN_UP_ARROW,
        KEY_EN_UNUSED,
    },
    //row 5
    {
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        "Key: JIS Specific TODO",
        "Spacebar LED 1",
        "Spacebar LED 2",
        KEY_EN_SPACE,
        "Spacebar LED 3",
        "Spacebar LED 4",
        "Key: JIS Specific TODO",
        KEY_EN_RIGHT_ALT,
        KEY_EN_RIGHT_WINDOWS,
        KEY_EN_RIGHT_FUNCTION,
        KEY_EN_UNUSED,
        KEY_EN_LEFT_ARROW,
        KEY_EN_DOWN_ARROW,
        KEY_EN_RIGHT_ARROW,
    }
};

static unsigned int matrix_map_60[5][14] = {
    {  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34 },
    {  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55 },
    {  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76 },
    {  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  NA,  97 },
    { 105, 106, 107,  NA, 109, 110, 111, 112, 113,  NA, 115, 116, 117, 118 }
};

static const char *led_names_60HE[5][14] {
    //row 0
    {
        KEY_EN_ESCAPE,
        KEY_EN_1,
        KEY_EN_2,
        KEY_EN_3,
        KEY_EN_4,
        KEY_EN_5,
        KEY_EN_6,
        KEY_EN_7,
        KEY_EN_8,
        KEY_EN_9,
        KEY_EN_0,
        KEY_EN_MINUS,
        KEY_EN_PLUS,
        KEY_EN_BACKSPACE,
    },
    //row 1
    {
        KEY_EN_TAB,
        KEY_EN_Q,
        KEY_EN_W,
        KEY_EN_E,
        KEY_EN_R,
        KEY_EN_T,
        KEY_EN_Y,
        KEY_EN_U,
        KEY_EN_I,
        KEY_EN_O,
        KEY_EN_P,
        KEY_EN_LEFT_BRACKET,
        KEY_EN_RIGHT_BRACKET,
        KEY_EN_BACK_SLASH,
    },
    //row 2
    {
        KEY_EN_CAPS_LOCK,
        KEY_EN_A,
        KEY_EN_S,
        KEY_EN_D,
        KEY_EN_F,
        KEY_EN_G,
        KEY_EN_H,
        KEY_EN_J,
        KEY_EN_K,
        KEY_EN_L,
        KEY_EN_SEMICOLON,
        KEY_EN_QUOTE,
        KEY_EN_POUND,
        KEY_EN_ANSI_ENTER,
    },
    //row 3
    {
        KEY_EN_LEFT_SHIFT,
        KEY_EN_ISO_BACK_SLASH,
        KEY_EN_Z,
        KEY_EN_X,
        KEY_EN_C,
        KEY_EN_V,
        KEY_EN_B,
        KEY_EN_N,
        KEY_EN_M,
        KEY_EN_COMMA,
        KEY_EN_PERIOD,
        KEY_EN_FORWARD_SLASH,
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_SHIFT,
    },
    //row 4
    {
        KEY_EN_LEFT_CONTROL,
        KEY_EN_LEFT_WINDOWS,
        KEY_EN_LEFT_ALT,
        KEY_EN_UNUSED,
        "Spacebar LED 1",
        "Spacebar LED 2",
        KEY_EN_SPACE,
        "Spacebar LED 3",
        "Spacebar LED 4",
        KEY_EN_UNUSED,
        KEY_EN_RIGHT_ALT,
        KEY_EN_MENU,
        KEY_EN_RIGHT_CONTROL,
        KEY_EN_RIGHT_FUNCTION,
    }
};

static unsigned int matrix_map_3pad[5][7] = {
    {   0,  NA,   2,  NA,   4,  NA,   6 },
    {  21,  NA,  NA,  NA,  NA,  NA,  27 },
    {  42,  43,  NA,  45,  NA,  47,  48 },
    {  63,  NA,  NA,  NA,  NA,  NA,  69 },
    {  NA,  85,  86,  NA,  88,  89,  NA }
};

static const char *led_names_3pad[5][7] {
    //row 0
    {
        //top, right to left
        "Lightbar 14",
        KEY_EN_UNUSED,
        "Lightbar 13",
        KEY_EN_UNUSED,
        "Lightbar 12",
        KEY_EN_UNUSED,
        "Lightbar 11",
    },
    //row 1
    {
        "Lighbar 1", //left side, top led
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        "Lightbar 10",//right side, top led
    },
    //row 2
    {
        "Lightbar 2",//left side, middle led
        "Keypad 1",
        KEY_EN_UNUSED,
        "Keypad 2",
        KEY_EN_UNUSED,
        "Keypad 3",
        "Lightbar 9",//right side, middle led
    },
    //row 3
    {
        "Lightbar 3",//left side, bottom led
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        KEY_EN_UNUSED,
        "Lightbar 8",//right side, bottom led
    },
    //row 4
    {
        //bottom leds, left to right
        KEY_EN_UNUSED,
        "Lightbar 4",
        "Lightbar 5",
        KEY_EN_UNUSED,
        "Lightbar 6",
        "Lightbar 7",
        KEY_EN_UNUSED,
    }
};

/**------------------------------------------------------------------*\
    @name Wooting Keyboards
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectWootingV1KeyboardControllers,DetectWootingV2KeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_WootingKeyboard::RGBController_WootingKeyboard(WootingKeyboardController* controller_ptr)
{
    controller          = controller_ptr;

    LOG_DEBUG("%sAdding meta data", WOOTING_CONTROLLER_NAME);
    name                = controller->GetName();
    vendor              = controller->GetVendor();
    type                = DEVICE_TYPE_KEYBOARD;
    description         = controller->GetDescription();
    location            = controller->GetLocation();
    serial              = controller->GetSerial();

    LOG_DEBUG("%sAdding modes", WOOTING_CONTROLLER_NAME);
    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0xFFFF;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_WootingKeyboard::~RGBController_WootingKeyboard()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for (unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if (zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_WootingKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    WOOTING_DEVICE_TYPE wooting_type = controller->GetWootingType();
    const char** led_names = nullptr;
    unsigned int* matrix_map = nullptr;
    size_t matrix_rows = 0;
    size_t matrix_columns = 0;

    if(wooting_type == WOOTING_KB_TKL)
    {
        matrix_map   = (unsigned int *)matrix_map_tkl;
        led_names = (const char **)led_names_tkl;
        matrix_rows = 6;
        matrix_columns = 17;
    }
    else if(wooting_type == WOOTING_KB_80PER)
    {
        matrix_map    = (unsigned int *)matrix_map_80HE;
        led_names = (const char **)led_names_80HE;
        matrix_rows = 6;
        matrix_columns = 17;
    }
    else if(wooting_type == WOOTING_KB_60PER)
    {
        matrix_map    = (unsigned int *)matrix_map_60;
        led_names = (const char **)led_names_60HE;
        matrix_rows = 5;
        matrix_columns = 14;
    }
    else if(wooting_type == WOOTING_KB_3PAD)
    {
        matrix_map    = (unsigned int *)matrix_map_3pad;
        led_names = (const char **)led_names_3pad;
        matrix_rows = 5;
        matrix_columns = 7;
    }
    else // Fullsize and default
    {
        matrix_map    = (unsigned int *)matrix_map_full;
        led_names = (const char **)led_names_full;
        matrix_rows = 6;
        matrix_columns = 21;
    }

    unsigned int* new_matrix = new unsigned int[matrix_rows * matrix_columns];
    unsigned int total_led_count = 0;

    for(size_t row = 0; row < matrix_rows; row++)
    {
        for(size_t col = 0; col < matrix_columns; col++)
        {
            size_t idx = row * matrix_columns + col;
            unsigned int hardware_index = matrix_map[idx];

            if(hardware_index == NA)
            {
                new_matrix[idx] = NA;
                continue;
            }

            led new_led;
            new_led.name  = led_names[idx];
            new_led.value = hardware_index;

            leds.push_back(new_led);

            // 3. The zone map now points to the LED's position in the vector
            new_matrix[idx] = total_led_count;
            total_led_count++;
        }
    }



    zone new_zone;

    new_zone.name                   = name.append(" zone");
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = total_led_count;
    new_zone.leds_max               = total_led_count;
    new_zone.leds_count             = total_led_count;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = (unsigned int)matrix_rows;
    new_zone.matrix_map->width      = (unsigned int)matrix_columns;
    new_zone.matrix_map->map        = new_matrix;

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_WootingKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_WootingKeyboard::DeviceUpdateLEDs()
{
    RGBColor framebuffer[WOOTING_RGB_ROWS * WOOTING_RGB_COLUMNS] = {0};

    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned int framebuffer_index = leds[led_idx].value;
        framebuffer[framebuffer_index] = colors[led_idx];
    }

    controller->SendDirect(&framebuffer[0], WOOTING_RGB_ROWS * WOOTING_RGB_COLUMNS);
}

void RGBController_WootingKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_WootingKeyboard::DeviceUpdateMode()
{
}
