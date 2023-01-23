/*-----------------------------------------*\
|  RazerDevices.cpp                         |
|                                           |
|  Razer device table static data           |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/4/2021    |
\*-----------------------------------------*/

#include "RazerDevices.h"

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
#define BLACKWIDOW_CHROMA_KEYMAP_SIZE (sizeof(blackwidow_chroma_keymap) / sizeof(blackwidow_chroma_keymap[0]))

static const razer_key blackwidow_chroma_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              "Key: M1",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              "Key: M2",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              "Key: M3",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              "Key: M4",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              "Key: M5",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_CHROMA_TE_KEYMAP_SIZE (sizeof(blackwidow_chroma_te_keymap) / sizeof(blackwidow_chroma_te_keymap[0]))

static const razer_key blackwidow_chroma_te_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_V3_TKL_KEYMAP_SIZE (sizeof(blackwidow_v3_tkl_keymap) / sizeof(blackwidow_v3_tkl_keymap[0]))

static const razer_key blackwidow_v3_tkl_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_V3_MINI_KEYMAP_SIZE (sizeof(blackwidow_v3_mini_keymap) / sizeof(blackwidow_v3_mini_keymap[0]))

static const razer_key blackwidow_v3_mini_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      0,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      1,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      1,      15,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      2,      13,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      3,      2,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
};

#define BOOK_13_2020_KEYMAP_SIZE (sizeof(book_13_2020_keymap) / sizeof(book_13_2020_keymap[0]))

static const razer_key book_13_2020_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             "Key: Power",               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      15,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLADE_14_2021_KEYMAP_SIZE (sizeof(blade_14_2021_keymap) / sizeof(blade_14_2021_keymap[0]))

static const razer_key blade_14_2021_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             "Key: Power",               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      15,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLADE_14_2022_KEYMAP_SIZE (sizeof(blade_14_2022_keymap) / sizeof(blade_14_2022_keymap[0]))

static const razer_key blade_14_2022_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_POWER,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      1,      1,              KEY_JP_CHEVRON,             RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      1,      12,             KEY_DE_ESZETT,              RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      1,      13,             KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      2,      7,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      2,      12,             KEY_DE_DIAERESIS_U,         RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      2,      13,             KEY_EN_PLUS,                RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      3,      11,             KEY_DE_DIAERESIS_O,         RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      3,      12,             KEY_DE_DIAERESIS_A,         RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      3,      15,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      2,              KEY_NORD_ANGLE_BRACKET,     RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      4,      3,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ANSI | RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS },
    {   0,      4,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_QWERTZ                        },
    {   0,      4,      15,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLADE_17_PRO_2021_KEYMAP_SIZE (sizeof(blade_17_pro_2021_keymap) / sizeof(blade_17_pro_2021_keymap[0]))

static const razer_key blade_17_pro_2021_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLADE_PRO_2017_KEYMAP_SIZE (sizeof(blade_pro_2017_keymap) / sizeof(blade_pro_2017_keymap[0]))

static const razer_key blade_pro_2017_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      2,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             "Key: Media Volume",        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      23,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      24,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLADE_STEALTH_2016_KEYMAP_SIZE (sizeof(blade_stealth_2016_keymap) / sizeof(blade_stealth_2016_keymap[0]))

static const razer_key blade_stealth_2016_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      3,      0,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      15,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_FUNCTION,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      5,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      8,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
};

#define CYNOSA_CHROMA_KEYMAP_SIZE (sizeof(cynosa_chroma_keymap) / sizeof(cynosa_chroma_keymap[0]))

static const razer_key cynosa_chroma_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define CYNOSA_CHROMA_V2_KEYMAP_SIZE (sizeof(cynosa_chroma_v2_keymap) / sizeof(cynosa_chroma_v2_keymap[0]))

static const razer_key cynosa_chroma_v2_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_BACKSPACE,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_MEDIA_VOLUME_UP,     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      17,             KEY_EN_MEDIA_VOLUME_DOWN,   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_ELITE_KEYMAP_SIZE (sizeof(huntsman_elite_keymap) / sizeof(huntsman_elite_keymap[0]))

static const razer_key huntsman_elite_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_V2_ANALOG_KEYMAP_SIZE (sizeof(huntsman_v2_analog_keymap) / sizeof(huntsman_v2_analog_keymap[0]))

static const razer_key huntsman_v2_analog_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_KEYMAP_SIZE (sizeof(huntsman_keymap) / sizeof(huntsman_keymap[0]))

static const razer_key huntsman_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_MINI_KEYMAP_SIZE (sizeof(huntsman_mini_keymap) / sizeof(huntsman_mini_keymap[0]))

static const razer_key huntsman_mini_keymap[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                         Layout Type                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      2,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      3,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_TE_KEYMAP_SIZE (sizeof(huntsman_te_keymap) / sizeof(huntsman_te_keymap[0]))

static const razer_key huntsman_te_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_V2_KEYMAP_SIZE (sizeof(huntsman_v2_keymap) / sizeof(huntsman_v2_keymap[0]))

static const razer_key huntsman_v2_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      13,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      17,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      2,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      17,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      18,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define HUNTSMAN_V2_TKL_KEYMAP_SIZE (sizeof(huntsman_v2_tkl_keymap) / sizeof(huntsman_v2_tkl_keymap[0]))

static const razer_key huntsman_v2_tkl_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      13,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      2,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_ELITE_KEYMAP_SIZE (sizeof(blackwidow_elite_keymap) / sizeof(blackwidow_elite_keymap[0]))

static const razer_key blackwidow_elite_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_CHROMA_V2_KEYMAP_SIZE (sizeof(blackwidow_chroma_v2_keymap) / sizeof(blackwidow_chroma_v2_keymap[0]))

static const razer_key blackwidow_chroma_v2_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              "Key: M1",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              "Key: M2",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              "Key: M3",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              "Key: M4",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              "Key: M5",                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_2019_KEYMAP_SIZE (sizeof(blackwidow_2019_keymap) / sizeof(blackwidow_2019_keymap[0]))

static const razer_key blackwidow_2019_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define BLACKWIDOW_V3_PRO_KEYMAP_SIZE (sizeof(blackwidow_v3_pro_keymap) / sizeof(blackwidow_v3_pro_keymap[0]))

static const razer_key blackwidow_v3_pro_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      2,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE,    RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      0,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      0,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      14,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      15,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      0,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      13,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      14,             "Logo",                     RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      17,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      2,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      3,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      13,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      15,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      17,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      6,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      18,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define ORNATA_CHROMA_KEYMAP_SIZE (sizeof(ornata_chroma_keymap) / sizeof(ornata_chroma_keymap[0]))

static const razer_key ornata_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      3,              KEY_EN_F1,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      4,              KEY_EN_F2,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      5,              KEY_EN_F3,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      6,              KEY_EN_F4,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      7,              KEY_EN_F5,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      8,              KEY_EN_F6,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      9,              KEY_EN_F7,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      10,             KEY_EN_F8,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      11,             KEY_EN_F9,                  RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      12,             KEY_EN_F10,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      13,             KEY_EN_F11,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      14,             KEY_EN_F12,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      2,              KEY_EN_1,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      3,              KEY_EN_2,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      4,              KEY_EN_3,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      5,              KEY_EN_4,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      6,              KEY_EN_5,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      7,              KEY_EN_6,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      8,              KEY_EN_7,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      9,              KEY_EN_8,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      10,             KEY_EN_9,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      11,             KEY_EN_0,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      12,             KEY_EN_MINUS,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      13,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      14,             KEY_EN_EQUALS,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      15,             KEY_EN_INSERT,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      16,             KEY_EN_HOME,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      17,             KEY_EN_PAGE_UP,             RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      1,              KEY_EN_TAB,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      2,              KEY_EN_Q,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      3,              KEY_EN_W,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      4,              KEY_EN_E,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      5,              KEY_EN_R,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      6,              KEY_EN_T,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      7,              KEY_EN_Y,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      8,              KEY_EN_U,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      9,              KEY_EN_I,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      10,             KEY_EN_O,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      11,             KEY_EN_P,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH,     RAZER_LAYOUT_TYPE_ANSI                          },
    {   0,      2,      15,             KEY_EN_DELETE,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      16,             KEY_EN_END,                 RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      18,             KEY_EN_NUMPAD_7,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      19,             KEY_EN_NUMPAD_8,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      20,             KEY_EN_NUMPAD_9,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      2,              KEY_EN_A,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      3,              KEY_EN_S,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      4,              KEY_EN_D,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      5,              KEY_EN_F,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      6,              KEY_EN_G,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      7,              KEY_EN_H,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      8,              KEY_EN_J,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      9,              KEY_EN_K,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      10,             KEY_EN_L,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      11,             KEY_EN_SEMICOLON,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      12,             KEY_EN_QUOTE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      13,             KEY_EN_POUND,               RAZER_LAYOUT_TYPE_ISO                           },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      18,             KEY_EN_NUMPAD_4,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      19,             KEY_EN_NUMPAD_5,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      3,      20,             KEY_EN_NUMPAD_6,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH,      RAZER_LAYOUT_TYPE_ISO | RAZER_LAYOUT_TYPE_JIS   },
    {   0,      4,      3,              KEY_EN_Z,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      4,              KEY_EN_X,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      5,              KEY_EN_C,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      6,              KEY_EN_V,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      7,              KEY_EN_B,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      8,              KEY_EN_N,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      9,              KEY_EN_M,                   RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      10,             KEY_EN_COMMA,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      11,             KEY_EN_PERIOD,              RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      16,             KEY_EN_UP_ARROW,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      18,             KEY_EN_NUMPAD_1,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      19,             KEY_EN_NUMPAD_2,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      20,             KEY_EN_NUMPAD_3,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS,        RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      3,              KEY_EN_LEFT_ALT,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      7,              KEY_EN_SPACE,               RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT,           RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION,      RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      13,             KEY_EN_MENU,                RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL,       RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW,          RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW,         RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      19,             KEY_EN_NUMPAD_0,            RAZER_LAYOUT_TYPE_ALL                           },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD,       RAZER_LAYOUT_TYPE_ALL                           },
};

