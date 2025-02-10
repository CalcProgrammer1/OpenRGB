/*---------------------------------------------------------*\
| KeyboardLayoutManager.cpp                                 |
|                                                           |
|   Helper library to produce keyboard layouts              |
|                                                           |
|   Chris M (Dr_No)                             04 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "KeyboardLayoutManager.h"

const char* KLM_CLASS_NAME              = "KLM";
const char* KEYBOARD_NAME_DEFAULT       = "DEFAULT ";
const char* KEYBOARD_NAME_ISO           = "ISO ";
const char* KEYBOARD_NAME_ANSI          = "ANSI ";
const char* KEYBOARD_NAME_JIS           = "JIS";
const char* KEYBOARD_NAME_AZERTY        = "AZERTY";
const char* KEYBOARD_NAME_QWERTY        = "QWERTY";
const char* KEYBOARD_NAME_QWERTZ        = "QWERTZ";

const char* KEYBOARD_NAME_FULL          = "Full 104 key ";
const char* KEYBOARD_NAME_TKL           = "Tenkeyless ";
const char* KEYBOARD_NAME_SIXTY         = "Sixty percent ";
const char* KEYBOARD_NAME_SEVENTY_FIVE  = "Seventy Five percent ";

const char* LOG_MSG_EMPTY               = "empty ";
const char* LOG_MSG_UNUSED_KEY          = "'unused' key";
const char* LOG_MSG_SHIFTING_RIGHT      = ", shifting keys right";
const char* LOG_MSG_CREATED_NEW         = "[%s] Created new %s%s with %d rows and %d columns containing %d keys";
const char* LOG_MSG_INSERT_BEFORE       = "[%s] Inserting %s before %s @ %02d, %02d%s";
const char* LOG_MSG_MISSING_OPCODE      = "[%s] Error: Opcode %d not found for %s @ %02d, %02d";

/*-------------------------------------------------------------------------*\
| Keyboard Base Maps                                                        |
|                                                                           |
|   The following maps define the following standardized sections of the    |
|   keyboard layout:                                                        |
|                                                                           |
|   *-----------------------------------*   *-----------*                   |
|   | Function Key Row (ESC, F1-F12)    |   | Extras    |                   |
|   *-----------------------------------*   |           |                   |
|                                           |           |                   |
|   *-----------------------------------*   |           |   *-----------*   |
|   | Main Key Block                    |   |           |   | Num Pad   |   |
|   |                                   |   |           |   |           |   |
|   |                                   |   |           |   |           |   |
|   *-----------------------------------*   *-----------*   *-----------*   |
|                                                                           |
|   The base keymap for a given keyboard size is assembled by combining the |
|   blocks used in that particular layout.                                  |
|                                                                           |
\*-------------------------------------------------------------------------*/

