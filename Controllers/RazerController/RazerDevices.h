#define RAZER_MAX_ZONES 6

#include <string>
#include "RGBController.h"

/*-----------------------------------------------------*\
| Razer vendor ID                                       |
\*-----------------------------------------------------*/
#define RAZER_VID                                       0x1532

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from OpenRazer                           |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define RAZER_BLACKWIDOW_2019_PID                       0x0241
#define RAZER_BLACKWIDOW_CHROMA_PID                     0x0203
#define RAZER_BLACKWIDOW_CHROMA_TE_PID                  0x0209
#define RAZER_BLACKWIDOW_CHROMA_V2_PID                  0x0221
#define RAZER_BLACKWIDOW_ELITE_PID                      0x0228
#define RAZER_BLACKWIDOW_ESSENTIAL_PID                  0x0237
#define RAZER_BLACKWIDOW_LITE_PID                       0x0235
#define RAZER_BLACKWIDOW_OVERWATCH_PID                  0x0211
#define RAZER_BLACKWIDOW_X_CHROMA_PID                   0x0216
#define RAZER_BLACKWIDOW_X_CHROMA_TE_PID                0x021A
#define RAZER_BLADE_2016_PID                            0x020F
#define RAZER_BLADE_LATE_2016_PID                       0x0224
#define RAZER_BLADE_2018_ADVANCED_PID                   0x0233
#define RAZER_BLADE_2018_BASE_PID                       0x023B
#define RAZER_BLADE_2018_MERCURY_PID                    0x0240
#define RAZER_BLADE_2019_ADVANCED_PID                   0x023A
#define RAZER_BLADE_2019_BASE_PID                       0x0246
#define RAZER_BLADE_2019_MERCURY_PID                    0x0245
#define RAZER_BLADE_2019_STUDIO_PID                     0x024D
#define RAZER_BLADE_2020_ADVANCED_PID                   0x0253
#define RAZER_BLADE_2020_BASE_PID                       0x0255
#define RAZER_BLADE_2021_BASE_PID                       0x026F

#define RAZER_BLADE_PRO_2016_PID                        0x0210
#define RAZER_BLADE_PRO_2017_PID                        0x0225
#define RAZER_BLADE_PRO_2017_FULLHD_PID                 0x022F
#define RAZER_BLADE_PRO_2019_PID                        0x0234
#define RAZER_BLADE_PRO_LATE_2019_PID                   0x024C

#define RAZER_BLADE_STEALTH_2016_PID                    0x0205
#define RAZER_BLADE_STEALTH_LATE_2016_PID               0x0220
#define RAZER_BLADE_STEALTH_2017_PID                    0x022D
#define RAZER_BLADE_STEALTH_LATE_2017_PID               0x0232
#define RAZER_BLADE_STEALTH_2019_PID                    0x0239
#define RAZER_BLADE_STEALTH_LATE_2019_PID               0x024A
#define RAZER_BLADE_STEALTH_2020_PID                    0x0252
#define RAZER_BLADE_STEALTH_LATE_2020_PID               0x0259


