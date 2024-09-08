#include "AsusAuraCoreLaptopDevices.h"

/*-------------------------------------------------------------------------*\
|  Aura Core Key Values                                                     |
|    NULL values are keys that are handed to the KLM but are removed as     |
|    a part of customisation. They are included to maintain expected key    |
|    count is aligned to the value count.                                   |
\*-------------------------------------------------------------------------*/

std::vector<unsigned int> aura_core_laptop_15_values =
{
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12         DEL  */
        21,         23,   24,   25,   26,   28,   29,   30,   31,   33,   34,   35,   36,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC        PLAY  */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \         STOP  */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR        PREV  */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,   96,   98,
    /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /   RSFT        ARWU  NEXT  */
       105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  119,
    /* LCTL  LWIN  LALT              SPC               RALT  RFNC  RMNU  RCTL        ARWL  ARWD  ARWR  */
       126,  128,  129,              131,              135,  136,  136,  137,
};

std::vector<unsigned int> aura_core_laptop_17_values =
{
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   NULL  NULL  PAUS                          */
        21,         23,   24,   25,   26,   28,   29,   30,   31,   33,   34,   35,   36,    0,    0,   39,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  NULL  NULL  NULL  NMLK  NMDV  NMTM  NMMI  */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   56,    0,    0,    0,   59,   60,   61,   62,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   NULL  NULL  NULL  NM7   NM8   NM9   NMPL  */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,    0,    0,    0,   80,   81,   82,   83,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                    NM4   NM5   NM6         */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,   96,   98,                    101,  102,  103,
    /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /   RSFT              ARWU        NM1   NM2   NM3   NMER  */
       105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  119,              139,        122,  123,  124,  125,
    /* LCTL  LWIN  LALT              SPC               RALT  RFNC  RMNU  RCTL              ARWL  ARWD  ARWR        NM0   NMPD        */
       126,  128,  129,              131,              135,  136,  136,  137,              159,  160,  161,        144,  145,
};

