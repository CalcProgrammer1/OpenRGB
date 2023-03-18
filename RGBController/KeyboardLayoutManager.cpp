/*---------------------------------------------------------------------*\
|  KeyboardLayoutManager.cpp                                            |
|                                                                       |
|  Helper library to produce keyboard layouts                           |
|                                                                       |
|  Chris M (Dr_No)          04 Feb 2023                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include "LogManager.h"
#include "KeyboardLayoutManager.h"

const char* KEYBOARD_NAME_ISO       = "ISO ";
const char* KEYBOARD_NAME_ANSI      = "ANSI ";
const char* KEYBOARD_NAME_JIS       = "JIS";
const char* KEYBOARD_NAME_AZERTY    = "AZERTY";
const char* KEYBOARD_NAME_QWERTY    = "QWERTY";
const char* KEYBOARD_NAME_QWERTZ    = "QWERTZ";

const char* KEYBOARD_NAME_FULL      = "Full 104 key ";
const char* KEYBOARD_NAME_TKL       = "Tenkeyless ";
const char* KEYBOARD_NAME_SIXTY     = "Sixty percent ";

const char* LOG_MSG_EMPTY           = "empty ";
const char* LOG_MSG_UNUSED_KEY      = "'unused' key ";
const char* LOG_MSG_SHIFTING_RIGHT  = ", shifting keys right";
const char* LOG_MSG_CREATED_NEW     = "[Keyboard Layout Manager] Created new %s%s with %d rows and %d columns containing %d keys";
const char* LOG_MSG_INSERT_BEFORE   = "[%s] Inserting %s before %s @ %02d, %02d%s";

static const std::vector<keyboard_led> keyboard_basemap =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Value,      Key                         |
    \*---------------------------------------------------------------------*/
    {   0,      0,      0,          0,          KEY_EN_ESCAPE,              },
    {   0,      0,      2,          0,          KEY_EN_F1,                  },
    {   0,      0,      3,          0,          KEY_EN_F2,                  },
    {   0,      0,      4,          0,          KEY_EN_F3,                  },
    {   0,      0,      5,          0,          KEY_EN_F4,                  },
    {   0,      0,      6,          0,          KEY_EN_F5,                  },
    {   0,      0,      7,          0,          KEY_EN_F6,                  },
    {   0,      0,      8,          0,          KEY_EN_F7,                  },
    {   0,      0,      9,          0,          KEY_EN_F8,                  },
    {   0,      0,      10,         0,          KEY_EN_F9,                  },
    {   0,      0,      11,         0,          KEY_EN_F10,                 },
    {   0,      0,      12,         0,          KEY_EN_F11,                 },
    {   0,      0,      13,         0,          KEY_EN_F12,                 },
    {   0,      0,      14,         0,          KEY_EN_PRINT_SCREEN,        },
    {   0,      0,      15,         0,          KEY_EN_SCROLL_LOCK,         },
    {   0,      0,      16,         0,          KEY_EN_PAUSE_BREAK,         },
    {   0,      1,      0,          0,          KEY_EN_BACK_TICK,           },
    {   0,      1,      1,          0,          KEY_EN_1,                   },
    {   0,      1,      2,          0,          KEY_EN_2,                   },
    {   0,      1,      3,          0,          KEY_EN_3,                   },
    {   0,      1,      4,          0,          KEY_EN_4,                   },
    {   0,      1,      5,          0,          KEY_EN_5,                   },
    {   0,      1,      6,          0,          KEY_EN_6,                   },
    {   0,      1,      7,          0,          KEY_EN_7,                   },
    {   0,      1,      8,          0,          KEY_EN_8,                   },
    {   0,      1,      9,          0,          KEY_EN_9,                   },
    {   0,      1,      10,         0,          KEY_EN_0,                   },
    {   0,      1,      11,         0,          KEY_EN_MINUS,               },
    {   0,      1,      12,         0,          KEY_EN_EQUALS,              },
    {   0,      1,      13,         0,          KEY_EN_BACKSPACE,           },
    {   0,      1,      14,         0,          KEY_EN_INSERT,              },
    {   0,      1,      15,         0,          KEY_EN_HOME,                },
    {   0,      1,      16,         0,          KEY_EN_PAGE_UP,             },
    {   0,      1,      17,         0,          KEY_EN_NUMPAD_LOCK,         },
    {   0,      1,      18,         0,          KEY_EN_NUMPAD_DIVIDE,       },
    {   0,      1,      19,         0,          KEY_EN_NUMPAD_TIMES,        },
    {   0,      1,      20,         0,          KEY_EN_NUMPAD_MINUS,        },
    {   0,      2,      0,          0,          KEY_EN_TAB,                 },
    {   0,      2,      1,          0,          KEY_EN_Q,                   },
    {   0,      2,      2,          0,          KEY_EN_W,                   },
    {   0,      2,      3,          0,          KEY_EN_E,                   },
    {   0,      2,      4,          0,          KEY_EN_R,                   },
    {   0,      2,      5,          0,          KEY_EN_T,                   },
    {   0,      2,      6,          0,          KEY_EN_Y,                   },
    {   0,      2,      7,          0,          KEY_EN_U,                   },
    {   0,      2,      8,          0,          KEY_EN_I,                   },
    {   0,      2,      9,          0,          KEY_EN_O,                   },
    {   0,      2,      10,         0,          KEY_EN_P,                   },
    {   0,      2,      11,         0,          KEY_EN_LEFT_BRACKET,        },
    {   0,      2,      12,         0,          KEY_EN_RIGHT_BRACKET,       },
    {   0,      2,      13,         0,          KEY_EN_ANSI_BACK_SLASH,     },
    {   0,      2,      14,         0,          KEY_EN_DELETE,              },
    {   0,      2,      15,         0,          KEY_EN_END,                 },
    {   0,      2,      16,         0,          KEY_EN_PAGE_DOWN,           },
    {   0,      2,      17,         0,          KEY_EN_NUMPAD_7,            },
    {   0,      2,      18,         0,          KEY_EN_NUMPAD_8,            },
    {   0,      2,      19,         0,          KEY_EN_NUMPAD_9,            },
    {   0,      2,      20,         0,          KEY_EN_NUMPAD_PLUS,         },
    {   0,      3,      0,          0,          KEY_EN_CAPS_LOCK,           },
    {   0,      3,      1,          0,          KEY_EN_A,                   },
    {   0,      3,      2,          0,          KEY_EN_S,                   },
    {   0,      3,      3,          0,          KEY_EN_D,                   },
    {   0,      3,      4,          0,          KEY_EN_F,                   },
    {   0,      3,      5,          0,          KEY_EN_G,                   },
    {   0,      3,      6,          0,          KEY_EN_H,                   },
    {   0,      3,      7,          0,          KEY_EN_J,                   },
    {   0,      3,      8,          0,          KEY_EN_K,                   },
    {   0,      3,      9,          0,          KEY_EN_L,                   },
    {   0,      3,      10,         0,          KEY_EN_SEMICOLON,           },
    {   0,      3,      11,         0,          KEY_EN_QUOTE,               },
    {   0,      3,      13,         0,          KEY_EN_ANSI_ENTER,          },
    {   0,      3,      17,         0,          KEY_EN_NUMPAD_4,            },
    {   0,      3,      18,         0,          KEY_EN_NUMPAD_5,            },
    {   0,      3,      19,         0,          KEY_EN_NUMPAD_6,            },
    {   0,      4,      0,          0,          KEY_EN_LEFT_SHIFT,          },
    {   0,      4,      2,          0,          KEY_EN_Z,                   },
    {   0,      4,      3,          0,          KEY_EN_X,                   },
    {   0,      4,      4,          0,          KEY_EN_C,                   },
    {   0,      4,      5,          0,          KEY_EN_V,                   },
    {   0,      4,      6,          0,          KEY_EN_B,                   },
    {   0,      4,      7,          0,          KEY_EN_N,                   },
    {   0,      4,      8,          0,          KEY_EN_M,                   },
    {   0,      4,      9,          0,          KEY_EN_COMMA,               },
    {   0,      4,      10,         0,          KEY_EN_PERIOD,              },
    {   0,      4,      11,         0,          KEY_EN_FORWARD_SLASH,       },
    {   0,      4,      13,         0,          KEY_EN_RIGHT_SHIFT,         },
    {   0,      4,      15,         0,          KEY_EN_UP_ARROW,            },
    {   0,      4,      17,         0,          KEY_EN_NUMPAD_1,            },
    {   0,      4,      18,         0,          KEY_EN_NUMPAD_2,            },
    {   0,      4,      19,         0,          KEY_EN_NUMPAD_3,            },
    {   0,      4,      20,         0,          KEY_EN_NUMPAD_ENTER,        },
    {   0,      5,      0,          0,          KEY_EN_LEFT_CONTROL,        },
    {   0,      5,      1,          0,          KEY_EN_LEFT_WINDOWS,        },
    {   0,      5,      2,          0,          KEY_EN_LEFT_ALT,            },
    {   0,      5,      6,          0,          KEY_EN_SPACE,               },
    {   0,      5,      10,         0,          KEY_EN_RIGHT_ALT,           },
    {   0,      5,      11,         0,          KEY_EN_RIGHT_FUNCTION,      },
    {   0,      5,      12,         0,          KEY_EN_MENU,                },
    {   0,      5,      13,         0,          KEY_EN_RIGHT_CONTROL,       },
    {   0,      5,      14,         0,          KEY_EN_LEFT_ARROW,          },
    {   0,      5,      15,         0,          KEY_EN_DOWN_ARROW,          },
    {   0,      5,      16,         0,          KEY_EN_RIGHT_ARROW,         },
    {   0,      5,      18,         0,          KEY_EN_NUMPAD_0,            },
    {   0,      5,      19,         0,          KEY_EN_NUMPAD_PERIOD,       },
};