#define RAZER_CYNOSA_CHROMA_PID                         0x022A
#define RAZER_CYNOSA_LITE_PID                           0x023F
#define RAZER_CYNOSA_V2_PID                             0x025E
#define RAZER_DEATHSTALKER_CHROMA_PID                   0x0204
#define RAZER_HUNTSMAN_ELITE_PID                        0x0226
#define RAZER_HUNTSMAN_PID                              0x0227
#define RAZER_HUNTSMAN_TE_PID                           0x0243
#define RAZER_ORBWEAVER_CHROMA_PID                      0x0207
#define RAZER_ORNATA_CHROMA_PID                         0x021E
#define RAZER_ORNATA_CHROMA_V2_PID                      0x025D
#define RAZER_TARTARUS_CHROMA_PID                       0x0208
#define RAZER_TARTARUS_V2_PID                           0x022B

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from OpenRazer                           |
\*-----------------------------------------------------*/
#define RAZER_ABYSSUS_1800_PID                          0x0020
#define RAZER_ABYSSUS_2000_PID                          0x005E
#define RAZER_ABYSSUS_ELITE_DVA_EDITION_PID             0x006A
#define RAZER_ABYSSUS_ESSENTIAL_PID                     0x006B
#define RAZER_ABYSSUS_PID                               0x0042
#define RAZER_ABYSSUS_V2_PID                            0x005B
#define RAZER_ATHERIS_RECEIVER_PID                      0x0062
#define RAZER_BASILISK_PID                              0x0064
#define RAZER_BASILISK_ESSENTIAL_PID                    0x0065
#define RAZER_BASILISK_X_HYPERSPEED_PID                 0x0083
#define RAZER_DEATHADDER_1800_PID                       0x0038
#define RAZER_DEATHADDER_2000_PID                       0x004F
#define RAZER_DEATHADDER_2013_PID                       0x0037
#define RAZER_DEATHADDER_3_5G_PID                       0x0016
#define RAZER_DEATHADDER_3500_PID                       0x0054
#define RAZER_DEATHADDER_CHROMA_PID                     0x0043
#define RAZER_DEATHADDER_ELITE_PID                      0x005C
#define RAZER_DEATHADDER_ESSENTIAL_PID                  0x006E
#define RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION_PID    0x0071
#define RAZER_DEATHADDER_V2_MINI_PID                    0x008C
#define RAZER_DEATHADDER_V2_PID                         0x0084
#define RAZER_DEATHADDER_V2_PRO_WIRED_PID               0x007C
#define RAZER_DEATHADDER_V2_PRO_WIRELESS_PID            0x007D
#define RAZER_DIAMONDBACK_CHROMA_PID                    0x004C
#define RAZER_IMPERATOR_PID                             0x002F
#define RAZER_LANCEHEAD_TE_WIRED_PID                    0x0060
#define RAZER_LANCEHEAD_2017_WIRED_PID                  0x0059
#define RAZER_LANCEHEAD_2017_WIRELESS_PID               0x005A
#define RAZER_LANCEHEAD_2019_WIRED_PID                  0x0070
#define RAZER_LANCEHEAD_2019_WIRELESS_PID               0x006F
#define RAZER_MAMBA_2012_WIRED_PID                      0x0024
#define RAZER_MAMBA_2012_WIRELESS_PID                   0x0025
#define RAZER_MAMBA_2015_WIRED_PID                      0x0044
#define RAZER_MAMBA_2015_WIRELESS_PID                   0x0045
#define RAZER_MAMBA_2018_WIRED_PID                      0x0073
#define RAZER_MAMBA_2018_WIRELESS_PID                   0x0072
#define RAZER_MAMBA_ELITE_PID                           0x006C
#define RAZER_MAMBA_TE_PID                              0x0046
#define RAZER_NAGA_2012_PID                             0x002E
#define RAZER_NAGA_2014_PID                             0x0040
#define RAZER_NAGA_CHROMA_PID                           0x0053
#define RAZER_NAGA_EPIC_CHROMA_DOCK_PID                 0x003F
#define RAZER_NAGA_EPIC_CHROMA_PID                      0x003E
#define RAZER_NAGA_HEX_PID                              0x0041
#define RAZER_NAGA_HEX_RED_PID                          0x0036
#define RAZER_NAGA_HEX_V2_PID                           0x0050
#define RAZER_NAGA_TRINITY_PID                          0x0067
#define RAZER_OROCHI_2011_PID                           0x0013
#define RAZER_OROCHI_2013_PID                           0x0039
#define RAZER_OROCHI_CHROMA_PID                         0x0048
#define RAZER_OUROBOROS_PID                             0x0032
#define RAZER_TAIPAN_PID                                0x0034
#define RAZER_VIPER_MINI_PID                            0x008A
#define RAZER_VIPER_PID                                 0x0078
#define RAZER_VIPER_ULTIMATE_WIRED_PID                  0x007A
#define RAZER_VIPER_ULTIMATE_WIRELESS_PID               0x007B