static const std::vector<keyboard_led> keyboard_zone_main =
{
    /*-----------------------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,     OpCode                              |
    \*-----------------------------------------------------------------------------------------------------------------------------*/
    {   0,      1,      0,          0,          KEY_EN_BACK_TICK,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      1,          0,          KEY_EN_1,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      2,          0,          KEY_EN_2,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      3,          0,          KEY_EN_3,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      4,          0,          KEY_EN_4,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      5,          0,          KEY_EN_5,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      6,          0,          KEY_EN_6,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      7,          0,          KEY_EN_7,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      8,          0,          KEY_EN_8,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      9,          0,          KEY_EN_9,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      10,         0,          KEY_EN_0,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      11,         0,          KEY_EN_MINUS,               KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      12,         0,          KEY_EN_EQUALS,              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      13,         0,          KEY_EN_BACKSPACE,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      0,          0,          KEY_EN_TAB,                 KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      1,          0,          KEY_EN_Q,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      2,          0,          KEY_EN_W,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      3,          0,          KEY_EN_E,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      4,          0,          KEY_EN_R,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      5,          0,          KEY_EN_T,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      6,          0,          KEY_EN_Y,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      7,          0,          KEY_EN_U,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      8,          0,          KEY_EN_I,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      9,          0,          KEY_EN_O,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      10,         0,          KEY_EN_P,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      11,         0,          KEY_EN_LEFT_BRACKET,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      12,         0,          KEY_EN_RIGHT_BRACKET,       KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      13,         0,          KEY_EN_ANSI_BACK_SLASH,     KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      0,          0,          KEY_EN_CAPS_LOCK,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      1,          0,          KEY_EN_A,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      2,          0,          KEY_EN_S,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      3,          0,          KEY_EN_D,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      4,          0,          KEY_EN_F,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      5,          0,          KEY_EN_G,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      6,          0,          KEY_EN_H,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      7,          0,          KEY_EN_J,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      8,          0,          KEY_EN_K,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      9,          0,          KEY_EN_L,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      10,         0,          KEY_EN_SEMICOLON,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      11,         0,          KEY_EN_QUOTE,               KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      12,         0,          KEY_EN_POUND,               KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },
    {   0,      3,      13,         0,          KEY_EN_ANSI_ENTER,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      0,          0,          KEY_EN_LEFT_SHIFT,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      1,          0,          KEY_EN_ISO_BACK_SLASH,      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      2,          0,          KEY_EN_Z,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      3,          0,          KEY_EN_X,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      4,          0,          KEY_EN_C,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      5,          0,          KEY_EN_V,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      6,          0,          KEY_EN_B,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      7,          0,          KEY_EN_N,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      8,          0,          KEY_EN_M,                   KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      9,          0,          KEY_EN_COMMA,               KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      10,         0,          KEY_EN_PERIOD,              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      11,         0,          KEY_EN_FORWARD_SLASH,       KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      13,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      0,          0,          KEY_EN_LEFT_CONTROL,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      1,          0,          KEY_EN_LEFT_WINDOWS,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      2,          0,          KEY_EN_LEFT_ALT,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      6,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      10,         0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      11,         0,          KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      12,         0,          KEY_EN_MENU,                KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      13,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
};

static const std::vector<keyboard_led> keyboard_zone_fn_row =
{
    /*-----------------------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,     OpCode                              |
    \*-----------------------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      0,          0,          KEY_EN_ESCAPE,              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      2,          0,          KEY_EN_F1,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      3,          0,          KEY_EN_F2,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      4,          0,          KEY_EN_F3,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      5,          0,          KEY_EN_F4,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      6,          0,          KEY_EN_F5,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      7,          0,          KEY_EN_F6,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      8,          0,          KEY_EN_F7,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      9,          0,          KEY_EN_F8,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      10,         0,          KEY_EN_F9,                  KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      11,         0,          KEY_EN_F10,                 KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      12,         0,          KEY_EN_F11,                 KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      13,         0,          KEY_EN_F12,                 KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
};

static const std::vector<keyboard_led> keyboard_zone_extras =
{
    /*-----------------------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,     OpCode                              |
    \*-----------------------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      14,         0,          KEY_EN_PRINT_SCREEN,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      15,         0,          KEY_EN_SCROLL_LOCK,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      0,      16,         0,          KEY_EN_PAUSE_BREAK,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      14,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      15,         0,          KEY_EN_HOME,                KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      16,         0,          KEY_EN_PAGE_UP,             KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      14,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      15,         0,          KEY_EN_END,                 KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      16,         0,          KEY_EN_PAGE_DOWN,           KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      15,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      14,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      16,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
};

static const std::vector<keyboard_led> keyboard_zone_numpad =
{
    /*-----------------------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,     OpCode                              |
    \*-----------------------------------------------------------------------------------------------------------------------------*/
    {   0,      1,      17,         0,          KEY_EN_NUMPAD_LOCK,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      18,         0,          KEY_EN_NUMPAD_DIVIDE,       KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      19,         0,          KEY_EN_NUMPAD_TIMES,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      1,      20,         0,          KEY_EN_NUMPAD_MINUS,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      17,         0,          KEY_EN_NUMPAD_7,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      18,         0,          KEY_EN_NUMPAD_8,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      19,         0,          KEY_EN_NUMPAD_9,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      2,      20,         0,          KEY_EN_NUMPAD_PLUS,         KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      17,         0,          KEY_EN_NUMPAD_4,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      18,         0,          KEY_EN_NUMPAD_5,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      3,      19,         0,          KEY_EN_NUMPAD_6,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      17,         0,          KEY_EN_NUMPAD_1,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      18,         0,          KEY_EN_NUMPAD_2,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      19,         0,          KEY_EN_NUMPAD_3,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      4,      20,         0,          KEY_EN_NUMPAD_ENTER,        KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      18,         0,          KEY_EN_NUMPAD_0,            KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    {   0,      5,      19,         0,          KEY_EN_NUMPAD_PERIOD,       KEY_EN_UNUSED,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
};

keyboard_keymap_overlay iso_azerty
{
    KEYBOARD_SIZE_FULL,
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_UNUSED,              KEY_FR_ASTERIX,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      1,          0,          KEY_EN_UNUSED,              KEY_NORD_ANGLE_BRACKET,     KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_FR_SUPER_2,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      1,          0,          KEY_EN_UNUSED,              KEY_FR_AMPERSAND,           KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      2,          0,          KEY_EN_UNUSED,              KEY_FR_ACUTE_E,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      3,          0,          KEY_EN_UNUSED,              KEY_FR_DOUBLEQUOTE,         KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      4,          0,          KEY_EN_UNUSED,              KEY_EN_QUOTE,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      5,          0,          KEY_EN_UNUSED,              KEY_FR_LEFT_PARENTHESIS,    KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      6,          0,          KEY_EN_UNUSED,              KEY_EN_MINUS,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      7,          0,          KEY_EN_UNUSED,              KEY_FR_GRAVE_E,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      8,          0,          KEY_EN_UNUSED,              KEY_FR_UNDERSCORE,          KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      9,          0,          KEY_EN_UNUSED,              KEY_FR_CEDILLA_C,           KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      10,         0,          KEY_EN_UNUSED,              KEY_FR_GRAVE_A,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      11,         0,          KEY_EN_UNUSED,              KEY_FR_RIGHT_PARENTHESIS,   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      1,          0,          KEY_EN_UNUSED,              KEY_EN_A,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      2,          0,          KEY_EN_UNUSED,              KEY_EN_Z,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      11,         0,          KEY_EN_UNUSED,              KEY_JP_CHEVRON,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      12,         0,          KEY_EN_UNUSED,              KEY_FR_DOLLAR,              KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      3,      1,          0,          KEY_EN_UNUSED,              KEY_EN_Q,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      3,      10,         0,          KEY_EN_UNUSED,              KEY_EN_M,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      3,      11,         0,          KEY_EN_UNUSED,              KEY_FR_GRAVE_U,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      2,          0,          KEY_EN_UNUSED,              KEY_EN_W,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      8,          0,          KEY_EN_UNUSED,              KEY_EN_COMMA,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      9,          0,          KEY_EN_UNUSED,              KEY_EN_SEMICOLON,           KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      10,         0,          KEY_EN_UNUSED,              KEY_JP_COLON,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      11,         0,          KEY_EN_UNUSED,              KEY_FR_EXCLAIMATION,        KEYBOARD_OPCODE_ADD_ALT_NAME,       },
    }
};

keyboard_keymap_overlay ansi_qwerty
{
    KEYBOARD_SIZE_FULL,
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      4,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
    }
};