keyboard_keymap_overlay iso_azerty
{
    KEYBOARD_SIZE_FULL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_FR_ASTERIX,             },
        {   0,      4,      1,          0,          KEY_NORD_ANGLE_BRACKET,     },
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      1,      0,          0,          KEY_FR_SUPER_2,             },
        {   0,      1,      1,          0,          KEY_FR_AMPERSAND,           },
        {   0,      1,      2,          0,          KEY_FR_ACUTE_E,             },
        {   0,      1,      3,          0,          KEY_FR_DOUBLEQUOTE,         },
        {   0,      1,      4,          0,          KEY_EN_QUOTE,               },
        {   0,      1,      5,          0,          KEY_FR_LEFT_PARENTHESIS,    },
        {   0,      1,      6,          0,          KEY_EN_MINUS,               },
        {   0,      1,      7,          0,          KEY_FR_GRAVE_E,             },
        {   0,      1,      8,          0,          KEY_FR_UNDERSCORE,          },
        {   0,      1,      9,          0,          KEY_FR_CEDILLA_C,           },
        {   0,      1,      10,         0,          KEY_FR_GRAVE_A,             },
        {   0,      1,      11,         0,          KEY_FR_RIGHT_PARENTHESIS,   },
        {   0,      2,      1,          0,          KEY_EN_A,                   },
        {   0,      2,      2,          0,          KEY_EN_Z,                   },
        {   0,      2,      11,         0,          KEY_JP_CHEVRON,             },
        {   0,      2,      12,         0,          KEY_FR_DOLLAR,              },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              },
        {   0,      3,      1,          0,          KEY_EN_Q,                   },
        {   0,      3,      10,         0,          KEY_EN_M,                   },
        {   0,      3,      11,         0,          KEY_FR_GRAVE_U,             },
        {   0,      4,      2,          0,          KEY_EN_W,                   },
        {   0,      4,      8,          0,          KEY_EN_COMMA,               },
        {   0,      4,      9,          0,          KEY_EN_SEMICOLON,           },
        {   0,      4,      10,         0,          KEY_JP_COLON,               },
        {   0,      4,      11,         0,          KEY_FR_EXCLAIMATION,        },
    }
};

