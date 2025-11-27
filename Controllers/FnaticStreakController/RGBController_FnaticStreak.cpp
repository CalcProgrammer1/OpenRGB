/*---------------------------------------------------------*\
| RGBController_FnaticStreak.cpp                            |
|                                                           |
|   RGBController for Fnatic Streak and miniStreak keyboard |
|                                                           |
|   Based on leddy project by Hanna Czenczek                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_FnaticStreak.h"
#include "RGBControllerKeyNames.h"

using namespace std::chrono_literals;

/*---------------------------------------------------------------------*\
| 0xFFFFFFFF indicates an unused entry in matrix                        |
\*---------------------------------------------------------------------*/
#define NA  0xFFFFFFFF

/*---------------------------------------------------------------------*\
| Fnatic Streak Full Size Matrix Map (6 rows x 22 columns)              |
| Based on leddy project keyboard.rs ledmap                             |
| Values are LED indices that map to the physical LED positions         |
\*---------------------------------------------------------------------*/
static unsigned int matrix_map_full[6][22] =
{
    /*        0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17    18    19    20    21  */
    /*       ESC  FNLK   F1    F2    F3    F4    F5    F6    F7    F8   SIG    F9   F10   F11   F12  PRSC  SCLK  PAUS  MMIC  GAME  MSPK   VOL  */
    {          1,    0,    7,   13,   19,   25,   31,   37,   43,   49,  120,   55,   67,   73,   79,   90,   93,   98,   91,   97,   92,  118 },
    /*       BKT     1     2     3     4     5     6     7     8     9     0     -     =    NA   BSP   INS  HOME  PGUP  NLCK   NP/   NP*   NP- */
    {          2,    8,   14,   20,   26,   32,   38,   44,   50,   56,   61,   62,   68,   NA,   80,   89,   94,   99,  100,  108,  109,  116 },
    /*       TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]    NA    BS   DEL   END  PGDN   NP7   NP8   NP9   NP+ */
    {          3,    9,   15,   21,   27,   33,   39,   45,   51,   57,   63,   69,   75,   NA,   81,   88,   95,   96,  101,  107,  110,  115 },
    /*      CAPS    NA     A     S     D     F     G     H     J     K     L     ;     '     #   ENT    NA    NA    NA   NP4   NP5   NP6    NA */
    {          4,   NA,   10,   16,   22,   28,   34,   40,   46,   52,   58,   64,   70,   76,   82,   NA,   NA,   NA,  102,  106,  111,   NA },
    /*      LSFT  ISO\     Z     X     C     V     B     N     M     ,     .     /    NA  RSFT    NA    NA    UP    NA   NP1   NP2   NP3  NPEN */
    {          5,   11,   17,   23,   29,   35,   41,   47,   53,   59,   65,   66,   NA,   77,   NA,   NA,   87,   NA,  103,  105,  112,  114 },
    /*      LCTL  LWIN  LALT    NA    NA    NA   SPC    NA    NA    NA  RALT    FN    NA  MENU  RCTL LEFT  DOWN  RGHT   NP0    NA   NP.    NA */
    {          6,   12,   18,   NA,   NA,   NA,   36,   NA,   NA,   NA,   60,   72,   NA,   78,   83,   84,   85,   86,  104,   NA,  113,   NA }
};

