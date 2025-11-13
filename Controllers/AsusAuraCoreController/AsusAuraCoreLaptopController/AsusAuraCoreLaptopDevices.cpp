#include "AsusAuraCoreLaptopDevices.h"

/*-------------------------------------------------------------------------*\
|  Aura Core Key Values                                                     |
|    NULL values are keys that are handed to the KLM but are removed as     |
|    a part of customisation. They are included to maintain expected key    |
|    count is aligned to the value count.                                   |
\*-------------------------------------------------------------------------*/

std::vector<unsigned int> aura_core_laptop_15_16_values =
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

std::vector<unsigned int> aura_core_laptop_17_g733qr_values =
{
        /*            volD  volU  Mute   Fan   ROG                                                                                             */
        2,     3,    4,    5,    6,
        /* ESC          F1    F2    F3    F4    F5    F6    F7    F8    F9   F10   F11   F12    DEL  NUM7  NUM8  NUM9                          */
        21,         23,   24,   25,   26,   28,   29,   30,   31,   33,   34,   35,   36,    38,   39,   40,   41,
        /* BKTK    1     2     3     4     5     6     7     8     9     0     -     =   BSPC  /NUM  NUM4  NUM5  NUM6                           */
        42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   56,    59,   60,   61,   62,
        /* TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]     #   *NUM  NUM1  NUM2  NUM3                           */
        63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   78,    80,   81,   82,   83,
        /* CPLK    A     S     D     F     G     H     J     K     L     ;     '      ENTR     -NUM  +NUM  NUM0   DOT                           */
        84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,       98,     101,  102,  103,  104,
        /* LSFT  ISO\    Z     X     C     V     B     N     M     ,     .     /      RSFT              ARWU                                    */
        105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,      118,              123,
        /* LCTL LFNC LWIN  LALT                 SPC                   RALT  PRSC  RFNC   RCTL     ARWL  ARWD  ARWR      KeyStone(only Red)      */
        126, 127, 128,  129,                 132,                   135,  137,  139,   141,     143,  144,  145,         175,
};
/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
keyboard_keymap_overlay_values g533zm_layout
{
    KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        aura_core_laptop_15_16_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      15,          37,        KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Delete
        {   0,      1,      15,          58,        KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      2,      15,          79,        KEY_EN_MEDIA_STOP,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Stop
        {   0,      3,      15,         100,        KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Previous
        {   0,      4,      13,         139,        KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Up Arrow
        {   0,      4,      15,         121,        KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Next
        {   0,      5,      12,         159,        KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Arrow
        {   0,      5,      13,         160,        KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Down Arrow
        {   0,      5,      14,         161,        KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Right Arrow
    }
};

keyboard_keymap_overlay_values g533zw_layout
{
    KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        aura_core_laptop_15_16_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      15,          37,        KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Delete
        {   0,      1,      15,          58,        KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      2,      15,          79,        KEY_EN_MEDIA_STOP,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Stop
        {   0,      3,      15,         100,        KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Previous
        {   0,      4,      15,         121,        KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Media Next
        {   0,      5,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Function key
        {   0,      5,       4,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Unused key
        {   0,      5,      11,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Func key
        {   0,      5,      11,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Context key
        {   0,      5,      13,         139,        KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Up Arrow
        {   0,      5,      15,         142,        KEY_EN_PRINT_SCREEN,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Print Screen
        {   0,      5,      16,         175,        "Keystone",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Keystone (Red only)
        {   0,      6,      12,         159,        KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Arrow on new row
        {   0,      6,      13,         160,        KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Down Arrow
        {   0,      6,      14,         161,        KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Right Arrow
        {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row before function keys
        {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Vol Up
        {   0,      0,       4,           4,        "Mic On / Off",             KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mic On / Off
        {   0,      0,       5,           5,        "Hyperfan",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Hyperfan
        {   0,      0,       6,           6,        "Armoury Crate",            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Armoury Crate
    }
};

keyboard_keymap_overlay_values g533zw_lid_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         167,        "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         176,        "Vertical Cut Left",        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         177,        "Vertical Cut Right",       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
    }
};

keyboard_keymap_overlay_values g533zw_lightbar_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         174,        "Lightbar L1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         173,        "Lightbar L2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         172,        "Lightbar L3",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
        {   0,      0,       3,         171,        "Lightbar R3",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R3
        {   0,      0,       4,         170,        "Lightbar R2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       5,         169,        "Lightbar R1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R1
    }
};

keyboard_keymap_overlay_values g614jz_keyboard_layout
{
    KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        aura_core_laptop_15_16_values,
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row and add Volume Down
        {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Volume Up
        {   0,      0,       4,           4,        KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mute
        {   0,      0,       5,           5,        "Key: Fan",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Fan" key
        {   0,      0,       6,           6,        "Key: ROG",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
        {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,      10,         136,        KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Fuction
        {   0,      6,      10,         136,        KEY_EN_RIGHT_WINDOWS,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key

        {   0,      1,      15,          37,        KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Delete
        {   0,      2,      15,          58,        KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Play / Pause
        {   0,      3,      15,          79,        KEY_EN_MEDIA_STOP,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Stop
        {   0,      3,      13,          76,        KEY_EN_ANSI_BACK_SLASH,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          }, // Force ANSI | even on ISO layouts
        {   0,      4,      15,         100,        KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Previous
        {   0,      5,      15,         121,        KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Next
        {   0,      5,      12,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // gap before arrow up key

        {   0,      6,      10,         136,        KEY_EN_PRINT_SCREEN,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
        {   0,      6,      15,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad

        // Close numpad gap completely
        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },

        // Move Arrow keys
        {   0,      6,      13,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Arrow Down
        {   0,      5,      13,         120,        KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Shift Arrow Ups4
        {   0,      6,      12,         140,        KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      13,         141,        KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      6,      14,         142,        KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values g614jz_lightbar_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,        0,        173,        "Lightbar L1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      1,        0,        172,        "Lightbar L2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
        {   0,      0,       14,        169,        "Lightbar R1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R1
        {   0,      1,       14,        170,        "Lightbar R2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
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
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row and add Volume Down
        {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Volume Up
        {   0,      0,       4,           4,        KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mute
        {   0,      0,       5,           5,        "Key: Fan",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Fan" key
        {   0,      0,       6,           6,        "Key: ROG",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
        {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,       8,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
        {   0,      6,      10,         136,        KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Fuction
        {   0,      6,      10,         136,        KEY_EN_RIGHT_WINDOWS,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key

        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      1,      15,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad
        {   0,      1,      15,          38,        KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Delete key
        {   0,      1,      17,          40,        KEY_EN_PRINT_SCREEN,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
        {   0,      1,      18,          41,        KEY_EN_HOME,                KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Home key
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      13,          76,        KEY_EN_ANSI_BACK_SLASH,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          }, // Force ANSI | even on ISO layouts
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      5,      12,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // gap before arrow up key
        {   0,      5,      13,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad

        {   0,      6,      11,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // 1 empty key before arrow keys
        {   0,      6,      15,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // trim extra column before numpad
        {   0,      6,      16,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // 1 empty key between numpad 0 and .

        // Close numpad gap completely
        {   0,      1,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      2,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      3,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      4,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      5,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
        {   0,      6,      14,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },

        // Move Arrow keys
        {   0,      5,      13,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Arrow Up
        {   0,      6,      13,           0,        KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Arrow Down
        {   0,      6,      12,         139,        KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Shift Arrow Up
        {   0,      7,      11,         159,        KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      7,      12,         160,        KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      7,      13,         161,        KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
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
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         174,        "Lightbar L1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         173,        "Lightbar L2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         172,        "Lightbar L3",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
        {   0,      0,       3,         171,        "Lightbar R3",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R3
        {   0,      0,       4,         170,        "Lightbar R2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       5,         169,        "Lightbar R1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R1
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
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,           7,        "Test01",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       1,           8,        "Test02",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       2,           9,        "Test03",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       3,          10,        "Test04",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       4,          11,        "Test05",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       5,          12,        "Test06",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       6,          13,        "Test07",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       7,          14,        "Test08",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       8,          15,        "Test09",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       9,          16,        "Test10",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      10,          17,        "Test11",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      11,          18,        "Test12",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      12,          19,        "Test13",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      13,          20,        "Test14",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      14,          22,        "Test15",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      15,          27,        "Test16",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      16,          55,        "Test17",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      17,          57,        "Test18",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,          58,        "Test19",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,          77,        "Test20",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,          78,        "Test21",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,          79,        "Test22",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      22,          96,        "Test23",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      23,          97,        "Test24",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      24,          99,        "Test25",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      25,         100,        "Test26",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      26,         104,        "Test27",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      27,         117,        "Test28",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      28,         118,        "Test29",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      29,         120,        "Test30",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      30,         121,        "Test31",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      31,         130,        "Test32",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      32,         132,        "Test33",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      33,         133,        "Test34",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      34,         134,        "Test35",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      35,         138,        "Test36",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      36,         140,        "Test37",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      37,         141,        "Test38",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      38,         142,        "Test39",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      39,         143,        "Test40",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      40,         146,        "Test41",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      41,         147,        "Test42",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      42,         148,        "Test43",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      43,         149,        "Test44",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      44,         150,        "Test45",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      45,         151,        "Test46",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      46,         152,        "Test47",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      47,         153,        "Test48",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      48,         154,        "Test49",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      49,         155,        "Test50",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      50,         156,        "Test51",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      51,         157,        "Test52",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      52,         158,        "Test53",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      53,         162,        "Test54",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      54,         163,        "Test55",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      55,         164,        "Test56",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      56,         165,        "Test57",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      57,         166,        "Test58",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      58,         167,        "Test59",                   KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
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
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values g733qr_keyboard_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* No base values, we build everything via edit keys */ },
            {
            /* Regional layout fixes (none yet) */
            }
        },
        {
        /*---------------------------------------------------------------------*\
        |  Row 0 - Top macro row (left to right)                               |
        \*---------------------------------------------------------------------*/
        //{ 0, 0, 0,   0,   KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT }, // GAP
        //{ 0, 0, 1,   0,   KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT }, // GAP
        { 0, 0, 2,   2,   KEY_EN_MEDIA_VOLUME_DOWN, KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 0, 3,   3,   KEY_EN_MEDIA_VOLUME_UP,   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 0, 4,   4,   KEY_EN_MEDIA_MUTE,        KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 0, 5,   5,   "Key: Fan",               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 0, 6,   6,   "Key: ROG",               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 1 - Esc, F-row, Delete, Num 7-9                                 |
        \*---------------------------------------------------------------------*/
        { 0, 1, 0,  21,  KEY_EN_ESCAPE,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 1, 1,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },  // GAP

        { 0, 1, 2,  23,  KEY_EN_F1,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1, 3,  24,  KEY_EN_F2,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1, 4,  25,  KEY_EN_F3,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1, 5,  26,  KEY_EN_F4,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 1, 6,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 1, 7,  28,  KEY_EN_F5,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1, 8,  29,  KEY_EN_F6,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1, 9,  30,  KEY_EN_F7,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,10,  31,  KEY_EN_F8,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 1,11,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 1,11,  33,  KEY_EN_F9,                KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,12,  34,  KEY_EN_F10,               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,13,  35,  KEY_EN_F11,               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,14,  36,  KEY_EN_F12,               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 1,16,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 1,15,  38,  KEY_EN_DELETE,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,16,  39,  KEY_EN_NUMPAD_7,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,17,  40,  KEY_EN_NUMPAD_8,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 1,18,  41,  KEY_EN_NUMPAD_9,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 2 - `123...BSPC, Num / 4 5 6                                     |
        \*---------------------------------------------------------------------*/
        { 0, 2, 0,  42,  KEY_EN_BACK_TICK,         KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 1,  43,  KEY_EN_1,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 2,  44,  KEY_EN_2,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 3,  45,  KEY_EN_3,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 4,  46,  KEY_EN_4,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 5,  47,  KEY_EN_5,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 6,  48,  KEY_EN_6,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 7,  49,  KEY_EN_7,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 8,  50,  KEY_EN_8,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2, 9,  51,  KEY_EN_9,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,10,  52,  KEY_EN_0,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,11,  53,  KEY_EN_MINUS,             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,12,  54,  KEY_EN_EQUALS,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,13,  56,  KEY_EN_BACKSPACE,         KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 2,15,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 2,15,  59,  KEY_EN_NUMPAD_DIVIDE,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,16,  60,  KEY_EN_NUMPAD_4,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,17,  61,  KEY_EN_NUMPAD_5,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 2,18,  62,  KEY_EN_NUMPAD_6,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 3 - Tab row + Num * 1 2 3                                       |
        \*---------------------------------------------------------------------*/
        { 0, 3, 0,  63,  KEY_EN_TAB,               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 1,  64,  KEY_EN_Q,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 2,  65,  KEY_EN_W,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 3,  66,  KEY_EN_E,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 4,  67,  KEY_EN_R,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 5,  68,  KEY_EN_T,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 6,  69,  KEY_EN_Y,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 7,  70,  KEY_EN_U,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 8,  71,  KEY_EN_I,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3, 9,  72,  KEY_EN_O,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,10,  73,  KEY_EN_P,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,11,  74,  KEY_EN_LEFT_BRACKET,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,12,  75,  KEY_EN_RIGHT_BRACKET,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,13,  78,  KEY_EN_POUND,             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 3,14,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 3,15,  80,  KEY_EN_NUMPAD_TIMES,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,16,  81,  KEY_EN_NUMPAD_1,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,17,  82,  KEY_EN_NUMPAD_2,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 3,18,  83,  KEY_EN_NUMPAD_3,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 4 - Caps row + Num - + 0 .                                      |
        \*---------------------------------------------------------------------*/
        { 0, 4, 0,  84,  KEY_EN_CAPS_LOCK,         KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 1,  85,  KEY_EN_A,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 2,  86,  KEY_EN_S,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 3,  87,  KEY_EN_D,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 4,  88,  KEY_EN_F,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 5,  89,  KEY_EN_G,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 6,  90,  KEY_EN_H,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 7,  91,  KEY_EN_J,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 8,  92,  KEY_EN_K,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4, 9,  93,  KEY_EN_L,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,10,  94,  KEY_EN_SEMICOLON,         KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,11,  95,  KEY_EN_QUOTE,             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,12,  98,  KEY_EN_ANSI_ENTER,        KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT},

        //{ 0, 4,14,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 4,15, 101,  KEY_EN_NUMPAD_MINUS,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,16, 102,  KEY_EN_NUMPAD_PLUS,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,17, 103,  KEY_EN_NUMPAD_0,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 4,18, 104,  KEY_EN_NUMPAD_PERIOD,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 5 - Shift row + Up Arrow                                        |
        \*---------------------------------------------------------------------*/
        { 0, 5, 0, 105,  KEY_EN_LEFT_SHIFT,        KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 1, 106,  KEY_EN_ANSI_BACK_SLASH,   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 2, 107,  KEY_EN_Z,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 3, 108,  KEY_EN_X,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 4, 109,  KEY_EN_C,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 5, 110,  KEY_EN_V,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 6, 111,  KEY_EN_B,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 7, 112,  KEY_EN_N,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 8, 113,  KEY_EN_M,                 KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5, 9, 114,  KEY_EN_COMMA,             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5,10, 115,  KEY_EN_PERIOD,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5,11, 116,  KEY_EN_FORWARD_SLASH,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 5,12, 118,  KEY_EN_RIGHT_SHIFT,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 5,14,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP
        //{ 0, 5,15,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP
        //{ 0, 5,16,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 5,16, 123,  KEY_EN_UP_ARROW,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        /*---------------------------------------------------------------------*\
        |  Row 6 - Ctrl/Win/Alt/Space/Fn/Arrows/Keystone                       |
        \*---------------------------------------------------------------------*/
        { 0, 6, 0, 126,  KEY_EN_LEFT_CONTROL,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6, 1, 127,  KEY_EN_LEFT_FUNCTION,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6, 2, 128,  KEY_EN_LEFT_WINDOWS,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6, 3, 129,  KEY_EN_LEFT_ALT,          KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6, 4, 132,  KEY_EN_SPACE,             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        { 0, 6, 9, 135,  KEY_EN_RIGHT_ALT,         KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6,10, 137,  KEY_EN_PRINT_SCREEN,      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6,11, 139,  KEY_EN_RIGHT_FUNCTION,    KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6,12, 141,  KEY_EN_RIGHT_CONTROL,     KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },

        //{ 0, 6,13,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP
        //{ 0, 6,14,   0,  KEY_EN_UNUSED,            KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   // GAP

        { 0, 6,15, 143,  KEY_EN_LEFT_ARROW,        KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6,16, 144,  KEY_EN_DOWN_ARROW,        KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        { 0, 6,17, 145,  KEY_EN_RIGHT_ARROW,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    }
};

keyboard_keymap_overlay_values g733qr_lid_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Regional layout fixes (none) */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone, Row, Column, Value, Key,      Alternate Name,     OpCode                                                                      |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         167,                 "ROG Logo",        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },  // Single lid/logo zone
        {   0,      0,       1,         176,        "Vertical Cut Left",        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         177,        "Vertical Cut Right",       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L3
    }
};

keyboard_keymap_overlay_values g733qr_lightbar_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Regional layout fixes (none) */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone, Row, Column, Value, Key,          Alternate Name,        OpCode                                                               |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,   0,     0,   174,   "Lightbar L1",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        {   0,   0,     1,   173,   "Lightbar L2",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        {   0,   0,     2,   172,   "Lightbar L3",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        {   0,   0,     3,   171,   "Lightbar R3",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        {   0,   0,     4,   170,   "Lightbar R2",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
        {   0,   0,     5,   169,   "Lightbar R1",   KEY_EN_UNUSED,        KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    }

};

keyboard_keymap_overlay_values g733qr_keystone_layout
{
    KEYBOARD_SIZE_EMPTY,
    {
        {   /* Values not set in empty keyboard */   },
        {
            /* Regional layout fixes (none) */
        }
    },
    {
        {   0,   0,     0, 175,  "Keystone",               KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
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
     /*------------------------------------------------------------------------------------------------------------------------------------*\
    | Edit Keys                                                                                                                             |
    |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
    \*-------------------------------------------------------------------------------------------------------------------------------------*/
     {   0,      0,       2,           2,        KEY_EN_MEDIA_VOLUME_DOWN,  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_ROW,         },  // Insert Row and add Volume Down
     {   0,      0,       3,           3,        KEY_EN_MEDIA_VOLUME_UP,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Volume Up
     {   0,      0,       4,           4,        KEY_EN_MEDIA_MUTE,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Mute
     {   0,      0,       5,           5,        "Key: Fan",                KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "Fan" key
     {   0,      0,       6,           6,        "Key: ROG",                KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert "ROG" key
     {   0,      6,       8,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
     {   0,      6,       8,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove part of Spacebar
     {   0,      6,       1,         127,        KEY_EN_LEFT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Left Fuction
     {   0,      6,      10,         136,        KEY_EN_PRINT_SCREEN,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Replace Right Fuction with PrtSc
     {   0,      6,      11,           0,        KEY_EN_RIGHT_WINDOWS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Right Win Key
     {   0,      1,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      1,      14,          38,        KEY_EN_DELETE,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert a Delete key
     {   0,      1,      16,          40,        KEY_EN_PRINT_SCREEN,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Print Screen key
     {   0,      1,      17,          41,        KEY_EN_HOME,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert a Home key
     {   0,      2,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      2,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      2,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      3,      13,          76,        KEY_EN_ANSI_BACK_SLASH,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          }, // Force ANSI | even on ISO layouts
     {   0,      3,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      3,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  }, // Numpad gap
     {   0,      3,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      4,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      12,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      13,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      5,      14,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
     {   0,      6,      15,           0,        KEY_EN_UNUSED,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
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
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         173,        "Lightbar L1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L1
        {   0,      0,       1,         172,        "Lightbar L2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar L2
        {   0,      0,       2,         170,        "Lightbar R2",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
        {   0,      0,       3,         169,        "Lightbar R1",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Lightbar R2
    }
};


/*-------------------------------------------------------------------------*\
|  AURA CORE LAPTOP DEVICES                                                 |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  ASUS ROG Strix SCAR 15 G533ZM                                |
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
|  ASUS ROG Strix SCAR 15 G533ZW                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       7 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const aura_core_laptop_zone g533zw_lid_zone =
{
    "Lid",
    &g533zw_lid_layout
};

static const aura_core_laptop_zone g533zw_kb_zone =
{
    ZONE_EN_KEYBOARD,
    &g533zw_layout
};

static const aura_core_laptop_zone g533zw_lightbar_zone =
{
    "Lightbar",
    &g533zw_lightbar_layout
};

static const aura_core_laptop_device g533zw_device =
{
    "G533ZW",
    {
        &g533zw_kb_zone,
        &g533zw_lid_zone,
        &g533zw_lightbar_zone,
        nullptr,
        nullptr,
        nullptr
    }
};

/*-------------------------------------------------------------*\
|  ASUS ROG Strix SCAR 16 G614JZ                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 19 Columns                                      |
\*-------------------------------------------------------------*/
static const aura_core_laptop_zone g614jz_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    &g614jz_keyboard_layout
};
static const aura_core_laptop_zone g614jz_lightbar_zone =
{
    "Lightbar",
    &g614jz_lightbar_layout
};


static const aura_core_laptop_device g614jz_device =
{
    "G614JZ",
    {
        &g614jz_keyboard_zone,
        &g614jz_lightbar_zone,
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
|  ASUS ROG Strix G17 G733QR                                   |
|                                                               |
|  Zones:                                                       |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       7 Rows, 19 Columns                                      |
\*-------------------------------------------------------------*/
static const aura_core_laptop_zone g733qr_keyboard_zone =
    {
        ZONE_EN_KEYBOARD,
        &g733qr_keyboard_layout
};

static const aura_core_laptop_zone g733qr_lid_zone =
    {
        "Lid",
        &g733qr_lid_layout
};

static const aura_core_laptop_zone g733qr_lightbar_zone =
    {
        "Lightbar",
        &g733qr_lightbar_layout
};

static const aura_core_laptop_zone g733qr_keystone_zone =
    {
        "Keystone",
        &g733qr_keystone_layout
};

static const aura_core_laptop_device g733qr_device =
    {
        "G733QR",
        {
            &g733qr_keyboard_zone,
            &g733qr_lid_zone,
            &g733qr_lightbar_zone,
            &g733qr_keystone_zone,
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
    &g533zw_device,
    &g614jz_device,
    &g713rw_device,
    &g733qr_device,
    &g814jv_device,
};

const unsigned int AURA_CORE_LAPTOP_DEVICE_COUNT = (sizeof(aura_core_laptop_device_list_data) / sizeof(aura_core_laptop_device_list_data[ 0 ]));
const aura_core_laptop_device** aura_core_laptop_device_list = aura_core_laptop_device_list_data;
