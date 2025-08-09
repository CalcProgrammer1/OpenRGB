/*---------------------------------------------------------*\
| RazerDevices.cpp                                          |
|                                                           |
|   Device list for Razer devices                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Sep 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RazerDevices.h"

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
keyboard_keymap_overlay_values razer_blackwidow_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ANSI_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      4,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Move 'Z' 1 right (Account for ISO key)
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right
        {   0,      0,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'F1' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      14,         0,          KEY_EN_EQUALS,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts most of row)
        {   0,      3,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Enter 1 right
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts most of row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right
        {   0,      5,      10,         0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert (Another) RGT_ALT (Shifts remainder of row)
        {   0,      5,      11,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_ALT for 'Logo'
    }
};

keyboard_keymap_overlay_values razer_blackwidow_2019_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Move Space 1 left (Shifts row)
        {   0,      5,      11,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blackwidow_chroma_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          "Key: M1",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 1 (Shifts row)
        {   0,      2,      0,          0,          "Key: M2",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 2 (Shifts row)
        {   0,      3,      0,          0,          "Key: M3",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 3 (Shifts row)
        {   0,      4,      0,          0,          "Key: M4",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 4 (Shifts row)
        {   0,      5,      0,          0,          "Key: M5",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 5 (Shifts row)
        {   0,      0,      20,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blackwidow_chroma_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          "Key: M1",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 1 (Shifts row)
        {   0,      2,      0,          0,          "Key: M2",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 2 (Shifts row)
        {   0,      3,      0,          0,          "Key: M3",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 3 (Shifts row)
        {   0,      4,      0,          0,          "Key: M4",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 4 (Shifts row)
        {   0,      5,      0,          0,          "Key: M5",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Inset Macro key 5 (Shifts row)
        {   0,      0,      20,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blackwidow_chroma_te_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      0,      20,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blackwidow_elite_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Move Space 1 left (Shifts row)
        {   0,      5,      11,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v3_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      5,      6,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Spacebar @ 5,6
        {   0,      5,      7,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Spacebar @ 5,7
        {   0,      5,     10,          0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert (another) Right Alt
        {   0,      5,     11,          0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap 'Logo' instead of Right ALt
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v3_mini_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_ESCAPE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap Escape in for Backtick
        {   0,      0,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backspace 1 right (Shifts row)
        {   0,      0,      15,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Delete Key
        {   0,      1,      15,         0,          KEY_EN_PAGE_UP,             KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Page Up Key
        {   0,      2,      15,         0,          KEY_EN_PAGE_DOWN,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Page Down Key
        {   0,      3,      14,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Insert Key
        {   0,      3,      15,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Up Arrow Key
        {   0,      4,      7,          0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
        {   0,      4,      12,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap Right Control in for Right Menu
        {   0,      4,      13,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Arrow
        {   0,      4,      14,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Down Arrow
        {   0,      4,      15,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Right Arrow
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v3_pro_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      14,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo'
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v3_tkl_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v4_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Esc key 1 right (Shifts row)
        {   0,      0,      1,          0,          "Key: M6",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M6 macro key (Shifts row)
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      22,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      1,          0,          "Key: M5",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M5 macro key (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      2,      1,          0,          "Key: M4",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M4 macro key (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      3,      1,          0,          "Key: M3",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M3 macro key (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      4,      1,          0,          "Key: M2",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M2 macro key (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      5,      1,          0,          "Key: M1",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M1 macro key (Shifts row)
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v4_pro_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      0,      1,          0,          "Key: Dial",                KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Command Dial (Shifts row)
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      22,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      1,          0,          "Key: M5",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M5 macro key (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      2,      1,          0,          "Key: M4",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M4 macro key (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      3,      1,          0,          "Key: M3",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M3 macro key (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      4,      1,          0,          "Key: M2",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M2 macro key (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      5,      1,          0,          "Key: M1",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M1 macro key (Shifts row)
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v4_pro_75_wired_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          "Left Underglow 0",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      2,          0,          KEY_EN_UNUSED,          KEY_EN_UNUSED,                  KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Delete gap between ESC and F1
        {   0,      0,     17,          0,          "Right Underglow 0",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          "Left Underglow 1",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,     14,          0,          KEY_EN_UNUSED,          KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Add gap between = and Backspace
        {   0,      1,     16,          0,          KEY_EN_DELETE,          KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,     17,          0,          "Right Underglow 1",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          "Left Underglow 2",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,     16,          0,          KEY_EN_PAGE_UP,         KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,     17,          0,          "Right Underglow 2",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          "Left Underglow 3",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,     16,          0,          KEY_EN_PAGE_DOWN,       KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,     17,          0,          "Right Underglow 3",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          "Left Underglow 4",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,     15,          0,          KEY_EN_UP_ARROW,        KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,     16,          0,          KEY_EN_INSERT,          KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,     17,          0,          "Right Underglow 4",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          "Left Underglow 5",     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,     13,          0,          KEY_EN_UNUSED,          KEY_EN_UNUSED,                  KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },  // Remove Menu key between Fn and RCtrl
        {   0,      5,     14,          0,          KEY_EN_LEFT_ARROW,      KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,     15,          0,          KEY_EN_DOWN_ARROW,      KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,     16,          0,          KEY_EN_RIGHT_ARROW,     KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,     17,          0,          "Right Underglow 5",    KEY_EN_UNUSED,                  KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values razer_blackwidow_v4_x_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          "Key: M6",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M6 macro key (Shifts row)
        {   0,      1,      0,          0,          "Key: M5",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M5 macro key (Shifts row)
        {   0,      2,      0,          0,          "Key: M4",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M4 macro key (Shifts row)
        {   0,      3,      0,          0,          "Key: M3",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M3 macro key (Shifts row)
        {   0,      4,      0,          0,          "Key: M2",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M2 macro key (Shifts row)
        {   0,      5,      0,          0,          "Key: M1",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert M1 macro key (Shifts row)
    }
};

keyboard_keymap_overlay_values razer_blackwidow_x_chroma_te_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
        {   0,      0,      20,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_blade_pro_2017_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Escape 1 right (Shifts row)
        {   0,      0,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Escape @ 0,1
        {   0,      0,      2,          0,          KEY_EN_ESCAPE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Escape @ 0,2
        {   0,      0,      15,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Insert' key
        {   0,      0,      17,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Delete key
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Previous track key
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Next track key
        {   0,      0,      21,         0,          "Key: Media Volume",        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Volume key
        {   0,      0,      23,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Play / Pause key
        {   0,      0,      24,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Volume Mute key
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      19,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      1,      20,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      1,      21,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      1,      22,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      1,      23,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      1,      24,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      2,      3,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Q' 1 right (Shifts row)
        {   0,      2,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Back slash 1 right -> 2,17
        {   0,      2,      19,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      2,      24,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      3,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'A' 1 right (Shifts row)
        {   0,      3,      3,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'A' 1 right (Shifts row)
        {   0,      3,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move ANSI Enter 1 right -> 3,18
        {   0,      3,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move ANSI Enter 1 right -> 3,18
        {   0,      3,      19,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      3,      24,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      4,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Z' 1 right (Shifts row)
        {   0,      4,      3,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Z' 1 right (Shifts row)
        {   0,      4,      14,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Arrow Up and shift row 1 right
        {   0,      4,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Right Shift 1 right -> 4,17
        {   0,      4,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Right Shift 1 right -> 4,18
        {   0,      4,      19,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      4,      24,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Trackpad
        {   0,      5,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Left Windows @ 5,1
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Function @ 5,2
        {   0,      5,      3,          0,          KEY_EN_LEFT_WINDOWS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Windows @ 5,3
        {   0,      5,      5,          0,          KEY_EN_LEFT_ALT,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Alt @ 5,5
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Spacebar @ 5,6
        {   0,      5,      7,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Spacebar @ 5,7
        {   0,      5,      11,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Right Function @ 5,11
        {   0,      5,      12,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap Right Control for Right Menu @ 5,12
        {   0,      5,      13,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap Arrow Left for Right Control @ 5,13
        {   0,      5,      14,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Arrow Down @ 5,14
        {   0,      5,      15,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Arrow Right @ 5,15
        {   0,      5,      16,         0,          KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Insert Right Function @ 5,16
        {   0,      5,      19,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
        {   0,      5,      20,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
        {   0,      5,      21,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
        {   0,      5,      22,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
        {   0,      5,      23,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
        {   0,      5,      24,         0,          "Trackpad",                 KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Trackpad
    }
};

keyboard_keymap_overlay_values razer_blade_15_2021_advanced_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ISO_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      2,      13,         0,          KEY_EN_ISO_ENTER,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_BACKSLASH
                    {   0,      3,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_ENTER
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      1,          0,          KEY_EN_ESCAPE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backspace 1 right
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      5,          0,          KEY_EN_LEFT_ALT,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      9,          0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ESC
        {   0,      0,      14,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap PRTSCN for INS
        {   0,      0,      15,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap SCRLCK for DEL
        {   0,      0,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PSE_BRK
        {   0,      1,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove INSERT
        {   0,      1,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove HOME
        {   0,      1,      18,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PGUP
        {   0,      2,      15,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove DEL
        {   0,      2,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove END
        {   0,      2,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PGDN
        {   0,      4,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove RGT_SHFT
        {   0,      4,      15,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWUP for RGT_SHFT
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap LFT_WIN for LFT_FNC
        {   0,      5,      3,          0,          KEY_EN_LEFT_WINDOWS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap LFT_ALT for LFT_WIN
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove SPACE
        {   0,      5,      10,         0,          KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_ALT for RGT_FNC
        {   0,      5,      11,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_FNC for RGT_CTL
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWLFT for RGT_MNU
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWUP for RGT_CTL
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWRGT for ARWLFT
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWDWN for ARWDWN
        {   0,      5,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ARWRGT
    }
};

keyboard_keymap_overlay_values razer_blade_15_2022_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ANSI_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      4,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Move 'Z' 1 right (Account for ISO key)
                }
            },
            {
                KEYBOARD_LAYOUT_ISO_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      2,      13,         0,          KEY_EN_ISO_ENTER,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_BACKSLASH
                    {   0,      3,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_ENTER
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Escape 1 right
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backspace 1 right
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Back slash 1 right
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move ANSI Enter 1 right
        {   0,      3,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move ANSI Enter 1 right
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      5,          0,          KEY_EN_LEFT_ALT,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Space 1 right
        {   0,      5,      9,          0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      14,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap PRTSCN for DEL
        {   0,      0,      15,         0,          KEY_EN_POWER,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap SCRLCK for POWER
        {   0,      0,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PSE_BRK
        {   0,      1,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove INSERT
        {   0,      1,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove HOME
        {   0,      1,      18,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PGUP
        {   0,      2,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove DEL
        {   0,      2,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove END
        {   0,      2,      18,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PGDN
        {   0,      4,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove RGT_SHFT
        {   0,      4,      15,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWUP for RGT_SHFT
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap LFT_WIN for LFT_FNC
        {   0,      5,      3,          0,          KEY_EN_LEFT_WINDOWS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap LFT_ALT for LFT_WIN
        {   0,      5,      10,         0,          KEY_EN_RIGHT_FUNCTION,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_ALT for RGT_FNC
        {   0,      5,      11,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_FNC for RGT_CTL
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWLFT for RGT_MNU
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWUP for RGT_CTL
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWRGT for ARWLFT
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWDWN for ARWDWN
        {   0,      5,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ARWRGT
    }
};

keyboard_keymap_overlay_values razer_blade_17_pro_2021_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Shift all rows right by one          */
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Rows 1-4 have an empty spot in       */
        {   0,      2,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* 14th column                          */
        {   0,      3,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      4,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      0,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove empty spot between Esc and F1 */
        {   0,      0,      14,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Swap in Delete after F12             */
        {   0,      0,      15,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Swap in Power after Delete           */
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Insert left Fn between Ctrl and Win  */
        {   0,      5,      4,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add empty spot between Win and Alt   */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove Space                         */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      11,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove Menu                          */
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Left Arrow at the end of row 5   */
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Up Arrow at the end of row 5     */
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Right Arrow at the end of row 5  */
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Down Arrow at the end of row 5   */
    }
};

keyboard_keymap_overlay_values razer_blade_stealth_2016_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Shift rows right by one              */
        {   0,      0,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove empty between Esc and F1      */
        {   0,      0,      14,         0,          KEY_EN_INSERT,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Insert at end of row             */
        {   0,      0,      15,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Delete at end of row             */
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Shift rows right by one              */
        {   0,      1,      15,         0,          KEY_EN_BACKSPACE,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add second Backspace at end of row   */
        {   0,      2,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add empty between tab and Q          */
        {   0,      2,      15,         0,          KEY_EN_ANSI_BACK_SLASH,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add second Backslash at end of row   */
        {   0,      3,      1,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add empty between caps and A         */
        {   0,      3,      15,         0,          KEY_EN_ANSI_ENTER,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add second Enter at end of row       */
        {   0,      4,      12,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Insert 1st Right Shift               */
        {   0,      4,      13,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Insert 2nd Right Shift               */
        {   0,      4,      14,         0,          KEY_EN_RIGHT_SHIFT,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Insert 3rd Right Shift               */
        {   0,      5,      1,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Insert left Fn between Ctrl and Win  */
        {   0,      5,      5,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Insert 1st Space                     */
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove empty between Space 1 and 2   */
        {   0,      5,      8,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove empty between Space 2 and 3   */
        {   0,      5,      8,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Insert 3rd Space                     */
        {   0,      5,      11,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove Menu                          */
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Left Arrow at the end of row 5   */
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Up Arrow at the end of row 5     */
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Right Arrow at the end of row 5  */
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Down Arrow at the end of row 5   */
    }
};

keyboard_keymap_overlay_values razer_cynosa_chroma_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 1
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 2
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 3
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 4
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 5
        {   0,      0,      20,         0,          "Logo",                     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert 'Logo' key
    }
};

keyboard_keymap_overlay_values razer_cynosa_chroma_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 1
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 2
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 3
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 4
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 5
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Previous track key
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Play / Pause key
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Next track key
        {   0,      0,      21,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Volume Mute key
        {   0,      4,      15,         0,          KEY_EN_MEDIA_VOLUME_UP,     KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Volume Up
        {   0,      4,      17,         0,          KEY_EN_MEDIA_VOLUME_DOWN,   KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Volume Down
    }
};

keyboard_keymap_overlay_values razer_deathstalker_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ANSI_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      4,       1,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Move 'Z' 1 right (Account for ISO key)
                }
            },
            {
                KEYBOARD_LAYOUT_ISO_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_BACKSLASH
                    {   0,      4,       1,         0,          KEY_EN_ISO_BACK_SLASH,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Add ISO_BACK_SLASH
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      1,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backspace 1 right
    }
};

keyboard_keymap_overlay_values razer_deathstalker_v2_pro_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ANSI_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      4,       1,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Move 'Z' 1 right (Account for ISO key)
                }
            },
            {
                KEYBOARD_LAYOUT_ISO_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_BACKSLASH
                    {   0,      4,       1,         0,          KEY_EN_ISO_BACK_SLASH,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Add ISO_BACK_SLASH
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values razer_deathstalker_v2_pro_tkl_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            {
                KEYBOARD_LAYOUT_ANSI_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode,                             |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      4,       1,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Move 'Z' 1 right (Account for ISO key)
                }
            },
            {
                KEYBOARD_LAYOUT_ISO_QWERTY,
                {
                    /*-------------------------------------------------------------------------------------------------------------------------------------*\
                    | Edit Keys                                                                                                                             |
                    |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name              OpCode,                             |
                    \*-------------------------------------------------------------------------------------------------------------------------------------*/
                    {   0,      2,      13,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove ANSI_BACKSLASH
                    {   0,      4,       1,         0,          KEY_EN_ISO_BACK_SLASH,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Add ISO_BACK_SLASH
                }
            },
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PRINT_SCREEN
        {   0,      0,      15,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove SCROLL_LOCK
        {   0,      0,      16,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove PAUSE_BREAK
    }
};

keyboard_keymap_overlay_values razer_full_size_shifted_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 1
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 2
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 3
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 4
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shifts row 5
    }
};

keyboard_keymap_overlay_values razer_huntsman_common_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values razer_huntsman_mini_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shift row 0
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shift row 1
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shift row 2
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shift row 3
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Shift row 4
    }
};

keyboard_keymap_overlay_values razer_huntsman_te_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move 'Esc' 1 right (Shifts row)
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_CTRL 1 right (Shifts row)
    }
};

keyboard_keymap_overlay_values razer_huntsman_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      17,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values razer_huntsman_v2_tkl_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
    }
};

keyboard_keymap_overlay_values razer_huntsman_v3_pro_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         0,          "Media group",              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          "Media: Volume Dial",       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT,  },
    }
};

keyboard_keymap_overlay_values razer_huntsman_v3_pro_tkl_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_TKL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Key,                        Alternate Name,             OpCode,                             |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      15,         0,          "Xbox Game Bar",            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      16,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      0,      17,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         0,          "Media: Volume Dial",       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values razer_laptop_common_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Shift all rows right by one          */
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Rows 1-4 have an empty spot in       */
        {   0,      2,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* 14th column                          */
        {   0,      3,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      4,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /*                                      */
        {   0,      0,      2,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove empty spot between Esc and F1 */
        {   0,      0,      14,         0,          KEY_EN_DELETE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Swap in Delete after F12             */
        {   0,      0,      15,         0,          KEY_EN_POWER,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Swap in Power after Delete           */
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Insert left Fn between Ctrl and Win  */
        {   0,      5,      4,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add empty spot between Win and Alt   */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove Space                         */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      9,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove unused                        */
        {   0,      5,      10,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_REMOVE_SHIFT_LEFT   },  /* Remove Right Fn                      */
        {   0,      5,      10,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY           },  /* Remove Menu                          */
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Left Arrow at the end of row 5   */
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Up Arrow at the end of row 5     */
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Right Arrow at the end of row 5  */
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT  },  /* Add Down Arrow at the end of row 5   */
    }
};

keyboard_keymap_overlay_values razer_laptop_with_spacebar_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_SEVENTY_FIVE,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Escape @ 0,0
        {   0,      0,      1,          0,          KEY_EN_ESCAPE,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Escape @ 0,1
        {   0,      0,      15,         0,          KEY_EN_POWER,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Power key
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backtick 1 right (Shifts row)
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Backspace 1 right
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Tab 1 right (Shifts row)
        {   0,      2,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Back slash 1 right
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Caps 1 right (Shifts row)
        {   0,      3,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move ANSI Enter 1 right
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move LFT_SHFT 1 right (Shifts row)
        {   0,      4,      14,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },  // Move Right Shift 1 right
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Left Control @ 5,0
        {   0,      5,      1,          0,          KEY_EN_LEFT_CONTROL,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Control @ 5,1
        {   0,      5,      2,          0,          KEY_EN_LEFT_FUNCTION,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Function @ 5,2
        {   0,      5,      3,          0,          KEY_EN_LEFT_WINDOWS,        KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Windows @ 5,3
        {   0,      5,      5,          0,          KEY_EN_LEFT_ALT,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Left Alt @ 5,5
        {   0,      5,      6,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Spacebar @ 5,6
        {   0,      5,      7,          0,          KEY_EN_SPACE,               KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Spacebar @ 5,7
        {   0,      5,      9,          0,          KEY_EN_RIGHT_ALT,           KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert Right Alt @ 5,9
        {   0,      5,      10,         0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Remove Right Alt @ 5,10
        {   0,      5,      11,         0,          KEY_EN_RIGHT_CONTROL,       KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap RGT_FNC for RGT_CTL
        {   0,      5,      12,         0,          KEY_EN_LEFT_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWLFT for RGT_MNU
        {   0,      5,      13,         0,          KEY_EN_UP_ARROW,            KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Swap ARWUP for RGT_CTL
        {   0,      5,      14,         0,          KEY_EN_RIGHT_ARROW,         KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert ARWRGT
        {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_SWAP_ONLY,          },  // Insert ARWDWN
    }
};

keyboard_keymap_overlay_values razer_ornata_chroma_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_FULL,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      18,         0,          KEY_EN_MEDIA_PREVIOUS,      KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      19,         0,          KEY_EN_MEDIA_PLAY_PAUSE,    KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      20,         0,          KEY_EN_MEDIA_NEXT,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,      21,         0,          KEY_EN_MEDIA_MUTE,          KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      4,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      5,      0,          0,          KEY_EN_UNUSED,              KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
};

keyboard_keymap_overlay_values razer_tartarus_v2_layout
{
    KEYBOARD_SIZE::KEYBOARD_SIZE_EMPTY,
    {
        {   /* ANSI Value set not used */   },
        {
            /* Add more regional layout fixes here */
        }
    },
    {
        /*-------------------------------------------------------------------------------------------------------------------------------------*\
        | Edit Keys                                                                                                                             |
        |   Zone,   Row,    Column,     Value,      Name,                       Alternate Name,             OpCode                              |
        \*-------------------------------------------------------------------------------------------------------------------------------------*/
        {   0,      0,       0,         0,          "Key: 01",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       1,         0,          "Key: 02",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       2,         0,          "Key: 03",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       3,         0,          "Key: 04",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      0,       4,         0,          "Key: 05",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,       0,         0,          "Key: 06",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,       1,         0,          "Key: 07",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,       2,         0,          "Key: 08",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,       3,         0,          "Key: 09",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      1,       4,         0,          "Key: 10",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       0,         0,          "Key: 11",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       1,         0,          "Key: 12",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       2,         0,          "Key: 13",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       3,         0,          "Key: 14",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      2,       4,         0,          "Key: 15",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       0,         0,          "Key: 16",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       1,         0,          "Key: 17",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       2,         0,          "Key: 18",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       3,         0,          "Key: 19",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       4,         0,          "Key: Scroll Wheel",        KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
        {   0,      3,       5,         0,          "Key: 20",                  KEY_EN_UNUSED,              KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT, },
    }
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    &razer_blackwidow_2019_layout
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blackwidow_chroma_layout
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blackwidow_chroma_te_layout
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blackwidow_chroma_v2_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    &razer_blackwidow_elite_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_blackwidow_v3_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_blackwidow_v3_pro_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_blackwidow_v3_pro_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_blackwidow_v3_pro_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_blackwidow_v3_tkl_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    &razer_blackwidow_v3_mini_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x9F,
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
    &razer_blackwidow_v3_mini_layout
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V4 1532:0287                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 23 Columns                                      |
|                                                               |
|  Zone "Underglow Left"                                        |
|       Linear                                                  |
|       1 Row, 9 Columns                                        |
|                                                               |
|  Zone "Underglow right"                                       |
|       Linear                                                  |
|       1 Row, 9 Columns                                        |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v4_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    23
};

static const razer_zone blackwidow_v4_lbl_zone =
{
    "Underglow Left",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_zone blackwidow_v4_lbr_zone =
{
    "Underglow Right",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device blackwidow_v4_device =
{
    "Razer Blackwidow V4",
    RAZER_BLACKWIDOW_V4_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    8,
    23,
    {
        &blackwidow_v4_zone,
        &blackwidow_v4_lbl_zone,
        &blackwidow_v4_lbr_zone,
        NULL,
        NULL,
        NULL
    },
    &razer_blackwidow_v4_layout
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V4 Pro 1532:028D                            |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 23 Columns                                      |
|                                                               |
|  Zone "Underglow Left"                                        |
|       Linear                                                  |
|       1 Row, 9 Columns                                        |
|                                                               |
|  Zone "Underglow right"                                       |
|       Linear                                                  |
|       1 Row, 9 Columns                                        |
|                                                               |
|  Zone "Void" - In testing these LEDs were not connected       |
|       Linear                                                  |
|       1 Row, 5 Columns                                        |
|                                                               |
|  Zone "Underglow Wrist Rest"                                  |
|       Linear                                                  |
|       1 Rows, 20 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v4_pro_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    23
};

static const razer_zone blackwidow_v4_pro_lbl_zone =
{
    "Underglow Left",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_zone blackwidow_v4_pro_lbr_zone =
{
    "Underglow Right",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_zone blackwidow_v4_pro_void_zone =
{
    "Void",
    ZONE_TYPE_LINEAR,
    1,
    5
};

static const razer_zone blackwidow_v4_pro_lbwr_zone =
{
    "Underglow Wrist Rest",
    ZONE_TYPE_LINEAR,
    1,
    20
};

static const razer_device blackwidow_v4_pro_device =
{
    "Razer Blackwidow V4 Pro",
    RAZER_BLACKWIDOW_V4_PRO_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    8,
    23,
    {
        &blackwidow_v4_pro_zone,
        &blackwidow_v4_pro_lbl_zone,
        &blackwidow_v4_pro_lbr_zone,
        &blackwidow_v4_pro_void_zone,
        &blackwidow_v4_pro_lbwr_zone,
        NULL
    },
    &razer_blackwidow_v4_pro_layout
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V4 Pro 75% 1532:02B3                        |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 18 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v4_pro_75_wired_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    18
};

static const razer_device blackwidow_v4_pro_75_wired_device =
{
    "Razer Blackwidow V4 Pro 75% (Wired)",
    RAZER_BLACKWIDOW_V4_PRO_75_WIRED_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    6,
    18,
    {
        &blackwidow_v4_pro_75_wired_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_blackwidow_v4_pro_75_wired_layout
};

/*-------------------------------------------------------------*\
|  Razer Blackwidow V4 X 1532:0293                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_v4_x_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_v4_x_device =
{
    "Razer Blackwidow V4 X",
    RAZER_BLACKWIDOW_V4_X_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    6,
    22,
    {
        &blackwidow_v4_x_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_blackwidow_v4_x_layout
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blackwidow_x_chroma_te_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_cynosa_chroma_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    &razer_cynosa_chroma_v2_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_full_size_shifted_layout
};

/*-------------------------------------------------------------*\
|  Razer Ornata Chroma V2 1532:025D                             |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone ornata_chroma_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device ornata_chroma_v2_device =
{
    "Razer Ornata Chroma V2",
    RAZER_ORNATA_CHROMA_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    6,
    22,
    {
        &ornata_chroma_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_ornata_chroma_v2_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Ornata V3 Rev2 1532:02A1                               |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       10 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone ornata_v3_rev2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    10
};

static const razer_device ornata_v3_rev2_device =
{
    "Razer Ornata V3 rev2",
    RAZER_ORNATA_V3_REV2_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    10,
    {
        &ornata_v3_rev2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Ornata V3 TKL   1532:02A3                                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone ornata_v3_tkl_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    8
};

static const razer_device ornata_v3_tkl_device =
{
    "Razer Ornata V3 TKL",
    RAZER_ORNATA_V3_TKL_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    8,
    {
        &ornata_v3_tkl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Ornata V3 X Rev2   1532:02A2                           |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone ornata_v3_x_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device ornata_v3_x_v2_device =
{
    "Razer Ornata V3 X Rev2",
    RAZER_ORNATA_V3_X_REV2_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    1,
    {
        &ornata_v3_x_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Deathstalker V2 1532:0295                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_v2_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device deathstalker_v2_device =
{
    "Razer Deathstalker V2",
    RAZER_DEATHSTALKER_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    6,
    22,
    {
        &deathstalker_v2_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_deathstalker_v2_layout
};

/*-------------------------------------------------------------*\
|  Razer DeathStalker V2 Pro TKL (Wired)     1532:0298          |
|                                (Wireless)  1532:0296          |
|                                (Bluetooth) 1532:0297          |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       84 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_v2_pro_tkl_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    17
};

static const razer_device deathstalker_v2_pro_tkl_wired_device =
{
    "Razer DeathStalker V2 Pro TKL (Wired)",
    RAZER_DEATHSTALKER_V2_PRO_TKL_WIRED_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    6,
    17,
    {
        &deathstalker_v2_pro_tkl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_deathstalker_v2_pro_tkl_layout
};

static const razer_device deathstalker_v2_pro_tkl_wireless_device =
{
    "Razer DeathStalker V2 Pro TKL (Wireless)",
    RAZER_DEATHSTALKER_V2_PRO_TKL_WIRELESS_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x9F,
    6,
    17,
    {
        &deathstalker_v2_pro_tkl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_deathstalker_v2_pro_tkl_layout
};

/*-------------------------------------------------------------*\
|  Razer DeathStalker V2 Pro (Wireless) 1532:0290               |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       104 LEDs                                                |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_v2_pro_wireless_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device deathstalker_v2_pro_wireless_device =
{
    "Razer DeathStalker V2 Pro (Wireless)",
    RAZER_DEATHSTALKER_V2_PRO_WIRELESS_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x9F,
    6,
    22,
    {
        &deathstalker_v2_pro_wireless_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_deathstalker_v2_pro_layout
};

/*-------------------------------------------------------------*\
|  Razer DeathStalker V2 Pro (Wired) 1532:0292                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       104 LEDs                                                |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_v2_pro_wired_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device deathstalker_v2_pro_wired_device =
{
    "Razer DeathStalker V2 Pro (Wired)",
    RAZER_DEATHSTALKER_V2_PRO_WIRED_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x9F,
    6,
    22,
    {
        &deathstalker_v2_pro_wired_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_deathstalker_v2_pro_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_full_size_shifted_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_common_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_common_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_mini_layout
};

/*-------------------------------------------------------------*\
|  Razer Huntsman Mini Analog 1532:0282                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       5 Rows, 15 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_mini_analog_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    5,
    15
};

static const razer_device huntsman_mini_analog_device =
{
    "Razer Huntsman Mini Analog",
    RAZER_HUNTSMAN_MINI_ANALOG_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    5,
    15,
    {
        &huntsman_mini_analog_keyboard_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_huntsman_mini_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_te_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_v2_tkl_layout
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    &razer_huntsman_v2_layout
};

/*-------------------------------------------------------------*\
|  Razer Huntsman V3 Pro 1532:02A6                              |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_v3_pro_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device huntsman_v3_pro_device =
{
    "Razer Huntsman V3 Pro",
    RAZER_HUNTSMAN_V3_PRO_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    6,
    22,
    {
        &huntsman_v3_pro_keyboard_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_huntsman_v3_pro_layout
};

/*-------------------------------------------------------------*\
|  Razer Huntsman V3 Pro TKL White 1532:02A7                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 19 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_v3_pro_tkl_keyboard_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    19
};

static const razer_device huntsman_v3_pro_tkl_device =
{
    "Razer Huntsman V3 Pro TKL White",
    RAZER_HUNTSMAN_V3_PRO_TKL_WHITE_PID,
    DEVICE_TYPE_KEYBOARD,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    6,
    19,
    {
        &huntsman_v3_pro_tkl_keyboard_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_huntsman_v3_pro_tkl_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (Late 2021 Advanced) 1532:2067                |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_late_2021_advanced_zone =
{
    "Keyboard",
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_late_2021_advanced_device =
{
    "Razer Blade 15 (Late 2021 Advanced)",
    RAZER_BLADE_LATE_2021_ADVANCED_PID,
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
    6,
    16,
    {
        &blade_15_late_2021_advanced_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_blade_15_2021_advanced_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_laptop_common_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x1F,
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
    &razer_laptop_with_spacebar_layout
};

/*-------------------------------------------------------------*\
|  Razer Blade 14 (2023) 1532:029D                              |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_14_2023_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_14_2023_device =
{
    "Razer Blade 14 (2023)",
    RAZER_BLADE_14_2023_PID,
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x1F,
    6,
    16,
    {
        &blade_14_2023_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_laptop_with_spacebar_layout
};

/*-------------------------------------------------------------*\
|  Razer Blade 15 (2022) 1532:028A                              |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2022_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2022_device =
{
    "Razer Blade 15 (2022)",
    RAZER_BLADE_15_2022_PID,
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x1F,
    6,
    16,
    {
        &blade_15_2022_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    &razer_blade_15_2022_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_laptop_common_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blade_pro_2017_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blade_17_pro_2021_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    &razer_blade_stealth_2016_layout
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    DEVICE_TYPE_LAPTOP,
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 35K 1532:00CB                              |
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
static const razer_zone basilisk_v3_35k_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_35k_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_35k_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device basilisk_v3_35k_device =
{
    "Razer Basilisk V3 35K",
    RAZER_BASILISK_V3_35K_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    11,
    {
        &basilisk_v3_35k_logo_zone,
        &basilisk_v3_35k_scroll_wheel_zone,
        &basilisk_v3_35k_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_pro_wired_scroll_wheel_zone =
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
    11
};

static const razer_device basilisk_v3_pro_wired_device =
{
    "Razer Basilisk V3 Pro (Wired)",
    RAZER_BASILISK_V3_PRO_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    13,
    {
        &basilisk_v3_pro_wired_scroll_wheel_zone,
        &basilisk_v3_pro_wired_logo_zone,
        &basilisk_v3_pro_wired_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_pro_wireless_scroll_wheel_zone =
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
    11
};

static const razer_device basilisk_v3_pro_wireless_device =
{
    "Razer Basilisk V3 Pro (Wireless)",
    RAZER_BASILISK_V3_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    13,
    {
        &basilisk_v3_pro_wireless_scroll_wheel_zone,
        &basilisk_v3_pro_wireless_logo_zone,
        &basilisk_v3_pro_wireless_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
};
/*-------------------------------------------------------------*\
|  Razer Basilisk V3 PRO 35K Wired 1532:00CC                    |
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
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_pro_35k_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_35k_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_35k_wired_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    11
};

static const razer_device basilisk_v3_pro_35k_wired_device =
{
    "Razer Basilisk V3 Pro 35K (Wired)",
    RAZER_BASILISK_V3_PRO_35K_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    13,
    {
        &basilisk_v3_pro_35k_wired_scroll_wheel_zone,
        &basilisk_v3_pro_35k_wired_logo_zone,
        &basilisk_v3_pro_35k_wired_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 PRO 35K Wireless 1532:00CD                 |
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
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_pro_35k_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_35k_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone basilisk_v3_pro_35k_wireless_ledstrip_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    11
};

static const razer_device basilisk_v3_pro_35k_wireless_device =
{
    "Razer Basilisk V3 Pro 35K (Wireless)",
    RAZER_BASILISK_V3_PRO_35K_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    13,
    {
        &basilisk_v3_pro_35k_wireless_scroll_wheel_zone,
        &basilisk_v3_pro_35k_wireless_logo_zone,
        &basilisk_v3_pro_35k_wireless_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_pro_bluetooth_scroll_wheel_zone =
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
    11
};

static const razer_device basilisk_v3_pro_bluetooth_device =
{
    "Razer Basilisk V3 Pro (Bluetooth)",
    RAZER_BASILISK_V3_PRO_BLUETOOTH_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    13,
    {
        &basilisk_v3_pro_bluetooth_scroll_wheel_zone,
        &basilisk_v3_pro_bluetooth_logo_zone,
        &basilisk_v3_pro_bluetooth_ledstrip_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Basilisk V3 X HyperSpeed 1532:00B9                     |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone basilisk_v3_x_hyperspeed_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device basilisk_v3_x_hyperspeed_device =
{
    "Razer Basilisk V3 X HyperSpeed",
    RAZER_BASILISK_V3_X_HYPERSPEED_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    1,
    {
        &basilisk_v3_x_hyperspeed_scroll_wheel_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Cobra 1532:00A3                                        |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone cobra_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device cobra_device =
{
    "Razer Cobra",
    RAZER_COBRA_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    1,
    {
        &cobra_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Cobra Pro Wired 1532:00AF                              |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Underglow"                                             |
|       Linear                                                  |
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone cobra_pro_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone cobra_pro_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone cobra_pro_wired_underglow_zone =
{
    "Underglow",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device cobra_pro_wired_device =
{
    "Razer Cobra Pro (Wired)",
    RAZER_COBRA_PRO_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    11,
    {
        &cobra_pro_wired_logo_zone,
        &cobra_pro_wired_scroll_wheel_zone,
        &cobra_pro_wired_underglow_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Cobra Pro Wireless 1532:00B0                           |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Underglow"                                             |
|       Linear                                                  |
|       11 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone cobra_pro_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone cobra_pro_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone cobra_pro_wireless_underglow_zone =
{
    "Underglow",
    ZONE_TYPE_LINEAR,
    1,
    9
};

static const razer_device cobra_pro_wireless_device =
{
    "Razer Cobra Pro (Wireless)",
    RAZER_COBRA_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    11,
    {
        &cobra_pro_wireless_logo_zone,
        &cobra_pro_wireless_scroll_wheel_zone,
        &cobra_pro_wireless_underglow_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Leviathan V2 1532:0532                                 |
|                                                               |
|  Zone "Speaker Underglow"                                     |
|       Linear                                                  |
|       18 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone leviathan_v2_speaker_zone =
{
    "Speaker Underglow",
    ZONE_TYPE_LINEAR,
    2,
    9
};

static const razer_device leviathan_v2_device =
{
    "Razer Leviathan V2",
    RAZER_LEVIATHAN_V2_PID,
    DEVICE_TYPE_SPEAKER,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    2,
    9,
    {
        &leviathan_v2_speaker_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Leviathan V2X 1532:054A                                |
|                                                               |
|  Zone "Speaker Underglow"                                     |
|       Linear                                                  |
|       14 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone leviathan_v2x_speaker_zone =
{
    "Speaker Underglow",
    ZONE_TYPE_LINEAR,
    1,
    14
};

static const razer_device leviathan_v2x_device =
{
    "Razer Leviathan V2 X",
    RAZER_LEVIATHAN_V2X_PID,
    DEVICE_TYPE_SPEAKER,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    14,
    {
        &leviathan_v2x_speaker_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Mamba Hyperflux (Wired)                                |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/

static const razer_zone mamba_hyperflux_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_hyperflux_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_hyperflux_device =
{
    "Razer Mamba Hyperflux (Wired)",
    RAZER_MAMBA_HYPERFLUX_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    1,
    2,
    {
        &mamba_hyperflux_scroll_wheel_zone,
        &mamba_hyperflux_logo_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Naga Chroma 1532:0053                                  |
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
    1,
    3,
    {
        &naga_chroma_scroll_wheel_zone,
        &naga_chroma_logo_zone,
        &naga_chroma_numpad_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Naga Trinity 1532:0067                                 |
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

static const razer_device naga_pro_wireless_device =
{
    "Razer Naga Pro (Wireless)",
    RAZER_NAGA_PRO_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Naga Pro V2   1532:00A7 (wired)   1532:00A8 (wireless) |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad"                                                |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone naga_pro_v2_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_pro_v2_numpad_zone =
{
    "Numpad",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device naga_pro_v2_wired_device =
{
    "Razer Naga Pro V2 (Wired)",
    RAZER_NAGA_PRO_V2_WIRED_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    2,
    {
        &naga_pro_v2_logo_zone,
        &naga_pro_v2_numpad_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
};

static const razer_device naga_pro_v2_wireless_device =
{
    "Razer Naga Pro V2 (Wireless)",
    RAZER_NAGA_PRO_V2_WIRELESS_PID,
    DEVICE_TYPE_MOUSE,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    2,
    {
        &naga_pro_v2_logo_zone,
        &naga_pro_v2_numpad_zone,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_CUSTOM,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    &razer_tartarus_v2_layout
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Firefly Hyperflux                                      |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Mousemat"                                              |
|       Linear                                                  |
|       12 LEDs                                                 |
\*-------------------------------------------------------------*/

static const razer_zone firefly_hyperflux_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone firefly_hyperflux_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone firefly_hyperflux_mousemat_zone =
{
    "Mousemat",
    ZONE_TYPE_LINEAR,
    1,
    12
};

static const razer_device firefly_hyperflux_device =
{
    "Razer Firefly Hyperflux",
    RAZER_FIREFLY_HYPERFLUX_PID,
    DEVICE_TYPE_MOUSEMAT,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    1,
    14,
    {
        &firefly_hyperflux_scroll_wheel_zone,
        &firefly_hyperflux_logo_zone,
        &firefly_hyperflux_mousemat_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Firefly V2                                             |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Firefly Pro V2                                         |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       17 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone firefly_v2_pro_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    17
};

static const razer_device firefly_v2_pro_device =
{
    "Razer Firefly V2 Pro",
    RAZER_FIREFLY_V2_PRO_PID,
    DEVICE_TYPE_MOUSEMAT,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    17,
    {
        &firefly_v2_pro_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Goliathus Chroma 3XL 1532:0C06                         |
|                                                               |
|  Zone "LED Strip"                                             |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone goliathus_chroma_3xl_zone =
{
    "LED Strip",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device goliathus_chroma_3xl_device =
{
    "Razer Goliathus Chroma 3XL",
    RAZER_GOLIATHUS_CHROMA_3XL_PID,
    DEVICE_TYPE_MOUSEMAT,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
    1,
    1,
    {
        &goliathus_chroma_3xl_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_NONE,
    0,
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
    NULL
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
    RAZER_MATRIX_TYPE_NONE,
    0,
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
    NULL
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
    RAZER_MATRIX_TYPE_NONE,
    0,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Kraken Kitty Edition V2 1532:0560                      |
|                                                               |
|  Zone "Headset"                                               |
|       Matrix                                                  |
|       4 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone kraken_kitty_black_v2_zone =
{
    "Cat ears",
    ZONE_TYPE_LINEAR,
    1,
    2
};

static const razer_zone kraken_kitty_black_v2_headset_left_zone =
{
    "Headset Left",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone kraken_kitty_black_v2_headset_right_zone =
{
    "Headset Right",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device kraken_kitty_black_v2_device =
{
    "Razer Kraken Kitty Black Edition V2",
    RAZER_KRAKEN_KITTY_BLACK_EDITION_V2_PID,
    DEVICE_TYPE_HEADSET,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    1,
    4,
    {
        &kraken_kitty_black_v2_headset_left_zone,
        &kraken_kitty_black_v2_headset_right_zone,
        &kraken_kitty_black_v2_zone,
        NULL,
        NULL,
        NULL
    },
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_STANDARD,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_LINEAR,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED_ARGB,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0xFF,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x1F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
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
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
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
    NULL
};

/*-------------------------------------------------------------*\
|  Razer Hanbo Chroma                                           |
|                                                               |
|  Zone "Pump"                                                  |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "Fan 1"                                                 |
|       Linear                                                  |
|       18 LEDs                                                 |
|                                                               |
|  Zone "Fan 2"                                                 |
|       Linear                                                  |
|       18 LEDs                                                 |
|                                                               |
|  Zone "Fan 3"                                                 |
|       Linear                                                  |
|       18 LEDs                                                 |
|                                                               |
\*-------------------------------------------------------------*/
static const razer_zone hanbo_chroma_pump_zone =
{
    "Pump",
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_zone hanbo_chroma_fan_one_zone =
{
    "Fan 1",
    ZONE_TYPE_LINEAR,
    1,
    18
};

static const razer_zone hanbo_chroma_fan_two_zone =
{
    "Fan 2",
    ZONE_TYPE_LINEAR,
    1,
    18
};

static const razer_zone hanbo_chroma_fan_three_zone =
{
    "Fan 3",
    ZONE_TYPE_LINEAR,
    1,
    18
};

static const razer_device hanbo_chroma_device =
{
    "Razer Hanbo Chroma",
    RAZER_HANBO_CHROMA_PID,
    DEVICE_TYPE_COOLER,
    RAZER_MATRIX_TYPE_EXTENDED,
    0x3F,
    1,
    70,
    {
        &hanbo_chroma_pump_zone,
        &hanbo_chroma_fan_one_zone,
        &hanbo_chroma_fan_two_zone,
        &hanbo_chroma_fan_three_zone,
        NULL,
        NULL
    },
    NULL
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
    &blackwidow_v4_device,
    &blackwidow_v4_pro_device,
    &blackwidow_v4_pro_75_wired_device,
    &blackwidow_v4_x_device,
    &blackwidow_x_chroma_device,
    &blackwidow_x_chroma_te_device,
    &cynosa_chroma_device,
    &cynosa_v2_device,
    &cynosa_lite_device,
    &deathstalker_chroma_device,
    &deathstalker_v2_device,
    &deathstalker_v2_pro_tkl_wired_device,
    &deathstalker_v2_pro_tkl_wireless_device,
    &deathstalker_v2_pro_wired_device,
    &deathstalker_v2_pro_wireless_device,
    &huntsman_device,
    &huntsman_elite_device,
    &huntsman_mini_device,
    &huntsman_mini_analog_device,
    &huntsman_te_device,
    &huntsman_v2_device,
    &huntsman_v2_analog_device,
    &huntsman_v2_tkl_device,
    &huntsman_v3_pro_device,
    &huntsman_v3_pro_tkl_device,
    &ornata_chroma_device,
    &ornata_chroma_v2_device,
    &ornata_v3_device,
    &ornata_v3_rev2_device,
    &ornata_v3_tkl_device,
    &ornata_v3_x_device,
    &ornata_v3_x_v2_device,
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
    &blade_15_late_2021_advanced_device,
    &blade_14_2021_device,
    &blade_14_2022_device,
    &blade_14_2023_device,
    &blade_15_2022_device,
    &book_13_2020_device,
    &blade_pro_2016_device,
    &blade_pro_2017_device,
    &blade_pro_2017_fullhd_device,
    &blade_pro_2019_device,
    &blade_pro_late_2019_device,
    &blade_pro_17_2020_device,
    &blade_pro_17_2021_device,
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
    &basilisk_v3_35k_device,
    &basilisk_v3_pro_wired_device,
    &basilisk_v3_pro_wireless_device,
    &basilisk_v3_pro_35k_wired_device,
    &basilisk_v3_pro_35k_wireless_device,
    &basilisk_v3_pro_bluetooth_device,
    &basilisk_v3_x_hyperspeed_device,
    &cobra_device,
    &cobra_pro_wired_device,
    &cobra_pro_wireless_device,
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
    &mamba_hyperflux_device,
    &naga_chroma_device,
    &naga_classic_device,
    &naga_epic_chroma_device,
    &naga_hex_v2_device,
    &naga_left_handed_device,
    &naga_trinity_device,
    &naga_pro_wired_device,
    &naga_pro_wireless_device,
    &naga_pro_v2_wired_device,
    &naga_pro_v2_wireless_device,
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
    &firefly_v2_pro_device,
    &goliathus_chroma_3xl_device,
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
    &kraken_kitty_black_v2_device,
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
    &leviathan_v2_device,
    &leviathan_v2x_device,
    &mug_holder_device,
    &mouse_dock_chroma_device,
    &mouse_dock_pro_device,
    &nommo_chroma_device,
    &nommo_pro_device,
    &o11_dynamic_device,
    &seiren_emote_device,
    &thunderbolt_4_dock_chroma_device,
    &hanbo_chroma_device
};

const unsigned int RAZER_NUM_DEVICES = (sizeof(razer_device_list) / sizeof(razer_device_list[ 0 ]));
const razer_device** device_list = razer_device_list;
