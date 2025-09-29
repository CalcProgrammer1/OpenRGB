/*---------------------------------------------------------*\
| RGBController_QMKVialRGB.cpp                              |
|                                                           |
|   RGBController for VialRGB QMK Keyboard Protocol         |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <map>
#include "hsv.h"
#include "RGBController_QMKVialRGB.h"
#include "RGBControllerKeyNames.h"

typedef struct
{
    unsigned short  value;
    std::string     name;
    bool            has_speed;
} vialrgb_mode;

static const vialrgb_mode vialrgb_modes[] =
{
    { VIALRGB_EFFECT_OFF,                       "Off",                          false },
    { VIALRGB_EFFECT_DIRECT,                    "Direct",                       false },
    { VIALRGB_EFFECT_SOLID_COLOR,               "Static",                       false },
    { VIALRGB_EFFECT_ALPHAS_MODS,               "Alphas Mods",                  true  },
    { VIALRGB_EFFECT_GRADIENT_UP_DOWN,          "Gradient Up Down",             true  },
    { VIALRGB_EFFECT_GRADIENT_LEFT_RIGHT,       "Gradient Left Right",          true  },
    { VIALRGB_EFFECT_BREATHING,                 "Breathing",                    true  },
    { VIALRGB_EFFECT_BAND_SAT,                  "Band Sat",                     true  },
    { VIALRGB_EFFECT_BAND_VAL,                  "Band Val",                     true  },
    { VIALRGB_EFFECT_BAND_PINWHEEL_SAT,         "Band Pinwheel Sat",            true  },
    { VIALRGB_EFFECT_BAND_PINWHEEL_VAL,         "Band Pinwheel Val",            true  },
    { VIALRGB_EFFECT_BAND_SPIRAL_SAT,           "Band Spiral Sat",              true  },
    { VIALRGB_EFFECT_BAND_SPIRAL_VAL,           "Band Spiral Val",              true  },
    { VIALRGB_EFFECT_CYCLE_ALL,                 "Cycle All",                    true  },
    { VIALRGB_EFFECT_CYCLE_LEFT_RIGHT,          "Cycle Left Right",             true  },
    { VIALRGB_EFFECT_CYCLE_UP_DOWN,             "Cycle Up Down",                true  },
    { VIALRGB_EFFECT_RAINBOW_MOVING_CHEVRON,    "Rainbow Moving Chevron",       true  },
    { VIALRGB_EFFECT_CYCLE_OUT_IN,              "Cycle Out In",                 true  },
    { VIALRGB_EFFECT_CYCLE_OUT_IN_DUAL,         "Cycle Out In Dual",            true  },
    { VIALRGB_EFFECT_CYCLE_PINWHEEL,            "Cycle Pinwheel",               true  },
    { VIALRGB_EFFECT_CYCLE_SPIRAL,              "Cycle Spiral",                 true  },
    { VIALRGB_EFFECT_DUAL_BEACON,               "Dual Beacon",                  true  },
    { VIALRGB_EFFECT_RAINBOW_BEACON,            "Rainbow Beacon",               true  },
    { VIALRGB_EFFECT_RAINBOW_PINWHEELS,         "Rainbow Pinwheels",            true  },
    { VIALRGB_EFFECT_RAINDROPS,                 "Raindrops",                    true  },
    { VIALRGB_EFFECT_JELLYBEAN_RAINDROPS,       "Jellybean Raindrops",          true  },
    { VIALRGB_EFFECT_HUE_BREATHING,             "Hue Breathing",                true  },
    { VIALRGB_EFFECT_HUE_PENDULUM,              "Hue Pendulum",                 true  },
    { VIALRGB_EFFECT_HUE_WAVE,                  "Hue Wave",                     true  },
    { VIALRGB_EFFECT_TYPING_HEATMAP,            "Typing Heatmap",               true  },
    { VIALRGB_EFFECT_DIGITAL_RAIN,              "Digital Rain",                 true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_SIMPLE,     "Solid Reactive Simple",        true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE,            "Solid Reactive",               true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_WIDE,       "Solid Reactive Wide",          true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTIWIDE,  "Solid Reactive Multiwide",     true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_CROSS,      "Solid Reactive Cross",         true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTICROSS, "Solid Reactive Multicross",    true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_NEXUS,      "Solid Reactive Nexus",         true  },
    { VIALRGB_EFFECT_SOLID_REACTIVE_MULTINEXUS, "Solid Reactive Multinexus",    true  },
    { VIALRGB_EFFECT_SPLASH,                    "Splash",                       true  },
    { VIALRGB_EFFECT_MULTISPLASH,               "Multisplash",                  true  },
    { VIALRGB_EFFECT_SOLID_SPLASH,              "Solid Splash",                 true  },
    { VIALRGB_EFFECT_SOLID_MULTISPLASH,         "Solid Multisplash",            true  },
    { VIALRGB_EFFECT_PIXEL_RAIN,                "Pixel Rain",                   true  },
    { VIALRGB_EFFECT_PIXEL_FRACTAL,             "Pixel Fractal",                true  },
};

enum
{
    KC_NO                   = 0x0000,
    KC_TRANSPARENT          = 0x0001,
    KC_A                    = 0x0004,
    KC_B                    = 0x0005,
    KC_C                    = 0x0006,
    KC_D                    = 0x0007,
    KC_E                    = 0x0008,
    KC_F                    = 0x0009,
    KC_G                    = 0x000A,
    KC_H                    = 0x000B,
    KC_I                    = 0x000C,
    KC_J                    = 0x000D,
    KC_K                    = 0x000E,
    KC_L                    = 0x000F,
    KC_M                    = 0x0010,
    KC_N                    = 0x0011,
    KC_O                    = 0x0012,
    KC_P                    = 0x0013,
    KC_Q                    = 0x0014,
    KC_R                    = 0x0015,
    KC_S                    = 0x0016,
    KC_T                    = 0x0017,
    KC_U                    = 0x0018,
    KC_V                    = 0x0019,
    KC_W                    = 0x001A,
    KC_X                    = 0x001B,
    KC_Y                    = 0x001C,
    KC_Z                    = 0x001D,
    KC_1                    = 0x001E,
    KC_2                    = 0x001F,
    KC_3                    = 0x0020,
    KC_4                    = 0x0021,
    KC_5                    = 0x0022,
    KC_6                    = 0x0023,
    KC_7                    = 0x0024,
    KC_8                    = 0x0025,
    KC_9                    = 0x0026,
    KC_0                    = 0x0027,
    KC_ENTER                = 0x0028,
    KC_ESCAPE               = 0x0029,
    KC_BACKSPACE            = 0x002A,
    KC_TAB                  = 0x002B,
    KC_SPACE                = 0x002C,
    KC_MINUS                = 0x002D,
    KC_EQUAL                = 0x002E,
    KC_LEFT_BRACKET         = 0x002F,
    KC_RIGHT_BRACKET        = 0x0030,
    KC_BACKSLASH            = 0x0031,
    KC_NONUS_HASH           = 0x0032,
    KC_SEMICOLON            = 0x0033,
    KC_QUOTE                = 0x0034,
    KC_GRAVE                = 0x0035,
    KC_COMMA                = 0x0036,
    KC_DOT                  = 0x0037,
    KC_SLASH                = 0x0038,
    KC_CAPS_LOCK            = 0x0039,
    KC_F1                   = 0x003A,
    KC_F2                   = 0x003B,
    KC_F3                   = 0x003C,
    KC_F4                   = 0x003D,
    KC_F5                   = 0x003E,
    KC_F6                   = 0x003F,
    KC_F7                   = 0x0040,
    KC_F8                   = 0x0041,
    KC_F9                   = 0x0042,
    KC_F10                  = 0x0043,
    KC_F11                  = 0x0044,
    KC_F12                  = 0x0045,
    KC_PRINT_SCREEN         = 0x0046,
    KC_SCROLL_LOCK          = 0x0047,
    KC_PAUSE                = 0x0048,
    KC_INSERT               = 0x0049,
    KC_HOME                 = 0x004A,
    KC_PAGE_UP              = 0x004B,
    KC_DELETE               = 0x004C,
    KC_END                  = 0x004D,
    KC_PAGE_DOWN            = 0x004E,
    KC_RIGHT                = 0x004F,
    KC_LEFT                 = 0x0050,
    KC_DOWN                 = 0x0051,
    KC_UP                   = 0x0052,
    KC_NUM_LOCK             = 0x0053,
    KC_KP_SLASH             = 0x0054,
    KC_KP_ASTERISK          = 0x0055,
    KC_KP_MINUS             = 0x0056,
    KC_KP_PLUS              = 0x0057,
    KC_KP_ENTER             = 0x0058,
    KC_KP_1                 = 0x0059,
    KC_KP_2                 = 0x005A,
    KC_KP_3                 = 0x005B,
    KC_KP_4                 = 0x005C,
    KC_KP_5                 = 0x005D,
    KC_KP_6                 = 0x005E,
    KC_KP_7                 = 0x005F,
    KC_KP_8                 = 0x0060,
    KC_KP_9                 = 0x0061,
    KC_KP_0                 = 0x0062,
    KC_KP_DOT               = 0x0063,
    KC_NONUS_BACKSLASH      = 0x0064,
    KC_APPLICATION          = 0x0065,
    KC_KB_POWER             = 0x0066,
    KC_KP_EQUAL             = 0x0067,
    KC_F13                  = 0x0068,
    KC_F14                  = 0x0069,
    KC_F15                  = 0x006A,
    KC_F16                  = 0x006B,
    KC_F17                  = 0x006C,
    KC_F18                  = 0x006D,
    KC_F19                  = 0x006E,
    KC_F20                  = 0x006F,
    KC_F21                  = 0x0070,
    KC_F22                  = 0x0071,
    KC_F23                  = 0x0072,
    KC_F24                  = 0x0073,
    KC_EXECUTE              = 0x0074,
    KC_HELP                 = 0x0075,
    KC_MENU                 = 0x0076,
    KC_SELECT               = 0x0077,
    KC_STOP                 = 0x0078,
    KC_AGAIN                = 0x0079,
    KC_UNDO                 = 0x007A,
    KC_CUT                  = 0x007B,
    KC_COPY                 = 0x007C,
    KC_PASTE                = 0x007D,
    KC_FIND                 = 0x007E,
    KC_KB_MUTE              = 0x007F,
    KC_KB_VOLUME_UP         = 0x0080,
    KC_KB_VOLUME_DOWN       = 0x0081,
    KC_LOCKING_CAPS_LOCK    = 0x0082,
    KC_LOCKING_NUM_LOCK     = 0x0083,
    KC_LOCKING_SCROLL_LOCK  = 0x0084,
    KC_KP_COMMA             = 0x0085,
    KC_KP_EQUAL_AS400       = 0x0086,
    KC_INTERNATIONAL_1      = 0x0087,
    KC_INTERNATIONAL_2      = 0x0088,
    KC_INTERNATIONAL_3      = 0x0089,
    KC_INTERNATIONAL_4      = 0x008A,
    KC_INTERNATIONAL_5      = 0x008B,
    KC_INTERNATIONAL_6      = 0x008C,
    KC_INTERNATIONAL_7      = 0x008D,
    KC_INTERNATIONAL_8      = 0x008E,
    KC_INTERNATIONAL_9      = 0x008F,
    KC_LANGUAGE_1           = 0x0090,
    KC_LANGUAGE_2           = 0x0091,
    KC_LANGUAGE_3           = 0x0092,
    KC_LANGUAGE_4           = 0x0093,
    KC_LANGUAGE_5           = 0x0094,
    KC_LANGUAGE_6           = 0x0095,
    KC_LANGUAGE_7           = 0x0096,
    KC_LANGUAGE_8           = 0x0097,
    KC_LANGUAGE_9           = 0x0098,
    KC_ALTERNATE_ERASE      = 0x0099,
    KC_SYSTEM_REQUEST       = 0x009A,
    KC_CANCEL               = 0x009B,
    KC_CLEAR                = 0x009C,
    KC_PRIOR                = 0x009D,
    KC_RETURN               = 0x009E,
    KC_SEPARATOR            = 0x009F,
    KC_OUT                  = 0x00A0,
    KC_OPER                 = 0x00A1,
    KC_CLEAR_AGAIN          = 0x00A2,
    KC_CRSEL                = 0x00A3,
    KC_EXSEL                = 0x00A4,
    KC_SYSTEM_POWER         = 0x00A5,
    KC_SYSTEM_SLEEP         = 0x00A6,
    KC_SYSTEM_WAKE          = 0x00A7,
    KC_AUDIO_MUTE           = 0x00A8,
    KC_AUDIO_VOL_UP         = 0x00A9,
    KC_AUDIO_VOL_DOWN       = 0x00AA,
    KC_MEDIA_NEXT_TRACK     = 0x00AB,
    KC_MEDIA_PREV_TRACK     = 0x00AC,
    KC_MEDIA_STOP           = 0x00AD,
    KC_MEDIA_PLAY_PAUSE     = 0x00AE,
    KC_MEDIA_SELECT         = 0x00AF,
    KC_MEDIA_EJECT          = 0x00B0,
    KC_MAIL                 = 0x00B1,
    KC_CALCULATOR           = 0x00B2,
    KC_MY_COMPUTER          = 0x00B3,
    KC_WWW_SEARCH           = 0x00B4,
    KC_WWW_HOME             = 0x00B5,
    KC_WWW_BACK             = 0x00B6,
    KC_WWW_FORWARD          = 0x00B7,
    KC_WWW_STOP             = 0x00B8,
    KC_WWW_REFRESH          = 0x00B9,
    KC_WWW_FAVORITES        = 0x00BA,
    KC_MEDIA_FAST_FORWARD   = 0x00BB,
    KC_MEDIA_REWIND         = 0x00BC,
    KC_BRIGHTNESS_UP        = 0x00BD,
    KC_BRIGHTNESS_DOWN      = 0x00BE,
    KC_CONTROL_PANEL        = 0x00BF,
    KC_ASSISTANT            = 0x00C0,
    KC_MISSION_CONTROL      = 0x00C1,
    KC_LAUNCHPAD            = 0x00C2,
    KC_LEFT_CTRL            = 0x00E0,
    KC_LEFT_SHIFT           = 0x00E1,
    KC_LEFT_ALT             = 0x00E2,
    KC_LEFT_GUI             = 0x00E3,
    KC_RIGHT_CTRL           = 0x00E4,
    KC_RIGHT_SHIFT          = 0x00E5,
    KC_RIGHT_ALT            = 0x00E6,
    KC_RIGHT_GUI            = 0x00E7,
};

static std::map<unsigned short, std::string> vialrgb_keynames =
{
    { KC_NO,            ""                      },
    { KC_TRANSPARENT,   ""                      },
    { KC_A,             KEY_EN_A                },
    { KC_B,             KEY_EN_B                },
    { KC_C,             KEY_EN_C                },
    { KC_D,             KEY_EN_D                },
    { KC_E,             KEY_EN_E                },
    { KC_F,             KEY_EN_F                },
    { KC_G,             KEY_EN_G                },
    { KC_H,             KEY_EN_H                },
    { KC_I,             KEY_EN_I                },
    { KC_J,             KEY_EN_J                },
    { KC_K,             KEY_EN_K                },
    { KC_L,             KEY_EN_L                },
    { KC_M,             KEY_EN_M                },
    { KC_N,             KEY_EN_N                },
    { KC_O,             KEY_EN_O                },
    { KC_P,             KEY_EN_P                },
    { KC_Q,             KEY_EN_Q                },
    { KC_R,             KEY_EN_R                },
    { KC_S,             KEY_EN_S                },
    { KC_T,             KEY_EN_T                },
    { KC_U,             KEY_EN_U                },
    { KC_V,             KEY_EN_V                },
    { KC_W,             KEY_EN_W                },
    { KC_X,             KEY_EN_X                },
    { KC_Y,             KEY_EN_Y                },
    { KC_Z,             KEY_EN_Z                },
    { KC_1,             KEY_EN_1                },
    { KC_2,             KEY_EN_2                },
    { KC_3,             KEY_EN_3                },
    { KC_4,             KEY_EN_4                },
    { KC_5,             KEY_EN_5                },
    { KC_6,             KEY_EN_6                },
    { KC_7,             KEY_EN_7                },
    { KC_8,             KEY_EN_8                },
    { KC_9,             KEY_EN_9                },
    { KC_0,             KEY_EN_0                },
    { KC_ENTER,         KEY_EN_ANSI_ENTER       },
    { KC_ESCAPE,        KEY_EN_ESCAPE           },
    { KC_BACKSPACE,     KEY_EN_BACKSPACE        },
    { KC_TAB,           KEY_EN_TAB              },
    { KC_SPACE,         KEY_EN_SPACE            },
    { KC_MINUS,         KEY_EN_MINUS            },
    { KC_EQUAL,         KEY_EN_EQUALS           },
    { KC_LEFT_BRACKET,  KEY_EN_LEFT_BRACKET     },
    { KC_RIGHT_BRACKET, KEY_EN_RIGHT_BRACKET    },
    { KC_BACKSLASH,     KEY_EN_BACK_SLASH       },
    { KC_NONUS_HASH,    KEY_EN_POUND            },
    { KC_SEMICOLON,     KEY_EN_SEMICOLON        },
    { KC_QUOTE,         KEY_EN_QUOTE            },
    { KC_GRAVE,         KEY_EN_BACK_TICK        },
    { KC_COMMA,         KEY_EN_COMMA            },
    { KC_DOT,           KEY_EN_PERIOD           },
    { KC_SLASH,         KEY_EN_FORWARD_SLASH    },
    { KC_CAPS_LOCK,     KEY_EN_CAPS_LOCK        },
    { KC_F1,            KEY_EN_F1               },
    { KC_F2,            KEY_EN_F2               },
    { KC_F3,            KEY_EN_F3               },
    { KC_F4,            KEY_EN_F4               },
    { KC_F5,            KEY_EN_F5               },
    { KC_F6,            KEY_EN_F6               },
    { KC_F7,            KEY_EN_F7               },
    { KC_F8,            KEY_EN_F8               },
    { KC_F9,            KEY_EN_F9               },
    { KC_F10,           KEY_EN_F10              },
    { KC_F11,           KEY_EN_F11              },
    { KC_F12,           KEY_EN_F12              },
    { KC_PRINT_SCREEN,  KEY_EN_PRINT_SCREEN     },
    { KC_SCROLL_LOCK,   KEY_EN_SCROLL_LOCK      },
    { KC_PAUSE,         KEY_EN_PAUSE_BREAK      },
    { KC_INSERT,        KEY_EN_INSERT           },
    { KC_HOME,          KEY_EN_HOME             },
    { KC_PAGE_UP,       KEY_EN_PAGE_UP          },
    { KC_DELETE,        KEY_EN_DELETE           },
    { KC_END,           KEY_EN_END              },
    { KC_PAGE_DOWN,     KEY_EN_PAGE_DOWN        },
    { KC_RIGHT,         KEY_EN_RIGHT_ARROW      },
    { KC_LEFT,          KEY_EN_LEFT_ARROW       },
    { KC_DOWN,          KEY_EN_DOWN_ARROW       },
    { KC_UP,            KEY_EN_UP_ARROW         },
    { KC_NUM_LOCK,      KEY_EN_NUMPAD_LOCK      },
    { KC_KP_SLASH,      KEY_EN_NUMPAD_DIVIDE    },
    { KC_KP_ASTERISK,   KEY_EN_NUMPAD_TIMES     },
    { KC_KP_MINUS,      KEY_EN_NUMPAD_MINUS     },
    { KC_KP_PLUS,       KEY_EN_NUMPAD_PLUS      },
    { KC_KP_ENTER,      KEY_EN_NUMPAD_ENTER     },
    { KC_KP_1,          KEY_EN_NUMPAD_1         },
    { KC_KP_2,          KEY_EN_NUMPAD_2         },
    { KC_KP_3,          KEY_EN_NUMPAD_3         },
    { KC_KP_4,          KEY_EN_NUMPAD_4         },
    { KC_KP_5,          KEY_EN_NUMPAD_5         },
    { KC_KP_6,          KEY_EN_NUMPAD_6         },
    { KC_KP_7,          KEY_EN_NUMPAD_7         },
    { KC_KP_8,          KEY_EN_NUMPAD_8         },
    { KC_KP_9,          KEY_EN_NUMPAD_9         },
    { KC_KP_0,          KEY_EN_NUMPAD_0         },
    { KC_KP_DOT,        KEY_EN_NUMPAD_PERIOD    },
    { KC_LEFT_CTRL,     KEY_EN_LEFT_CONTROL     },
    { KC_LEFT_SHIFT,    KEY_EN_LEFT_SHIFT       },
    { KC_LEFT_ALT,      KEY_EN_LEFT_ALT         },
    { KC_LEFT_GUI,      KEY_EN_LEFT_WINDOWS     },
    { KC_RIGHT_CTRL,    KEY_EN_RIGHT_CONTROL    },
    { KC_RIGHT_SHIFT,   KEY_EN_RIGHT_SHIFT      },
    { KC_RIGHT_ALT,     KEY_EN_RIGHT_ALT        },
    { KC_RIGHT_GUI,     KEY_EN_RIGHT_WINDOWS    },
};

RGBController_QMKVialRGB::RGBController_QMKVialRGB(QMKVialRGBController* controller_ptr)
{
    controller                  = controller_ptr;
    name                        = controller->GetName();
    description                 = "QMK VialRGB Device";
    vendor                      = controller->GetVendor();
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();
    version                     = controller->GetVersion();
    type                        = DEVICE_TYPE_KEYBOARD;

    /*-----------------------------------------------------*\
    | Read mode list                                        |
    \*-----------------------------------------------------*/
    for(std::size_t effect_idx = 0; effect_idx < controller->GetEffectCount(); effect_idx++)
    {
        mode new_mode;
        new_mode.name           = vialrgb_modes[controller->GetEffect(effect_idx)].name;
        new_mode.value          = vialrgb_modes[controller->GetEffect(effect_idx)].value;

        if(new_mode.value == VIALRGB_EFFECT_DIRECT)
        {
            new_mode.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            new_mode.color_mode = MODE_COLORS_PER_LED;
        }

        if(new_mode.value >= VIALRGB_EFFECT_SOLID_COLOR)
        {
            new_mode.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
            new_mode.color_mode = MODE_COLORS_MODE_SPECIFIC;
            new_mode.colors_min = 1;
            new_mode.colors_max = 1;
            new_mode.colors.resize(1);
        }

        if(vialrgb_modes[controller->GetEffect(effect_idx)].has_speed)
        {
            new_mode.flags     |= MODE_FLAG_HAS_SPEED;
            new_mode.speed_min  = 0;
            new_mode.speed_max  = 255;
            new_mode.speed      = 128;
        }

        modes.push_back(new_mode);
    }

    /*-----------------------------------------------------*\
    | Read current mode                                     |
    \*-----------------------------------------------------*/
    unsigned short  cur_mode;
    unsigned char   cur_speed;
    unsigned char   cur_hue;
    unsigned char   cur_sat;
    unsigned char   cur_val;

    controller->GetMode(&cur_mode, &cur_speed, &cur_hue, &cur_sat, &cur_val);

    active_mode = cur_mode;

    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        modes[active_mode].speed = cur_speed;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hsv_t       hsv_color;
        hsv_color.hue                   = ((float)cur_hue * (360.0f / 256.0f));
        hsv_color.saturation            = cur_sat;
        hsv_color.value                 = cur_val;

        RGBColor    rgb_color           = hsv2rgb(&hsv_color);
        modes[active_mode].colors[0]    = rgb_color;
    }

    SetupZones();
}