keyboard_keymap_overlay iso_qwerty
{
    KEYBOARD_SIZE_FULL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_POUND,               },
        {   0,      4,      1,          0,          KEY_EN_ISO_BACK_SLASH,      },
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              },
    }
};

keyboard_keymap_overlay iso_qwertz
{
    KEYBOARD_SIZE_FULL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_POUND,               },
        {   0,      4,      1,          0,          KEY_NORD_ANGLE_BRACKET,     },
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      1,      0,          0,          KEY_JP_CHEVRON,             },
        {   0,      1,      11,         0,          KEY_DE_ESZETT,              },
        {   0,      1,      12,         0,          KEY_EN_BACK_TICK,           },
        {   0,      2,      6,          0,          KEY_EN_Z,                   },
        {   0,      2,      11,         0,          KEY_DE_DIAERESIS_U,         },
        {   0,      2,      12,         0,          KEY_EN_PLUS,                },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              },
        {   0,      3,      10,         0,          KEY_DE_DIAERESIS_O,         },
        {   0,      3,      11,         0,          KEY_DE_DIAERESIS_A,         },
        {   0,      4,      2,          0,          KEY_EN_Y,                   },
        {   0,      4,      11,         0,          KEY_EN_MINUS,               },
    }
};

keyboard_keymap_overlay jis
{
    KEYBOARD_SIZE_FULL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
        {   0,      3,      12,         0,          KEY_EN_RIGHT_BRACKET,       },
        {   0,      4,      12,         0,          KEY_EN_BACK_SLASH,          },
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      1,      12,         0,          KEY_JP_CHEVRON,             },
        {   0,      2,      11,         0,          KEY_JP_AT,                  },
        {   0,      2,      12,         0,          KEY_EN_LEFT_BRACKET,        },
        {   0,      3,      11,         0,          KEY_JP_COLON,               },
        {   0,      2,      13,         0,          KEY_EN_UNUSED,              },
    }
};

