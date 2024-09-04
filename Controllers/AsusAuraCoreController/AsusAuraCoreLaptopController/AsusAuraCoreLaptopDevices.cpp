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
        {   0,      0,       5,           5,        "Key: Sun",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Sun" key
        {   0,      0,       6,           6,        "Key: ROG",                 KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
        {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
        {   0,      6,      12,         136,        KEY_EN_RIGHT_FUNCTION,      KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Fuction
        {   0,      6,      12,         136,        KEY_EN_RIGHT_WINDOWS,       KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key
        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      1,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      1,      16,          38,        KEY_EN_DELETE,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Delete key
        {   0,      1,      18,          40,        KEY_EN_PRINT_SCREEN,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
        {   0,      1,      19,          41,        KEY_EN_HOME,                KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Home key
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      2,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      3,      15,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      4,      16,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      5,      14,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      6,      16,           0,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
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
        {   0,      0,       0,         126,        "Lightbar L1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         127,        "Lightbar L2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         128,        "Lightbar L3",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
        {   0,      0,       3,         144,        "Lightbar R3",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R3
        {   0,      0,       4,         145,        "Lightbar R2",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       5,         125,        "Lightbar R1",              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R1
    }
};

keyboard_keymap_overlay_values g713rw_test_layout
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
        {   0,      0,       0,           7,        "Test01",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       1,           8,        "Test02",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       2,           9,        "Test03",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       3,          10,        "Test04",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       4,          11,        "Test05",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       5,          12,        "Test06",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       6,          13,        "Test07",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       7,          14,        "Test08",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       8,          15,        "Test09",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       9,          16,        "Test10",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      10,          17,        "Test11",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      11,          18,        "Test12",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      12,          19,        "Test13",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      13,          20,        "Test14",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      14,          22,        "Test15",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      15,          27,        "Test16",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      16,          55,        "Test17",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      17,          57,        "Test18",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,          58,        "Test19",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,          77,        "Test20",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,          78,        "Test21",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,          79,        "Test22",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      22,          96,        "Test23",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      23,          97,        "Test24",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      24,          99,        "Test25",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      25,         100,        "Test26",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      26,         104,        "Test27",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      27,         117,        "Test28",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      28,         118,        "Test29",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      29,         120,        "Test30",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      30,         121,        "Test31",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      31,         130,        "Test32",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      32,         132,        "Test33",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      33,         133,        "Test34",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      34,         134,        "Test35",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      35,         138,        "Test36",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      36,         140,        "Test37",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      37,         141,        "Test38",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      38,         142,        "Test39",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      39,         143,        "Test40",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      40,         146,        "Test41",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      41,         147,        "Test42",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      42,         148,        "Test43",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      43,         149,        "Test44",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      44,         150,        "Test45",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      45,         151,        "Test46",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      46,         152,        "Test47",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      47,         153,        "Test48",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      48,         154,        "Test49",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      49,         155,        "Test50",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      50,         156,        "Test51",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      51,         157,        "Test52",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      52,         158,        "Test53",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      53,         162,        "Test54",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      54,         163,        "Test55",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      55,         164,        "Test56",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      56,         165,        "Test57",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      57,         166,        "Test58",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      58,         167,        "Test59",                   KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values g733zm_layout
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

static const aura_core_laptop_zone g713rw_test_zone =
{
    "TEST",
    &g713rw_test_layout
};

static const aura_core_laptop_device g713rw_device =
{
    "G713RW",
    {
        &g713rw_keyboard_zone,
        &g713rw_lightbar_zone,
        &g713rw_test_zone,
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
};

const unsigned int AURA_CORE_LAPTOP_DEVICE_COUNT = (sizeof(aura_core_laptop_device_list_data) / sizeof(aura_core_laptop_device_list_data[ 0 ]));
const aura_core_laptop_device** aura_core_laptop_device_list = aura_core_laptop_device_list_data;