RGBController_QMKVialRGB::~RGBController_QMKVialRGB()
{
    delete controller;
}

void RGBController_QMKVialRGB::SetupZones()
{
    /*-----------------------------------------------------*\
    | Build matrix map                                      |
    \*-----------------------------------------------------*/
    unsigned char max_col = 0;
    unsigned char max_row = 0;

    for(unsigned short led_index = 0; led_index < controller->GetLEDCount(); led_index++)
    {
        vialrgb_led_info info = controller->GetLEDInfo(led_index);

        if(info.col > max_col)
        {
            max_col = info.col;
        }

        if(info.row > max_row)
        {
            max_row = info.row;
        }
    }

    unsigned char height = max_row + 1;
    unsigned char width  = max_col + 1;

    unsigned int* matrix_map = new unsigned int[width * height];

    memset(matrix_map, 0xFF, (sizeof(unsigned int) * (width * height)));

    for(unsigned short led_index = 0; led_index < controller->GetLEDCount(); led_index++)
    {
        vialrgb_led_info info = controller->GetLEDInfo(led_index);

        matrix_map[(width * info.row) + info.col] = (unsigned int)led_index;
    }

    /*-----------------------------------------------------*\
    | Create keyboard zone                                  |
    \*-----------------------------------------------------*/
    zone keyboard;

    keyboard.name               = "Keyboard";
    keyboard.type               = ZONE_TYPE_MATRIX;
    keyboard.leds_min           = controller->GetLEDCount();
    keyboard.leds_max           = controller->GetLEDCount();
    keyboard.leds_count         = controller->GetLEDCount();
    keyboard.matrix_map         = new matrix_map_type;
    keyboard.matrix_map->height = height;
    keyboard.matrix_map->width  = width;
    keyboard.matrix_map->map    = matrix_map;

    zones.push_back(keyboard);

    /*-----------------------------------------------------*\
    | Create keyboard LEDs                                  |
    \*-----------------------------------------------------*/
    for(unsigned short led_idx = 0; led_idx < controller->GetLEDCount(); led_idx++)
    {
        led new_led;
        new_led.name = vialrgb_keynames[controller->GetKeycode(led_idx)];

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_QMKVialRGB::ResizeZone(int zone, int new_size)
{

}

void RGBController_QMKVialRGB::DeviceUpdateLEDs()
{
    controller->SendLEDs(colors.size(), colors.data());
}

void RGBController_QMKVialRGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKVialRGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKVialRGB::DeviceUpdateMode()
{
    unsigned char hue = 0;
    unsigned char sat = 0;
    unsigned char val = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        hsv_t hsv_color;
        rgb2hsv(modes[active_mode].colors[0], &hsv_color);

        hue = (unsigned char)((float)hsv_color.hue * (256.0f / 360.0f));
        sat = hsv_color.saturation;
        val = hsv_color.value;
    }
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, hue, sat, val);
}
