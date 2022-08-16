#include "CorsairPeripheralV2Devices.h"

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
#define K60_KEYMAP_SIZE (sizeof(K60_keymap) / sizeof(K60_keymap[0]))

static const corsair_v2_led K60_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,     Index,      Name                        |
    \*---------------------------------------------------------------------*/
    {   0,      0,       0,          41,        KEY_EN_ESCAPE               },
    {   0,      0,       2,          58,        KEY_EN_F1                   },
    {   0,      0,       3,          59,        KEY_EN_F2                   },
    {   0,      0,       4,          60,        KEY_EN_F3                   },
    {   0,      0,       5,          61,        KEY_EN_F4                   },
    {   0,      0,       6,          62,        KEY_EN_F5                   },
    {   0,      0,       7,          63,        KEY_EN_F6                   },
    {   0,      0,       8,          64,        KEY_EN_F7                   },
    {   0,      0,       9,          65,        KEY_EN_F8                   },
    {   0,      0,      10,          66,        KEY_EN_F9                   },
    {   0,      0,      11,          67,        KEY_EN_F10                  },
    {   0,      0,      12,          68,        KEY_EN_F11                  },
    {   0,      0,      13,          69,        KEY_EN_F12                  },
    {   0,      0,      14,          70,        KEY_EN_PRINT_SCREEN         },
    {   0,      0,      15,          71,        KEY_EN_SCROLL_LOCK          },
    {   0,      0,      16,          72,        KEY_EN_PAUSE_BREAK          },
    {   0,      1,       0,          53,        KEY_EN_BACK_TICK            },
    {   0,      1,       1,          30,        KEY_EN_1                    },
    {   0,      1,       2,          31,        KEY_EN_2                    },
    {   0,      1,       3,          32,        KEY_EN_3                    },
    {   0,      1,       4,          33,        KEY_EN_4                    },
    {   0,      1,       5,          34,        KEY_EN_5                    },
    {   0,      1,       6,          35,        KEY_EN_6                    },
    {   0,      1,       7,          36,        KEY_EN_7                    },
    {   0,      1,       8,          37,        KEY_EN_8                    },
    {   0,      1,       9,          38,        KEY_EN_9                    },
    {   0,      1,      10,          39,        KEY_EN_0                    },
    {   0,      1,      11,          45,        KEY_EN_MINUS                },
    {   0,      1,      12,          46,        KEY_EN_EQUALS               },
    {   0,      1,      13,          42,        KEY_EN_BACKSPACE            },
    {   0,      1,      14,          73,        KEY_EN_INSERT               },
    {   0,      1,      15,          74,        KEY_EN_HOME                 },
    {   0,      1,      16,          75,        KEY_EN_PAGE_UP              },
    {   0,      1,      17,          83,        KEY_EN_NUMPAD_LOCK          },
    {   0,      1,      18,          84,        KEY_EN_NUMPAD_DIVIDE        },
    {   0,      1,      19,          85,        KEY_EN_NUMPAD_TIMES         },
    {   0,      1,      20,          86,        KEY_EN_NUMPAD_MINUS         },
    {   0,      2,       0,          43,        KEY_EN_TAB                  },
    {   0,      2,       1,          20,        KEY_EN_Q                    },
    {   0,      2,       2,          26,        KEY_EN_W                    },
    {   0,      2,       3,           8,        KEY_EN_E                    },
    {   0,      2,       4,          21,        KEY_EN_R                    },
    {   0,      2,       5,          23,        KEY_EN_T                    },
    {   0,      2,       6,          28,        KEY_EN_Y                    },
    {   0,      2,       7,          24,        KEY_EN_U                    },
    {   0,      2,       8,          12,        KEY_EN_I                    },
    {   0,      2,       9,          18,        KEY_EN_O                    },
    {   0,      2,      10,          19,        KEY_EN_P                    },
    {   0,      2,      11,          47,        KEY_EN_LEFT_BRACKET         },
    {   0,      2,      12,          48,        KEY_EN_RIGHT_BRACKET        },
    {   0,      2,      13,          49,        KEY_EN_ANSI_BACK_SLASH      },
    {   0,      2,      14,          76,        KEY_EN_DELETE               },
    {   0,      2,      15,          77,        KEY_EN_END                  },
    {   0,      2,      16,          78,        KEY_EN_PAGE_DOWN            },
    {   0,      2,      17,          95,        KEY_EN_NUMPAD_7             },
    {   0,      2,      18,          96,        KEY_EN_NUMPAD_8             },
    {   0,      2,      19,          97,        KEY_EN_NUMPAD_9             },
    {   0,      2,      20,          87,        KEY_EN_NUMPAD_PLUS          },
    {   0,      3,       0,          57,        KEY_EN_CAPS_LOCK            },
    {   0,      3,       1,           4,        KEY_EN_A                    },
    {   0,      3,       2,          22,        KEY_EN_S                    },
    {   0,      3,       3,           7,        KEY_EN_D                    },
    {   0,      3,       4,           9,        KEY_EN_F                    },
    {   0,      3,       5,          10,        KEY_EN_G                    },
    {   0,      3,       6,          11,        KEY_EN_H                    },
    {   0,      3,       7,          13,        KEY_EN_J                    },
    {   0,      3,       8,          14,        KEY_EN_K                    },
    {   0,      3,       9,          15,        KEY_EN_L                    },
    {   0,      3,      10,          51,        KEY_EN_SEMICOLON            },
    {   0,      3,      11,          52,        KEY_EN_QUOTE                },
    {   0,      3,      12,          50,        KEY_EN_POUND                },
    {   0,      3,      13,          40,        KEY_EN_ANSI_ENTER           },
    {   0,      3,      17,          92,        KEY_EN_NUMPAD_4             },
    {   0,      3,      18,          93,        KEY_EN_NUMPAD_5             },
    {   0,      3,      19,          94,        KEY_EN_NUMPAD_6             },
    {   0,      4,       0,         106,        KEY_EN_LEFT_SHIFT           },
    {   0,      4,       1,         100,        KEY_EN_ISO_BACK_SLASH       },
    {   0,      4,       2,          29,        KEY_EN_Z                    },
    {   0,      4,       3,          27,        KEY_EN_X                    },
    {   0,      4,       4,           6,        KEY_EN_C                    },
    {   0,      4,       5,          25,        KEY_EN_V                    },
    {   0,      4,       6,           5,        KEY_EN_B                    },
    {   0,      4,       7,          17,        KEY_EN_N                    },
    {   0,      4,       8,          16,        KEY_EN_M                    },
    {   0,      4,       9,          54,        KEY_EN_COMMA                },
    {   0,      4,      10,          55,        KEY_EN_PERIOD               },
    {   0,      4,      11,          56,        KEY_EN_FORWARD_SLASH        },
    {   0,      4,      13,         110,        KEY_EN_RIGHT_SHIFT          },
    {   0,      4,      15,          82,        KEY_EN_UP_ARROW             },
    {   0,      4,      17,          89,        KEY_EN_NUMPAD_1             },
    {   0,      4,      18,          90,        KEY_EN_NUMPAD_2             },
    {   0,      4,      19,          91,        KEY_EN_NUMPAD_3             },
    {   0,      4,      20,          88,        KEY_EN_NUMPAD_ENTER         },
    {   0,      5,       0,         105,        KEY_EN_LEFT_CONTROL         },
    {   0,      5,       1,         108,        KEY_EN_LEFT_WINDOWS         },
    {   0,      5,       2,         107,        KEY_EN_LEFT_ALT             },
    {   0,      5,       6,          44,        KEY_EN_SPACE                },
    {   0,      5,      10,         111,        KEY_EN_RIGHT_ALT            },
    {   0,      5,      11,         122,        KEY_EN_RIGHT_FUNCTION       },
    {   0,      5,      12,         101,        KEY_EN_MENU                 },
    {   0,      5,      13,         109,        KEY_EN_RIGHT_CONTROL        },
    {   0,      5,      14,          80,        KEY_EN_LEFT_ARROW           },
    {   0,      5,      15,          81,        KEY_EN_DOWN_ARROW           },
    {   0,      5,      16,          79,        KEY_EN_RIGHT_ARROW          },
    {   0,      5,      18,          98,        KEY_EN_NUMPAD_0             },
    {   0,      5,      19,          99,        KEY_EN_NUMPAD_PERIOD        },
};