/*-----------------------------------------------------*\
| Headset product IDs                                   |
\*-----------------------------------------------------*/
#define RAZER_KRAKEN_CLASSIC_ALT_PID                    0x0506
#define RAZER_KRAKEN_CLASSIC_PID                        0x0501
#define RAZER_KRAKEN_KITTY_EDITION_PID                  0x0F19
#define RAZER_KRAKEN_PID                                0x0504
#define RAZER_KRAKEN_ULTIMATE_PID                       0x0527
#define RAZER_KRAKEN_V2_PID                             0x0510
#define RAZER_TIAMAT_71_V2_PID                          0x0F03

/*-----------------------------------------------------*\
| Accessory product IDs                                 |
|   List taken from OpenRazer                           |
\*-----------------------------------------------------*/
#define RAZER_BASE_STATION_CHROMA_PID                   0x0F08
#define RAZER_BASE_STATION_V2_CHROMA_PID                0x0F20
#define RAZER_CHARGING_PAD_CHROMA_PID                   0x0F26
#define RAZER_CHROMA_ADDRESSABLE_RGB_CONTROLLER_PID     0x0F1F
#define RAZER_CHROMA_HDK_PID                            0x0F09
#define RAZER_CHROMA_MUG_PID                            0x0F07
#define RAZER_CHROMA_PC_CASE_LIGHTING_KIT_PID           0x0F0E
#define RAZER_CORE_PID                                  0x0215
#define RAZER_FIREFLY_HYPERFLUX_PID                     0x0068
#define RAZER_FIREFLY_PID                               0x0C00
#define RAZER_FIREFLY_V2_PID                            0x0C04
#define RAZER_GOLIATHUS_CHROMA_EXTENDED_PID             0x0C02
#define RAZER_GOLIATHUS_CHROMA_PID                      0x0C01
#define RAZER_MOUSE_BUNGEE_V3_CHROMA_PID                0x0F1D
#define RAZER_NOMMO_CHROMA_PID                          0x0517
#define RAZER_NOMMO_PRO_PID                             0x0518

typedef struct
{
    std::string name;
    unsigned int type;
    unsigned int rows;
    unsigned int cols;
} razer_zone;

typedef struct
{
    unsigned int zone;
    unsigned int row;
    unsigned int col;
    const char* name;
} razer_key;

typedef struct
{
    std::string name;
    unsigned short pid;
    device_type type;
    bool matrix_type;
    unsigned int rows;
    unsigned int cols;
    const razer_zone* zones[RAZER_MAX_ZONES];
    const razer_key* keymap;
    unsigned int keymap_size;
} razer_device;

/*-------------------------------------------------------------------------*\
|  KEYMAPS                                                                  |
\*-------------------------------------------------------------------------*/
#define BLACKWIDOW_CHROMA_KEYMAP_SIZE (sizeof(blackwidow_chroma_keymap) / sizeof(blackwidow_chroma_keymap[0]))

static const razer_key blackwidow_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      0,              "Key: M1"                           },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      0,              "Key: M2"                           },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      0,              "Key: M3"                           },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      0,              "Key: M4"                           },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      0,              "Key: M5"                           },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define BLACKWIDOW_CHROMA_TE_KEYMAP_SIZE (sizeof(blackwidow_chroma_te_keymap) / sizeof(blackwidow_chroma_te_keymap[0]))

static const razer_key blackwidow_chroma_te_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
};

#define BLADE_PRO_2017_KEYMAP_SIZE (sizeof(blade_pro_2017_keymap) / sizeof(blade_pro_2017_keymap[0]))