keyboard_keymap_overlay iso_qwerty
{
    KEYBOARD_SIZE_FULL,
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
    }
};

keyboard_keymap_overlay iso_qwertz
{
    KEYBOARD_SIZE_FULL,
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_UNUSED,              KEY_EN_POUND,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      1,          0,          KEY_EN_UNUSED,              KEY_NORD_ANGLE_BRACKET,     KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_JP_CHEVRON,             KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      11,         0,          KEY_EN_UNUSED,              KEY_DE_ESZETT,              KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      1,      12,         0,          KEY_EN_UNUSED,              KEY_EN_BACK_TICK,           KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      6,          0,          KEY_EN_UNUSED,              KEY_EN_Z,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      11,         0,          KEY_EN_UNUSED,              KEY_DE_DIAERESIS_U,         KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      12,         0,          KEY_EN_UNUSED,              KEY_EN_PLUS,                KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      3,      10,         0,          KEY_EN_UNUSED,              KEY_DE_DIAERESIS_O,         KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      3,      11,         0,          KEY_EN_UNUSED,              KEY_DE_DIAERESIS_A,         KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      2,          0,          KEY_EN_UNUSED,              KEY_EN_Y,                   KEYBOARD_OPCODE_ADD_ALT_NAME,       },
        {   0,      4,      11,         0,          KEY_EN_UNUSED,              KEY_EN_MINUS,               KEYBOARD_OPCODE_ADD_ALT_NAME,       },
    }
};

keyboard_keymap_overlay jis
{
    KEYBOARD_SIZE_FULL,
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_RIGHT_BRACKET,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      4,      12,         0,          KEY_EN_BACK_SLASH,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      1,      12,         0,          KEY_JP_CHEVRON,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      2,      11,         0,          KEY_JP_AT,                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      2,      12,         0,          KEY_EN_LEFT_BRACKET,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      3,      11,         0,          KEY_JP_COLON,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_ADD_ALT_NAME,    },
    }
};

KeyboardLayoutManager::KeyboardLayoutManager(KEYBOARD_LAYOUT layout, KEYBOARD_SIZE size) : KeyboardLayoutManager(layout, size, {})
{
}