#define TARTARUS_V2_KEYMAP_SIZE (sizeof(tartarus_v2_keymap) / sizeof(tartarus_v2_keymap[0]))

static const razer_key tartarus_v2_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,              "Key: 01"                           },
    {   0,      0,      1,              "Key: 02"                           },
    {   0,      0,      2,              "Key: 03"                           },
    {   0,      0,      3,              "Key: 04"                           },
    {   0,      0,      4,              "Key: 05"                           },
    {   0,      1,      0,              "Key: 06"                           },
    {   0,      1,      1,              "Key: 07"                           },
    {   0,      1,      2,              "Key: 08"                           },
    {   0,      1,      3,              "Key: 09"                           },
    {   0,      1,      4,              "Key: 10"                           },
    {   0,      2,      0,              "Key: 11"                           },
    {   0,      2,      1,              "Key: 12"                           },
    {   0,      2,      2,              "Key: 13"                           },
    {   0,      2,      3,              "Key: 14"                           },
    {   0,      2,      4,              "Key: 15"                           },
    {   0,      3,      0,              "Key: 16"                           },
    {   0,      3,      1,              "Key: 17"                           },
    {   0,      3,      2,              "Key: 18"                           },
    {   0,      3,      3,              "Key: 19"                           },
    {   0,      3,      4,              "Key: Scroll Wheel"                 },
    {   0,      3,      5,              "Key: 20"                           },
};

#define TARTARUS_PRO_KEYMAP_SIZE (sizeof(tartarus_pro_keymap) / sizeof(tartarus_pro_keymap[0]))

static const razer_key tartarus_pro_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,              "Keypad 01"                         },
    {   0,      0,      1,              "Keypad 02"                         },
    {   0,      0,      2,              "Keypad 03"                         },
    {   0,      0,      3,              "Keypad 04"                         },
    {   0,      0,      4,              "Keypad 05"                         },
    {   0,      1,      0,              "Keypad 06"                         },
    {   0,      1,      1,              "Keypad 07"                         },
    {   0,      1,      2,              "Keypad 08"                         },
    {   0,      1,      3,              "Keypad 09"                         },
    {   0,      1,      4,              "Keypad 10"                         },
    {   0,      2,      0,              "Keypad 11"                         },
    {   0,      2,      1,              "Keypad 12"                         },
    {   0,      2,      2,              "Keypad 13"                         },
    {   0,      2,      3,              "Keypad 14"                         },
    {   0,      2,      4,              "Keypad 15"                         },
    {   0,      3,      0,              "Keypad 16"                         },
    {   0,      3,      1,              "Keypad 17"                         },
    {   0,      3,      2,              "Keypad 18"                         },
    {   0,      3,      3,              "Keypad 19"                         },
    {   0,      3,      4,              "Key Scroll Wheel"                  },
    {   1,      0,      0,              "Keypad 20"                         },
};