std::vector<unsigned int> aura_core_laptop_18_values =
{
    /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12   NULL  NULL  PAUS                          */
        21,         23,   24,   25,   26,   28,   29,   30,   31,   33,   34,   35,   36,    0,    0,   39,
    /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  NULL  NULL  NULL  NMLK  NMDV  NMTM  NMMI  */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   56,    0,    0,    0,   59,   60,   61,   62,
    /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     \   NULL  NULL  NULL  NM7   NM8   NM9   NMPL  */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,    0,    0,    0,   80,   81,   82,   83,
    /* CPLK    A     S     D     F     G     H     J     K     L     ;     "     #   ENTR                    NM4   NM5   NM6         */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,   96,   98,                    101,  102,  103,
    /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /   RSFT              ARWU        NM1   NM2   NM3   NMER  */
        105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  119,              121,        122,  123,  124,  125,
    /* LCTL  LWIN  LALT              SPC               RALT  RFNC  RMNU  RCTL              ARWL  ARWD  ARWR        NM0   NMPD        */
        126,  128,  129,              131,              135,  136,  136,  137,              141,  142,  143,        144,  145,
};

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
keyboard_keymap_overlay_values g533zm_layout
{
    KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        aura_core_laptop_15_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,      15,          37,        KEY_EN_DELETE,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Delete
        {   0,      1,      15,          58,        KEY_EN_MEDIA_PLAY_PAUSE,    KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      2,      15,          79,        KEY_EN_MEDIA_STOP,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      3,      15,         100,        KEY_EN_MEDIA_PREVIOUS,      KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      4,      13,         139,        KEY_EN_UP_ARROW,            KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Up Arrow
        {   0,      4,      15,         121,        KEY_EN_MEDIA_NEXT,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      5,      12,         159,        KEY_EN_LEFT_ARROW,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Arrow
        {   0,      5,      13,         160,        KEY_EN_DOWN_ARROW,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Down Arrow
        {   0,      5,      14,         161,        KEY_EN_RIGHT_ARROW,         KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Right Arrow
    }
};

keyboard_keymap_overlay_values g713rw_keyboard_layout
{
    KEYBOARD_SIZE_FULL,
    {
        aura_core_laptop_17_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,   KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row and add Volume Down
        {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,     KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Volume Up
        {   0,      0,       4,           4,        KEY_EN_MEDIA_MUTE,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mute
        {   0,      0,       5,           5,        "Key: Fan",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Fan" key
        {   0,      0,       6,           6,        "Key: ROG",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
        {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,      10,         136,        KEY_EN_RIGHT_FUNCTION,      KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Fuction
        {   0,      6,      10,         136,        KEY_EN_RIGHT_WINDOWS,       KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key

        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      1,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad
        {   0,      1,      15,          38,        KEY_EN_DELETE,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Delete key
        {   0,      1,      17,          40,        KEY_EN_PRINT_SCREEN,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
        {   0,      1,      18,          41,        KEY_EN_HOME,                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Home key
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      13,          76,        KEY_EN_ANSI_BACK_SLASH,     KEYBOARD_OPCODE_SWAP_ONLY,          }, // Force ANSI | even on ISO layouts
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      5,      12,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // gap before arrow up key
        {   0,      5,      13,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad

        {   0,      6,      11,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // 1 empty key before arrow keys
        {   0,      6,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad
        {   0,      6,      16,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // 1 empty key between numpad 0 and .

        // Close numpad gap completely
        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      5,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      6,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },

        // Move Arrow keys
        {   0,      5,      13,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Arrow Up
        {   0,      6,      13,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Arrow Down
        {   0,      6,      12,         139,        KEY_EN_UP_ARROW,            KEYBOARD_OPCODE_SWAP_ONLY,          },  // Shift Arrow Up
        {   0,      7,      11,         159,        KEY_EN_LEFT_ARROW,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      7,      12,         160,        KEY_EN_DOWN_ARROW,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      7,      13,         161,        KEY_EN_RIGHT_ARROW,         KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values g713rw_lightbar_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         174,        "Lightbar L1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         173,        "Lightbar L2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         172,        "Lightbar L3",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
        {   0,      0,       3,         171,        "Lightbar R3",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R3
        {   0,      0,       4,         170,        "Lightbar R2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       5,         169,        "Lightbar R1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R1
    }
};

keyboard_keymap_overlay_values g814jv_keyboard_layout
{
    KEYBOARD_SIZE_FULL,
    {
        aura_core_laptop_18_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
     /*---------------------------------------------------------------------------------------------------------*\
    | Edit Keys                                                                                                 |
    |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
    \*---------------------------------------------------------------------------------------------------------*/
     {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,   KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row and add Volume Down
     {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,     KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Volume Up
     {   0,      0,       4,           4,        KEY_EN_MEDIA_MUTE,          KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mute
     {   0,      0,       5,           5,        "Key: Fan",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Fan" key
     {   0,      0,       6,           6,        "Key: ROG",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
     {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
     {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
     {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
     {   0,      6,      11,         136,        KEY_EN_PRINT_SCREEN,        KEYBOARD_OPCODE_SWAP_ONLY,          },  // Replace Right Fuction with PrtSc
     {   0,      6,      12,         136,        KEY_EN_RIGHT_WINDOWS,       KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key
     {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      1,      14,          38,        KEY_EN_DELETE,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert a Delete key
     {   0,      1,      16,          40,        KEY_EN_PRINT_SCREEN,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
     {   0,      1,      17,          41,        KEY_EN_HOME,                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Home key
     {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      3,      13,          76,        KEY_EN_ANSI_BACK_SLASH,     KEYBOARD_OPCODE_SWAP_ONLY,          }, // Force ANSI | even on ISO layouts
     {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  }, // Numpad gap
     {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      12,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      13,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      6,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     }
};

keyboard_keymap_overlay_values g814jv_lightbar_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*---------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                 |
        |   Zone,   Row,    Column,     Value,      Key,                        OpCode,                             |
        \*---------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         173,        "Lightbar L1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         172,        "Lightbar L2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         170,        "Lightbar R2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       3,         169,        "Lightbar R1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
    }
};


/*-------------------------------------------------------------------------*\
|  AURA CORE LAPTOP DEVICES                                                 |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  ASUS ROG Strix SCAR 15                                       |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       7 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const aura_core_laptop_zone g533zm_zone =
{
    ZONE_EN_KEYBOARD,
    &g533zm_layout
};

static const aura_core_laptop_device g533zm_device =
{
    "G533ZM",
    {
        &g533zm_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    }
};

/*-------------------------------------------------------------*\
|  ASUS ROG Strix SCAR G713RW                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       7 Rows, 20 Columns                                      |
\*-------------------------------------------------------------*/
static const aura_core_laptop_zone g713rw_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    &g713rw_keyboard_layout
};

static const aura_core_laptop_zone g713rw_lightbar_zone =
{
    "Lightbar",
    &g713rw_lightbar_layout
};

static const aura_core_laptop_device g713rw_device =
{
    "G713RW",
    {
        &g713rw_keyboard_zone,
        &g713rw_lightbar_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    }
};
/*-------------------------------------------------------------*\
|  ASUS ROG Strix G18 (G814JV)                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       7 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/

static const aura_core_laptop_zone g814rw_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    &g814jv_keyboard_layout
};

static const aura_core_laptop_zone g814jv_lightbar_zone =
{
    "Lightbar",
    &g814jv_lightbar_layout
};

static const aura_core_laptop_device g814jv_device =
{
    "G814JV",
    {
        &g814rw_keyboard_zone,
        &g814jv_lightbar_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    }
};

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
const aura_core_laptop_device* aura_core_laptop_device_list_data[] =
{
/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
    &g533zm_device,
    &g713rw_device,
    &g814jv_device,
};

const unsigned int AURA_CORE_LAPTOP_DEVICE_COUNT = (sizeof(aura_core_laptop_device_list_data) / sizeof(aura_core_laptop_device_list_data[ 0 ]));
const aura_core_laptop_device** aura_core_laptop_device_list = aura_core_laptop_device_list_data;
