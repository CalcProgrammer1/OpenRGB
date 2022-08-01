#define RAZER_MAX_ZONES 6

#include <string>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"

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
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      0,              "Key: M1"                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      0,              "Key: M2"                           },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      0,              "Key: M3"                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      0,              "Key: M4"                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      0,              "Key: M5"                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define BLACKWIDOW_CHROMA_TE_KEYMAP_SIZE (sizeof(blackwidow_chroma_te_keymap) / sizeof(blackwidow_chroma_te_keymap[0]))

static const razer_key blackwidow_chroma_te_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
};

#define BLADE_PRO_2017_KEYMAP_SIZE (sizeof(blade_pro_2017_keymap) / sizeof(blade_pro_2017_keymap[0]))

static const razer_key blade_pro_2017_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      2,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_INSERT                       },
    {   0,      0,      17,             KEY_EN_DELETE                       },
    {   0,      0,      19,             KEY_EN_MEDIA_PREVIOUS               },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT                   },
    {   0,      0,      21,             "Key: Media Volume"                 },
    {   0,      0,      23,             KEY_EN_MEDIA_PLAY_PAUSE             },
    {   0,      0,      24,             KEY_EN_MEDIA_MUTE                   },
    {   0,      1,      2,              KEY_EN_BACK_TICK                    },
    {   0,      1,      3,              KEY_EN_1                            },
    {   0,      1,      4,              KEY_EN_2                            },
    {   0,      1,      5,              KEY_EN_3                            },
    {   0,      1,      6,              KEY_EN_4                            },
    {   0,      1,      7,              KEY_EN_5                            },
    {   0,      1,      8,              KEY_EN_6                            },
    {   0,      1,      9,              KEY_EN_7                            },
    {   0,      1,      10,             KEY_EN_8                            },
    {   0,      1,      11,             KEY_EN_9                            },
    {   0,      1,      12,             KEY_EN_0                            },
    {   0,      1,      13,             KEY_EN_MINUS                        },
    {   0,      1,      14,             KEY_EN_EQUALS                       },
    {   0,      1,      15,             KEY_EN_BACKSPACE                    },
    {   0,      2,      2,              KEY_EN_TAB                          },
    {   0,      2,      4,              KEY_EN_Q                            },
    {   0,      2,      5,              KEY_EN_W                            },
    {   0,      2,      6,              KEY_EN_E                            },
    {   0,      2,      7,              KEY_EN_R                            },
    {   0,      2,      8,              KEY_EN_T                            },
    {   0,      2,      9,              KEY_EN_Y                            },
    {   0,      2,      10,             KEY_EN_U                            },
    {   0,      2,      11,             KEY_EN_I                            },
    {   0,      2,      12,             KEY_EN_O                            },
    {   0,      2,      13,             KEY_EN_P                            },
    {   0,      2,      14,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      15,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      17,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      4,              KEY_EN_A                            },
    {   0,      3,      5,              KEY_EN_S                            },
    {   0,      3,      6,              KEY_EN_D                            },
    {   0,      3,      7,              KEY_EN_F                            },
    {   0,      3,      8,              KEY_EN_G                            },
    {   0,      3,      9,              KEY_EN_H                            },
    {   0,      3,      10,             KEY_EN_J                            },
    {   0,      3,      11,             KEY_EN_K                            },
    {   0,      3,      12,             KEY_EN_L                            },
    {   0,      3,      13,             KEY_EN_SEMICOLON                    },
    {   0,      3,      14,             KEY_EN_QUOTE                        },
    {   0,      3,      18,             KEY_EN_ANSI_ENTER                   },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      4,              KEY_EN_Z                            },
    {   0,      4,      5,              KEY_EN_X                            },
    {   0,      4,      6,              KEY_EN_C                            },
    {   0,      4,      7,              KEY_EN_V                            },
    {   0,      4,      8,              KEY_EN_B                            },
    {   0,      4,      9,              KEY_EN_N                            },
    {   0,      4,      10,             KEY_EN_M                            },
    {   0,      4,      11,             KEY_EN_COMMA                        },
    {   0,      4,      12,             KEY_EN_PERIOD                       },
    {   0,      4,      13,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_FUNCTION                },
    {   0,      5,      3,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      5,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      13,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      14,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      15,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      16,             KEY_EN_RIGHT_FUNCTION               },
};

#define BLADE_STEALTH_KEYMAP_SIZE (sizeof(blade_stealth_keymap) / sizeof(blade_stealth_keymap[0]))