keyboard_keymap_overlay full_to_tkl
{
    KEYBOARD_SIZE_FULL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      1,      17,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      18,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      19,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      20,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      17,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      18,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      19,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      20,         0,          KEY_EN_UNUSED,              },
        {   0,      3,      17,         0,          KEY_EN_UNUSED,              },
        {   0,      3,      18,         0,          KEY_EN_UNUSED,              },
        {   0,      3,      19,         0,          KEY_EN_UNUSED,              },
        {   0,      4,      17,         0,          KEY_EN_UNUSED,              },
        {   0,      4,      18,         0,          KEY_EN_UNUSED,              },
        {   0,      4,      19,         0,          KEY_EN_UNUSED,              },
        {   0,      4,      20,         0,          KEY_EN_UNUSED,              },
        {   0,      5,      18,         0,          KEY_EN_UNUSED,              },
        {   0,      5,      19,         0,          KEY_EN_UNUSED,              },
    }
};

keyboard_keymap_overlay tkl_to_sixty
{
    KEYBOARD_SIZE_TKL,
    {
        /*---------------------------------------------------------------------*\
        | Insert Keys                                                           |
        \*---------------------------------------------------------------------*/
    },
    {
        /*---------------------------------------------------------------------*\
        | Swap Keys                                                             |
        \*---------------------------------------------------------------------*/
        {   0,      0,      0,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      2,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      3,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      4,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      5,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      6,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      7,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      8,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      9,          0,          KEY_EN_UNUSED,              },
        {   0,      0,      10,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      11,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      12,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      13,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      14,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      15,         0,          KEY_EN_UNUSED,              },
        {   0,      0,      16,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      0,          0,          KEY_EN_ESCAPE,              },
        {   0,      1,      14,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      15,         0,          KEY_EN_UNUSED,              },
        {   0,      1,      16,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      14,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      15,         0,          KEY_EN_UNUSED,              },
        {   0,      2,      16,         0,          KEY_EN_UNUSED,              },
        {   0,      4,      15,         0,          KEY_EN_UNUSED,              },
        {   0,      5,      14,         0,          KEY_EN_UNUSED,              },
        {   0,      5,      15,         0,          KEY_EN_UNUSED,              },
        {   0,      5,      16,         0,          KEY_EN_UNUSED,              },
    }
};

KeyboardLayoutManager::KeyboardLayoutManager(KEYBOARD_LAYOUT layout, KEYBOARD_SIZE size) : KeyboardLayoutManager(layout, size, {})
{
}