static const razer_key blade_pro_2017_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      2,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Insert"                       },
    {   0,      0,      17,             "Key: Delete"                       },
    {   0,      0,      19,             "Key: Media Previous"               },
    {   0,      0,      20,             "Key: Media Next"                   },
    {   0,      0,      21,             "Key: Media Volume"                 },
    {   0,      0,      23,             "Key: Media Play/Pause"             },
    {   0,      0,      24,             "Key: Media Mute"                   },
    {   0,      1,      2,              "Key: `"                            },
    {   0,      1,      3,              "Key: 1"                            },
    {   0,      1,      4,              "Key: 2"                            },
    {   0,      1,      5,              "Key: 3"                            },
    {   0,      1,      6,              "Key: 4"                            },
    {   0,      1,      7,              "Key: 5"                            },
    {   0,      1,      8,              "Key: 6"                            },
    {   0,      1,      9,              "Key: 7"                            },
    {   0,      1,      10,             "Key: 8"                            },
    {   0,      1,      11,             "Key: 9"                            },
    {   0,      1,      12,             "Key: 0"                            },
    {   0,      1,      13,             "Key: -"                            },
    {   0,      1,      14,             "Key: ="                            },
    {   0,      1,      15,             "Key: Backspace"                    },
    {   0,      2,      2,              "Key: Tab"                          },
    {   0,      2,      4,              "Key: Q"                            },
    {   0,      2,      5,              "Key: W"                            },
    {   0,      2,      6,              "Key: E"                            },
    {   0,      2,      7,              "Key: R"                            },
    {   0,      2,      8,              "Key: T"                            },
    {   0,      2,      9,              "Key: Y"                            },
    {   0,      2,      10,             "Key: U"                            },
    {   0,      2,      11,             "Key: I"                            },
    {   0,      2,      12,             "Key: O"                            },
    {   0,      2,      13,             "Key: P"                            },
    {   0,      2,      14,             "Key: ["                            },
    {   0,      2,      15,             "Key: ]"                            },
    {   0,      2,      17,             "Key: \\ (ANSI)"                    },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      4,              "Key: A"                            },
    {   0,      3,      5,              "Key: S"                            },
    {   0,      3,      6,              "Key: D"                            },
    {   0,      3,      7,              "Key: F"                            },
    {   0,      3,      8,              "Key: G"                            },
    {   0,      3,      9,              "Key: H"                            },
    {   0,      3,      10,             "Key: J"                            },
    {   0,      3,      11,             "Key: K"                            },
    {   0,      3,      12,             "Key: L"                            },
    {   0,      3,      13,             "Key: ;"                            },
    {   0,      3,      14,             "Key: '"                            },
    {   0,      3,      18,             "Key: Enter"                        },
    {   0,      4,      0,              "Key: Left Shift"                   },
    {   0,      4,      4,              "Key: Z"                            },
    {   0,      4,      5,              "Key: X"                            },
    {   0,      4,      6,              "Key: C"                            },
    {   0,      4,      7,              "Key: V"                            },
    {   0,      4,      8,              "Key: B"                            },
    {   0,      4,      9,              "Key: N"                            },
    {   0,      4,      10,             "Key: M"                            },
    {   0,      4,      11,             "Key: ,"                            },
    {   0,      4,      12,             "Key: ."                            },
    {   0,      4,      13,             "Key: /"                            },
    {   0,      4,      14,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Right Shift"                  },
    {   0,      5,      0,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Fn"                      },
    {   0,      5,      3,              "Key: Left Windows"                 },
    {   0,      5,      5,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      10,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Control"                },
    {   0,      5,      13,             "Key: Left Arrow"                   },
    {   0,      5,      14,             "Key: Down Arrow"                   },
    {   0,      5,      15,             "Key: Right Arrow"                  },
    {   0,      5,      16,             "Key: Right Fn"                     },
};

#define BLADE_STEALTH_2016_KEYMAP_SIZE (sizeof(blade_stealth_2016_keymap) / sizeof(blade_stealth_2016_keymap[0]))

static const razer_key blade_stealth_2016_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      2,              "Key: F1"                           },
    {   0,      0,      3,              "Key: F2"                           },
    {   0,      0,      4,              "Key: F3"                           },
    {   0,      0,      5,              "Key: F4"                           },
    {   0,      0,      6,              "Key: F5"                           },
    {   0,      0,      7,              "Key: F6"                           },
    {   0,      0,      8,              "Key: F7"                           },
    {   0,      0,      9,              "Key: F8"                           },
    {   0,      0,      10,             "Key: F9"                           },
    {   0,      0,      11,             "Key: F10"                          },
    {   0,      0,      12,             "Key: F11"                          },
    {   0,      0,      13,             "Key: F12"                          },
    {   0,      0,      14,             "Key: Insert"                       },
    {   0,      0,      15,             "Key: Delete"                       },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Backspace"                    },
    {   0,      2,      0,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: \\ (ANSI)"                    },
    {   0,      3,      0,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      15,             "Key: Enter"                        },
    {   0,      4,      0,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: Z"                            },
    {   0,      4,      3,              "Key: X"                            },
    {   0,      4,      4,              "Key: C"                            },
    {   0,      4,      5,              "Key: V"                            },
    {   0,      4,      6,              "Key: B"                            },
    {   0,      4,      7,              "Key: N"                            },
    {   0,      4,      8,              "Key: M"                            },
    {   0,      4,      9,              "Key: ,"                            },
    {   0,      4,      10,             "Key: ."                            },
    {   0,      4,      11,             "Key: /"                            },
    {   0,      4,      12,             "Key: Right Shift"                  },
    {   0,      4,      13,             "Key: Right Shift"                  },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      5,      0,              "Key: Left Control"                 },
    {   0,      5,      1,              "Key: Left Fn"                      },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      5,              "Key: Space"                        },
    {   0,      5,      6,              "Key: Space"                        },
    {   0,      5,      8,              "Key: Space"                        },
    {   0,      5,      9,              "Key: Right Alt"                    },
    {   0,      5,      10,             "Key: Right Fn"                     },
    {   0,      5,      11,             "Key: Right Control"                },
    {   0,      5,      12,             "Key: Left Arrow"                   },
    {   0,      5,      13,             "Key: Up Arrow"                     },
    {   0,      5,      14,             "Key: Right Arrow"                  },
    {   0,      5,      15,             "Key: Down Arrow"                   },
};