/*---------------------------------------------------------------------*\
| Fnatic miniStreak TKL Matrix Map (6 rows x 18 columns)                |
| Based on leddy project keyboard.rs ledmap                             |
\*---------------------------------------------------------------------*/
static unsigned int matrix_map_mini[6][18] =
{
    /*        0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17  */
    /*       ESC  FNLK   F1    F2    F3    F4    F5    F6    F7    F8   SIG    F9   F10   F11   F12  PRSC  SCLK  PAUS */
    {          1,    0,    7,   13,   19,   25,   31,   37,   43,   49,  103,   55,   67,   73,   79,   90,   93,   98 },
    /*       BKT     1     2     3     4     5     6     7     8     9     0     -     =    NA   BSP   INS  HOME  PGUP */
    {          2,    8,   14,   20,   26,   32,   38,   44,   50,   56,   61,   62,   68,   NA,   80,   89,   94,   99 },
    /*       TAB     Q     W     E     R     T     Y     U     I     O     P     [     ]    NA    BS   DEL   END  PGDN */
    {          3,    9,   15,   21,   27,   33,   39,   45,   51,   57,   63,   69,   75,   NA,   81,   88,   95,   96 },
    /*      CAPS    NA     A     S     D     F     G     H     J     K     L     ;     '     #   ENT    NA    NA    NA */
    {          4,   NA,   10,   16,   22,   28,   34,   40,   46,   52,   58,   64,   70,   76,   82,   NA,   NA,   NA },
    /*      LSFT  ISO\     Z     X     C     V     B     N     M     ,     .     /    NA  RSFT    NA    NA    UP    NA */
    {          5,   11,   17,   23,   29,   35,   41,   47,   53,   59,   65,   66,   NA,   77,   NA,   NA,   87,   NA },
    /*      LCTL  LWIN  LALT    NA    NA    NA   SPC    NA    NA    NA  RALT    FN    NA  MENU  RCTL LEFT  DOWN  RGHT */
    {          6,   12,   18,   NA,   NA,   NA,   36,   NA,   NA,   NA,   60,   72,   NA,   78,   83,   84,   85,   86 }
};