static const razer_key blade_stealth_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      2,              KEY_EN_F1                           },
    {   0,      0,      3,              KEY_EN_F2                           },
    {   0,      0,      4,              KEY_EN_F3                           },
    {   0,      0,      5,              KEY_EN_F4                           },
    {   0,      0,      6,              KEY_EN_F5                           },
    {   0,      0,      7,              KEY_EN_F6                           },
    {   0,      0,      8,              KEY_EN_F7                           },
    {   0,      0,      9,              KEY_EN_F8                           },
    {   0,      0,      10,             KEY_EN_F9                           },
    {   0,      0,      11,             KEY_EN_F10                          },
    {   0,      0,      12,             KEY_EN_F11                          },
    {   0,      0,      13,             KEY_EN_F12                          },
    {   0,      0,      14,             KEY_EN_INSERT                       },
    {   0,      0,      15,             KEY_EN_DELETE                       },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_BACKSPACE                    },
    {   0,      2,      0,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      3,      0,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      15,             KEY_EN_ANSI_ENTER                   },
    {   0,      4,      0,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_Z                            },
    {   0,      4,      3,              KEY_EN_X                            },
    {   0,      4,      4,              KEY_EN_C                            },
    {   0,      4,      5,              KEY_EN_V                            },
    {   0,      4,      6,              KEY_EN_B                            },
    {   0,      4,      7,              KEY_EN_N                            },
    {   0,      4,      8,              KEY_EN_M                            },
    {   0,      4,      9,              KEY_EN_COMMA                        },
    {   0,      4,      10,             KEY_EN_PERIOD                       },
    {   0,      4,      11,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      12,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      13,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      5,      0,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      1,              KEY_EN_LEFT_FUNCTION                },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      5,              KEY_EN_SPACE                        },
    {   0,      5,      6,              KEY_EN_SPACE                        },
    {   0,      5,      8,              KEY_EN_SPACE                        },
    {   0,      5,      9,              KEY_EN_RIGHT_ALT                    },
    {   0,      5,      10,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      11,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      12,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      13,             KEY_EN_UP_ARROW                     },
    {   0,      5,      14,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      15,             KEY_EN_DOWN_ARROW                   },
};

#define CYNOSA_CHROMA_KEYMAP_SIZE (sizeof(cynosa_chroma_keymap) / sizeof(cynosa_chroma_keymap[0]))

static const razer_key cynosa_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define HUNTSMAN_ELITE_KEYMAP_SIZE (sizeof(huntsman_elite_keymap) / sizeof(huntsman_elite_keymap[0]))

static const razer_key huntsman_elite_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS               },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE             },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT                   },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE                   },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define HUNTSMAN_KEYMAP_SIZE (sizeof(huntsman_keymap) / sizeof(huntsman_keymap[0]))

static const razer_key huntsman_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define HUNTSMAN_TE_KEYMAP_SIZE (sizeof(huntsman_te_keymap) / sizeof(huntsman_te_keymap[0]))

static const razer_key huntsman_te_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
};

#define BLACKWIDOW_ELITE_KEYMAP_SIZE (sizeof(blackwidow_elite_keymap) / sizeof(blackwidow_elite_keymap[0]))

static const razer_key blackwidow_elite_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      18,             KEY_EN_MEDIA_PREVIOUS               },
    {   0,      0,      19,             KEY_EN_MEDIA_PLAY_PAUSE             },
    {   0,      0,      20,             KEY_EN_MEDIA_NEXT                   },
    {   0,      0,      21,             KEY_EN_MEDIA_MUTE                   },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      6,              KEY_EN_SPACE                        },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      11,             "Logo"                              },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define BLACKWIDOW_2019_KEYMAP_SIZE (sizeof(blackwidow_2019_keymap) / sizeof(blackwidow_2019_keymap[0]))

static const razer_key blackwidow_2019_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      6,              KEY_EN_SPACE                        },
    {   0,      5,      10,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      11,             "Logo"                              },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             "Key: Number Pad ,"                 },
};

#define ORNATA_CHROMA_KEYMAP_SIZE (sizeof(ornata_chroma_keymap) / sizeof(ornata_chroma_keymap[0]))

static const razer_key ornata_chroma_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      7,              KEY_EN_SPACE                        },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      12,             KEY_EN_RIGHT_FUNCTION               },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
};

#define BLACKWIDOW_CHROMA_V2_KEYMAP_SIZE (sizeof(blackwidow_chroma_v2_keymap) / sizeof(blackwidow_chroma_v2_keymap[0]))