KeyboardLayoutManager::KeyboardLayoutManager(KEYBOARD_LAYOUT layout, KEYBOARD_SIZE size, layout_values values)
{
    /*---------------------------------------------------------------------*\
    | Store given size bitfield                                             |
    \*---------------------------------------------------------------------*/
    physical_size = size;

    /*---------------------------------------------------------------------*\
    | If the given size is EMPTY, we are done.  No keys need added          |
    \*---------------------------------------------------------------------*/
    if(physical_size == KEYBOARD_SIZE::KEYBOARD_SIZE_EMPTY)
    {
        LOG_INFO(LOG_MSG_CREATED_NEW, KLM_CLASS_NAME, name.c_str(), LOG_MSG_EMPTY, rows, cols, physical_size);
        return;
    }

    /*---------------------------------------------------------------------*\
    | Add sections to the keymap based on KEYBOARD_SIZE bitfield            |
    \*---------------------------------------------------------------------*/
    if(physical_size & KEYBOARD_ZONE_MAIN)
    {
        InsertKeys(keyboard_zone_main);
    }

    if(physical_size & KEYBOARD_ZONE_FN_ROW)
    {
        InsertKeys(keyboard_zone_fn_row);
    }

    if(physical_size & KEYBOARD_ZONE_EXTRA)
    {
        InsertKeys(keyboard_zone_extras);
    }

    if(physical_size & KEYBOARD_ZONE_NUMPAD)
    {
        InsertKeys(keyboard_zone_numpad);
    }

    /*---------------------------------------------------------------------*\
    | Add any values passed into the constructor before switching layouts   |
    |   and declare a value set for any changes afterwards                  |
    \*---------------------------------------------------------------------*/
    for(size_t key_idx = 0; key_idx < (unsigned int)values.default_values.size() && key_idx < keymap.size(); key_idx++)
    {
        keymap[key_idx].value = values.default_values[key_idx];
    }

    /*---------------------------------------------------------------------*\
    | Modify the base default QWERTY layout to the desired regional layout  |
    \*---------------------------------------------------------------------*/
    std::string tmp_name;

    switch(layout)
    {
        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_DEFAULT:
        default:
            tmp_name = KEYBOARD_NAME_DEFAULT;
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ANSI_QWERTY:
            ChangeKeys(ansi_qwerty);
            tmp_name = KEYBOARD_NAME_ANSI;
            tmp_name.append(KEYBOARD_NAME_QWERTY);
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTY:
            ChangeKeys(iso_qwerty);
            tmp_name = KEYBOARD_NAME_ISO;
            tmp_name.append(KEYBOARD_NAME_QWERTY);
            break;

        /*-------------------------------------------------*\
        | Non-English, non-QWERTY layouts are disabled      |
        | until proper translation feature is implemented   |
        \*-------------------------------------------------*/
        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_AZERTY:
            ChangeKeys(iso_azerty);
            tmp_name = KEYBOARD_NAME_AZERTY;
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTZ:
            ChangeKeys(iso_qwertz);
            tmp_name = KEYBOARD_NAME_QWERTZ;
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_JIS:
            ChangeKeys(jis);
            tmp_name = KEYBOARD_NAME_JIS;
            break;
    }

    /*---------------------------------------------------------------------*\
    | If the regional layouts were passed in count() returns true before    |
    |   attempting to swap keys.                                            |
    \*---------------------------------------------------------------------*/
    bool found_overlay = (bool)values.regional_overlay.count(layout);

    LOG_DEBUG("[%s] Regional overlay %d was %sfound.", KLM_CLASS_NAME, layout, (found_overlay) ? KEY_EN_UNUSED : "not ");
    if(found_overlay)
    {
        LOG_DEBUG("[%s] Processing regional overlay for %s", KLM_CLASS_NAME, tmp_name.c_str());
        SwapKeys(values.regional_overlay.find(layout)->second);
    }

    /*---------------------------------------------------------------------*\
    | Size specific fixes                                                   |
    \*---------------------------------------------------------------------*/
    switch(size)
    {
        case KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY:
            /*-------------------------------------------------------------*\
            | Remove the empty Function row and swap in the Escape key      |
            \*-------------------------------------------------------------*/
            name = KEYBOARD_NAME_SIXTY;
            RemoveRow(0);
            SwapKey(keyboard_zone_fn_row[0]);
            break;

        case KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE:
            name = KEYBOARD_NAME_SEVENTY_FIVE;
            break;

        case KEYBOARD_SIZE::KEYBOARD_SIZE_TKL:
            name = KEYBOARD_NAME_TKL;
            break;

        case KEYBOARD_SIZE::KEYBOARD_SIZE_FULL:
            name = KEYBOARD_NAME_FULL;
            break;

        default:
            /*-------------------------------------------------------------*\
            | If the keyboard size is not a standard size output            |
            |   the combined number as a string                             |
            \*-------------------------------------------------------------*/
            name = "Size (";
            name.append(std::to_string(size) + ") ");
    }

    /*---------------------------------------------------------------------*\
    | Ensure rows and cols are accurate by updating dimensions              |
    \*---------------------------------------------------------------------*/
    UpdateDimensions();

    LOG_INFO(LOG_MSG_CREATED_NEW, KLM_CLASS_NAME, name.c_str(), tmp_name.c_str(), rows, cols, keymap.size());
}

KeyboardLayoutManager::~KeyboardLayoutManager()
{

}

void KeyboardLayoutManager::ChangeKeys(key_set edit_keys)
{
    OpCodeSwitch(edit_keys);
}

void KeyboardLayoutManager::ChangeKeys(keyboard_keymap_overlay new_layout)
{
    OpCodeSwitch(new_layout.edit_keys);
}

void KeyboardLayoutManager::ChangeKeys(keyboard_keymap_overlay_values new_layout)
{
    OpCodeSwitch(new_layout.edit_keys);
}