/*---------------------------------------------------------------------*\
| LED Names - Full keyboard (124 LEDs, indices 0-123)                   |
| Names are in LED index order based on leddy keyboard.rs constants     |
\*---------------------------------------------------------------------*/
static const char* led_names_full[] =
{
    /* 0   */ "Key: Fn Lock",
    /* 1   */ KEY_EN_ESCAPE,
    /* 2   */ KEY_EN_BACK_TICK,
    /* 3   */ KEY_EN_TAB,
    /* 4   */ KEY_EN_CAPS_LOCK,
    /* 5   */ KEY_EN_LEFT_SHIFT,
    /* 6   */ KEY_EN_LEFT_CONTROL,
    /* 7   */ KEY_EN_F1,
    /* 8   */ KEY_EN_1,
    /* 9   */ KEY_EN_Q,
    /* 10  */ KEY_EN_A,
    /* 11  */ KEY_EN_ISO_BACK_SLASH,
    /* 12  */ KEY_EN_LEFT_WINDOWS,
    /* 13  */ KEY_EN_F2,
    /* 14  */ KEY_EN_2,
    /* 15  */ KEY_EN_W,
    /* 16  */ KEY_EN_S,
    /* 17  */ KEY_EN_Z,
    /* 18  */ KEY_EN_LEFT_ALT,
    /* 19  */ KEY_EN_F3,
    /* 20  */ KEY_EN_3,
    /* 21  */ KEY_EN_E,
    /* 22  */ KEY_EN_D,
    /* 23  */ KEY_EN_X,
    /* 24  */ KEY_EN_UNUSED,
    /* 25  */ KEY_EN_F4,
    /* 26  */ KEY_EN_4,
    /* 27  */ KEY_EN_R,
    /* 28  */ KEY_EN_F,
    /* 29  */ KEY_EN_C,
    /* 30  */ KEY_EN_UNUSED,
    /* 31  */ KEY_EN_F5,
    /* 32  */ KEY_EN_5,
    /* 33  */ KEY_EN_T,
    /* 34  */ KEY_EN_G,
    /* 35  */ KEY_EN_V,
    /* 36  */ KEY_EN_SPACE,
    /* 37  */ KEY_EN_F6,
    /* 38  */ KEY_EN_6,
    /* 39  */ KEY_EN_Y,
    /* 40  */ KEY_EN_H,
    /* 41  */ KEY_EN_B,
    /* 42  */ KEY_EN_UNUSED,
    /* 43  */ KEY_EN_F7,
    /* 44  */ KEY_EN_7,
    /* 45  */ KEY_EN_U,
    /* 46  */ KEY_EN_J,
    /* 47  */ KEY_EN_N,
    /* 48  */ KEY_EN_UNUSED,
    /* 49  */ KEY_EN_F8,
    /* 50  */ KEY_EN_8,
    /* 51  */ KEY_EN_I,
    /* 52  */ KEY_EN_K,
    /* 53  */ KEY_EN_M,
    /* 54  */ KEY_EN_UNUSED,
    /* 55  */ KEY_EN_F9,
    /* 56  */ KEY_EN_9,
    /* 57  */ KEY_EN_O,
    /* 58  */ KEY_EN_L,
    /* 59  */ KEY_EN_COMMA,
    /* 60  */ KEY_EN_RIGHT_ALT,
    /* 61  */ KEY_EN_0,
    /* 62  */ KEY_EN_MINUS,
    /* 63  */ KEY_EN_P,
    /* 64  */ KEY_EN_SEMICOLON,
    /* 65  */ KEY_EN_PERIOD,
    /* 66  */ KEY_EN_FORWARD_SLASH,
    /* 67  */ KEY_EN_F10,
    /* 68  */ KEY_EN_EQUALS,
    /* 69  */ KEY_EN_LEFT_BRACKET,
    /* 70  */ KEY_EN_QUOTE,
    /* 71  */ KEY_EN_UNUSED,
    /* 72  */ KEY_EN_RIGHT_FUNCTION,
    /* 73  */ KEY_EN_F11,
    /* 74  */ KEY_EN_UNUSED,
    /* 75  */ KEY_EN_RIGHT_BRACKET,
    /* 76  */ KEY_EN_POUND,
    /* 77  */ KEY_EN_RIGHT_SHIFT,
    /* 78  */ KEY_EN_MENU,
    /* 79  */ KEY_EN_F12,
    /* 80  */ KEY_EN_BACKSPACE,
    /* 81  */ KEY_EN_ANSI_BACK_SLASH,
    /* 82  */ KEY_EN_ANSI_ENTER,
    /* 83  */ KEY_EN_RIGHT_CONTROL,
    /* 84  */ KEY_EN_LEFT_ARROW,
    /* 85  */ KEY_EN_DOWN_ARROW,
    /* 86  */ KEY_EN_RIGHT_ARROW,
    /* 87  */ KEY_EN_UP_ARROW,
    /* 88  */ KEY_EN_DELETE,
    /* 89  */ KEY_EN_INSERT,
    /* 90  */ KEY_EN_PRINT_SCREEN,
    /* 91  */ "Key: Mute Mic",
    /* 92  */ "Key: Mute Speaker",
    /* 93  */ KEY_EN_SCROLL_LOCK,
    /* 94  */ KEY_EN_HOME,
    /* 95  */ KEY_EN_END,
    /* 96  */ KEY_EN_PAGE_DOWN,
    /* 97  */ "Key: Gaming Mode",
    /* 98  */ KEY_EN_PAUSE_BREAK,
    /* 99  */ KEY_EN_PAGE_UP,
    /* 100 */ KEY_EN_NUMPAD_LOCK,
    /* 101 */ KEY_EN_NUMPAD_7,
    /* 102 */ KEY_EN_NUMPAD_4,
    /* 103 */ KEY_EN_NUMPAD_1,
    /* 104 */ KEY_EN_NUMPAD_0,
    /* 105 */ KEY_EN_NUMPAD_2,
    /* 106 */ KEY_EN_NUMPAD_5,
    /* 107 */ KEY_EN_NUMPAD_8,
    /* 108 */ KEY_EN_NUMPAD_DIVIDE,
    /* 109 */ KEY_EN_NUMPAD_TIMES,
    /* 110 */ KEY_EN_NUMPAD_9,
    /* 111 */ KEY_EN_NUMPAD_6,
    /* 112 */ KEY_EN_NUMPAD_3,
    /* 113 */ KEY_EN_NUMPAD_PERIOD,
    /* 114 */ KEY_EN_NUMPAD_ENTER,
    /* 115 */ KEY_EN_NUMPAD_PLUS,
    /* 116 */ KEY_EN_NUMPAD_MINUS,
    /* 117 */ KEY_EN_UNUSED,
    /* 118 */ "Key: Volume Knob",
    /* 119 */ KEY_EN_UNUSED,
    /* 120 */ "Key: Signature Plate",
    /* 121 */ KEY_EN_UNUSED,
    /* 122 */ KEY_EN_UNUSED,
    /* 123 */ KEY_EN_UNUSED,
};