/*-------------------------------------------------------------------------*\
|  KEYBOARDS                                                                |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Blackwidow 2019 1532:0241                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_2019_device =
{
    "Razer BlackWidow 2019",
    RAZER_BLACKWIDOW_2019_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_2019_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_2019_keymap,
    BLACKWIDOW_2019_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer BlackWidow Chroma                                      |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_chroma_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_chroma_device =
{
    "Razer BlackWidow Chroma",
    RAZER_BLACKWIDOW_CHROMA_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_chroma_keymap,
    BLACKWIDOW_CHROMA_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow Chroma Overwatch 1532:0211                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_chroma_overwatch_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_chroma_overwatch_device =
{
    "Razer Blackwidow Chroma Overwatch",
    RAZER_BLACKWIDOW_OVERWATCH_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_chroma_overwatch_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 1532:024E                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_v3_device =
{
    "Razer Blackwidow V3",
    RAZER_BLACKWIDOW_V3_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_v3_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer BlackWidow Chroma Tournament Edition                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_chroma_te_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_chroma_te_device =
{
    "Razer BlackWidow Chroma Tournament Edition",
    RAZER_BLACKWIDOW_CHROMA_TE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_chroma_te_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_chroma_te_keymap,
    BLACKWIDOW_CHROMA_TE_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow Elite 1532:0228                             |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_elite_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_elite_device =
{
    "Razer BlackWidow Elite",
    RAZER_BLACKWIDOW_ELITE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_elite_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_elite_keymap,
    BLACKWIDOW_ELITE_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow Chroma V2 1532:0221                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_chroma_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_chroma_v2_device =
{
    "Razer BlackWidow Chroma V2",
    RAZER_BLACKWIDOW_CHROMA_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_chroma_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_chroma_v2_keymap,
    BLACKWIDOW_CHROMA_V2_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 Pro (Wired) 1532:025A                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_pro_wired_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_v3_pro_wired_device =
{
    "Razer BlackWidow V3 Pro (Wired)",
    RAZER_BLACKWIDOW_V3_PRO_WIRED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_v3_pro_wired_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_pro_keymap,
    BLACKWIDOW_V3_PRO_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 Pro (Bluetooth) 1532:025B                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_pro_bluetooth_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_v3_pro_bluetooth_device =
{
    "Razer BlackWidow V3 Pro (Bluetooth)",
    RAZER_BLACKWIDOW_V3_PRO_BLUETOOTH_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_v3_pro_bluetooth_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_pro_keymap,
    BLACKWIDOW_V3_PRO_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 Pro (Wireless) 1532:025C                 |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_pro_wireless_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_v3_pro_wireless_device =
{
    "Razer BlackWidow V3 Pro (Wireless)",
    RAZER_BLACKWIDOW_V3_PRO_WIRELESS_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_v3_pro_wireless_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_pro_keymap,
    BLACKWIDOW_V3_PRO_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 TKL 1532:0A24                            |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 18 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_tkl_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    18
};

static const razer_device blackwidow_v3_tkl_device =
{
    "Razer BlackWidow V3 TKL",
    RAZER_BLACKWIDOW_V3_TKL_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    18,
    {
        &blackwidow_v3_tkl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_tkl_keymap,
    BLACKWIDOW_V3_TKL_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 Mini (Wired) 1532:0258                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       5 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v3_mini_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    5,
    16
};

static const razer_device blackwidow_v3_mini_wired_device =
{
    "Razer BlackWidow V3 Mini (Wired)",
    RAZER_BLACKWIDOW_V3_MINI_WIRED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    5,
    16,
    {
        &blackwidow_v3_mini_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_mini_keymap,
    BLACKWIDOW_V3_MINI_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V3 Mini (Wireless) 1532:0271                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       5 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_device blackwidow_v3_mini_wireless_device =
{
    "Razer BlackWidow V3 Mini (Wireless)",
    RAZER_BLACKWIDOW_V3_MINI_WIRELESS_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    5,
    16,
    {
        &blackwidow_v3_mini_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_v3_mini_keymap,
    BLACKWIDOW_V3_MINI_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow X Chroma 1532:0216                          |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_x_chroma_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_x_chroma_device =
{
    "Razer BlackWidow X Chroma",
    RAZER_BLACKWIDOW_X_CHROMA_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_x_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer BlackWidow X Chroma Tournament Edition 1532:021A       |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_x_chroma_te_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_x_chroma_te_device =
{
    "Razer BlackWidow X Chroma Tournament Edition",
    RAZER_BLACKWIDOW_X_CHROMA_TE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &blackwidow_x_chroma_te_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blackwidow_chroma_te_keymap,
    BLACKWIDOW_CHROMA_TE_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Cynosa Chroma 1532:022A                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone cynosa_chroma_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device cynosa_chroma_device =
{
    "Razer Cynosa Chroma",
    RAZER_CYNOSA_CHROMA_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &cynosa_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    cynosa_chroma_keymap,
    CYNOSA_CHROMA_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Cynosa v2 1532:025E                                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone cynosa_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device cynosa_v2_device =
{
    "Razer Cynosa v2",
    RAZER_CYNOSA_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &cynosa_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    cynosa_chroma_v2_keymap,
    CYNOSA_CHROMA_V2_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Cynosa Lite 1532:023F                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone cynosa_lite_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device cynosa_lite_device =
{
    "Razer Cynosa Lite",
    RAZER_CYNOSA_LITE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    1,
    {
        &cynosa_lite_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Ornata Chroma                                          |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone ornata_chroma_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device ornata_chroma_device =
{
    "Razer Ornata Chroma",
    RAZER_ORNATA_CHROMA_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &ornata_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    ornata_chroma_keymap,
    ORNATA_CHROMA_KEYMAP_SIZE
};


/*-------------------------------------------------------------*\
|  Razer Ornata V3                                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       10 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone ornata_v3_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    10
};

static const razer_device ornata_v3_device =
{
    "Razer Ornata V3",
    RAZER_ORNATA_V3_PID,
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    10,
    {
        &ornata_v3_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};


/*-------------------------------------------------------------*\
|  Razer Ornata V3 X                                            |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone ornata_v3_x_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device ornata_v3_x_device =
{
    "Razer Ornata V3 X",
    RAZER_ORNATA_V3_X_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    1,
    {
        &ornata_v3_x_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};


/*-------------------------------------------------------------*\
|  Razer DeathStalker Chroma                                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       12 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_chroma_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    12
};

static const razer_device deathstalker_chroma_device =
{
    "Razer DeathStalker Chroma",
    RAZER_DEATHSTALKER_CHROMA_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    12,
    {
        &deathstalker_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Huntsman 1532:0227                                     |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device huntsman_device =
{
    "Razer Huntsman",
    RAZER_HUNTSMAN_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &huntsman_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_keymap,
    HUNTSMAN_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman Elite                                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
|                                                               |
| Zone "Underglow"                                              |
|       Matrix                                                  |
|       3 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_elite_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_zone huntsman_elite_underglow_zone =
{
    "Underglow",
    ZONE_TYPE_MATRIX,
    3,
    22
};

static const razer_device huntsman_elite_device =
{
    "Razer Huntsman Elite",
    RAZER_HUNTSMAN_ELITE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    9,
    22,
    {
        &huntsman_elite_keyboard_zone,
        &huntsman_elite_underglow_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_elite_keymap,
    HUNTSMAN_ELITE_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman V2 Analog                                     |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
|                                                               |
| Zone "Underglow"                                              |
|       Matrix                                                  |
|       3 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_v2_analog_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_zone huntsman_v2_analog_underglow_zone =
{
    "Underglow",
    ZONE_TYPE_MATRIX,
    3,
    22
};

static const razer_device huntsman_v2_analog_device =
{
    "Razer Huntsman V2 Analog",
    RAZER_HUNTSMAN_V2_ANALOG_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    9,
    22,
    {
        &huntsman_v2_analog_keyboard_zone,
        &huntsman_v2_analog_underglow_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_v2_analog_keymap,
    HUNTSMAN_V2_ANALOG_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman Mini 1532:0257                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       5 Rows, 15 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_mini_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    5,
    15
};

static const razer_device huntsman_mini_device =
{
    "Razer Huntsman Mini",
    RAZER_HUNTSMAN_MINI_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    5,
    15,
    {
        &huntsman_mini_keyboard_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_mini_keymap,
    HUNTSMAN_MINI_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman TE 1532:0243                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 18 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_te_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    18
};

static const razer_device huntsman_te_device =
{
    "Razer Huntsman Tournament Edition",
    RAZER_HUNTSMAN_TE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    18,
    {
        &huntsman_te_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_te_keymap,
    HUNTSMAN_TE_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman V2 TKL 1532:026B                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 17 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_v2_tkl_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    17
};

static const razer_device huntsman_v2_tkl_device =
{
    "Razer Huntsman V2 TKL",
    RAZER_HUNTSMAN_V2_TKL_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    17,
    {
        &huntsman_v2_tkl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_v2_tkl_keymap,
    HUNTSMAN_V2_TKL_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Huntsman V2 1532:026C                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_v2_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device huntsman_v2_device =
{
    "Razer Huntsman V2",
    RAZER_HUNTSMAN_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    22,
    {
        &huntsman_v2_keyboard_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    huntsman_v2_keymap,
    HUNTSMAN_V2_KEYMAP_SIZE
};

/*-------------------------------------------------------------------------*\
|  LAPTOPS                                                                  |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Blade (2016)                                           |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_2016_device =
{
    "Razer Blade (2016)",
    RAZER_BLADE_2016_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_2016_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade (Late 2016)                                      |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_late_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_late_2016_device =
{
    "Razer Blade (Late 2016)",
    RAZER_BLADE_LATE_2016_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_late_2016_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2018 Advanced)                               |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2018_advanced_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2018_advanced_device =
{
    "Razer Blade 15 (2018 Advanced)",
    RAZER_BLADE_2018_ADVANCED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2018_advanced_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2018 Base)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2018_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2018_base_device =
{
    "Razer Blade 15 (2018 Base)",
    RAZER_BLADE_2018_BASE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2018_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2018 Mercury)                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2018_mercury_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2018_mercury_device =
{
    "Razer Blade 15 (2018 Mercury)",
    RAZER_BLADE_2018_MERCURY_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2018_mercury_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2019 Advanced)                               |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2019_advanced_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2019_advanced_device =
{
    "Razer Blade 15 (2019 Advanced)",
    RAZER_BLADE_2019_ADVANCED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2019_advanced_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2019 Base)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2019_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_15_2019_base_device =
{
    "Razer Blade 15 (2019 Base)",
    RAZER_BLADE_2019_BASE_PID,
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    1,
    {
        &blade_15_2019_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2019 Mercury)                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2019_mercury_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2019_mercury_device =
{
    "Razer Blade 15 (2019 Mercury)",
    RAZER_BLADE_2019_MERCURY_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2019_mercury_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2019 Studio)                                 |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2019_studio_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2019_studio_device =
{
    "Razer Blade 15 (2019 Studio)",
    RAZER_BLADE_2019_STUDIO_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2019_studio_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2020 Advanced)                               |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2020_advanced_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2020_advanced_device =
{
    "Razer Blade 15 (2020 Advanced)",
    RAZER_BLADE_2020_ADVANCED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2020_advanced_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2020 Base)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2020_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device blade_15_2020_base_device =
{
    "Razer Blade 15 (2020 Base)",
    RAZER_BLADE_2020_BASE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    16,
    {
        &blade_15_2020_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade (Late 2020)                                      |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_late_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_late_2020_device =
{
    "Razer Blade (Late 2020)",
    RAZER_BLADE_LATE_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_late_2020_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2021 Advanced)                               |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2021_advanced_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2021_advanced_device =
{
    "Razer Blade 15 (2021 Advanced)",
    RAZER_BLADE_2021_ADVANCED_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2021_advanced_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2021 Base)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2021_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device blade_15_2021_base_device =
{
    "Razer Blade 15 (2021 Base)",
    RAZER_BLADE_2021_BASE_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    16,
    {
        &blade_15_2021_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2021 Base) 1532:027A                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2021_base_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device blade_15_2021_base_v2_device =
{
    "Razer Blade 15 (2021 Base)",
    RAZER_BLADE_2021_BASE_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    16,
    {
        &blade_15_2021_base_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade 14 (2021)                                        |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_14_2021_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_14_2021_device =
{
    "Razer Blade 14 (2021)",
    RAZER_BLADE_14_2021_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_14_2021_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_14_2021_keymap,
    BLADE_14_2021_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blade 14 (2022) 1532:028C                              |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_14_2022_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_14_2022_device =
{
    "Razer Blade 14 (2022)",
    RAZER_BLADE_14_2022_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_14_2022_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_14_2022_keymap,
    BLADE_14_2022_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Book 13 (2020)                                         |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone book_13_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device book_13_2020_device =
{
    "Razer Book 13 (2020)",
    RAZER_BOOK_13_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &book_13_2020_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    book_13_2020_keymap,
    BOOK_13_2020_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro (2016)                                       |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 25 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    25
};

static const razer_device blade_pro_2016_device =
{
    "Razer Blade Pro (2016)",
    RAZER_BLADE_PRO_2016_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    25,
    {
        &blade_pro_2016_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro (2017)                                       |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 25 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_2017_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    25
};

static const razer_device blade_pro_2017_device =
{
    "Razer Blade Pro (2017)",
    RAZER_BLADE_PRO_2017_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    25,
    {
        &blade_pro_2017_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_pro_2017_keymap,
    BLADE_PRO_2017_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro (2017 FullHD)                                |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 25 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_2017_fullhd_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    25
};

static const razer_device blade_pro_2017_fullhd_device =
{
    "Razer Blade Pro (2017 FullHD)",
    RAZER_BLADE_PRO_2017_FULLHD_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    25,
    {
        &blade_pro_2017_fullhd_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro (2019)                                       |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_pro_2019_device =
{
    "Razer Blade Pro (2019)",
    RAZER_BLADE_PRO_2019_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_pro_2019_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro (Late 2019)                                  |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_late_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_pro_late_2019_device =
{
    "Razer Blade Pro (Late 2019)",
    RAZER_BLADE_PRO_LATE_2019_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_pro_late_2019_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro 17 (2020)                                    |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_17_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_pro_17_2020_device =
{
    "Razer Blade Pro 17 (2020)",
    RAZER_BLADE_PRO_17_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_pro_17_2020_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Pro 17 (2021)                                    |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_17_2021_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_pro_17_2021_device =
{
    "Razer Blade Pro 17 (2021)",
    RAZER_BLADE_PRO_17_2021_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_pro_17_2021_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_17_pro_2021_keymap,
    BLADE_17_PRO_2021_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (2016)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_2016_device =
{
    "Razer Blade Stealth (2016)",
    RAZER_BLADE_STEALTH_2016_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_2016_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_stealth_2016_keymap,
    BLADE_STEALTH_2016_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (Late 2016)                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_late_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_late_2016_device =
{
    "Razer Blade Stealth (Late 2016)",
    RAZER_BLADE_STEALTH_LATE_2016_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_late_2016_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (2017)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_2017_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_2017_device =
{
    "Razer Blade Stealth (2017)",
    RAZER_BLADE_STEALTH_2017_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_2017_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (Late 2017)                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_late_2017_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_late_2017_device =
{
    "Razer Blade Stealth (Late 2017)",
    RAZER_BLADE_STEALTH_LATE_2017_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_late_2017_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (2019)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_2019_device =
{
    "Razer Blade Stealth (2019)",
    RAZER_BLADE_STEALTH_2019_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_2019_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (Late 2019)                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_late_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_stealth_late_2019_device =
{
    "Razer Blade Stealth (Late 2019)",
    RAZER_BLADE_STEALTH_LATE_2019_PID,
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    1,
    {
        &blade_stealth_late_2019_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (2020)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_stealth_2020_device =
{
    "Razer Blade Stealth (2020)",
    RAZER_BLADE_STEALTH_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    1,
    {
        &blade_stealth_2020_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Blade Stealth (Late 2020)                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_late_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device blade_stealth_late_2020_device =
{
    "Razer Blade Stealth (Late 2020)",
    RAZER_BLADE_STEALTH_LATE_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    16,
    {
        &blade_stealth_late_2020_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------------------*\
|  MICE                                                                     |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Abyssus Elite DVa Edition 1532:006A                    |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone abyssus_elite_dva_edition_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device abyssus_elite_dva_edition_device =
{
    "Razer Abyssus Elite DVa Edition",
    RAZER_ABYSSUS_ELITE_DVA_EDITION_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &abyssus_elite_dva_edition_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Abyssus Essential 1532:006B                            |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone abyssus_essential_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device abyssus_essential_device =
{
    "Razer Abyssus Essential",
    RAZER_ABYSSUS_ESSENTIAL_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &abyssus_essential_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk 1532:0064                                     |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_device =
{
    "Razer Basilisk",
    RAZER_BASILISK_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &basilisk_logo_zone,
        &basilisk_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk Essential                                     |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_essential_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_essential_device =
{
    "Razer Basilisk Essential",
    RAZER_BASILISK_ESSENTIAL_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &basilisk_essential_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk Ultimate (Wired) 1532:0086                    |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Left LED Strip"                                        |
|       Linear                                                  |
|       8 LED                                                   |
|                                                               |
|  Zone "Right LED Strip"                                       |
|       Linear                                                  |
|       4 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_ultimate_wired_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    4
};

static const razer_zone basilisk_ultimate_wired_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_zone basilisk_ultimate_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_ultimate_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_ultimate_wired_device =
{
    "Razer Basilisk Ultimate",
    RAZER_BASILISK_ULTIMATE_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    14,
    {
        &basilisk_ultimate_wired_scroll_wheel_zone,
        &basilisk_ultimate_wired_logo_zone,
        &basilisk_ultimate_wired_left_zone,
        &basilisk_ultimate_wired_right_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk Ultimate (Wireless) 1532:0088                 |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Left LED Strip"                                        |
|       Linear                                                  |
|       8 LED                                                   |
|                                                               |
|  Zone "Right LED Strip"                                       |
|       Linear                                                  |
|       4 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_ultimate_wireless_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    4
};

static const razer_zone basilisk_ultimate_wireless_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_zone basilisk_ultimate_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_ultimate_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_ultimate_wireless_device =
{
    "Razer Basilisk Ultimate (Wireless)",
    RAZER_BASILISK_ULTIMATE_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    14,
    {
        &basilisk_ultimate_wireless_scroll_wheel_zone,
        &basilisk_ultimate_wireless_logo_zone,
        &basilisk_ultimate_wireless_left_zone,
        &basilisk_ultimate_wireless_right_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V2 1532:0085                                  |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v2_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v2_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_v2_device =
{
    "Razer Basilisk V2",
    RAZER_BASILISK_V2_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &basilisk_v2_scroll_wheel_zone,
        &basilisk_v2_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 1532:0099                                  |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       9 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device basilisk_v3_device =
{
    "Razer Basilisk V3",
    RAZER_BASILISK_V3_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    11,
    {
        &basilisk_v3_logo_zone,
        &basilisk_v3_scroll_wheel_zone,
        &basilisk_v3_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 PRO Wired 1532:00AA                        |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       9 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_scroll_pro_wired_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_wired_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device basilisk_v3_pro_wired_device =
{
    "Razer Basilisk V3 Pro (Wired)",
    RAZER_BASILISK_V3_PRO_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    11,
    {
        &basilisk_v3_logo_zone,
        &basilisk_v3_scroll_wheel_zone,
        &basilisk_v3_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 PRO Wireless 1532:00AB                     |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       9 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_scroll_pro_wireless_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_wireless_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device basilisk_v3_pro_wireless_device =
{
    "Razer Basilisk V3 Pro (Wireless)",
    RAZER_BASILISK_V3_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    11,
    {
        &basilisk_v3_logo_zone,
        &basilisk_v3_scroll_wheel_zone,
        &basilisk_v3_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 PRO Bluetooth 1532:00AC                    |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       9 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_scroll_pro_bluetooth_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_bluetooth_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_bluetooth_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device basilisk_v3_pro_bluetooth_device =
{
    "Razer Basilisk V3 Pro (Bluetooth)",
    RAZER_BASILISK_V3_PRO_BLUETOOTH_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    11,
    {
        &basilisk_v3_logo_zone,
        &basilisk_v3_scroll_wheel_zone,
        &basilisk_v3_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};
/*-------------------------------------------------------------*\
|  Razer DeathAdder Chroma                                      |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_chroma_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_chroma_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_chroma_device =
{
    "Razer DeathAdder Chroma",
    RAZER_DEATHADDER_CHROMA_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_chroma_logo_zone,
        &deathadder_chroma_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Deathadder Elite                                       |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_elite_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_elite_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_elite_device =
{
    "Razer DeathAdder Elite",
    RAZER_DEATHADDER_ELITE_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_elite_logo_zone,
        &deathadder_elite_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Deathadder Essential 1532:006E                         |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_essential_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_essential_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_essential_device =
{
    "Razer DeathAdder Essential",
    RAZER_DEATHADDER_ESSENTIAL_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_essential_logo_zone,
        &deathadder_essential_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Deathadder Essential V2 1532:0098                      |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_essential_v2_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_essential_v2_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_essential_v2_device =
{
    "Razer DeathAdder Essential V2",
    RAZER_DEATHADDER_ESSENTIAL_V2_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_essential_v2_logo_zone,
        &deathadder_essential_v2_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Deathadder Essential White Edition 1532:0071           |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_essential_white_edition_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_essential_white_edition_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_essential_white_edition_device =
{
    "Razer DeathAdder Essential (White Edition)",
    RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_essential_white_edition_logo_zone,
        &deathadder_essential_white_edition_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer DeathAdder V2 1532:0084                                |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_v2_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone deathadder_v2_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_v2_device =
{
    "Razer DeathAdder V2",
    RAZER_DEATHADDER_V2_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_v2_scroll_wheel_zone,
        &deathadder_v2_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer DeathAdder V2 Mini 1532:008C                           |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_v2_mini_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_v2_mini_device =
{
    "Razer DeathAdder V2 Mini",
    RAZER_DEATHADDER_V2_MINI_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &deathadder_v2_mini_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer DeathAdder V2 Pro (Wired) 1532:007C	                |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_v2_pro_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_v2_pro_wired_device =
{
    "Razer DeathAdder V2 (Wired)",
    RAZER_DEATHADDER_V2_PRO_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &deathadder_v2_pro_wired_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer DeathAdder V2 Pro (Wireless) 1532:007D                 |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone deathadder_v2_pro_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device deathadder_v2_pro_wireless_device =
{
    "Razer DeathAdder V2 (Wireless)",
    RAZER_DEATHADDER_V2_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    1,
    {
        &deathadder_v2_pro_wireless_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Diamondback Chroma                                     |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       19 LEDs                                                 |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone diamondback_chroma_led_strip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    19
};

static const razer_zone diamondback_chroma_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone diamondback_chroma_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device diamondback_chroma_device =
{
    "Razer Diamondback Chroma",
    RAZER_DIAMONDBACK_CHROMA_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    21,
    {
        &diamondback_chroma_led_strip_zone,
        &diamondback_chroma_logo_zone,
        &diamondback_chroma_scroll_wheel_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead 2017 (Wired)                                 |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone lancehead_2017_wired_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2017_wired_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2017_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_2017_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_2017_wired_device =
{
    "Razer Lancehead 2017 (Wired)",
    RAZER_LANCEHEAD_2017_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_2017_wired_right_zone,
        &lancehead_2017_wired_left_zone,
        &lancehead_2017_wired_logo_zone,
        &lancehead_2017_wired_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead 2017 (Wireless)                              |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone lancehead_2017_wireless_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2017_wireless_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2017_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_2017_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_2017_wireless_device =
{
    "Razer Lancehead 2017 (Wireless)",
    RAZER_LANCEHEAD_2017_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_2017_wireless_right_zone,
        &lancehead_2017_wireless_left_zone,
        &lancehead_2017_wireless_logo_zone,
        &lancehead_2017_wireless_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead 2019 (Wired)                                 |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone lancehead_2019_wired_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2019_wired_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2019_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_2019_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_2019_wired_device =
{
    "Razer Lancehead 2019 (Wired)",
    RAZER_LANCEHEAD_2019_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_2019_wired_right_zone,
        &lancehead_2019_wired_left_zone,
        &lancehead_2019_wired_logo_zone,
        &lancehead_2019_wired_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead 2019 (Wireless)                              |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone lancehead_2019_wireless_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2019_wireless_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_2019_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_2019_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_2019_wireless_device =
{
    "Razer Lancehead 2019 (Wireless)",
    RAZER_LANCEHEAD_2019_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_2019_wireless_right_zone,
        &lancehead_2019_wireless_left_zone,
        &lancehead_2019_wireless_logo_zone,
        &lancehead_2019_wireless_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead Tournament Edition 1532:0060                 |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone lancehead_te_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_te_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_te_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_te_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_te_device =
{
    "Razer Lancehead Tournament Edition",
    RAZER_LANCEHEAD_TE_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_te_scroll_wheel_zone,
        &lancehead_te_logo_zone,
        &lancehead_te_right_zone,
        &lancehead_te_left_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba 2012 (Wired)                                     |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mamba_2012_wired_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_LINEAR,
    1,
    1
};

static const razer_device mamba_2012_wired_device =
{
    "Razer Mamba 2012 (Wired)",
    RAZER_MAMBA_2012_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_2012_wired_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba 2012 (Wireless)                                  |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mamba_2012_wireless_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_LINEAR,
    1,
    1
};

static const razer_device mamba_2012_wireless_device =
{
    "Razer Mamba 2012 (Wireless)",
    RAZER_MAMBA_2012_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_2012_wireless_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba 2015 (Wired)                                     |
|                                                               |
|  Zone "Chroma Zone"                                           |
|       Single                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone mamba_2015_wired_zone =
{
    "Chroma Zone",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device mamba_2015_wired_device =
{
    "Razer Mamba 2015 (Wired)",
    RAZER_MAMBA_2015_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_2015_wired_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba 2015 (Wireless)                                  |
|                                                               |
|  Zone "Chroma Zone"                                           |
|       Single                                                  |
|       15 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone mamba_2015_wireless_zone =
{
    "Chroma Zone",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device mamba_2015_wireless_device =
{
    "Razer Mamba (Wireless)",
    RAZER_MAMBA_2015_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_2015_wireless_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba 2018 (Wired)                                     |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mamba_2018_wired_logo_zone =
{
    "Logo Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_2018_wired_scroll_wheel_zone =
{
    "Scroll Wheel Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_2018_wired_device =
{
    "Razer Mamba 2018 (Wired)",
    RAZER_MAMBA_2018_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    2,
    {
        &mamba_2018_wired_scroll_wheel_zone,
        &mamba_2018_wired_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba Wireless (2018) Wireless 1532:0072               |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/

static const razer_zone mamba_2018_wireless_logo_zone =
{
    "Logo Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_2018_wireless_scroll_wheel_zone =
{
    "Scroll Wheel Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_2018_wireless_device =
{
    "Razer Mamba 2018 (Wireless)",
    RAZER_MAMBA_2018_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    2,
    {
        &mamba_2018_wireless_scroll_wheel_zone,
        &mamba_2018_wireless_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba Elite                                            |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       9 LEDs                                                  |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       9 LEDs                                                  |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone mamba_elite_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_elite_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_elite_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_zone mamba_elite_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device mamba_elite_device =
{
    "Razer Mamba Elite",
    RAZER_MAMBA_ELITE_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    20,
    {
        &mamba_elite_scroll_wheel_zone,
        &mamba_elite_logo_zone,
        &mamba_elite_left_zone,
        &mamba_elite_right_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mamba Tournament Edition                               |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mamba_te_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone mamba_te_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone mamba_te_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_te_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_te_device =
{
    "Razer Mamba Tournament Edition",
    RAZER_MAMBA_TE_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &mamba_te_left_zone,
        &mamba_te_right_zone,
        &mamba_te_logo_zone,
        &mamba_te_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Chroma                                            |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_chroma_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_chroma_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_chroma_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_chroma_device =
{
    "Razer Naga Chroma",
    RAZER_NAGA_CHROMA_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_chroma_logo_zone,
        &naga_chroma_scroll_wheel_zone,
        &naga_chroma_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Classic 1532:0093                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_classic_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_classic_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_classic_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_classic_device =
{
    "Razer Naga Classic",
    RAZER_NAGA_CLASSIC_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_classic_logo_zone,
        &naga_classic_scroll_wheel_zone,
        &naga_classic_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Hex V2 1532:0050                                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_hex_v2_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_hex_v2_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_hex_v2_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_hex_v2_device =
{
    "Razer Naga Hex V2",
    RAZER_NAGA_HEX_V2_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_hex_v2_logo_zone,
        &naga_hex_v2_scroll_wheel_zone,
        &naga_hex_v2_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Left Handed 1532:008D                             |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_left_handed_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_left_handed_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_left_handed_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_left_handed_device =
{
    "Razer Naga Left Handed",
    RAZER_NAGA_LEFT_HANDED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_left_handed_logo_zone,
        &naga_left_handed_scroll_wheel_zone,
        &naga_left_handed_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Trinity 1532:0067                                  |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_trinity_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_trinity_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_trinity_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_trinity_device =
{
    "Razer Naga Trinity",
    RAZER_NAGA_TRINITY_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_trinity_scroll_wheel_zone,
        &naga_trinity_logo_zone,
        &naga_trinity_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Pro 1532:008F (wired) 1532:0090 (wireless)        |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_pro_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_pro_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_pro_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_pro_wired_device =
{
    "Razer Naga Pro (Wired)",
    RAZER_NAGA_PRO_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_pro_scroll_wheel_zone,
        &naga_pro_logo_zone,
        &naga_pro_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

static const razer_device naga_pro_wireless_device =
{
    "Razer Naga Pro (Wireless)",
    RAZER_NAGA_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    3,
    {
        &naga_pro_scroll_wheel_zone,
        &naga_pro_logo_zone,
        &naga_pro_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Viper 8kHz 1532:0091                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Matrix                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone viper_8khz_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device viper_8khz_device =
{
    "Razer Viper 8kHz",
    RAZER_VIPER_8KHZ_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    1,
    {
        &viper_8khz_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Viper Mini 1532:008A                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Matrix                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone viper_mini_logo_zone =
{
    "Logo",      //Matrix of one as per https://github.com/openrazer/openrazer/blob/master/daemon/openrazer_daemon/hardware/mouse.py#L27
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device viper_mini_device =
{
    "Razer Viper Mini",
    RAZER_VIPER_MINI_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    1,
    {
        &viper_mini_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Viper Ultimate Wired 1532:007A                         |
|                                                               |
|  Zone "Logo"                                                  |
|       Matrix                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone viper_ultimate_wired_logo_zone =
{
    "Logo",      //Matrix of one as per https://github.com/openrazer/openrazer/blob/master/daemon/openrazer_daemon/hardware/mouse.py#L1690
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device viper_ultimate_wired_device =
{
    "Razer Viper Ultimate (Wired)",
    RAZER_VIPER_ULTIMATE_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    1,
    {
        &viper_ultimate_wired_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Viper Ultimate Wireless 1532:007B                      |
|                                                               |
|  Zone "Logo"                                                  |
|       Matrix                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone viper_ultimate_wireless_logo_zone =
{
    "Logo",      //Matrix of one as per https://github.com/openrazer/openrazer/blob/master/daemon/openrazer_daemon/hardware/mouse.py#L1690
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device viper_ultimate_wireless_device =
{
    "Razer Viper Ultimate (Wireless)",
    RAZER_VIPER_ULTIMATE_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    1,
    {
        &viper_ultimate_wireless_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Viper 1532:0078                                        |
|                                                               |
|  Zone "Logo"                                                  |
|       Matrix                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone viper_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device viper_device =
{
    "Razer Viper",
    RAZER_VIPER_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    1,
    {
        &viper_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Naga Epic Chroma                                       |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_epic_chroma_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_epic_chroma_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_epic_chroma_device =
{
    "Razer Naga Epic Chroma",
    RAZER_NAGA_EPIC_CHROMA_PID,
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &naga_epic_chroma_scroll_wheel_zone,
        &naga_epic_chroma_numpad_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------------------*\
|  KEYPADS                                                                  |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Orbweaver Chroma                                       |
|                                                               |
|  Zone "Keypad"                                                |
|       Matrix                                                  |
|       4 Rows, 5 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone orbweaver_chroma_zone =
{
    "Keypad",
    ZONE_TYPE_MATRIX,
    4,
    5
};

static const razer_device orbweaver_chroma_device =
{
    "Razer Orbweaver Chroma",
    RAZER_ORBWEAVER_CHROMA_PID,
    DEVICE_TYPE_KEYPAD,
    true,
    4,
    5,
    {
        &orbweaver_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Tartarus Chroma                                        |
|                                                               |
|  Zone "Keypad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone tartarus_chroma_zone =
{
    "Keypad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device tartarus_chroma_device =
{
    "Razer Tartarus Chroma",
    RAZER_TARTARUS_CHROMA_PID,
    DEVICE_TYPE_KEYPAD,
    true,
    1,
    1,
    {
        &tartarus_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
| Razer Tartarus Pro 1532:0244                                  |
|                                                               |
|  Zone "Keypad"                                                |
|       Matrix                                                  |
|       4 Rows, 5 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone tartarus_pro_zone =
{
    "Keypad",
    ZONE_TYPE_MATRIX,
    4,
    5
};

static const razer_zone tartarus_pro_K20 =
{
    "Keypad LED 20",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device tartarus_pro_device =
{
    "Razer Tartarus Pro",
    RAZER_TARTARUS_PRO_PID,
    DEVICE_TYPE_KEYPAD,
    true,
    1,
    21,
    {
        &tartarus_pro_zone,
        &tartarus_pro_K20,
        NULL,
        NULL,
        NULL,
        NULL
    },
    /*-------------------------------------------------------------*\
    Need to be Implemented to read upper defined keymap(Not working now)
    tartarus_pro_keymap,
    TARTARUS_PRO_KEYMAP_SIZE
    \*-------------------------------------------------------------*/
    NULL,
    0
};