void KeyboardLayoutManager::OpCodeSwitch(key_set change_keys)
{
    LOG_DEBUG("[%s] %d keys to edit", KLM_CLASS_NAME, change_keys.size());

    for(size_t chg_key_idx = 0; chg_key_idx < (unsigned int)change_keys.size(); chg_key_idx++)
    {
        switch(change_keys[chg_key_idx].opcode)
        {
            case KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT:
                InsertKey(change_keys[chg_key_idx]);
                break;

            case KEYBOARD_OPCODE_SWAP_ONLY:
                SwapKey(change_keys[chg_key_idx]);
                break;

            case KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT:
                RemoveKey(change_keys[chg_key_idx]);
                break;

            case KEYBOARD_OPCODE_INS_SHFT_ADJACENT:
                //TODO: Insert, then find next unused and remove shift left
                //SwapKey(change_keys[chg_key_idx]);
                break;

            case KEYBOARD_OPCODE_INSERT_ROW:
                if(InsertRow(change_keys[chg_key_idx].row))
                {
                    SwapKey(change_keys[chg_key_idx]);
                }
                break;

            case KEYBOARD_OPCODE_REMOVE_ROW:
                RemoveRow(change_keys[chg_key_idx].row);
                break;

            case KEYBOARD_OPCODE_ADD_ALT_NAME:
                AddAltName(change_keys[chg_key_idx]);
                break;

            default:
                LOG_DEBUG(LOG_MSG_MISSING_OPCODE, KLM_CLASS_NAME, change_keys[chg_key_idx].opcode,
                          change_keys[chg_key_idx].name, change_keys[chg_key_idx].row, change_keys[chg_key_idx].col);
        }
    }

    UpdateDimensions();
}