#define CYNOSA_CHROMA_KEYMAP_SIZE (sizeof(cynosa_chroma_keymap) / sizeof(cynosa_chroma_keymap[0]))

static const razer_key cynosa_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define HUNTSMAN_ELITE_KEYMAP_SIZE (sizeof(huntsman_elite_keymap) / sizeof(huntsman_elite_keymap[0]))

static const razer_key huntsman_elite_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      18,             "Key: Media Previous"               },
    {   0,      0,      19,             "Key: Media Play/Pause"             },
    {   0,      0,      20,             "Key: Media Next"                   },
    {   0,      0,      21,             "Key: Media Mute"                   },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define HUNTSMAN_KEYMAP_SIZE (sizeof(huntsman_keymap) / sizeof(huntsman_keymap[0]))

static const razer_key huntsman_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define HUNTSMAN_TE_KEYMAP_SIZE (sizeof(huntsman_te_keymap) / sizeof(huntsman_te_keymap[0]))

static const razer_key huntsman_te_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
};

#define BLACKWIDOW_ELITE_KEYMAP_SIZE (sizeof(blackwidow_elite_keymap) / sizeof(blackwidow_elite_keymap[0]))

static const razer_key blackwidow_elite_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      18,             "Key: Media Previous"               },
    {   0,      0,      19,             "Key: Media Play/Pause"             },
    {   0,      0,      20,             "Key: Media Next"                   },
    {   0,      0,      21,             "Key: Media Mute"                   },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      6,              "Key: Space"                        },
    {   0,      5,      10,             "Key: Right Alt"                    },
    {   0,      5,      11,             "Logo"                              },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define BLACKWIDOW_CHROMA_V2_KEYMAP_SIZE (sizeof(blackwidow_chroma_v2_keymap) / sizeof(blackwidow_chroma_v2_keymap[0]))