static const razer_key blackwidow_chroma_v2_keymap[] =
{
    /*---------------------------------------------------------------------*\
    |   Zone,   Row,    Column,         Key                                 |
    \*---------------------------------------------------------------------*/
    {   0,      0,      1,              KEY_EN_ESCAPE                       },
    {   0,      0,      3,              KEY_EN_F1                           },
    {   0,      0,      4,              KEY_EN_F2                           },
    {   0,      0,      5,              KEY_EN_F3                           },
    {   0,      0,      6,              KEY_EN_F4                           },
    {   0,      0,      7,              KEY_EN_F5                           },
    {   0,      0,      8,              KEY_EN_F6                           },
    {   0,      0,      9,              KEY_EN_F7                           },
    {   0,      0,      10,             KEY_EN_F8                           },
    {   0,      0,      11,             KEY_EN_F9                           },
    {   0,      0,      12,             KEY_EN_F10                          },
    {   0,      0,      13,             KEY_EN_F11                          },
    {   0,      0,      14,             KEY_EN_F12                          },
    {   0,      0,      15,             KEY_EN_PRINT_SCREEN                 },
    {   0,      0,      16,             KEY_EN_SCROLL_LOCK                  },
    {   0,      0,      17,             KEY_EN_PAUSE_BREAK                  },
    {   0,      0,      20,             "Logo"                              },
    {   0,      1,      0,              "Key: M1"                           },
    {   0,      1,      1,              KEY_EN_BACK_TICK                    },
    {   0,      1,      2,              KEY_EN_1                            },
    {   0,      1,      3,              KEY_EN_2                            },
    {   0,      1,      4,              KEY_EN_3                            },
    {   0,      1,      5,              KEY_EN_4                            },
    {   0,      1,      6,              KEY_EN_5                            },
    {   0,      1,      7,              KEY_EN_6                            },
    {   0,      1,      8,              KEY_EN_7                            },
    {   0,      1,      9,              KEY_EN_8                            },
    {   0,      1,      10,             KEY_EN_9                            },
    {   0,      1,      11,             KEY_EN_0                            },
    {   0,      1,      12,             KEY_EN_MINUS                        },
    {   0,      1,      13,             KEY_EN_EQUALS                       },
    {   0,      1,      14,             KEY_EN_BACKSPACE                    },
    {   0,      1,      15,             KEY_EN_INSERT                       },
    {   0,      1,      16,             KEY_EN_HOME                         },
    {   0,      1,      17,             KEY_EN_PAGE_UP                      },
    {   0,      1,      18,             KEY_EN_NUMPAD_LOCK                  },
    {   0,      1,      19,             KEY_EN_NUMPAD_DIVIDE                },
    {   0,      1,      20,             KEY_EN_NUMPAD_TIMES                 },
    {   0,      1,      21,             KEY_EN_NUMPAD_MINUS                 },
    {   0,      2,      0,              "Key: M2"                           },
    {   0,      2,      1,              KEY_EN_TAB                          },
    {   0,      2,      2,              KEY_EN_Q                            },
    {   0,      2,      3,              KEY_EN_W                            },
    {   0,      2,      4,              KEY_EN_E                            },
    {   0,      2,      5,              KEY_EN_R                            },
    {   0,      2,      6,              KEY_EN_T                            },
    {   0,      2,      7,              KEY_EN_Y                            },
    {   0,      2,      8,              KEY_EN_U                            },
    {   0,      2,      9,              KEY_EN_I                            },
    {   0,      2,      10,             KEY_EN_O                            },
    {   0,      2,      11,             KEY_EN_P                            },
    {   0,      2,      12,             KEY_EN_LEFT_BRACKET                 },
    {   0,      2,      13,             KEY_EN_RIGHT_BRACKET                },
    {   0,      2,      14,             KEY_EN_ANSI_BACK_SLASH              },
    {   0,      2,      15,             KEY_EN_DELETE                       },
    {   0,      2,      16,             KEY_EN_END                          },
    {   0,      2,      17,             KEY_EN_PAGE_DOWN                    },
    {   0,      2,      18,             KEY_EN_NUMPAD_7                     },
    {   0,      2,      19,             KEY_EN_NUMPAD_8                     },
    {   0,      2,      20,             KEY_EN_NUMPAD_9                     },
    {   0,      2,      21,             KEY_EN_NUMPAD_PLUS                  },
    {   0,      3,      0,              "Key: M3"                           },
    {   0,      3,      1,              KEY_EN_CAPS_LOCK                    },
    {   0,      3,      2,              KEY_EN_A                            },
    {   0,      3,      3,              KEY_EN_S                            },
    {   0,      3,      4,              KEY_EN_D                            },
    {   0,      3,      5,              KEY_EN_F                            },
    {   0,      3,      6,              KEY_EN_G                            },
    {   0,      3,      7,              KEY_EN_H                            },
    {   0,      3,      8,              KEY_EN_J                            },
    {   0,      3,      9,              KEY_EN_K                            },
    {   0,      3,      10,             KEY_EN_L                            },
    {   0,      3,      11,             KEY_EN_SEMICOLON                    },
    {   0,      3,      12,             KEY_EN_QUOTE                        },
    {   0,      3,      13,             KEY_EN_POUND                        },
    {   0,      3,      14,             KEY_EN_ANSI_ENTER                   },
    {   0,      3,      18,             KEY_EN_NUMPAD_4                     },
    {   0,      3,      19,             KEY_EN_NUMPAD_5                     },
    {   0,      3,      20,             KEY_EN_NUMPAD_6                     },
    {   0,      4,      0,              "Key: M4"                           },
    {   0,      4,      1,              KEY_EN_LEFT_SHIFT                   },
    {   0,      4,      2,              KEY_EN_ISO_BACK_SLASH               },
    {   0,      4,      3,              KEY_EN_Z                            },
    {   0,      4,      4,              KEY_EN_X                            },
    {   0,      4,      5,              KEY_EN_C                            },
    {   0,      4,      6,              KEY_EN_V                            },
    {   0,      4,      7,              KEY_EN_B                            },
    {   0,      4,      8,              KEY_EN_N                            },
    {   0,      4,      9,              KEY_EN_M                            },
    {   0,      4,      10,             KEY_EN_COMMA                        },
    {   0,      4,      11,             KEY_EN_PERIOD                       },
    {   0,      4,      12,             KEY_EN_FORWARD_SLASH                },
    {   0,      4,      14,             KEY_EN_RIGHT_SHIFT                  },
    {   0,      4,      16,             KEY_EN_UP_ARROW                     },
    {   0,      4,      18,             KEY_EN_NUMPAD_1                     },
    {   0,      4,      19,             KEY_EN_NUMPAD_2                     },
    {   0,      4,      20,             KEY_EN_NUMPAD_3                     },
    {   0,      4,      21,             KEY_EN_NUMPAD_ENTER                 },
    {   0,      5,      0,              "Key: M5"                           },
    {   0,      5,      1,              KEY_EN_LEFT_CONTROL                 },
    {   0,      5,      2,              KEY_EN_LEFT_WINDOWS                 },
    {   0,      5,      3,              KEY_EN_LEFT_ALT                     },
    {   0,      5,      11,             KEY_EN_RIGHT_ALT                    },
    {   0,      5,      13,             KEY_EN_MENU                         },
    {   0,      5,      14,             KEY_EN_RIGHT_CONTROL                },
    {   0,      5,      15,             KEY_EN_LEFT_ARROW                   },
    {   0,      5,      16,             KEY_EN_DOWN_ARROW                   },
    {   0,      5,      17,             KEY_EN_RIGHT_ARROW                  },
    {   0,      5,      19,             KEY_EN_NUMPAD_0                     },
    {   0,      5,      20,             KEY_EN_NUMPAD_PERIOD                },
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_chroma_te_device =
{
    "Razer BlackWidow Chroma Tournament Edition",
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device blackwidow_x_chroma_device =
{
    "Razer BlackWidow X Chroma",
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    22
};

static const razer_device cynosa_chroma_device =
{
    "Razer Cynosa Chroma",
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
    "Razer Cynosa V2",
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device cynosa_lite_device =
{
    "Razer Cynosa Lite",
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    12
};

static const razer_device deathstalker_chroma_device =
{
    "Razer DeathStalker Chroma",
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
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    18
};

static const razer_device huntsman_te_device =
{
    "Razer Huntsman Tournament Edition",
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
|  Razer Blade Stealth                                          |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_device =
{
    "Razer Blade Stealth",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    blade_stealth_keymap,
    BLADE_STEALTH_KEYMAP_SIZE
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
|  Razer Blade Stealth (Mid 2017)                               |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_mid_2017_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_stealth_mid_2017_device =
{
    "Razer Blade Stealth (Mid 2017)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_stealth_mid_2017_zone,
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
|  Razer Blade Stealth (Early 2020) 1532:0252                   |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_stealth_early_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_stealth_early_2020_device =
{
    "Razer Blade Stealth (Early 2020)",
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
|  Razer Blade (QHD)                                            |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_qhd_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_qhd_device =
{
    "Razer Blade (QHD)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_qhd_zone,
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
|  Razer Blade 15 (2018)                                        |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_2018_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_2018_device =
{
    "Razer Blade 15 (2018)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_2018_zone,
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
|  Razer Blade 15 (2018) Mercury                                |
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
    "Razer Blade 15 (2018) Mercury",
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
|  Razer Blade 15 (2018) Base Model                             |
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
    "Razer Blade 15 (2018) Base Model",
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
|  Razer Blade 15 (2019) Advanced                               |
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
    "Razer Blade 15 (2019) Advanced",
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
|  Razer Blade 15 (Mid 2019) Mercury                            |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_mid_2019_mercury_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_mid_2019_mercury_device =
{
    "Razer Blade 15 (Mid 2019) Mercury White",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_mid_2019_mercury_zone,
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
|  Razer Blade 15 (Mid 2019) Base Model                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_mid_2019_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device blade_15_mid_2019_base_device =
{
    "Razer Blade 15 (Mid 2019) Base Model",
    DEVICE_TYPE_KEYBOARD,
    false,
    1,
    1,
    {
        &blade_15_mid_2019_base_zone,
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
|  Razer Blade 15 (Early 2020) Base Model 1532:0255             |
|                                                               |
|  Zone "Keyboard"                                              |
|       Linear                                                  |
|       1 Row, 16 Columns                                       |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_early_2020_base_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_LINEAR,
    1,
    16
};

static const razer_device blade_15_early_2020_base_device =
{
    "Razer Blade 15 Base (Early 2020)",
    DEVICE_TYPE_KEYBOARD,
    true,
    1,
    16,
    {
        &blade_15_early_2020_base_zone,
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
|  Razer Blade 15 Studio Edition (2019)                         |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_studio_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_studio_2019_device =
{
    "Razer Blade 15 Studio Edition (2019)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_studio_2019_zone,
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
|  Razer Blade Pro (Late 2016)                                  |
|                                                               |
|  Zone "Keyboard"                                              |
|       Matrix                                                  |
|       6 Rows, 25 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_late_2016_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    25
};

static const razer_device blade_pro_late_2016_device =
{
    "Razer Blade Pro (Late 2016)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    25,
    {
        &blade_pro_late_2016_zone,
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
|  Razer Blade Pro FullHD (2017)                                |
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
    "Razer Blade Pro FullHD (2017)",
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
|  Razer Blade Pro 17 (2019)                                    |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_pro_17_2019_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_pro_17_2019_device =
{
    "Razer Blade Pro 17 (2019)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_pro_17_2019_zone,
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
|  Razer Blade Advanced (2020) 1532:0253                        |
|                                                               |
|   Zone "Keyboard"                                             |
|       Matrix                                                  |
|       6 Rows, 16 Columns                                      |
\*-------------------------------------------------------------*/
static const razer_zone blade_15_advanced_2020_zone =
{
    ZONE_EN_KEYBOARD,
    ZONE_TYPE_MATRIX,
    6,
    16
};

static const razer_device blade_15_advanced_2020_device =
{
    "Razer Blade 15 Advanced (2020)",
    DEVICE_TYPE_KEYBOARD,
    true,
    6,
    16,
    {
        &blade_15_advanced_2020_zone,
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
    DEVICE_TYPE_MOUSE,
    false,
    1,
    2,
    {
        &deathadder_v2_logo_zone,
        &deathadder_v2_scroll_wheel_zone,
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
|  Razer Lancehead V2 (Wired) 1532:0070                         |
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
static const razer_zone lancehead_v2_wired_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_v2_wired_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_v2_wired_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_v2_wired_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_v2_wired_device =
{
    "Razer Lancehead Wireless (Wired)",
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_v2_wired_right_zone,
        &lancehead_v2_wired_left_zone,
        &lancehead_v2_wired_logo_zone,
        &lancehead_v2_wired_scroll_wheel_zone,
        NULL,
        NULL
    },
    NULL,
    0
};

/*-------------------------------------------------------------*\
|  Razer Lancehead V2 (Wireless) 1532:006F                      |
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
static const razer_zone lancehead_v2_wireless_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_v2_wireless_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    1,
    7
};

static const razer_zone lancehead_v2_wireless_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone lancehead_v2_wireless_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device lancehead_v2_wireless_device =
{
    "Razer Lancehead Wireless (Receiver)",
    DEVICE_TYPE_MOUSE,
    true,
    1,
    16,
    {
        &lancehead_v2_wireless_right_zone,
        &lancehead_v2_wireless_left_zone,
        &lancehead_v2_wireless_logo_zone,
        &lancehead_v2_wireless_scroll_wheel_zone,
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
|  Razer Mamba (Wired)                                          |
|                                                               |
|  Zone "Chroma Zone"                                           |
|       Single                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone mamba_wired_zone =
{
    "Chroma Zone",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device mamba_wired_device =
{
    "Razer Mamba (Wired)",
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_wired_zone,
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
|  Razer Mamba (Wireless)                                       |
|                                                               |
|  Zone "Chroma Zone"                                           |
|       Single                                                  |
|       15 LED                                                  |
\*-------------------------------------------------------------*/
static const razer_zone mamba_wireless_zone =
{
    "Chroma Zone",
    ZONE_TYPE_LINEAR,
    1,
    15
};

static const razer_device mamba_wireless_device =
{
    "Razer Mamba (Wireless)",
    DEVICE_TYPE_MOUSE,
    false,
    1,
    15,
    {
        &mamba_wireless_zone,
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
|  Razer Mamba Wireless (2018) Wired 1532:0073                  |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mamba_wireless_2018_wired_logo_zone =
{
    "Logo Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_wireless_2018_wired_scroll_wheel_zone =
{
    "Scroll Wheel Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_wireless_2018_wired_device =
{
    "Razer Mamba Wireless (Wired)",
    DEVICE_TYPE_MOUSE,
    true,
    1,
    2,
    {
        &mamba_wireless_2018_wired_scroll_wheel_zone,
        &mamba_wireless_2018_wired_logo_zone,
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

static const razer_zone mamba_wireless_2018_wireless_logo_zone =
{
    "Logo Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_zone mamba_wireless_2018_wireless_scroll_wheel_zone =
{
    "Scroll Wheel Zone",
    ZONE_TYPE_SINGLE,
    1,
    1
};

static const razer_device mamba_wireless_2018_wireless_device =
{
    "Razer Mamba Wireless (Receiver)",
    DEVICE_TYPE_MOUSE,
    true,
    1,
    2,
    {
        &mamba_wireless_2018_wireless_scroll_wheel_zone,
        &mamba_wireless_2018_wireless_logo_zone,
        NULL,
        NULL,
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
    &blade_stealth_device,
    &blade_stealth_late_2016_device,
    &blade_stealth_mid_2017_device,
    &blade_stealth_late_2017_device,
    &blade_stealth_2019_device,
    &blade_stealth_late_2019_device,
    &blade_stealth_early_2020_device,
    &blade_late_2016_device,
    &blade_qhd_device,
    &blade_15_2018_device,
    &blade_15_2018_mercury_device,
    &blade_15_2018_base_device,
    &blade_15_2019_advanced_device,
    &blade_15_mid_2019_mercury_device,
    &blade_15_mid_2019_base_device,
    &blade_15_early_2020_base_device,
    &blade_15_studio_2019_device,
    &blade_pro_late_2016_device,
    &blade_pro_2017_device,
    &blade_pro_2017_fullhd_device,
    &blade_pro_17_2019_device,
    &blade_pro_late_2019_device,
    &blade_15_advanced_2020_device,
/*-----------------------------------------------------------------*\
|  MICE                                                             |
\*-----------------------------------------------------------------*/
    &abyssus_elite_dva_edition_device,
    &abyssus_essential_device,
    &basilisk_device,
    &deathadder_chroma_device,
    &deathadder_elite_device,
    &deathadder_essential_device,
    &deathadder_essential_white_edition_device,
    &deathadder_v2_device,
    &diamondback_chroma_device,
    &lancehead_te_device,
    &lancehead_v2_wired_device,
    &lancehead_v2_wireless_device,
    &mamba_2012_wired_device,
    &mamba_2012_wireless_device,
    &mamba_wired_device,
    &mamba_wireless_device,
    &mamba_te_device,
    &mamba_elite_device,
    &mamba_wireless_2018_wired_device,
    &mamba_wireless_2018_wireless_device,
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
    &core_device,
    &mug_holder_device,
    &chromahdk_device,
    &base_station_device,
    &nommo_pro_device,
    &nommo_chroma_device
};