KeyboardLayoutManager::KeyboardLayoutManager(KEYBOARD_LAYOUT layout, KEYBOARD_SIZE size, layout_values values)
{
    if(size == KEYBOARD_SIZE::KEYBOARD_SIZE_EMPTY)
    {
        LOG_INFO(LOG_MSG_CREATED_NEW, name.c_str(), LOG_MSG_EMPTY, rows, cols, keymap.size());
        return;
    }

    std::string tmp_name;

    keymap  = keyboard_basemap;
    rows = 6;
    cols = 21;

    /*---------------------------------------------------------------------*\
    | Add any values passed into the constructor before switching layouts   |
    |   and declare a value set for any changes afterwards                  |
    \*---------------------------------------------------------------------*/
    for(size_t key_idx = 0; key_idx < values.ansi.size() && key_idx < keymap.size(); key_idx++)
    {
        keymap[key_idx].value = values.ansi[key_idx];
    }

    switch(layout)
    {
        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_AZERTY:
            ChangeKeys(iso_azerty);
            tmp_name = KEYBOARD_NAME_AZERTY;
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTY:
            ChangeKeys(iso_qwerty);
            tmp_name = KEYBOARD_NAME_ISO;
            tmp_name.append(KEYBOARD_NAME_QWERTY);
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_ISO_QWERTZ:
            ChangeKeys(iso_qwertz);
            tmp_name = KEYBOARD_NAME_QWERTZ;
            break;

        case KEYBOARD_LAYOUT::KEYBOARD_LAYOUT_JIS:
            ChangeKeys(jis);
            tmp_name = KEYBOARD_NAME_JIS;
            break;

        default:
            tmp_name = KEYBOARD_NAME_ANSI;
            tmp_name.append(KEYBOARD_NAME_QWERTY);
            break;
    }

    /*---------------------------------------------------------------------*\
    | If the regional layouts were passed in count() returns true before    |
    |   attempting to swap keys.                                            |
    \*---------------------------------------------------------------------*/
    LOG_DEBUG("[%s] Looking for a regional overlay %d and found: %d", name.c_str(), layout, values.regional_overlay.count(layout));
    if(values.regional_overlay.count(layout))
    {
        LOG_DEBUG("[%s] Found a regional overlay swapping keys", name.c_str());
        SwapKeys(values.regional_overlay.find(layout)->second);
    }

    switch(size)
    {
        case KEYBOARD_SIZE::KEYBOARD_SIZE_SIXTY:
            ChangeKeys(full_to_tkl);
            ChangeKeys(tkl_to_sixty);
            name = KEYBOARD_NAME_SIXTY + tmp_name;
            break;

        case KEYBOARD_SIZE::KEYBOARD_SIZE_TKL:
            ChangeKeys(full_to_tkl);
            name = KEYBOARD_NAME_TKL + tmp_name;
            break;

        default:
            name = KEYBOARD_NAME_FULL + tmp_name;
            break;
    }

    LOG_INFO(LOG_MSG_CREATED_NEW, name.c_str(), KEY_EN_UNUSED, rows, cols, keymap.size());
}

KeyboardLayoutManager::~KeyboardLayoutManager()
{

}

void KeyboardLayoutManager::ChangeKeys(keyboard_keymap_overlay new_layout)
{
    InsertKeys(new_layout.insert);
    SwapKeys(new_layout.swap);
    UpdateDimensions();
}

void KeyboardLayoutManager::ChangeKeys(keyboard_keymap_overlay_values new_layout)
{
    InsertKeys(new_layout.insert);
    SwapKeys(new_layout.swap);
    UpdateDimensions();
}

void KeyboardLayoutManager::InsertKeys(std::vector<keyboard_led> ins_keys)
{
    LOG_DEBUG("[%s] Inserting %d keys", name.c_str(), ins_keys.size());

    for(size_t i = 0; i < ins_keys.size(); i++)
    {
        /*---------------------------------------------------------------------*\
        | Find the insertion point                                              |
        \*---------------------------------------------------------------------*/
        std::vector<keyboard_led>::iterator key = keymap.begin();

        while(  key != keymap.end()                                                                         &&
              ((ins_keys[i].col <  cols && !(ins_keys[i].row == key->row && ins_keys[i].col <= key->col))   ||
              ( ins_keys[i].col >= cols && ins_keys[i].row >= key->row)))
        {
            key++;
        }

        if(key == keymap.end())
        {
            LOG_DEBUG(LOG_MSG_INSERT_BEFORE, name.c_str(), ins_keys[i].name, "the end", ins_keys[i].row, ins_keys[i].col, KEY_EN_UNUSED);
            keymap.push_back(ins_keys[i]);
            break;
        }
        else if(strlen(ins_keys[i].name) == 0)
        {
            LOG_DEBUG(LOG_MSG_INSERT_BEFORE, name.c_str(), LOG_MSG_UNUSED_KEY, key->name, key->row, key->col, LOG_MSG_SHIFTING_RIGHT);
            key->col++;
        }
        else
        {
            LOG_DEBUG(LOG_MSG_INSERT_BEFORE, name.c_str(), ins_keys[i].name, key->name, key->row, key->col, KEY_EN_UNUSED);
            key = keymap.insert(key, ins_keys[i]);
            key++;
        }

        /*---------------------------------------------------------------------*\
        | Provided key is not the end of the vector, until the end of the row   |
        |   and assuming the column is equal to the key to the left             |
        |   (i.e. sharing the same spot) shift 1 column right                   |
        \*---------------------------------------------------------------------*/
        for(std::vector<keyboard_led>::iterator shift = key + 1;
            shift != keymap.end() && shift->row == key->row &&
            shift->col == key->col; ++shift, ++key)
        {
            shift->col++;
        }
    }
}