static const razer_key blackwidow_chroma_v2_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      0,              "Key: M1"                           },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      0,              "Key: M2"                           },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      0,              "Key: M3"                           },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      0,              "Key: M4"                           },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      0,              "Key: M5"                           },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
};

#define BLACKWIDOW_2019_KEYMAP_SIZE (sizeof(blackwidow_2019_keymap) / sizeof(blackwidow_2019_keymap[0]))

static const razer_key blackwidow_2019_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      6,              "Key: Space"                        },
    {   0,      5,      10,             "Key: Right Alt"                    },
    {   0,      5,      11,             "Logo"                              },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ,"                 },
};

#define ORNATA_CHROMA_KEYMAP_SIZE (sizeof(ornata_chroma_keymap) / sizeof(ornata_chroma_keymap[0]))

static const razer_key ornata_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              "Key: Escape"                       },
    {   0,      0,      3,              "Key: F1"                           },
    {   0,      0,      4,              "Key: F2"                           },
    {   0,      0,      5,              "Key: F3"                           },
    {   0,      0,      6,              "Key: F4"                           },
    {   0,      0,      7,              "Key: F5"                           },
    {   0,      0,      8,              "Key: F6"                           },
    {   0,      0,      9,              "Key: F7"                           },
    {   0,      0,      10,             "Key: F8"                           },
    {   0,      0,      11,             "Key: F9"                           },
    {   0,      0,      12,             "Key: F10"                          },
    {   0,      0,      13,             "Key: F11"                          },
    {   0,      0,      14,             "Key: F12"                          },
    {   0,      0,      15,             "Key: Print Screen"                 },
    {   0,      0,      16,             "Key: Scroll Lock"                  },
    {   0,      0,      17,             "Key: Pause/Break"                  },
    {   0,      1,      1,              "Key: `"                            },
    {   0,      1,      2,              "Key: 1"                            },
    {   0,      1,      3,              "Key: 2"                            },
    {   0,      1,      4,              "Key: 3"                            },
    {   0,      1,      5,              "Key: 4"                            },
    {   0,      1,      6,              "Key: 5"                            },
    {   0,      1,      7,              "Key: 6"                            },
    {   0,      1,      8,              "Key: 7"                            },
    {   0,      1,      9,              "Key: 8"                            },
    {   0,      1,      10,             "Key: 9"                            },
    {   0,      1,      11,             "Key: 0"                            },
    {   0,      1,      12,             "Key: -"                            },
    {   0,      1,      13,             "Key: ="                            },
    {   0,      1,      14,             "Key: Backspace"                    },
    {   0,      1,      15,             "Key: Insert"                       },
    {   0,      1,      16,             "Key: Home"                         },
    {   0,      1,      17,             "Key: Page Up"                      },
    {   0,      1,      18,             "Key: Num Lock"                     },
    {   0,      1,      19,             "Key: Number Pad /"                 },
    {   0,      1,      20,             "Key: Number Pad *"                 },
    {   0,      1,      21,             "Key: Number Pad -"                 },
    {   0,      2,      1,              "Key: Tab"                          },
    {   0,      2,      2,              "Key: Q"                            },
    {   0,      2,      3,              "Key: W"                            },
    {   0,      2,      4,              "Key: E"                            },
    {   0,      2,      5,              "Key: R"                            },
    {   0,      2,      6,              "Key: T"                            },
    {   0,      2,      7,              "Key: Y"                            },
    {   0,      2,      8,              "Key: U"                            },
    {   0,      2,      9,              "Key: I"                            },
    {   0,      2,      10,             "Key: O"                            },
    {   0,      2,      11,             "Key: P"                            },
    {   0,      2,      12,             "Key: ["                            },
    {   0,      2,      13,             "Key: ]"                            },
    {   0,      2,      14,             "Key: \\ (ANSI)"                    },
    {   0,      2,      15,             "Key: Delete"                       },
    {   0,      2,      16,             "Key: End"                          },
    {   0,      2,      17,             "Key: Page Down"                    },
    {   0,      2,      18,             "Key: Number Pad 7"                 },
    {   0,      2,      19,             "Key: Number Pad 8"                 },
    {   0,      2,      20,             "Key: Number Pad 9"                 },
    {   0,      2,      21,             "Key: Number Pad +"                 },
    {   0,      3,      1,              "Key: Caps Lock"                    },
    {   0,      3,      2,              "Key: A"                            },
    {   0,      3,      3,              "Key: S"                            },
    {   0,      3,      4,              "Key: D"                            },
    {   0,      3,      5,              "Key: F"                            },
    {   0,      3,      6,              "Key: G"                            },
    {   0,      3,      7,              "Key: H"                            },
    {   0,      3,      8,              "Key: J"                            },
    {   0,      3,      9,              "Key: K"                            },
    {   0,      3,      10,             "Key: L"                            },
    {   0,      3,      11,             "Key: ;"                            },
    {   0,      3,      12,             "Key: '"                            },
    {   0,      3,      13,             "Key: #"                            },
    {   0,      3,      14,             "Key: Enter"                        },
    {   0,      3,      18,             "Key: Number Pad 4"                 },
    {   0,      3,      19,             "Key: Number Pad 5"                 },
    {   0,      3,      20,             "Key: Number Pad 6"                 },
    {   0,      4,      1,              "Key: Left Shift"                   },
    {   0,      4,      2,              "Key: \\ (ISO)"                     },
    {   0,      4,      3,              "Key: Z"                            },
    {   0,      4,      4,              "Key: X"                            },
    {   0,      4,      5,              "Key: C"                            },
    {   0,      4,      6,              "Key: V"                            },
    {   0,      4,      7,              "Key: B"                            },
    {   0,      4,      8,              "Key: N"                            },
    {   0,      4,      9,              "Key: M"                            },
    {   0,      4,      10,             "Key: ,"                            },
    {   0,      4,      11,             "Key: ."                            },
    {   0,      4,      12,             "Key: /"                            },
    {   0,      4,      14,             "Key: Right Shift"                  },
    {   0,      4,      16,             "Key: Up Arrow"                     },
    {   0,      4,      18,             "Key: Number Pad 1"                 },
    {   0,      4,      19,             "Key: Number Pad 2"                 },
    {   0,      4,      20,             "Key: Number Pad 3"                 },
    {   0,      4,      21,             "Key: Number Pad Enter"             },
    {   0,      5,      1,              "Key: Left Control"                 },
    {   0,      5,      2,              "Key: Left Windows"                 },
    {   0,      5,      3,              "Key: Left Alt"                     },
    {   0,      5,      7,              "Key: Space"                        },
    {   0,      5,      11,             "Key: Right Alt"                    },
    {   0,      5,      12,             "Key: Right Fn"                     },
    {   0,      5,      13,             "Key: Menu"                         },
    {   0,      5,      14,             "Key: Right Control"                },
    {   0,      5,      15,             "Key: Left Arrow"                   },
    {   0,      5,      16,             "Key: Down Arrow"                   },
    {   0,      5,      17,             "Key: Right Arrow"                  },
    {   0,      5,      19,             "Key: Number Pad 0"                 },
    {   0,      5,      20,             "Key: Number Pad ."                 },
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer BlackWidow Chroma Tournament Edition                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_chroma_te_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer Blackwidow X Chroma 1532:0216                          |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 22 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blackwidow_x_chroma_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    NULL,
    0
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
    "Keyboard",
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
    "Keyboard",
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
    NULL,
    0
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer DeathStalker Chroma                                    |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       12 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone deathstalker_chroma_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer Huntsman TE 1532:0243                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 18 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone huntsman_te_zone =
{
    "Keyboard",
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

/*-------------------------------------------------------------------------*\
|  LAPTOPS                                                                  |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Razer Blade (2016)                                           |
|                                                               |
|  Zone "Keyboard"                                              |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_2016_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer Blade 15 (2021 Base)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2021_base_zone =
{
    "Keyboard",
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
|  Razer Blade Pro (2016)                                       |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 25 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_2016_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|  Razer Blade Stealth (2016)                                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_2016_zone =
{
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
    "Keyboard",
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
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_late_2020_zone =
{
    "Keyboard",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_stealth_late_2020_device =
{
    "Razer Blade Stealth (Late 2020)",
    RAZER_BLADE_STEALTH_LATE_2020_PID,
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    1,
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
        &lancehead_te_right_zone,
        &lancehead_te_left_zone,
        &lancehead_te_logo_zone,
        &lancehead_te_scroll_wheel_zone,
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
|  Razer Naga Trinity 1532:0067                                  |
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
static const razer_zone naga_trinity_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone naga_trinity_scroll_wheel_zone =
{
    "Scroll Wheel",
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
        &naga_trinity_logo_zone,
        &naga_trinity_scroll_wheel_zone,
        &naga_trinity_numpad_zone,
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
    DEVICE_TYPE_KEYBOARD,
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
    DEVICE_TYPE_KEYBOARD,
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
| Razer Tartarus V2 1532:0208                                   |
|                                                               |
|  Zone "Keypad"                                                |
|       Matrix                                                  |
|       4 Rows, 5 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone tartarus_v2_zone =
{
    "Keypad",
    ZONE_TYPE_MATRIX,
    4,
    5
};

static const razer_device tartarus_v2_device =
{
    "Razer Tartarus V2",
    RAZER_TARTARUS_V2_PID,
    DEVICE_TYPE_KEYBOARD,
    true,
    4,
    5,
    {
        &tartarus_v2_zone,
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
|  Razer Firefly V2                                     |
|                                                               |
|  Zone "LED Strip"                                             |
|       Matrix                                                  |
|       19 LEDs                                                  |
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
    DEVICE_TYPE_UNKNOWN,
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
    DEVICE_TYPE_UNKNOWN,
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
    DEVICE_TYPE_UNKNOWN,
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

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
#define RAZER_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const razer_device* device_list[] =
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
    &blackwidow_x_chroma_device,
    &blackwidow_x_chroma_te_device,
    &cynosa_chroma_device,
    &cynosa_v2_device,
    &cynosa_lite_device,
    &deathstalker_chroma_device,
    &huntsman_device,
    &huntsman_elite_device,
    &huntsman_te_device,
    &ornata_chroma_device,
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
    &blade_15_2021_base_device,
    &blade_pro_2016_device,
    &blade_pro_2017_device,
    &blade_pro_2017_fullhd_device,
    &blade_pro_2019_device,
    &blade_pro_late_2019_device,
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
    &deathadder_chroma_device,
    &deathadder_elite_device,
    &deathadder_essential_device,
    &deathadder_essential_white_edition_device,
    &deathadder_v2_device,
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
    &naga_epic_chroma_device,
    &naga_hex_v2_device,
    &naga_trinity_device,
    &viper_mini_device,
    &viper_ultimate_wired_device,
    &viper_ultimate_wireless_device,
    &viper_device,
/*-----------------------------------------------------------------*\
|  KEYPADS                                                          |
\*-----------------------------------------------------------------*/
    &orbweaver_chroma_device,
    &tartarus_chroma_device,
    &tartarus_v2_device,
/*-----------------------------------------------------------------*\
|  MOUSEMATS                                                        |
\*-----------------------------------------------------------------*/
    &firefly_device,
    &firefly_hyperflux_device,
    &firefly_v2_device,
    &goliathus_device,
    &goliathus_extended_device,
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
    &charging_pad_chroma_device,
    &chromaargb_device,
    &chromahdk_device,
    &chroma_pc_case_lighting_kit_device,
    &core_device,
    &mug_holder_device,
    &nommo_chroma_device,
    &nommo_pro_device,
};