void KeyboardLayoutManager::InsertKey(keyboard_led ins_key)
{
    /*---------------------------------------------------------------------*\
    | Get the insertion point                                               |
    \*---------------------------------------------------------------------*/
    unsigned int    ins_row     = ins_key.row;
    unsigned int    ins_col     = ins_key.col;
    const char*     ins_name    = ins_key.name;

    unsigned int    key_idx     = 0;

    for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
    {
        /*---------------------------------------------------------------------*\
        | Search through all existing keys and determine where in the list to   |
        | insert the new key.  Order is row first, then column.                 |
        \*---------------------------------------------------------------------*/
        if((ins_row < keymap[key_idx].row) || ((ins_row == keymap[key_idx].row) && (ins_col <= keymap[key_idx].col)))
        {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | Determine whether to update row shift or not                          |
    \*---------------------------------------------------------------------*/
    bool update_row = true;

    /*---------------------------------------------------------------------*\
    | If the search reached the end, put the new key at the end of the list |
    \*---------------------------------------------------------------------*/
    if(key_idx == (unsigned int)keymap.size())
    {
        LOG_DEBUG(LOG_MSG_INSERT_BEFORE, KLM_CLASS_NAME, ins_name, "the end", ins_row, ins_col, KEY_EN_UNUSED);
        keymap.push_back(ins_key);
        update_row = false;
    }

    /*---------------------------------------------------------------------*\
    | If inserting an empty key in the middle of the list, the key entry is |
    | not actually added.  Instead, increment the col field of all keys on  |
    | the same row after the inserted key.                                  |
    \*---------------------------------------------------------------------*/
    else if(strlen(ins_name) == 0)
    {
        LOG_DEBUG(LOG_MSG_INSERT_BEFORE, KLM_CLASS_NAME, LOG_MSG_UNUSED_KEY, keymap[key_idx].name, keymap[key_idx].row, keymap[key_idx].col, LOG_MSG_SHIFTING_RIGHT);
    }
    else
    {
        LOG_DEBUG(LOG_MSG_INSERT_BEFORE, KLM_CLASS_NAME, ins_name, keymap[key_idx].name, ins_row, ins_col, KEY_EN_UNUSED);
        keymap.insert(keymap.begin() + key_idx, ins_key);
        key_idx++;
    }

    /*---------------------------------------------------------------------*\
    | If update_row is true, key at key_idx is not the end of the vector.   |
    | For the remaining keys, if the row is equal to the inserted key row,  |
    | shift 1 column right                                                  |
    \*---------------------------------------------------------------------*/
    if(update_row)
    {
        for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
        {
            if((keymap[key_idx].row == ins_row) && (keymap[key_idx].col >= ins_col))
            {
                keymap[key_idx].col++;
            }

            if(keymap[key_idx].row > ins_row)
            {
                break;
            }
        }
    }
}

void KeyboardLayoutManager::InsertKeys(std::vector<keyboard_led> ins_keys)
{
    LOG_DEBUG("[%s] %d keys to insert", KLM_CLASS_NAME, ins_keys.size());

    /*---------------------------------------------------------------------*\
    | Insert new keys one by one                                            |
    \*---------------------------------------------------------------------*/
    for(unsigned int ins_key_idx = 0; ins_key_idx < (unsigned int)ins_keys.size(); ins_key_idx++)
    {
        InsertKey(ins_keys[ins_key_idx]);
    }

    /*---------------------------------------------------------------------*\
    | Ensure rows and cols are accurate by updating dimensions after insert |
    \*---------------------------------------------------------------------*/
    UpdateDimensions();
}

void KeyboardLayoutManager::SwapKey(keyboard_led swp_key)
{
    /*---------------------------------------------------------------------*\
    | Get the swap point                                                    |
    \*---------------------------------------------------------------------*/
    unsigned int    swp_row     = swp_key.row;
    unsigned int    swp_col     = swp_key.col;
    const char*     swp_name    = swp_key.name;
    unsigned int    swp_value   = swp_key.value;

    /*---------------------------------------------------------------------*\
    | If the keymap is empty, insert the key                                |
    \*---------------------------------------------------------------------*/
    if(keymap.size() == 0)
    {
        keymap.push_back(swp_key);
        return;
    }

    /*---------------------------------------------------------------------*\
    | Otherwise, loop through and either swap an existing entry or insert   |
    | a new entry if the given location does not already have a key present |
    \*---------------------------------------------------------------------*/
    for(unsigned int key_idx = 0; key_idx < (unsigned int)keymap.size(); key_idx++)
    {
        /*---------------------------------------------------------------------*\
        | If the row and column are identical, we've found the swap location    |
        \*---------------------------------------------------------------------*/
        if((swp_row == keymap[key_idx].row) && (swp_col == keymap[key_idx].col))
        {
            std::string tmp_name = (strlen(swp_name) == 0) ? LOG_MSG_UNUSED_KEY : swp_name;
            LOG_DEBUG("[%s] Swapping in %s and %s out @ %02d, %02d", KLM_CLASS_NAME, tmp_name.c_str(), keymap[key_idx].name, swp_row, swp_col);

            /*---------------------------------------------------------------------*\
            | If the key to be swapped in is an unused key, we want to remove the   |
            | entry from the keymap rather than perform a swap                      |
            \*---------------------------------------------------------------------*/
            if(strlen(swp_name) == 0)
            {
                keymap.erase(keymap.begin() + key_idx);
            }
            /*---------------------------------------------------------------------*\
            | Otherwise, update the entry at this position with the new name and    |
            | value                                                                 |
            \*---------------------------------------------------------------------*/
            else
            {
                keymap[key_idx].name    = swp_name;
                keymap[key_idx].value   = swp_value;
            }
            break;
        }

        /*---------------------------------------------------------------------*\
        | If the key row is greater than the swap key row OR the key row is     |
        | equal to the swap key row and the key column is greater than the swap |
        | key column, we've gone past the swap location without a match.  In    |
        | this situation, we need to insert the swap key into the empty location|
        | without performing a shift right.                                     |
        \*---------------------------------------------------------------------*/
        if((keymap[key_idx].row > swp_row)
         ||((keymap[key_idx].row == swp_row) && (keymap[key_idx].col > swp_col)))
        {
            /*---------------------------------------------------------------------*\
            | Only insert the new key if the new key is not unused                  |
            \*---------------------------------------------------------------------*/
            if(strlen(swp_name) != 0)
            {
                LOG_DEBUG(LOG_MSG_INSERT_BEFORE, KLM_CLASS_NAME, swp_name, keymap[key_idx].name, swp_row, swp_col, KEY_EN_UNUSED);
                if(key_idx == 0)
                {
                    keymap.insert(keymap.begin(), swp_key);
                }
                else
                {
                    keymap.insert(keymap.begin() + (key_idx - 1), swp_key);
                }
            }
            break;
        }
    }
}

void KeyboardLayoutManager::SwapKeys(std::vector<keyboard_led> swp_keys)
{
    LOG_DEBUG("[%s] %d keys to swap", KLM_CLASS_NAME, swp_keys.size());

    /*---------------------------------------------------------------------*\
    | Swap keys one by one                                                  |
    \*---------------------------------------------------------------------*/
    for(unsigned int swp_key_idx = 0; swp_key_idx < (unsigned int)swp_keys.size(); swp_key_idx++)
    {
        SwapKey(swp_keys[swp_key_idx]);
    }
}

void KeyboardLayoutManager::RemoveKey(keyboard_led rmv_key)
{
    /*---------------------------------------------------------------------*\
    | Get the remove point                                                  |
    \*---------------------------------------------------------------------*/
    unsigned int    rmv_row     = rmv_key.row;
    unsigned int    rmv_col     = rmv_key.col;

    /*---------------------------------------------------------------------*\
    | Loop through and find the entry to remove                             |
    \*---------------------------------------------------------------------*/
    for(unsigned int key_idx = 0; key_idx < (unsigned int)keymap.size(); key_idx++)
    {
        /*---------------------------------------------------------------------*\
        | If the row and column are identical, we've found the swap location    |
        \*---------------------------------------------------------------------*/
        if((rmv_row == keymap[key_idx].row) && (rmv_col == keymap[key_idx].col))
        {
            LOG_DEBUG("[%s] Removing %s @ %02d, %02d and shifting keys left", KLM_CLASS_NAME, keymap[key_idx].name, rmv_row, rmv_col);
            keymap.erase(keymap.begin() + key_idx);

            for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
            {
                if(rmv_row == keymap[key_idx].row)
                {
                    keymap[key_idx].col--;
                }
                else
                {
                    break;
                }
            }

            break;
        }

        if((rmv_row == keymap[key_idx].row) && (rmv_col < keymap[key_idx].col))
        {
            LOG_DEBUG("[%s] Removing unused key @ %02d, %02d and shifting keys left", KLM_CLASS_NAME, rmv_row, rmv_col);

            for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
            {
                if(rmv_row == keymap[key_idx].row)
                {
                    keymap[key_idx].col--;
                }
                else
                {
                    break;
                }
            }

            break;
        }
    }
}

bool KeyboardLayoutManager::InsertRow(uint8_t ins_row)
{
    /*---------------------------------------------------------------------*\
    | Check row is valid to Insert                                          |
    \*---------------------------------------------------------------------*/
    if(ins_row >= rows)
    {
        LOG_DEBUG("[%s] Inserting row %d failed as rows currently = %d", KLM_CLASS_NAME, ins_row, rows);
        return false;
    }

    /*---------------------------------------------------------------------*\
    | Loop through to find the first key in the row to insert               |
    \*---------------------------------------------------------------------*/
    unsigned int key_idx = 0;

    for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
    {
        if(ins_row <= keymap[key_idx].row)
        {
            break;
        }
    }

    LOG_DEBUG("[%s] Attempting to insert row %d before %s at index %d",
              KLM_CLASS_NAME, ins_row, keymap[key_idx].name, key_idx);
    /*---------------------------------------------------------------------*\
    | Loop through the remaining rows and adjust row number                 |
    \*---------------------------------------------------------------------*/
    if(ins_row <= keymap[key_idx].row)
    {
        for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
        {
            keymap[key_idx].row++;
        }

        LOG_DEBUG("[%s] Insert row %d successful", KLM_CLASS_NAME, ins_row);
    }
    return true;
}

void KeyboardLayoutManager::RemoveRow(uint8_t rmv_row)
{
    /*---------------------------------------------------------------------*\
    | Check row is valid to remove                                          |
    \*---------------------------------------------------------------------*/
    if(rmv_row >= rows)
    {
        LOG_DEBUG("[%s] Removing row %d failed as rows currently = %d", KLM_CLASS_NAME, rmv_row, rows);
        return;
    }

    /*---------------------------------------------------------------------*\
    | Loop through and remove any keys in the row                           |
    \*---------------------------------------------------------------------*/
    unsigned int key_idx = 0;

    while(key_idx < (unsigned int)keymap.size() && rmv_row >= keymap[key_idx].row)
    {
        if(rmv_row == keymap[key_idx].row)
        {
            LOG_DEBUG("[%s] Removing %s @ %02d, %02d from row %d", KLM_CLASS_NAME, keymap[key_idx].name, keymap[key_idx].row, keymap[key_idx].col, rmv_row);
            keymap.erase(keymap.begin() + key_idx);
        }
        else
        {
            key_idx++;
        }
    }

    /*---------------------------------------------------------------------*\
    | Loop through the remaining rows and adjust row number                 |
    \*---------------------------------------------------------------------*/
    if(rmv_row < keymap[key_idx].row)
    {
        for(/*key_idx*/; key_idx < (unsigned int)keymap.size(); key_idx++)
        {
            keymap[key_idx].row--;
        }

        LOG_DEBUG("[%s] Remove row %d successful", KLM_CLASS_NAME, rmv_row);
    }
}

void KeyboardLayoutManager::AddAltName(keyboard_led key)
{
    /*---------------------------------------------------------------------*\
    | Get the edit point                                                    |
    \*---------------------------------------------------------------------*/
    unsigned int    edit_row        = key.row;
    unsigned int    edit_col        = key.col;
    const char*     edit_alt_name   = key.alt_name;

    /*---------------------------------------------------------------------*\
    | Otherwise, loop through and find the edit location                    |
    \*---------------------------------------------------------------------*/
    for(unsigned int key_idx = 0; key_idx < keymap.size(); key_idx++)
    {
        /*---------------------------------------------------------------------*\
        | If the row and column are identical, we've found the edit location    |
        \*---------------------------------------------------------------------*/
        if((edit_row == keymap[key_idx].row) && (edit_col == keymap[key_idx].col))
        {
            /*---------------------------------------------------------------------*\
            | Update the entry at this position with the new translated name        |
            \*---------------------------------------------------------------------*/
            LOG_DEBUG("[%s] Adding alternate name %s to %s @ %02d, %02d", KLM_CLASS_NAME, edit_alt_name, keymap[key_idx].name, keymap[key_idx].row, keymap[key_idx].col);
            keymap[key_idx].alt_name = edit_alt_name;
            break;
        }
    }
}

std::string KeyboardLayoutManager::GetName()
{
    return name;
}

KEYBOARD_LAYOUT KeyboardLayoutManager::GetLayout()
{
    return layout;
}

KEYBOARD_SIZE KeyboardLayoutManager::GetPhysicalSize()
{
    return physical_size;
}

unsigned int KeyboardLayoutManager::GetKeyCount()
{
    return (unsigned int)keymap.size();
}

std::string KeyboardLayoutManager::GetKeyNameAt(unsigned int key_idx)
{
    if(key_idx < (unsigned int)keymap.size())
    {
        return keymap[key_idx].name;
    }

    return KEY_EN_UNUSED;
}

std::string KeyboardLayoutManager::GetKeyNameAt(unsigned int row, unsigned int col)
{
    for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
    {
        if(key->row == row && key->col == col)
        {
            return key->name;
        }
    }

    return KEY_EN_UNUSED;
}

unsigned int KeyboardLayoutManager::GetKeyValueAt(unsigned int key_idx)
{
    if(key_idx < keymap.size())
    {
        return keymap[key_idx].value;
    }

    return -1;
}

unsigned int KeyboardLayoutManager::GetKeyValueAt(unsigned int row, unsigned int col)
{
    for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
    {
        if(key->row == row && key->col == col)
        {
            return key->value;
        }
    }

    return -1;
}

std::string KeyboardLayoutManager::GetKeyAltNameAt(unsigned int key_idx)
{
    if(key_idx < keymap.size())
    {
        return keymap[key_idx].alt_name;
    }

    return KEY_EN_UNUSED;
}

std::string KeyboardLayoutManager::GetKeyAltNameAt(unsigned int row, unsigned int col)
{
    for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
    {
        if(key->row == row && key->col == col)
        {
            return key->alt_name;
        }
    }

    return KEY_EN_UNUSED;
}

unsigned int KeyboardLayoutManager::GetRowCount()
{
    return rows;
}

unsigned int KeyboardLayoutManager::GetColumnCount()
{
    return cols;
}

void KeyboardLayoutManager::GetKeyMap(unsigned int* map_ptr)
{
    GetKeyMap(map_ptr, KEYBOARD_MAP_FILL_TYPE_INDEX, rows, cols);
}

void KeyboardLayoutManager::GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type)
{
    GetKeyMap(map_ptr, fill_type, rows, cols);
}

void KeyboardLayoutManager::GetKeyMap(unsigned int* map_ptr, KEYBOARD_MAP_FILL_TYPE fill_type, uint8_t height = 0, uint8_t width = 0)
{
    unsigned int no_key = -1;
    width               = std::max(width, cols);
    height              = std::max(height, rows);

    for(unsigned int r = 0; r < height; r++)
    {
        unsigned int offset = r * width;

        for(unsigned int c = 0; c < width; c++)
        {
            map_ptr[offset + c] = no_key;
        }
    }

    for(unsigned int i = 0; i < (unsigned int)keymap.size(); i++)
    {
        unsigned int offset = (keymap[i].row * width) + keymap[i].col;
        switch(fill_type)
        {
            case KEYBOARD_MAP_FILL_TYPE_COUNT:
                map_ptr[offset] = i;
                break;

            case KEYBOARD_MAP_FILL_TYPE_VALUE:
                map_ptr[offset] = keymap[i].value;
                break;

            case KEYBOARD_MAP_FILL_TYPE_INDEX:
            default:
                map_ptr[offset] = offset;
                break;
        }
    }
}

void KeyboardLayoutManager::UpdateDimensions()
{
    /*---------------------------------------------------------------------*\
    | Compute max_row and max_col.                                          |
    \*---------------------------------------------------------------------*/
    uint8_t max_row = 0;
    uint8_t max_col = 0;

    /*---------------------------------------------------------------------*\
    | Search through the keymap and find the maximum row and column values  |
    \*---------------------------------------------------------------------*/
    for(unsigned int key_idx = 0; key_idx < (unsigned int)keymap.size(); key_idx++)
    {
        if(keymap[key_idx].row > max_row)
        {
            max_row = keymap[key_idx].row;
        }
        if(keymap[key_idx].col > max_col)
        {
            max_col = keymap[key_idx].col;
        }
    }

    /*---------------------------------------------------------------------*\
    | The size is one greater than the highest row/column value             |
    \*---------------------------------------------------------------------*/
    rows = max_row + 1;
    cols = max_col + 1;
}