/*-------------------------------------------------------------------------*\
|  CORSAIR DEVICES                                                          |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Corsair K55 RGB Pro 1B1C:1BA4                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 6 Columns                                        |
\*-------------------------------------------------------------*/
static const corsair_v2_zone k55_rgb_pro_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    6
};

static const corsair_v2_device k55_rgb_pro_device =
{
    CORSAIR_K55_RGB_PRO_PID,
    false,
    DEVICE_TYPE_KEYBOARD,
    CORSAIR_V2_MODE_SW,
    1,
    6,
    {
        &k55_rgb_pro_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    nullptr,
    0
};

/*-------------------------------------------------------------*\
|  Corsair K60 RGB Pro 1B1C:1BA0                                |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 21 Columns                                      |
\*-------------------------------------------------------------*/
static const corsair_v2_zone k60_rgb_pro_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    21
};

static const corsair_v2_device k60_rgb_pro_device =
{
    CORSAIR_K60_RGB_PRO_PID,
    false,
    DEVICE_TYPE_KEYBOARD,
    CORSAIR_V2_MODE_SW,
    6,
    21,
    {
        &k60_rgb_pro_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    K60_keymap,
    K60_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Corsair K60 RGB Pro Low Profile 1B1C:1BAD                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 21 Columns                                      |
\*-------------------------------------------------------------*/
static const corsair_v2_zone k60_rgb_pro_lp_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    21
};

static const corsair_v2_device k60_rgb_pro_lp_device =
{
    CORSAIR_K60_RGB_PRO_LP_PID,
    false,
    DEVICE_TYPE_KEYBOARD,
    CORSAIR_V2_MODE_SW,
    6,
    21,
    {
        &k60_rgb_pro_lp_zone,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    },
    K60_keymap,
    K60_KEYMAP_SIZE
};

/*-------------------------------------------------------------*\
|  Corsair MM700 1B1C:1B9B                                      |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|                                                               |
|  Zone "Edge"                                                  |
|       Linear                                                  |
|       1 Row, 2 Columns                                        |
\*-------------------------------------------------------------*/
static const corsair_v2_zone mm700_right_zone =
{
    "Right",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_v2_zone mm700_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_v2_zone mm700_left_zone =
{
    "Left",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const corsair_v2_device mm700_device =
{
    CORSAIR_MM700_PID,
    false,
    DEVICE_TYPE_MOUSEMAT,
    CORSAIR_V2_MODE_SW,
    1,
    3,
    {
        &mm700_left_zone,
        &mm700_right_zone,
        &mm700_logo_zone,
        nullptr,
        nullptr,
        nullptr
    },
    nullptr,
    0
};

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
const corsair_v2_device* corsair_v2_device_list_data[] =
{
/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
    &k55_rgb_pro_device,
    &k60_rgb_pro_device,
    &k60_rgb_pro_lp_device,

/*-----------------------------------------------------------------*\
|  MOUSEMATS                                                        |
\*-----------------------------------------------------------------*/
    &mm700_device,
};

const unsigned int CORSAIR_V2_DEVICE_COUNT = (sizeof(corsair_v2_device_list_data) / sizeof(corsair_v2_device_list_data[ 0 ]));
const corsair_v2_device** corsair_v2_device_list = corsair_v2_device_list_data;