void KeyboardLayoutManager::SwapKeys(std::vector<keyboard_led> swp_keys)
{
    LOG_DEBUG("[%s] Swapping %d keys", name.c_str(), swp_keys.size());

    for(size_t i = 0; i < swp_keys.size(); i++)
    {
        bool not_swapped = true;
        for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
        {
            if(key->row == swp_keys[i].row && key->col == swp_keys[i].col)
            {
                LOG_DEBUG("[%s] Swapping in %s and %s out @ %02d, %02d", name.c_str(), swp_keys[i].name, key->name, key->row, key->col);
                key->name   = swp_keys[i].name;
                key->value  = swp_keys[i].value;
                not_swapped = false;
                break;
            }
        }
        /*---------------------------------------------------------------------*\
        | If the key was not able to be swapped then insert the key instead     |
        \*---------------------------------------------------------------------*/
        if(not_swapped)
        {
            std::vector<keyboard_led> insert;
            insert.push_back(swp_keys[i]);
            InsertKeys(insert);
        }
    }
}

void KeyboardLayoutManager::RemoveKeys(std::vector<keyboard_led> rmv_keys)
{
    for(size_t i = 0; i < rmv_keys.size(); i++)
    {
        for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
        {
            if(key->row == rmv_keys[i].row && key->col == rmv_keys[i].col)
            {
                LOG_DEBUG("[%s] Removing %s @ %02d, %02d and shifting keys left", name.c_str(), key->name, key->row, key->col);
                for(std::vector<keyboard_led>::iterator shift = key + 1; shift != keymap.end() && shift->row == key->row; ++shift)
                {
                    shift->col--;
                }
                keymap.erase(key);
                break;
            }
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
    return keymap.size();
}

std::string KeyboardLayoutManager::GetKeyNameAt(unsigned int key_idx)
{
    if(key_idx < keymap.size())
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

    for(size_t r = 0; r < height; r++)
    {
        size_t offset   = r * width;

        for(size_t c = 0; c < width; c++)
        {
            map_ptr[offset + c] = no_key;
        }
    }

    for(size_t i = 0; i < keymap.size(); i++)
    {
        size_t offset   = (keymap[i].row * width) + keymap[i].col;
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
    | Recalculate row and column values                                     |
    |   First trim all "unused" entries by dropping zero length key names   |
    \*---------------------------------------------------------------------*/
    std::vector<keyboard_led> km = keymap;
    keymap.clear();
    for(std::vector<keyboard_led>::iterator key = km.begin(); key != km.end(); ++key)
    {
        if(strlen(key->name) > 0)
        {
            keymap.push_back(*key);
        }
    }

    /*-------------------------------------*\
    | Sixty percent KBs fix                 |
    |   Check the first row is still row 0  |
    \*-------------------------------------*/
    if(keymap.begin()->row > 0)
    {
        for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
        {
            key->row--;
        }
    }

    /*---------------------------------------------------------------------*\
    | Compute max_row and max_col.                                          |
    \*---------------------------------------------------------------------*/
    uint8_t max_row = 0;
    uint8_t max_col = 0;
    for(std::vector<keyboard_led>::iterator key = keymap.begin(); key != keymap.end(); ++key)
    {
        if(key->row > max_row)
        {
            max_row = key->row;
        }
        if(key->col > max_col)
        {
            max_col = key->col;
        }
    }

    rows = max_row + 1;
    cols = max_col + 1;
}