/*---------------------------------------------------------------------*\
| LED Names - Mini keyboard (106 LEDs, indices 0-105)                   |
| Same as full but with different signature plate location              |
\*---------------------------------------------------------------------*/
static const char* led_names_mini[] =
{
    /* 0   */ "Key: Fn Lock",
    /* 1   */ KEY_EN_ESCAPE,
    /* 2   */ KEY_EN_BACK_TICK,
    /* 3   */ KEY_EN_TAB,
    /* 4   */ KEY_EN_CAPS_LOCK,
    /* 5   */ KEY_EN_LEFT_SHIFT,
    /* 6   */ KEY_EN_LEFT_CONTROL,
    /* 7   */ KEY_EN_F1,
    /* 8   */ KEY_EN_1,
    /* 9   */ KEY_EN_Q,
    /* 10  */ KEY_EN_A,
    /* 11  */ KEY_EN_ISO_BACK_SLASH,
    /* 12  */ KEY_EN_LEFT_WINDOWS,
    /* 13  */ KEY_EN_F2,
    /* 14  */ KEY_EN_2,
    /* 15  */ KEY_EN_W,
    /* 16  */ KEY_EN_S,
    /* 17  */ KEY_EN_Z,
    /* 18  */ KEY_EN_LEFT_ALT,
    /* 19  */ KEY_EN_F3,
    /* 20  */ KEY_EN_3,
    /* 21  */ KEY_EN_E,
    /* 22  */ KEY_EN_D,
    /* 23  */ KEY_EN_X,
    /* 24  */ KEY_EN_UNUSED,
    /* 25  */ KEY_EN_F4,
    /* 26  */ KEY_EN_4,
    /* 27  */ KEY_EN_R,
    /* 28  */ KEY_EN_F,
    /* 29  */ KEY_EN_C,
    /* 30  */ KEY_EN_UNUSED,
    /* 31  */ KEY_EN_F5,
    /* 32  */ KEY_EN_5,
    /* 33  */ KEY_EN_T,
    /* 34  */ KEY_EN_G,
    /* 35  */ KEY_EN_V,
    /* 36  */ KEY_EN_SPACE,
    /* 37  */ KEY_EN_F6,
    /* 38  */ KEY_EN_6,
    /* 39  */ KEY_EN_Y,
    /* 40  */ KEY_EN_H,
    /* 41  */ KEY_EN_B,
    /* 42  */ KEY_EN_UNUSED,
    /* 43  */ KEY_EN_F7,
    /* 44  */ KEY_EN_7,
    /* 45  */ KEY_EN_U,
    /* 46  */ KEY_EN_J,
    /* 47  */ KEY_EN_N,
    /* 48  */ KEY_EN_UNUSED,
    /* 49  */ KEY_EN_F8,
    /* 50  */ KEY_EN_8,
    /* 51  */ KEY_EN_I,
    /* 52  */ KEY_EN_K,
    /* 53  */ KEY_EN_M,
    /* 54  */ KEY_EN_UNUSED,
    /* 55  */ KEY_EN_F9,
    /* 56  */ KEY_EN_9,
    /* 57  */ KEY_EN_O,
    /* 58  */ KEY_EN_L,
    /* 59  */ KEY_EN_COMMA,
    /* 60  */ KEY_EN_RIGHT_ALT,
    /* 61  */ KEY_EN_0,
    /* 62  */ KEY_EN_MINUS,
    /* 63  */ KEY_EN_P,
    /* 64  */ KEY_EN_SEMICOLON,
    /* 65  */ KEY_EN_PERIOD,
    /* 66  */ KEY_EN_FORWARD_SLASH,
    /* 67  */ KEY_EN_F10,
    /* 68  */ KEY_EN_EQUALS,
    /* 69  */ KEY_EN_LEFT_BRACKET,
    /* 70  */ KEY_EN_QUOTE,
    /* 71  */ KEY_EN_UNUSED,
    /* 72  */ KEY_EN_RIGHT_FUNCTION,
    /* 73  */ KEY_EN_F11,
    /* 74  */ KEY_EN_UNUSED,
    /* 75  */ KEY_EN_RIGHT_BRACKET,
    /* 76  */ KEY_EN_POUND,
    /* 77  */ KEY_EN_RIGHT_SHIFT,
    /* 78  */ KEY_EN_MENU,
    /* 79  */ KEY_EN_F12,
    /* 80  */ KEY_EN_BACKSPACE,
    /* 81  */ KEY_EN_ANSI_BACK_SLASH,
    /* 82  */ KEY_EN_ANSI_ENTER,
    /* 83  */ KEY_EN_RIGHT_CONTROL,
    /* 84  */ KEY_EN_LEFT_ARROW,
    /* 85  */ KEY_EN_DOWN_ARROW,
    /* 86  */ KEY_EN_RIGHT_ARROW,
    /* 87  */ KEY_EN_UP_ARROW,
    /* 88  */ KEY_EN_DELETE,
    /* 89  */ KEY_EN_INSERT,
    /* 90  */ KEY_EN_PRINT_SCREEN,
    /* 91  */ KEY_EN_UNUSED,
    /* 92  */ KEY_EN_UNUSED,
    /* 93  */ KEY_EN_SCROLL_LOCK,
    /* 94  */ KEY_EN_HOME,
    /* 95  */ KEY_EN_END,
    /* 96  */ KEY_EN_PAGE_DOWN,
    /* 97  */ KEY_EN_UNUSED,
    /* 98  */ KEY_EN_PAUSE_BREAK,
    /* 99  */ KEY_EN_PAGE_UP,
    /* 100 */ KEY_EN_UNUSED,
    /* 101 */ KEY_EN_UNUSED,
    /* 102 */ KEY_EN_UNUSED,
    /* 103 */ "Key: Signature Plate",
    /* 104 */ KEY_EN_UNUSED,
    /* 105 */ KEY_EN_UNUSED,
};