/*-------------------------------------------------------------*\
| Razer Tartarus V2 1532:0208                                   |
|                                                               |
|  Zone "Keypad"                                                |
|       Matrix                                                  |
|       4 Rows, 6 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone tartarus_v2_zone =
{
    "Keypad",
    ZONE_TYPE_MATRIX,
    4,
    6
};

static const razer_device tartarus_v2_device =
{
    "Razer Tartarus V2",
    RAZER_TARTARUS_V2_PID,
    DEVICE_TYPE_KEYPAD,
    true,
    4,
    6,
    {
        &tartarus_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    tartarus_v2_keymap,
    TARTARUS_V2_KEYMAP_SIZE
};

/*-------------------------------------------------------------------------*\
|  MOUSEMATS                                                                |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Firefly                                                |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone firefly_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device firefly_device =
{
    "Razer Firefly",
    RAZER_FIREFLY_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    15,
    {
        &firefly_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Firefly Hyperflux                                      |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       1 LEDs                                                  |
\*-------------------------------------------------------------*/
static const razer_zone firefly_hyperflux_zone =
{
    "LED Strip",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device firefly_hyperflux_device =
{
    "Razer Firefly Hyperflux",
    RAZER_FIREFLY_HYPERFLUX_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    1,
    {
        &firefly_hyperflux_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Firefly V2                                             |
|                                                               |
|  Zone "LED Strip"                                             |
|       Matrix                                                  |
|       19 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone firefly_v2_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    19
};

static const razer_device firefly_v2_device =
{
    "Razer Firefly V2",
    RAZER_FIREFLY_V2_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    19,
    {
        &firefly_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Goliathus                                              |
|                                                               |
|  Zone "LED Strip"                                             |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone goliathus_zone =
{
    "LED Strip",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device goliathus_device =
{
    "Razer Goliathus",
    RAZER_GOLIATHUS_CHROMA_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    1,
    {
        &goliathus_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Goliathus Extended                                     |
|                                                               |
|  Zone "LED Strip"                                             |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone goliathus_extended_zone =
{
    "LED Strip",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device goliathus_extended_device =
{
    "Razer Goliathus Extended",
    RAZER_GOLIATHUS_CHROMA_EXTENDED_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    1,
    {
        &goliathus_extended_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Strider Chroma                                         |
|                                                               |
|  Zone "LED Strip"                                             |
|       Matrix                                                  |
|       19 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone strider_chroma_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    19
};

static const razer_device strider_chroma_device =
{
    "Razer Strider Chroma",
    RAZER_STRIDER_CHROMA_PID,
    DEVICE_TYPE_MOUSEMAT,
    true,
    1,
    19,
    {
        &strider_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------------------*\
|  HEADSETS                                                                 |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Kraken 7.1 Chroma                                      |
|                                                               |
|  Zone "Headset"                                               |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone kraken_chroma_zone =
{
    "Headset",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device kraken_chroma_device =
{
    "Razer Kraken 7.1 Chroma",
    RAZER_KRAKEN_PID,
    DEVICE_TYPE_HEADSET,
    true,
    1,
    1,
    {
        &kraken_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Kraken 7.1 V2                                          |
|                                                               |
|  Zone "Headset"                                               |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone kraken_v2_zone =
{
    "Headset",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device kraken_v2_device =
{
    "Razer Kraken 7.1 V2",
    RAZER_KRAKEN_V2_PID,
    DEVICE_TYPE_HEADSET,
    true,
    1,
    1,
    {
        &kraken_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Kraken Ultimate 1532:0527                              |
|                                                               |
|  Zone "Headset"                                               |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone kraken_ultimate_zone =
{
    "Headset",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device kraken_ultimate_device =
{
    "Razer Kraken Ultimate",
    RAZER_KRAKEN_ULTIMATE_PID,
    DEVICE_TYPE_HEADSET,
    true,
    1,
    1,
    {
        &kraken_ultimate_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Kraken Kitty Edition 1532:0F19                         |
|                                                               |
|  Zone "Headset"                                               |
|       Matrix                                                  |
|       4 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone kraken_kitty_zone =
{
    "Headset",
    ZONE_TYPE_LINEAR,
    1,
    4
};

static const razer_device kraken_kitty_device =
{
    "Razer Kraken Kitty Edition",
    RAZER_KRAKEN_KITTY_EDITION_PID,
    DEVICE_TYPE_HEADSET,
    true,
    1,
    4,
    {
        &kraken_kitty_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Tiamat 7.1 V2                                          |
|                                                               |
|  Zone "Controller"                                            |
|       Linear                                                  |
|       15 LEDs                                                 |
|                                                               |
|  Zone "Headset Left"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Headset Right"                                         |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone tiamat_71_v2_controller_zone =
{
    "Controller",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_zone tiamat_71_v2_headset_left_zone =
{
    "Headset Left",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone tiamat_71_v2_headset_right_zone =
{
    "Headset Right",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device tiamat_71_v2_device =
{
    "Razer Tiamat 7.1 V2",
    RAZER_TIAMAT_71_V2_PID,
    DEVICE_TYPE_HEADSET,
    true,
    1,
    17,
    {
        &tiamat_71_v2_controller_zone,
        &tiamat_71_v2_headset_left_zone,
        &tiamat_71_v2_headset_right_zone,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------------------*\
|  OTHER                                                                    |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Core                                                   |
|                                                               |
|  Zone "Side Window Lights"                                    |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       8 LEDs                                                  |
\*-------------------------------------------------------------*/
static const razer_zone core_side_zone =
{
    "Side Window Lights",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone core_led_strip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_device core_device =
{
    "Razer Core",
    RAZER_CORE_PID,
    DEVICE_TYPE_GPU,
    true,
    1,
    9,
    {
        &core_side_zone,
        &core_led_strip_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Core X                                                 |
|                                                               |
|  Zone "Side Window Lights"                                    |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone core_x_side_zone =
{
    "Side Window Lights",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone core_x_led_strip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device core_x_device =
{
    "Razer Core X",
    RAZER_CORE_X_PID,
    DEVICE_TYPE_GPU,
    true,
    1,
    16,
    {
        &core_x_side_zone,
        &core_x_led_strip_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Chroma Mug Holder                                      |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone mug_holder_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device mug_holder_device =
{
    "Razer Chroma Mug Holder",
    RAZER_CHROMA_MUG_PID,
    DEVICE_TYPE_ACCESSORY,
    true,
    1,
    15,
    {
        &mug_holder_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Chroma Addressable RGB Controller                      |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       80 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone chromaargb_zone_1 =
{
    "Channel 1",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_zone chromaargb_zone_2 =
{
    "Channel 2",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_zone chromaargb_zone_3 =
{
    "Channel 3",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_zone chromaargb_zone_4 =
{
    "Channel 4",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_zone chromaargb_zone_5 =
{
    "Channel 5",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_zone chromaargb_zone_6 =
{
    "Channel 6",
    ZONE_TYPE_LINEAR,
    1,
    80
};

static const razer_device chromaargb_device =
{
    "Razer Chroma Addressable RGB Controller",
    RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    6,
    80,
    {
        &chromaargb_zone_1,
        &chromaargb_zone_2,
        &chromaargb_zone_3,
        &chromaargb_zone_4,
        &chromaargb_zone_5,
        &chromaargb_zone_6
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Chroma HDK                                             |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone chromahdk_zone_1 =
{
    "Channel 1",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_zone chromahdk_zone_2 =
{
    "Channel 2",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_zone chromahdk_zone_3 =
{
    "Channel 3",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_zone chromahdk_zone_4 =
{
    "Channel 4",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device chromahdk_device =
{
    "Razer Chroma HDK",
    RAZER_CHROMA_HDK_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    4,
    16,
    {
        &chromahdk_zone_1,
        &chromahdk_zone_2,
        &chromahdk_zone_3,
        &chromahdk_zone_4,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Chroma PC Case Lighting Kit                            |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone chroma_pc_case_lighting_kit_zone_1 =
{
    "Channel 1",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_zone chroma_pc_case_lighting_kit_zone_2 =
{
    "Channel 2",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device chroma_pc_case_lighting_kit_device =
{
    "Razer Chroma PC Case Lighting Kit",
    RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    2,
    16,
    {
        &chroma_pc_case_lighting_kit_zone_1,
        &chroma_pc_case_lighting_kit_zone_2,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Base Station Chroma                                    |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone base_station_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device base_station_device =
{
    "Razer Base Station Chroma",
    RAZER_BASE_STATION_CHROMA_PID,
    DEVICE_TYPE_HEADSET_STAND,
    true,
    1,
    15,
    {
        &base_station_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};


/*-------------------------------------------------------------*\
|  Razer Mouse Bungee V3 Chroma                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       8 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone mouse_bungee_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_device mouse_bungee_device =
{
    "Razer Mouse Bungee V3 Chroma",
    RAZER_MOUSE_BUNGEE_V3_CHROMA_PID,
    DEVICE_TYPE_MOUSE,
    true,
    1,
    8,
    {
        &mouse_bungee_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Base Station V2 Chroma                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       8 LEDs                                                  |
\*-------------------------------------------------------------*/
static const razer_zone base_station_v2_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_device base_station_v2_device =
{
    "Razer Base Station V2 Chroma",
    RAZER_BASE_STATION_V2_CHROMA_PID,
    DEVICE_TYPE_HEADSET_STAND,
    true,
    1,
    8,
    {
        &base_station_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Laptop Stand Chroma 1532:0F0D                          |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone laptop_stand_chroma_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device laptop_stand_chroma_device =
{
    "Razer Laptop Stand Chroma",
    RAZER_LAPTOP_STAND_CHROMA_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    1,
    15,
    {
        &laptop_stand_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Laptop Stand Chroma V2 1532:0F2B                       |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone laptop_stand_chroma_v2_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device laptop_stand_chroma_v2_device =
{
    "Razer Laptop Stand Chroma V2",
    RAZER_LAPTOP_STAND_CHROMA_V2_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    1,
    15,
    {
        &laptop_stand_chroma_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mouse Dock Chroma 1532:007E                            |
|                                                               |
|  Zone "Base"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mouse_dock_chroma_base_zone =
{
    "Base",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mouse_dock_chroma_device =
{
    "Razer Mouse Dock Chroma",
    RAZER_MOUSE_DOCK_CHROMA_PID,
    DEVICE_TYPE_LIGHT,
    true,
    1,
    1,
    {
        &mouse_dock_chroma_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Mouse Dock Pro 1532:00A4                               |
|                                                               |
|  Zone "Base"                                                  |
|       Linear                                                  |
|       9 LEDs                                                  |
\*-------------------------------------------------------------*/
static const razer_zone mouse_dock_pro_base_zone =
{
    "Base",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device mouse_dock_pro_device =
{
    "Razer Mouse Dock Pro",
    RAZER_MOUSE_DOCK_PRO_PID,
    DEVICE_TYPE_LIGHT,
    true,
    1,
    9,
    {
        &mouse_dock_pro_base_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Nommo Pro                                              |
|                                                               |
|  Zone "Left Speaker"                                          |
|       Linear                                                  |
|       8 LEDs                                                  |
|                                                               |
|  Zone "Right Speaker"                                         |
|       Linear                                                  |
|       8 LEDs                                                  |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone nommo_pro_left_zone =
{
    "Left Speaker",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_zone nommo_pro_right_zone =
{
    "Right Speaker",
    ZONE_TYPE_LINEAR,
    1,
    8
};

static const razer_device nommo_pro_device =
{
    "Razer Nommo Pro",
    RAZER_NOMMO_PRO_PID,
    DEVICE_TYPE_SPEAKER,
    true,
    2,
    8,
    {
        &nommo_pro_left_zone,
        &nommo_pro_right_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Nommo Chroma                                           |
|                                                               |
|  Zone "Right Speaker"                                         |
|       Linear                                                  |
|       8 LEDs                                                  |
|                                                               |
|  Zone "Left Speaker"                                          |
|       Linear                                                  |
|       8 LEDs                                                  |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone nommo_chroma_right_zone =
{
    "Right Speaker",
    ZONE_TYPE_LINEAR,
    1,
    24
};

static const razer_zone nommo_chroma_left_zone =
{
    "Left Speaker",
    ZONE_TYPE_LINEAR,
    1,
    24
};

static const razer_device nommo_chroma_device =
{
    "Razer Nommo Chroma",
    RAZER_NOMMO_CHROMA_PID,
    DEVICE_TYPE_SPEAKER,
    true,
    2,
    24,
    {
        &nommo_chroma_right_zone,
        &nommo_chroma_left_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Charging Pad Chroma                                    |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       10 LEDs                                                 |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone charging_pad_chroma_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    10
};

static const razer_device charging_pad_chroma_device =
{
    "Razer Charging Pad Chroma",
    RAZER_CHARGING_PAD_CHROMA_PID,
    DEVICE_TYPE_ACCESSORY,
    true,
    1,
    10,
    {
        &charging_pad_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  O11 Dynamic - Razer Edition 1532:0F13                        |
|                                                               |
|  Zone "Case LEDs"                                             |
|       Matrix                                                  |
|       64 LEDs                                                 |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone o11_dynamic_case_zone =
{
    "Case LEDs",
    ZONE_TYPE_LINEAR,
    4,
    16
};

static const razer_device o11_dynamic_device =
{
    "Lian Li O11 Dynamic - Razer Edition",
    RAZER_O11_DYNAMIC_PID,
    DEVICE_TYPE_LEDSTRIP,
    true,
    4,
    16,
    {
        &o11_dynamic_case_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Seiren Emote 1532:0F1B                                 |
|                                                               |
|  Zone "8-Bit LED Matrix"                                      |
|       Matrix                                                  |
|       64 LEDs                                                 |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone seiren_emote_zone =
{
    "8-Bit LED Matrix",
    ZONE_TYPE_MATRIX,
    8,
    8
};

static const razer_device seiren_emote_device =
{
    "Razer Seiren Emote",
    RAZER_SEIREN_EMOTE_PID,
    DEVICE_TYPE_MICROPHONE,
    true,
    4,
    16,
    {
        &seiren_emote_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Thunderbolt 4 Dock Chroma                              |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       12 LEDs                                                 |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone thunderbolt_4_dock_chroma_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    12
};

static const razer_device thunderbolt_4_dock_chroma_device =
{
    "Razer Thunderbolt 4 Dock Chroma",
    RAZER_THUNDERBOLT_4_DOCK_CHROMA_PID,
    DEVICE_TYPE_ACCESSORY,
    true,
    1,
    12,
    {
        &thunderbolt_4_dock_chroma_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
const razer_device* razer_device_list[] =
{
/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
    &blackwidow_2019_device,
    &blackwidow_chroma_device,
    &blackwidow_chroma_overwatch_device,
    &blackwidow_chroma_te_device,
    &blackwidow_chroma_v2_device,
    &blackwidow_elite_device,
    &blackwidow_v3_device,
    &blackwidow_v3_pro_wired_device,
    &blackwidow_v3_pro_wireless_device,
    &blackwidow_v3_pro_bluetooth_device,
    &blackwidow_v3_tkl_device,
    &blackwidow_v3_mini_wired_device,
    &blackwidow_v3_mini_wireless_device,
    &blackwidow_x_chroma_device,
    &blackwidow_x_chroma_te_device,
    &cynosa_chroma_device,
    &cynosa_v2_device,
    &cynosa_lite_device,
    &deathstalker_chroma_device,
    &huntsman_device,
    &huntsman_elite_device,
    &huntsman_mini_device,
    &huntsman_te_device,
    &huntsman_v2_device,
    &huntsman_v2_analog_device,
    &huntsman_v2_tkl_device,
    &ornata_chroma_device,
    &ornata_v3_device,
    &ornata_v3_x_device,
/*-----------------------------------------------------------------*\
|  LAPTOPS                                                          |
\*-----------------------------------------------------------------*/
    &blade_2016_device,
    &blade_late_2016_device,
    &blade_15_2018_advanced_device,
    &blade_15_2018_base_device,
    &blade_15_2018_mercury_device,
    &blade_15_2019_advanced_device,
    &blade_15_2019_base_device,
    &blade_15_2019_mercury_device,
    &blade_15_2019_studio_device,
    &blade_15_2020_advanced_device,
    &blade_15_2020_base_device,
    &blade_late_2020_device,
    &blade_15_2021_advanced_device,
    &blade_15_2021_base_device,
    &blade_15_2021_base_v2_device,
    &blade_14_2021_device,
    &blade_14_2022_device,
    &book_13_2020_device,
    &blade_pro_2016_device,
    &blade_pro_2017_device,
    &blade_pro_2017_fullhd_device,
    &blade_pro_2019_device,
    &blade_pro_late_2019_device,
    &blade_pro_17_2020_device,
    &blade_pro_17_2021_device,
    &blade_stealth_2019_device,
    &blade_stealth_2016_device,
    &blade_stealth_late_2016_device,
    &blade_stealth_2017_device,
    &blade_stealth_late_2017_device,
    &blade_stealth_2019_device,
    &blade_stealth_late_2019_device,
    &blade_stealth_2020_device,
    &blade_stealth_late_2020_device,
/*-----------------------------------------------------------------*\
|  MICE                                                             |
\*-----------------------------------------------------------------*/
    &abyssus_elite_dva_edition_device,
    &abyssus_essential_device,
    &basilisk_device,
    &basilisk_essential_device,
    &basilisk_ultimate_wired_device,
    &basilisk_ultimate_wireless_device,
    &basilisk_v2_device,
    &basilisk_v3_device,
    &basilisk_v3_pro_wired_device,
    &basilisk_v3_pro_wireless_device,
    &basilisk_v3_pro_bluetooth_device,
    &deathadder_chroma_device,
    &deathadder_elite_device,
    &deathadder_essential_device,
    &deathadder_essential_v2_device,
    &deathadder_essential_white_edition_device,
    &deathadder_v2_device,
    &deathadder_v2_mini_device,
    &deathadder_v2_pro_wired_device,
    &deathadder_v2_pro_wireless_device,
    &diamondback_chroma_device,
    &lancehead_2017_wired_device,
    &lancehead_2017_wireless_device,
    &lancehead_2019_wired_device,
    &lancehead_2019_wireless_device,
    &lancehead_te_device,
    &mamba_2012_wired_device,
    &mamba_2012_wireless_device,
    &mamba_2015_wired_device,
    &mamba_2015_wireless_device,
    &mamba_2018_wired_device,
    &mamba_2018_wireless_device,
    &mamba_te_device,
    &mamba_elite_device,
    &naga_chroma_device,
    &naga_classic_device,
    &naga_epic_chroma_device,
    &naga_hex_v2_device,
    &naga_left_handed_device,
    &naga_trinity_device,
    &naga_pro_wired_device,
    &naga_pro_wireless_device,
    &viper_8khz_device,
    &viper_mini_device,
    &viper_ultimate_wired_device,
    &viper_ultimate_wireless_device,
    &viper_device,
/*-----------------------------------------------------------------*\
|  KEYPADS                                                          |
\*-----------------------------------------------------------------*/
    &orbweaver_chroma_device,
    &tartarus_chroma_device,
    &tartarus_pro_device,
    &tartarus_v2_device,
/*-----------------------------------------------------------------*\
|  MOUSEMATS                                                        |
\*-----------------------------------------------------------------*/
    &firefly_device,
    &firefly_hyperflux_device,
    &firefly_v2_device,
    &goliathus_device,
    &goliathus_extended_device,
    &strider_chroma_device,
/*-----------------------------------------------------------------*\
|  HEADSETS                                                         |
\*-----------------------------------------------------------------*/
    &kraken_chroma_device,
    &kraken_v2_device,
    &kraken_ultimate_device,
    &kraken_kitty_device,
    &tiamat_71_v2_device,
/*-----------------------------------------------------------------*\
|  OTHER                                                            |
\*-----------------------------------------------------------------*/
    &base_station_device,
    &base_station_v2_device,
    &mouse_bungee_device,
    &charging_pad_chroma_device,
    &chromaargb_device,
    &chromahdk_device,
    &chroma_pc_case_lighting_kit_device,
    &core_device,
    &core_x_device,
    &laptop_stand_chroma_device,
    &laptop_stand_chroma_v2_device,
    &mug_holder_device,
    &mouse_dock_chroma_device,
    &mouse_dock_pro_device,
    &nommo_chroma_device,
    &nommo_pro_device,
    &o11_dynamic_device,
    &seiren_emote_device,
    &thunderbolt_4_dock_chroma_device,
};

const unsigned int RAZER_NUM_DEVICES = (sizeof(razer_device_list) / sizeof(razer_device_list[ 0 ]));
const razer_device** device_list = razer_device_list;