/**------------------------------------------------------------------*\
    @name Fnatic Streak
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectFnaticStreakKeyboard
    @comment The Fnatic Streak and miniStreak are gaming keyboards
    with per-key RGB lighting. Supports hardware effects like Wave,
    Pulse, Reactive, Rain, Gradient, and Fade.
\*-------------------------------------------------------------------*/

RGBController_FnaticStreak::RGBController_FnaticStreak(FnaticStreakController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "Fnatic";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "Fnatic Streak Keyboard";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();

    /*-----------------------------------------------------*\
    | Direct mode - per-key color control                   |
    \*-----------------------------------------------------*/
    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFFFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0;
    Direct.brightness_max   = 100;
    Direct.brightness       = 100;
    modes.push_back(Direct);

    /*-----------------------------------------------------*\
    | Pulse mode - breathing/pulsing effect                 |
    \*-----------------------------------------------------*/
    mode Pulse;
    Pulse.name              = "Pulse";
    Pulse.value             = FNATIC_STREAK_CMD_PULSE;
    Pulse.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Pulse.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Pulse.speed_min         = 0;
    Pulse.speed_max         = 100;
    Pulse.speed             = 50;
    Pulse.brightness_min    = 0;
    Pulse.brightness_max    = 100;
    Pulse.brightness        = 100;
    Pulse.colors_min        = 1;
    Pulse.colors_max        = 1;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    /*-----------------------------------------------------*\
    | Wave mode - wave rolling across keyboard              |
    \*-----------------------------------------------------*/
    mode Wave;
    Wave.name               = "Wave";
    Wave.value              = FNATIC_STREAK_CMD_WAVE;
    Wave.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min          = 0;
    Wave.speed_max          = 100;
    Wave.speed              = 50;
    Wave.brightness_min     = 0;
    Wave.brightness_max     = 100;
    Wave.brightness         = 100;
    Wave.direction          = MODE_DIRECTION_RIGHT;
    Wave.colors_min         = 1;
    Wave.colors_max         = 1;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    /*-----------------------------------------------------*\
    | Reactive mode - LED lights on keypress                |
    \*-----------------------------------------------------*/
    mode Reactive;
    Reactive.name           = "Reactive";
    Reactive.value          = FNATIC_STREAK_CMD_REACTIVE;
    Reactive.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Reactive.speed_min      = 0;
    Reactive.speed_max      = 100;
    Reactive.speed          = 50;
    Reactive.brightness_min = 0;
    Reactive.brightness_max = 100;
    Reactive.brightness     = 100;
    Reactive.colors_min     = 1;
    Reactive.colors_max     = 1;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    /*-----------------------------------------------------*\
    | Reactive Ripple mode - ripple effect on keypress      |
    \*-----------------------------------------------------*/
    mode ReactiveRipple;
    ReactiveRipple.name     = "Reactive Ripple";
    ReactiveRipple.value    = FNATIC_STREAK_CMD_REACTIVE_RIPPLE;
    ReactiveRipple.flags    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactiveRipple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    ReactiveRipple.speed_min = 0;
    ReactiveRipple.speed_max = 100;
    ReactiveRipple.speed    = 50;
    ReactiveRipple.brightness_min = 0;
    ReactiveRipple.brightness_max = 100;
    ReactiveRipple.brightness     = 100;
    ReactiveRipple.colors_min = 1;
    ReactiveRipple.colors_max = 1;
    ReactiveRipple.colors.resize(1);
    modes.push_back(ReactiveRipple);

    /*-----------------------------------------------------*\
    | Rain mode - raindrop effect                           |
    \*-----------------------------------------------------*/
    mode Rain;
    Rain.name               = "Rain";
    Rain.value              = FNATIC_STREAK_CMD_RAIN;
    Rain.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_BRIGHTNESS;
    Rain.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Rain.speed_min          = 0;
    Rain.speed_max          = 100;
    Rain.speed              = 50;
    Rain.brightness_min     = 0;
    Rain.brightness_max     = 100;
    Rain.brightness         = 100;
    Rain.direction          = MODE_DIRECTION_DOWN;
    Rain.colors_min         = 1;
    Rain.colors_max         = 1;
    Rain.colors.resize(1);
    modes.push_back(Rain);

    /*-----------------------------------------------------*\
    | Spectrum Cycle (Fade) mode - fade through colors      |
    \*-----------------------------------------------------*/
    mode SpectrumCycle;
    SpectrumCycle.name      = "Spectrum Cycle";
    SpectrumCycle.value     = FNATIC_STREAK_CMD_FADE;
    SpectrumCycle.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    SpectrumCycle.color_mode = MODE_COLORS_RANDOM;
    SpectrumCycle.speed_min = 0;
    SpectrumCycle.speed_max = 100;
    SpectrumCycle.speed     = 50;
    SpectrumCycle.brightness_min = 0;
    SpectrumCycle.brightness_max = 100;
    SpectrumCycle.brightness     = 100;
    modes.push_back(SpectrumCycle);

    /*-----------------------------------------------------*\
    | Rainbow Gradient mode - static rainbow gradient       |
    \*-----------------------------------------------------*/
    mode RainbowGradient;
    RainbowGradient.name    = "Rainbow Gradient";
    RainbowGradient.value   = FNATIC_STREAK_CMD_GRADIENT;
    RainbowGradient.flags   = MODE_FLAG_HAS_BRIGHTNESS;
    RainbowGradient.color_mode = MODE_COLORS_NONE;
    RainbowGradient.brightness_min = 0;
    RainbowGradient.brightness_max = 100;
    RainbowGradient.brightness     = 100;
    modes.push_back(RainbowGradient);

    SetupZones();

    /*-----------------------------------------------------*\
    | Initialize last_update_time to now                    |
    \*-----------------------------------------------------*/
    last_update_time = std::chrono::steady_clock::now();

    /*-----------------------------------------------------*\
    | The Fnatic Streak requires periodic packets to        |
    | maintain direct control. Start a keepalive thread.    |
    \*-----------------------------------------------------*/
    keepalive_thread_run = true;
    keepalive_thread = new std::thread(&RGBController_FnaticStreak::KeepaliveThread, this);
}

RGBController_FnaticStreak::~RGBController_FnaticStreak()
{
    keepalive_thread_run = false;
    keepalive_thread->join();
    delete keepalive_thread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_FnaticStreak::SetupZones()
{
    bool            is_mini         = (controller->GetKeyboardType() == FNATIC_STREAK_TYPE_MINI);
    unsigned int    total_led_count = is_mini ? 106 : 124;
    unsigned int    matrix_cols     = is_mini ? 18 : 22;
    unsigned int*   matrix_map_ptr  = is_mini ? (unsigned int*)matrix_map_mini : (unsigned int*)matrix_map_full;
    const char**    led_name_ptr    = is_mini ? led_names_mini : led_names_full;

    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name               = ZONE_EN_KEYBOARD;
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = total_led_count;
    new_zone.leds_max           = total_led_count;
    new_zone.leds_count         = total_led_count;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = matrix_cols;
    new_zone.matrix_map->map    = matrix_map_ptr;
    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name  = led_name_ptr[led_idx];
        new_led.value = led_idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_FnaticStreak::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_FnaticStreak::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SoftwareEffectStart();

    /*-----------------------------------------------------*\
    | Apply brightness scaling to colors                    |
    \*-----------------------------------------------------*/
    unsigned int brightness = modes[active_mode].brightness;
    controller->SetLEDsDirect(leds, colors, brightness);
    controller->SendRGBToDevice();
}

void RGBController_FnaticStreak::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_FnaticStreak::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_FnaticStreak::DeviceUpdateMode()
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char color_mode = FNATIC_STREAK_COLOR_MODE_RAINBOW;
    unsigned char speed = (unsigned char)modes[active_mode].speed;
    unsigned char direction = FNATIC_STREAK_DIRECTION_RIGHT;
    unsigned int  brightness = modes[active_mode].brightness;

    /*-----------------------------------------------------*\
    | Get color from mode if available and apply brightness |
    \*-----------------------------------------------------*/
    if(modes[active_mode].colors.size() > 0)
    {
        r = (unsigned char)(RGBGetRValue(modes[active_mode].colors[0]) * brightness / 100);
        g = (unsigned char)(RGBGetGValue(modes[active_mode].colors[0]) * brightness / 100);
        b = (unsigned char)(RGBGetBValue(modes[active_mode].colors[0]) * brightness / 100);
        color_mode = FNATIC_STREAK_COLOR_MODE_SINGLE;
    }

    /*-----------------------------------------------------*\
    | Check for random color mode                           |
    \*-----------------------------------------------------*/
    if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
    {
        color_mode = FNATIC_STREAK_COLOR_MODE_RAINBOW;
    }

    /*-----------------------------------------------------*\
    | Convert OpenRGB direction to Fnatic direction         |
    \*-----------------------------------------------------*/
    switch(modes[active_mode].direction)
    {
        case MODE_DIRECTION_RIGHT:
            direction = FNATIC_STREAK_DIRECTION_RIGHT;
            break;
        case MODE_DIRECTION_LEFT:
            direction = FNATIC_STREAK_DIRECTION_LEFT;
            break;
        case MODE_DIRECTION_DOWN:
            direction = FNATIC_STREAK_DIRECTION_DOWN;
            break;
        case MODE_DIRECTION_UP:
            direction = FNATIC_STREAK_DIRECTION_UP;
            break;
        default:
            direction = FNATIC_STREAK_DIRECTION_RIGHT;
            break;
    }

    /*-----------------------------------------------------*\
    | Send hardware effect command based on mode            |
    \*-----------------------------------------------------*/
    switch(modes[active_mode].value)
    {
        case 0xFFFF:
            /*-------------------------------------------------*\
            | Direct mode - handled by DeviceUpdateLEDs         |
            \*-------------------------------------------------*/
            break;

        case FNATIC_STREAK_CMD_PULSE:
            controller->SetPulse(color_mode, r, g, b, speed);
            break;

        case FNATIC_STREAK_CMD_WAVE:
            controller->SetWave(color_mode, r, g, b, speed, direction);
            break;

        case FNATIC_STREAK_CMD_REACTIVE:
            controller->SetReactive(color_mode, r, g, b, speed, false);
            break;

        case FNATIC_STREAK_CMD_REACTIVE_RIPPLE:
            controller->SetReactiveRipple(color_mode, r, g, b, speed, false);
            break;

        case FNATIC_STREAK_CMD_RAIN:
            controller->SetRain(color_mode, r, g, b, speed, direction);
            break;

        case FNATIC_STREAK_CMD_GRADIENT:
            {
                /*---------------------------------------------*\
                | Send rainbow gradient with brightness scaling |
                \*---------------------------------------------*/
                unsigned char gradient_colors[6][3] =
                {
                    { (unsigned char)(0xFF * brightness / 100), 0x00, 0x00 },
                    { (unsigned char)(0xFF * brightness / 100), (unsigned char)(0xFF * brightness / 100), 0x00 },
                    { 0x00, (unsigned char)(0xFF * brightness / 100), 0x00 },
                    { 0x00, (unsigned char)(0xFF * brightness / 100), (unsigned char)(0xFF * brightness / 100) },
                    { 0x00, 0x00, (unsigned char)(0xFF * brightness / 100) },
                    { (unsigned char)(0xFF * brightness / 100), 0x00, (unsigned char)(0xFF * brightness / 100) },
                };
                unsigned char gradient_positions[6] = { 0, 20, 40, 60, 80, 100 };
                controller->SetGradient(gradient_colors, gradient_positions, 6);
            }
            break;

        case FNATIC_STREAK_CMD_FADE:
            {
                /*---------------------------------------------*\
                | Send rainbow fade with brightness scaling     |
                \*---------------------------------------------*/
                unsigned char fade_colors[6][3] =
                {
                    { (unsigned char)(0xFF * brightness / 100), 0x00, 0x00 },
                    { (unsigned char)(0xFF * brightness / 100), (unsigned char)(0xFF * brightness / 100), 0x00 },
                    { 0x00, (unsigned char)(0xFF * brightness / 100), 0x00 },
                    { 0x00, (unsigned char)(0xFF * brightness / 100), (unsigned char)(0xFF * brightness / 100) },
                    { 0x00, 0x00, (unsigned char)(0xFF * brightness / 100) },
                    { (unsigned char)(0xFF * brightness / 100), 0x00, (unsigned char)(0xFF * brightness / 100) },
                };
                unsigned char fade_positions[6] = { 0, 20, 40, 60, 80, 100 };
                controller->SetFade(FNATIC_STREAK_COLOR_MODE_RAINBOW, fade_colors, fade_positions, 6, speed);
            }
            break;

        default:
            break;
    }
}

void RGBController_FnaticStreak::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if(active_mode == 0)
        {
            /*-------------------------------------------------*\
            | In Direct mode, send keepalive to prevent         |
            | keyboard from reverting to profile effect         |
            \*-------------------------------------------------*/
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(500))
            {
                controller->SendKeepalive();
            }
        }
        std::this_thread::sleep_for(100ms);
    }
}
