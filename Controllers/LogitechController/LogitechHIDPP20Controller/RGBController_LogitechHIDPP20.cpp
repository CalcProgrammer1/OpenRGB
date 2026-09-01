/*---------------------------------------------------------*\
| RGBController_LogitechHIDPP20.cpp                         |
|                                                           |
|   RGBController for unified Logitech HID++ 2.0 devices    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include "RGBController_LogitechHIDPP20.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"
#include "LogitechHIDPP20IdleSettings.h"
#include "LogManager.h"

/*---------------------------------------------------------*\
| Sentinel for "this LED's last write didn't ACK".          |
| Stored in sent_colors[i] to force the next frame to       |
| re-push the LED regardless of color delta. The high byte  |
| 0xFF is unreachable from any ToRGBColor(r,g,b) value      |
| (those have high byte 0), so the sentinel never collides  |
| with a real color including black (0x00000000) or         |
| white (0x00FFFFFF).                                       |
\*---------------------------------------------------------*/
static constexpr RGBColor HIDPP20_UNCOMMITTED = 0xFF000000;

/*---------------------------------------------------------*\
| Effect period range, in milliseconds. Matches the         |
| 1000..20000ms range Logitech firmware tests against and   |
| the vendor app clamps to in observed wire captures.       |
| Out-of-band values can                                    |
| produce flashy / invisible animations on real hardware,   |
| so the slider stays clamped here on our side.             |
\*---------------------------------------------------------*/
static const uint16_t HIDPP20_PERIOD_MIN_MS = 1000;
static const uint16_t HIDPP20_PERIOD_MAX_MS = 20000;

/*---------------------------------------------------------*\
| Ripple has its own narrower, much faster period range.    |
| Values taken from G915's LOGITECH_G915_SPEED_RIPPLE_*     |
| constants: 2ms..200ms. A ripple feels right when quick;   |
| using the breathing range (1..20s) makes it invisible.    |
\*---------------------------------------------------------*/
static const uint16_t HIDPP20_RIPPLE_PERIOD_MIN_MS = 2;
static const uint16_t HIDPP20_RIPPLE_PERIOD_MAX_MS = 200;

/*---------------------------------------------------------*\
| Speed slider range presented to the user. 1..100 matches  |
| our brightness convention. Higher = faster animation,     |
| inverted on the wire because lower period = faster cycle. |
\*---------------------------------------------------------*/
static const int      HIDPP20_SPEED_SLIDER_MIN = 1;
static const int      HIDPP20_SPEED_SLIDER_MAX = 100;

static uint16_t SliderToPeriodMs(int slider, uint16_t period_min_ms, uint16_t period_max_ms)
{
    if(slider <= HIDPP20_SPEED_SLIDER_MIN) return period_max_ms;
    if(slider >= HIDPP20_SPEED_SLIDER_MAX) return period_min_ms;

    const int period_range = period_max_ms - period_min_ms;
    const int slider_range = HIDPP20_SPEED_SLIDER_MAX - HIDPP20_SPEED_SLIDER_MIN;

    return (uint16_t)(period_max_ms
                    - ((slider - HIDPP20_SPEED_SLIDER_MIN) * period_range) / slider_range);
}

static uint16_t SpeedSliderToPeriodMs(int slider)
{
    return SliderToPeriodMs(slider, HIDPP20_PERIOD_MIN_MS, HIDPP20_PERIOD_MAX_MS);
}

static uint16_t RippleSpeedSliderToPeriodMs(int slider)
{
    return SliderToPeriodMs(slider, HIDPP20_RIPPLE_PERIOD_MIN_MS, HIDPP20_RIPPLE_PERIOD_MAX_MS);
}

/*---------------------------------------------------------*\
| Color Wave 0x0016 carries a direction byte. Map OpenRGB's |
| 6 direction slots onto the Logitech wire values (Solaar   |
| LedDirectionChoices). Logitech defines 8 directions; the  |
| G515 uses 6 of them, which line up 1:1 with OpenRGB's set |
| (In / Out are not exposed on this keyboard).              |
\*---------------------------------------------------------*/
static uint8_t WaveDirectionToWire(unsigned int dir)
{
    switch(dir)
    {
        case MODE_DIRECTION_LEFT:       return 6;   /* Left       */
        case MODE_DIRECTION_RIGHT:      return 1;   /* Right      */
        case MODE_DIRECTION_UP:         return 7;   /* Up         */
        case MODE_DIRECTION_DOWN:       return 2;   /* Down       */
        case MODE_DIRECTION_HORIZONTAL: return 3;   /* Center Out */
        case MODE_DIRECTION_VERTICAL:   return 8;   /* Center In  */
        default:                        return 1;  /* Right      */
    }
}

/**------------------------------------------------------------------*\
    @name Logitech HID++ 2.0
    @category Keyboard,Mouse,Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechHIDPP20
    @comment
        Unified HID++ 2.0 controller that dynamically discovers device
        capabilities via feature probing. Supports per-key lighting
        (0x8081/0x8080) and zone-based effects (0x8071/0x8070).
\*-------------------------------------------------------------------*/

static const char* zone_location_name(uint16_t location)
{
    switch(location)
    {
        case 0x0001: return "Primary";
        case 0x0002: return "Logo";
        case 0x0003: return "Left Side";
        case 0x0004: return "Right Side";
        case 0x0005: return "Combined";
        case 0x0006: return "Primary 1";
        case 0x0007: return "Primary 2";
        case 0x0008: return "Primary 3";
        case 0x0009: return "Primary 4";
        case 0x000A: return "Primary 5";
        case 0x000B: return "Primary 6";
        default:
        {
            static char buf[16];
            snprintf(buf, sizeof(buf), "Zone 0x%04X", location);
            return buf;
        }
    }
}

/*---------------------------------------------------------*\
| Speakers report the generic location codes but the lamps  |
| are front and rear per satellite.                         |
\*---------------------------------------------------------*/
static const char* speaker_zone_location_name(uint16_t location)
{
    switch(location)
    {
        case 0x0001: return "Left Front";
        case 0x0002: return "Right Front";
        case 0x0003: return "Left Rear";
        case 0x0004: return "Right Rear";
        default:     return zone_location_name(location);
    }
}

/*---------------------------------------------------------*\
| HID++ per-key zone ID to OpenRGB key name mapping         |
| Zone IDs follow Solaar's KEYCODES (special_keys.py)       |
| Used to look up zone IDs by key name after KLM builds     |
| the keymap in its own sorted order.                       |
\*---------------------------------------------------------*/
static const std::map<std::string, unsigned int> hidpp20_key_name_to_zone =
{
    { KEY_EN_A,                1  },
    { KEY_EN_B,                2  },
    { KEY_EN_C,                3  },
    { KEY_EN_D,                4  },
    { KEY_EN_E,                5  },
    { KEY_EN_F,                6  },
    { KEY_EN_G,                7  },
    { KEY_EN_H,                8  },
    { KEY_EN_I,                9  },
    { KEY_EN_J,               10  },
    { KEY_EN_K,               11  },
    { KEY_EN_L,               12  },
    { KEY_EN_M,               13  },
    { KEY_EN_N,               14  },
    { KEY_EN_O,               15  },
    { KEY_EN_P,               16  },
    { KEY_EN_Q,               17  },
    { KEY_EN_R,               18  },
    { KEY_EN_S,               19  },
    { KEY_EN_T,               20  },
    { KEY_EN_U,               21  },
    { KEY_EN_V,               22  },
    { KEY_EN_W,               23  },
    { KEY_EN_X,               24  },
    { KEY_EN_Y,               25  },
    { KEY_EN_Z,               26  },
    { KEY_EN_1,               27  },
    { KEY_EN_2,               28  },
    { KEY_EN_3,               29  },
    { KEY_EN_4,               30  },
    { KEY_EN_5,               31  },
    { KEY_EN_6,               32  },
    { KEY_EN_7,               33  },
    { KEY_EN_8,               34  },
    { KEY_EN_9,               35  },
    { KEY_EN_0,               36  },
    { KEY_EN_ANSI_ENTER,      37  },
    { KEY_EN_ESCAPE,          38  },
    { KEY_EN_BACKSPACE,       39  },
    { KEY_EN_TAB,             40  },
    { KEY_EN_SPACE,           41  },
    { KEY_EN_MINUS,           42  },
    { KEY_EN_EQUALS,          43  },
    { KEY_EN_LEFT_BRACKET,    44  },
    { KEY_EN_RIGHT_BRACKET,   45  },
    { KEY_EN_ANSI_BACK_SLASH, 46  },   /* ANSI only; ISO/JIS use 47 + 97 */
    { KEY_EN_POUND,           47  },   /* ISO/JIS POUND (#/  row 3 col 12)  */
    { KEY_EN_ISO_BACK_SLASH,  97  },   /* ISO/JIS extra key (row 4 col 1)   */
    { KEY_EN_SEMICOLON,       48  },
    { KEY_EN_QUOTE,           49  },
    { KEY_EN_BACK_TICK,       50  },
    { KEY_EN_COMMA,           51  },
    { KEY_EN_PERIOD,          52  },
    { KEY_EN_FORWARD_SLASH,   53  },
    { KEY_EN_CAPS_LOCK,       54  },
    { KEY_EN_F1,              55  },
    { KEY_EN_F2,              56  },
    { KEY_EN_F3,              57  },
    { KEY_EN_F4,              58  },
    { KEY_EN_F5,              59  },
    { KEY_EN_F6,              60  },
    { KEY_EN_F7,              61  },
    { KEY_EN_F8,              62  },
    { KEY_EN_F9,              63  },
    { KEY_EN_F10,             64  },
    { KEY_EN_F11,             65  },
    { KEY_EN_F12,             66  },
    { KEY_EN_PRINT_SCREEN,    67  },
    { KEY_EN_SCROLL_LOCK,     68  },
    { KEY_EN_PAUSE_BREAK,     69  },
    { KEY_EN_INSERT,          70  },
    { KEY_EN_HOME,            71  },
    { KEY_EN_PAGE_UP,         72  },
    { KEY_EN_DELETE,          73  },
    { KEY_EN_END,             74  },
    { KEY_EN_PAGE_DOWN,       75  },
    { KEY_EN_RIGHT_ARROW,     76  },
    { KEY_EN_LEFT_ARROW,      77  },
    { KEY_EN_DOWN_ARROW,      78  },
    { KEY_EN_UP_ARROW,        79  },
    { KEY_EN_RIGHT_FUNCTION, 111  },
    { KEY_EN_MENU,            98  },

    /*-----------------------------------------------------*\
    | Positions 99-103, decoded from the enumeration's      |
    | correspondence with USB HID usages (zone = usage - 3; |
    | the legacy G915 table carries the raw usages, Solaar  |
    | names 99 POWER). No current board stuffs them, so     |
    | they classify as known-but-unstuffed, not unknown.    |
    \*-----------------------------------------------------*/
    { KEY_EN_POWER,           99  },
    { KEY_EN_NUMPAD_EQUAL,    100 },
    { KEY_EN_F13,             101 },
    { KEY_EN_F14,             102 },
    { KEY_EN_F15,             103 },

    /*-----------------------------------------------------*\
    | Numpad zones (Solaar KEYCODES 80-96).                 |
    | Required for any full-size HID++ keyboard.            |
    \*-----------------------------------------------------*/
    { KEY_EN_NUMPAD_LOCK,     80  },
    { KEY_EN_NUMPAD_DIVIDE,   81  },
    { KEY_EN_NUMPAD_TIMES,    82  },
    { KEY_EN_NUMPAD_MINUS,    83  },
    { KEY_EN_NUMPAD_PLUS,     84  },
    { KEY_EN_NUMPAD_ENTER,    85  },
    { KEY_EN_NUMPAD_1,        86  },
    { KEY_EN_NUMPAD_2,        87  },
    { KEY_EN_NUMPAD_3,        88  },
    { KEY_EN_NUMPAD_4,        89  },
    { KEY_EN_NUMPAD_5,        90  },
    { KEY_EN_NUMPAD_6,        91  },
    { KEY_EN_NUMPAD_7,        92  },
    { KEY_EN_NUMPAD_8,        93  },
    { KEY_EN_NUMPAD_9,        94  },
    { KEY_EN_NUMPAD_0,        95  },
    { KEY_EN_NUMPAD_PERIOD,   96  },

    { KEY_EN_LEFT_CONTROL,   104  },
    { KEY_EN_LEFT_SHIFT,     105  },
    { KEY_EN_LEFT_ALT,       106  },
    { KEY_EN_LEFT_WINDOWS,   107  },
    { KEY_EN_RIGHT_CONTROL,  108  },
    { KEY_EN_RIGHT_SHIFT,    109  },
    { KEY_EN_RIGHT_ALT,      110  },
    { KEY_EN_RIGHT_WINDOWS,  111  },

    /*-----------------------------------------------------*\
    | G915 (and similar) out-of-KLM LEDs.                   |
    | Zone IDs from Solaar KEYCODES. Names match the legacy |
    | G915 controller so existing users don't see their LED |
    | labels change when they move onto the unified driver. |
    \*-----------------------------------------------------*/
    { "Key: Brightness",            153 },
    { KEY_EN_MEDIA_PLAY_PAUSE,      155 },
    { KEY_EN_MEDIA_MUTE,            156 },
    { KEY_EN_MEDIA_NEXT,            157 },
    { KEY_EN_MEDIA_PREVIOUS,        158 },
    { "Key: G1",                    180 },
    { "Key: G2",                    181 },
    { "Key: G3",                    182 },
    { "Key: G4",                    183 },
    { "Key: G5",                    184 },
    { "Logo",                       210 },
};

/*---------------------------------------------------------*\
| Named extras: real positions on G-boards that no KLM      |
| layout places (media strip, G-keys, logo). Zone-keyed,    |
| but the numbering is board-specific, the PRO X RAPID      |
| renumbered brightness/play/next to 150/152/154, so the    |
| same label may sit at a different id on another board.    |
\*---------------------------------------------------------*/
static const std::map<uint16_t, std::string> hidpp20_extras_names =
{
    { 150, "Brightness"        },   /* PRO X RAPID renumbering */
    { 152, "Media Play/Pause"  },   /* PRO X RAPID renumbering */
    { 153, "Brightness"        },
    { 154, "Media Next"        },   /* PRO X RAPID renumbering */
    { 155, "Media Play/Pause"  },
    { 156, "Media Mute"        },
    { 157, "Media Next"        },
    { 158, "Media Previous"    },
    { 180, "G1"                },
    { 181, "G2"                },
    { 182, "G3"                },
    { 183, "G4"                },
    { 184, "G5"                },
    { 210, "Logo"              },
};

/*---------------------------------------------------------*\
| What key does the enumeration define at this position?    |
| The firmware reports a superset and the board stuffs a    |
| subset; a defined-but-unclaimed position is unstuffed     |
| (ISO keys on ANSI, numpad on TKL, F13-F15/Power), not     |
| unknown. "" = not defined at all.                         |
\*---------------------------------------------------------*/
static std::string HIDPP20ZonePositionName(uint16_t zid)
{
    for(const std::pair<const std::string, unsigned int>& kv : hidpp20_key_name_to_zone)
    {
        if(kv.second == zid)
        {
            return(kv.first);
        }
    }

    return("");
}

/*---------------------------------------------------------*\
| Per-board zone policy for studied boards:                 |
|  excluded            zones this board misreports          |
|  suppress_unclaimed  every unclaimed zone is a phantom    |
|                      (the real special keys are all       |
|                      placed by the layout edit)           |
| Boards without an entry surface what they report: named   |
| extras stay named, true unknowns get an Unmapped zone.    |
\*---------------------------------------------------------*/
struct KbZonePolicy
{
    uint16_t            pid;
    std::set<uint16_t>  excluded;
    bool                suppress_unclaimed;
};

static const KbZonePolicy known_kb_zone_policies[] =
{
    /*-----------------------------------------------------*\
    | PRO X RAPID: the probed top strip is the complete     |
    | special-key set; the firmware also enumerates the     |
    | canonical media/G-key ids, which aren't wired.        |
    \*-----------------------------------------------------*/
    { 0xC35B, {}, true },
};

static const KbZonePolicy* FindKbZonePolicy(uint16_t pid_wired, uint16_t pid_wireless)
{
    for(const KbZonePolicy& p : known_kb_zone_policies)
    {
        if(p.pid == pid_wired || p.pid == pid_wireless)
        {
            return(&p);
        }
    }

    return(nullptr);
}

/*---------------------------------------------------------*\
| Keyboard layout edits: KLM key_set overlays for boards    |
| that differ from stock (e.g. a media row above the        |
| F-row). keyboard_led.value carries the HID++ zone id,     |
| read back via GetKeyValueAt, so renumbered boards         |
| address the right LED with no special case in the         |
| commit path. Keyed by PID (wired or wireless). F-row      |
| columns: ESC=0, F1..F12=2..13, PrtSc group 14-16.         |
\*---------------------------------------------------------*/
struct KbLayoutEdit
{
    uint16_t             pid;
    const keyboard_led*  keys;
    size_t               key_count;
};

/*---------------------------------------------------------*\
| PRO X RAPID top strip: INSERT_ROW adds row 0 above        |
| the F-row, the rest INSERT_SHIFT_RIGHT into it.           |
| Zone ids hardware-probed on this board. Game Mode         |
| (F3) is a non-RGB toggle; this model has no               |
| separately addressable logo LED, so none is listed.       |
\*---------------------------------------------------------*/
static const keyboard_led proxrapid_top_strip[] =
{
    { 0, 0, 5,  150, "Brightness",       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_ROW         },   /* above F4  */
    { 0, 0, 10, 155, "Media Previous",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   /* above F9  */
    { 0, 0, 11, 152, "Media Play/Pause", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   /* above F10 */
    { 0, 0, 12, 154, "Media Next",       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   /* above F11 */
    { 0, 0, 13, 153, "Media Mute",       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },   /* above F12 */
};

static const KbLayoutEdit known_kb_layout_edits[] =
{
    { 0xC35B, proxrapid_top_strip, 5 },   /* PRO X RAPID */
};

static key_set FindKbLayoutEdit(uint16_t pid_wired, uint16_t pid_wireless)
{
    for(const KbLayoutEdit& e : known_kb_layout_edits)
    {
        if(e.pid == pid_wired || e.pid == pid_wireless)
        {
            return key_set(e.keys, e.keys + e.key_count);
        }
    }

    return key_set();
}

/*---------------------------------------------------------*\
| OpenRGB key name -> USB HID Keyboard/Keypad usage (page   |
| 0x07), used by the 0x8080 path whose keyIds ARE these     |
| usages. Sibling to hidpp20_key_name_to_zone (the 0x8081   |
| sequential zones). This is the HID spec, not a device     |
| table.                                                    |
\*---------------------------------------------------------*/
static const std::map<std::string, unsigned int> hidpp20_key_name_to_usage =
{
    { KEY_EN_A,                 0x04 },
    { KEY_EN_B,                 0x05 },
    { KEY_EN_C,                 0x06 },
    { KEY_EN_D,                 0x07 },
    { KEY_EN_E,                 0x08 },
    { KEY_EN_F,                 0x09 },
    { KEY_EN_G,                 0x0A },
    { KEY_EN_H,                 0x0B },
    { KEY_EN_I,                 0x0C },
    { KEY_EN_J,                 0x0D },
    { KEY_EN_K,                 0x0E },
    { KEY_EN_L,                 0x0F },
    { KEY_EN_M,                 0x10 },
    { KEY_EN_N,                 0x11 },
    { KEY_EN_O,                 0x12 },
    { KEY_EN_P,                 0x13 },
    { KEY_EN_Q,                 0x14 },
    { KEY_EN_R,                 0x15 },
    { KEY_EN_S,                 0x16 },
    { KEY_EN_T,                 0x17 },
    { KEY_EN_U,                 0x18 },
    { KEY_EN_V,                 0x19 },
    { KEY_EN_W,                 0x1A },
    { KEY_EN_X,                 0x1B },
    { KEY_EN_Y,                 0x1C },
    { KEY_EN_Z,                 0x1D },
    { KEY_EN_1,                 0x1E },
    { KEY_EN_2,                 0x1F },
    { KEY_EN_3,                 0x20 },
    { KEY_EN_4,                 0x21 },
    { KEY_EN_5,                 0x22 },
    { KEY_EN_6,                 0x23 },
    { KEY_EN_7,                 0x24 },
    { KEY_EN_8,                 0x25 },
    { KEY_EN_9,                 0x26 },
    { KEY_EN_0,                 0x27 },
    { KEY_EN_ANSI_ENTER,        0x28 },
    { KEY_EN_ESCAPE,            0x29 },
    { KEY_EN_BACKSPACE,         0x2A },
    { KEY_EN_TAB,               0x2B },
    { KEY_EN_SPACE,             0x2C },
    { KEY_EN_MINUS,             0x2D },
    { KEY_EN_EQUALS,            0x2E },
    { KEY_EN_LEFT_BRACKET,      0x2F },
    { KEY_EN_RIGHT_BRACKET,     0x30 },
    { KEY_EN_ANSI_BACK_SLASH,   0x31 },     /* ANSI */
    { KEY_EN_POUND,             0x32 },     /* ISO  */
    { KEY_EN_SEMICOLON,         0x33 },
    { KEY_EN_QUOTE,             0x34 },
    { KEY_EN_BACK_TICK,         0x35 },
    { KEY_EN_COMMA,             0x36 },
    { KEY_EN_PERIOD,            0x37 },
    { KEY_EN_FORWARD_SLASH,     0x38 },
    { KEY_EN_CAPS_LOCK,         0x39 },
    { KEY_EN_F1,                0x3A },
    { KEY_EN_F2,                0x3B },
    { KEY_EN_F3,                0x3C },
    { KEY_EN_F4,                0x3D },
    { KEY_EN_F5,                0x3E },
    { KEY_EN_F6,                0x3F },
    { KEY_EN_F7,                0x40 },
    { KEY_EN_F8,                0x41 },
    { KEY_EN_F9,                0x42 },
    { KEY_EN_F10,               0x43 },
    { KEY_EN_F11,               0x44 },
    { KEY_EN_F12,               0x45 },
    { KEY_EN_PRINT_SCREEN,      0x46 },
    { KEY_EN_SCROLL_LOCK,       0x47 },
    { KEY_EN_PAUSE_BREAK,       0x48 },
    { KEY_EN_INSERT,            0x49 },
    { KEY_EN_HOME,              0x4A },
    { KEY_EN_PAGE_UP,           0x4B },
    { KEY_EN_DELETE,            0x4C },
    { KEY_EN_END,               0x4D },
    { KEY_EN_PAGE_DOWN,         0x4E },
    { KEY_EN_RIGHT_ARROW,       0x4F },
    { KEY_EN_LEFT_ARROW,        0x50 },
    { KEY_EN_DOWN_ARROW,        0x51 },
    { KEY_EN_UP_ARROW,          0x52 },
    { KEY_EN_NUMPAD_LOCK,       0x53 },
    { KEY_EN_NUMPAD_DIVIDE,     0x54 },
    { KEY_EN_NUMPAD_TIMES,      0x55 },
    { KEY_EN_NUMPAD_MINUS,      0x56 },
    { KEY_EN_NUMPAD_PLUS,       0x57 },
    { KEY_EN_NUMPAD_ENTER,      0x58 },
    { KEY_EN_NUMPAD_1,          0x59 },
    { KEY_EN_NUMPAD_2,          0x5A },
    { KEY_EN_NUMPAD_3,          0x5B },
    { KEY_EN_NUMPAD_4,          0x5C },
    { KEY_EN_NUMPAD_5,          0x5D },
    { KEY_EN_NUMPAD_6,          0x5E },
    { KEY_EN_NUMPAD_7,          0x5F },
    { KEY_EN_NUMPAD_8,          0x60 },
    { KEY_EN_NUMPAD_9,          0x61 },
    { KEY_EN_NUMPAD_0,          0x62 },
    { KEY_EN_NUMPAD_PERIOD,     0x63 },
    { KEY_EN_ISO_BACK_SLASH,    0x64 },     /* ISO */
    { KEY_JP_RO,                0x87 },     /* International1: JIS Ro, ABNT2 slash */
    { "Keypad Comma",           0x85 },     /* ABNT2 / JIS numpad comma */
    { KEY_EN_MENU,              0x65 },
    { KEY_EN_LEFT_CONTROL,      0xE0 },
    { KEY_EN_LEFT_SHIFT,        0xE1 },
    { KEY_EN_LEFT_ALT,          0xE2 },
    { KEY_EN_LEFT_WINDOWS,      0xE3 },
    { KEY_EN_RIGHT_CONTROL,     0xE4 },
    { KEY_EN_RIGHT_SHIFT,       0xE5 },
    { KEY_EN_RIGHT_ALT,         0xE6 },
    { KEY_EN_RIGHT_WINDOWS,     0xE7 },
    { KEY_EN_RIGHT_FUNCTION,    0xE7 },     /* boards that place Fn where Right GUI sits (G Pro) */
};

/*---------------------------------------------------------*\
| Reverse lookup over hidpp20_key_name_to_usage: HID usage  |
| code -> key name, or nullptr when the code is not a       |
| standard keyboard key (a vendor-local logo / G-key /      |
| indicator id).                                            |
\*---------------------------------------------------------*/
static const char* hidpp20_key_name_for_usage(unsigned int usage)
{
    for(const std::pair<const std::string, unsigned int>& kv : hidpp20_key_name_to_usage)
    {
        if(kv.second == usage)
        {
            return kv.first.c_str();
        }
    }

    return nullptr;
}

/*---------------------------------------------------------*\
| ABNT2 wire overrides for the 0x8080 keyboard build. KLM's |
| ABNT2 overlay inserts the bottom-row ; key, which sits at |
| the ANSI / position (usage 0x38), and leaves / next to    |
| right shift, whose lighting id is NOT its input usage:    |
| the key types International1 (0x87) but lights on 0x88    |
| (G512 ABNT2, hardware-verified: 0x88 is what fn2          |
| enumerates and what SetKeyColors paints; 0x87 controls    |
| nothing). Values carry (keyType << 8) | keyId and land    |
| in the GetKeyValueAt branch like the strip keys.          |
\*---------------------------------------------------------*/
static const layout_values hidpp20_kb_8080_values =
{
    {},
    {
        {
            KEYBOARD_LAYOUT_ABNT2,
            {
                { 0, 4, 11, 0x0138, KEY_EN_SEMICOLON,     KEY_EN_UNUSED, KEYBOARD_OPCODE_SWAP_ONLY },
                { 0, 4, 12, 0x0188, KEY_EN_FORWARD_SLASH, KEY_EN_UNUSED, KEYBOARD_OPCODE_SWAP_ONLY },
            },
        },
    },
};

/*---------------------------------------------------------*\
| Mouse LED layout table                                    |
| Each entry defines a matrix layout for a known mouse.     |
| Looked up by substring match on device name.              |
| To add a new mouse: add an entry with name pattern,       |
| grid dimensions, LED count, map, and LED names.           |
\*---------------------------------------------------------*/
#define ML_NA 0xFFFFFFFF

struct MouseLayout
{
    const char*         name_match;
    unsigned int        rows;
    unsigned int        cols;
    unsigned int        led_count;
    const unsigned int* map;
    const char* const*  led_names;
};

static const unsigned int g502x_map[3 * 7] =
{
    /*  C . . . . . B  */
        2, ML_NA, ML_NA, ML_NA, ML_NA, ML_NA, 1,
    /*  . D H G F E .  */
        ML_NA, 3, 7, 6, 5, 4, ML_NA,
    /*  . . . . . . A  */
        ML_NA, ML_NA, ML_NA, ML_NA, ML_NA, ML_NA, 0,
};

static const char* g502x_led_names[] =
{
    "LED A", "LED B", "LED C", "LED D",
    "LED E", "LED F", "LED G", "LED H",
};

static const MouseLayout known_mouse_layouts[] =
{
    { "G502 X",  3, 7, 8, g502x_map, g502x_led_names },
    /*-----------------------------------------------------*\
    | Add new mice here:                                    |
    | { "G PRO X", rows, cols, count, map_ptr, names_ptr }, |
    \*-----------------------------------------------------*/
    { nullptr, 0, 0, 0, nullptr, nullptr }
};

static const MouseLayout* FindMouseLayout(const std::string& device_name)
{
    for(const MouseLayout* ml = known_mouse_layouts; ml->name_match != nullptr; ml++)
    {
        if(device_name.find(ml->name_match) != std::string::npos)
        {
            return ml;
        }
    }

    return nullptr;
}

/*---------------------------------------------------------*\
| 0x0620 headset physical layout: firmware reports zone IDs |
| but no geometry, so this places known devices' IDs onto   |
| earcup matrices (mirrors Solaar). Keyed by firmware PID.  |
| Centurion device names are unreliable. Unknown headsets   |
| fall back to one LED per zone.                            |
\*---------------------------------------------------------*/
struct HeadsetEarcup
{
    const char*          name;       /* OpenRGB zone name                    */
    unsigned int         rows;
    unsigned int         cols;
    unsigned int         led_count;
    const unsigned int*  map;        /* zone-relative LED indices (rows*cols) */
    const unsigned char* zone_ids;   /* firmware zone ID per LED, in LED order*/
};

struct HeadsetLayout
{
    uint16_t             pid;          /* matches pid_wireless or pid_wired   */
    unsigned int         earcup_count;
    const HeadsetEarcup* earcups;
};

/*---------------------------------------------------------*\
| G522: 8 LEDs, two 2x2 earcup grids (outer view):          |
|   Left  8 7 / 4 3      Right  6 5 / 2 1                   |
| LEDs are listed in grid reading order (TL, TR, BL, BR) so |
| each earcup's matrix map is the identity {0,1,2,3}.       |
\*---------------------------------------------------------*/
static const unsigned int  g522_earcup_map[2 * 2] = { 0, 1, 2, 3 };
static const unsigned char g522_left_zone_ids[4]  = { 8, 7, 4, 3 };
static const unsigned char g522_right_zone_ids[4] = { 6, 5, 2, 1 };

static const HeadsetEarcup g522_earcups[] =
{
    { "Left Earcup",  2, 2, 4, g522_earcup_map, g522_left_zone_ids  },
    { "Right Earcup", 2, 2, 4, g522_earcup_map, g522_right_zone_ids },
};

static const HeadsetLayout known_headset_layouts[] =
{
    { 0x0B18, 2, g522_earcups },    /* G522 LIGHTSPEED */
    { 0x0000, 0, nullptr }
};

static const HeadsetLayout* FindHeadsetLayout(uint16_t pid_wireless, uint16_t pid_wired)
{
    for(const HeadsetLayout* hl = known_headset_layouts; hl->earcups != nullptr; hl++)
    {
        if(hl->pid == pid_wireless || hl->pid == pid_wired)
        {
            return hl;
        }
    }

    return nullptr;
}

/*---------------------------------------------------------*\
| 0x8080 special-key strips: the model-specific keys a      |
| board carries beyond the standard main block, placed      |
| over the KLM-generated matrix (which supplies the main    |
| block per the 0x4540 layout). keyboard_led.value carries  |
| the wire address (keyType << 8) | keyId, read back via    |
| GetKeyValueAt. The main block resolves by key name to     |
| its HID usage, so regional frames need no per-model data. |
| full_size supplies the frame when fn2 enumeration is      |
| unavailable (has_numpad needs it).                        |
\*---------------------------------------------------------*/
struct Kb8080Strip
{
    uint16_t             pid;
    bool                 full_size;
    const keyboard_led*  keys;
    size_t               key_count;

    /*-----------------------------------------------------*\
    | Studied board: the firmware enumerates a canonical    |
    | id superset and everything physically present is      |
    | claimed by the matrix (KLM adapts per 0x4540) or the  |
    | strip, so unclaimed ids are phantoms. Suppressed      |
    | unless the plugin's Show Unmapped override is on.     |
    \*-----------------------------------------------------*/
    bool                 suppress_unclaimed;
};

/*---------------------------------------------------------*\
| G810/G610 shared strip: logo, indicator row and Mute      |
| above the F-row, media block right of Pause. Positions    |
| follow the legacy matrix.                                 |
\*---------------------------------------------------------*/
static const keyboard_led g810_top_strip[] =
{
    { 0, 0, 0,  0x1001, "Logo",                  KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_ROW         },
    { 0, 0, 12, 0x4005, "Num Lock Indicator",    KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 13, 0x4003, "Caps Lock Indicator",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 14, 0x4004, "Scroll Lock Indicator", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 16, 0x4002, "Game Mode",             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 18, 0x4001, "Lighting",              KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 19, 0x02E2, KEY_EN_MEDIA_MUTE,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 1, 17, 0x02CD, KEY_EN_MEDIA_PLAY_PAUSE, KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 1, 18, 0x02B7, KEY_EN_MEDIA_STOP,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 1, 19, 0x02B6, KEY_EN_MEDIA_PREVIOUS,   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 1, 20, 0x02B5, KEY_EN_MEDIA_NEXT,       KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
};

/*---------------------------------------------------------*\
| G Pro (wired TKL): logo and indicator row above the       |
| F-row, no media keys or Num Lock indicator.               |
\*---------------------------------------------------------*/
static const keyboard_led gpro_top_strip[] =
{
    { 0, 0, 0,  0x1001, "Logo",                  KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_ROW         },
    { 0, 0, 12, 0x4003, "Caps Lock Indicator",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 13, 0x4004, "Scroll Lock Indicator", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 15, 0x4002, "Game Mode",             KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 16, 0x4001, "Lighting",              KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
};

/*---------------------------------------------------------*\
| G512: two indicator LEDs in the F-row line above the      |
| numpad, Caps Lock above Num Lock and Game Mode above      |
| numpad /. Ids match the G810 family; positions and ids    |
| hardware-reported (G512 ABNT2 tester). No logo, media     |
| or lock-state row on this board.                          |
\*---------------------------------------------------------*/
static const keyboard_led g512_top_strip[] =
{
    { 0, 0, 17, 0x4003, "Caps Lock Indicator", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 18, 0x4002, "Game Mode Indicator", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
};

/*---------------------------------------------------------*\
| G910: G6-G9 in a row directly above F1-F4 (photo-         |
| verified), Logo left of Esc in the F row, G1-G5 down the  |
| left edge, Nameplate on the front edge. Wire ids from     |
| the legacy G910 map. Deliberately absent, as in the       |
| legacy map: the media cluster above the numpad (on/off    |
| backlight, not RGB; the firmware still advertises media   |
| keyType 0x02, see the C32B quirk) and the four small      |
| mode keys left of G6. Painting the advertised extras      |
| controls nothing (hardware-observed), so unclaimed ids    |
| are suppressed as phantoms; the plugin's Show Unmapped    |
| override surfaces them for auditing.                      |
\*---------------------------------------------------------*/
static const keyboard_led g910_side_strip[] =
{
    { 0, 0, 3, 0x0406, "Key: G6",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_ROW         },
    { 0, 0, 4, 0x0407, "Key: G7",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 5, 0x0408, "Key: G8",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 0, 6, 0x0409, "Key: G9",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 1, 0, 0x1001, "Logo",      KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 2, 0, 0x0401, "Key: G1",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 3, 0, 0x0402, "Key: G2",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 4, 0, 0x0403, "Key: G3",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 5, 0, 0x0404, "Key: G4",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 6, 0, 0x0405, "Key: G5",   KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
    { 0, 7, 3, 0x1002, "Nameplate", KEY_EN_UNUSED, KEYBOARD_OPCODE_INSERT_SHIFT_RIGHT },
};

/*---------------------------------------------------------*\
| An entry with no keys still marks a known 0x8080 board:   |
| it keeps per-key usable and the main block claimable      |
| when fn2 enumeration is unavailable, and supplies the     |
| frame size.                                               |
\*---------------------------------------------------------*/
static const Kb8080Strip known_kb_8080_strips[] =
{
    { 0xC331, true,  g810_top_strip, 11, true  },  /* G810             */
    { 0xC337, true,  g810_top_strip, 11, true  },  /* G810             */
    { 0xC333, true,  g810_top_strip, 11, true  },  /* G610             */
    { 0xC338, true,  g810_top_strip, 11, true  },  /* G610             */
    { 0xC342, true,  g512_top_strip, 2,  true  },  /* G512             */
    { 0xC33C, true,  g512_top_strip, 2,  true  },  /* G512 RGB         */
    { 0xC32B, true,  g910_side_strip, 11, true  }, /* G910 Orion Spark */
    { 0xC335, true,  g910_side_strip, 11, true  }, /* G910             */
    { 0xC339, false, gpro_top_strip, 5,  true  },  /* G Pro            */
};

static const Kb8080Strip* FindKb8080Strip(uint16_t pid_wired, uint16_t pid_wireless)
{
    for(const Kb8080Strip& s : known_kb_8080_strips)
    {
        if(s.pid == pid_wired || s.pid == pid_wireless)
        {
            return(&s);
        }
    }

    return(nullptr);
}

RGBController_LogitechHIDPP20::RGBController_LogitechHIDPP20(LogitechHIDPP20Controller* controller_ptr)
{
    controller = controller_ptr;

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    name        = caps.device_name;
    vendor      = "Logitech";
    description = "Logitech HID++ 2.0 Device";
    version     = caps.firmware_version;
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    switch(caps.device_type)
    {
        case LOGITECH_DEVICE_TYPE_KEYBOARD:
            type = DEVICE_TYPE_KEYBOARD;
            break;
        case LOGITECH_DEVICE_TYPE_NUMPAD:
            type = DEVICE_TYPE_KEYPAD;
            break;
        case LOGITECH_DEVICE_TYPE_MOUSE:
        case LOGITECH_DEVICE_TYPE_TRACKBALL:
            type = DEVICE_TYPE_MOUSE;
            break;
        case LOGITECH_DEVICE_TYPE_HEADSET:
            type = DEVICE_TYPE_HEADSET;
            break;
        case LOGITECH_DEVICE_TYPE_MOUSEPAD:
            type = DEVICE_TYPE_MOUSEMAT;
            break;
        case LOGITECH_DEVICE_TYPE_JOYSTICK:
        case LOGITECH_DEVICE_TYPE_GAMEPAD:
            type = DEVICE_TYPE_GAMEPAD;
            break;
        case LOGITECH_DEVICE_TYPE_SPEAKER:
            type = DEVICE_TYPE_SPEAKER;
            break;
        case LOGITECH_DEVICE_TYPE_MICROPHONE:
            type = DEVICE_TYPE_MICROPHONE;
            break;
        case LOGITECH_DEVICE_TYPE_LIGHT:
            type = DEVICE_TYPE_LIGHT;
            break;
        default:
            type = DEVICE_TYPE_UNKNOWN;
            break;
    }

    /*-----------------------------------------------------*\
    | Build mode list from discovered capabilities          |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Direct mode: per-key control via 0x8081               |
    \*-----------------------------------------------------*/
    if(caps.has_perkey)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*-----------------------------------------------------*\
    | Direct mode: per-key control via 0x8080, a            |
    | separate feature.                                     |
    \*-----------------------------------------------------*/
    if(PerKey8080Capable())
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*-----------------------------------------------------*\
    | Direct mode: zone-only devices (G560, G810            |
    | family). One LED per cluster, colors ride             |
    | per-zone Static writes.                               |
    \*-----------------------------------------------------*/
    if(caps.has_zone_effects && !caps.has_perkey && !PerKey8080Capable()
       && !caps.is_headset_rgb_hostmode)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*-----------------------------------------------------*\
    | Off mode: always available                            |
    \*-----------------------------------------------------*/
    {
        mode Off;
        Off.name       = "Off";
        Off.value      = 0xFF;
        Off.flags      = 0;
        Off.color_mode = MODE_COLORS_NONE;
        modes.push_back(Off);
    }

    /*-----------------------------------------------------*\
    | 0x0620 Headset RGB Hostmode has no effect cards.      |
    | Provide a single Direct mode that maps every LED to   |
    | the frame buffer; SetHeadsetRGBHostmodeColors         |
    | writes them straight to the earcup zones.             |
    \*-----------------------------------------------------*/
    if(caps.is_headset_rgb_hostmode)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*-----------------------------------------------------*\
    | Effect modes from zone cluster discovery              |
    | Scan effects from the first cluster (effects          |
    | are usually the same across clusters)                 |
    \*-----------------------------------------------------*/
    if(caps.has_zone_effects && !caps.zone_clusters.empty() && !caps.is_headset_rgb_hostmode)
    {
        const HIDPP20ZoneCluster& cluster = caps.zone_clusters[0];

        /*-------------------------------------------------*\
        | Per-LED effect colors: multi-cluster devices      |
        | whose only paint path is zone effects, one color  |
        | per cluster. Per-key devices take a single mode   |
        | color: the UI repaints per-key on any per-LED     |
        | mode set, which overwrites the zone effect.       |
        \*-------------------------------------------------*/
        const bool per_led_colors = (caps.zone_clusters.size() > 1)
                                 && !caps.has_perkey
                                 && !PerKey8080Capable();

        /*-------------------------------------------------*\
        | 0x8070 keyboard firmware ignores the Cycle, Wave  |
        | and Breathing intensity byte (G810/G910           |
        | hardware); the same slots drive brightness on     |
        | the mice. No slider for a dead control; the wire  |
        | still sends 100 in that slot.                     |
        \*-------------------------------------------------*/
        const bool effect_brightness = !(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS
                                      && caps.device_type == LOGITECH_DEVICE_TYPE_KEYBOARD);

        for(size_t i = 0; i < cluster.effects.size(); i++)
        {
            const HIDPP20Effect& fx = cluster.effects[i];

            switch(fx.effect_id)
            {
                case 0x0001:    // Static / Fixed Color
                {
                    mode Static;
                    Static.name       = "Static";
                    Static.value      = fx.index;

                    if(per_led_colors)
                    {
                        Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
                        Static.color_mode = MODE_COLORS_PER_LED;
                    }
                    else
                    {
                        Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
                        Static.colors_min = 1;
                        Static.colors_max = 1;
                        Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
                        Static.colors.resize(1);
                    }
                    modes.push_back(Static);
                    break;
                }

                case 0x0003:    // Color Cycle / Spectrum
                {
                    mode Cycle;
                    Cycle.name           = "Spectrum Cycle";
                    Cycle.value          = fx.index;
                    Cycle.flags          = MODE_FLAG_HAS_SPEED;
                    if(effect_brightness)
                    {
                        Cycle.flags     |= MODE_FLAG_HAS_BRIGHTNESS;
                    }
                    Cycle.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Cycle.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Cycle.speed          = 80;     /* ~4.9s, lively medium */
                    Cycle.brightness_min = 1;
                    Cycle.brightness_max = 100;
                    Cycle.brightness     = 100;
                    Cycle.color_mode     = MODE_COLORS_NONE;
                    modes.push_back(Cycle);
                    break;
                }

                case 0x000A:    // Breathing
                {
                    mode Breathing;
                    Breathing.name           = "Breathing";
                    Breathing.value          = fx.index;
                    Breathing.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Breathing.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Breathing.speed          = 70;     /* ~6.8s, calm medium */
                    Breathing.brightness_min = 1;
                    Breathing.brightness_max = 100;
                    Breathing.brightness     = 100;

                    if(per_led_colors)
                    {
                        Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR
                                             | MODE_FLAG_HAS_SPEED
                                             | MODE_FLAG_HAS_BRIGHTNESS;
                        Breathing.color_mode = MODE_COLORS_PER_LED;
                    }
                    else
                    {
                        Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                             | MODE_FLAG_HAS_SPEED
                                             | MODE_FLAG_HAS_BRIGHTNESS;
                        Breathing.colors_min = 1;
                        Breathing.colors_max = 1;
                        Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
                        Breathing.colors.resize(1);
                    }

                    if(!effect_brightness)
                    {
                        Breathing.flags &= ~MODE_FLAG_HAS_BRIGHTNESS;
                    }

                    modes.push_back(Breathing);
                    break;
                }

                case 0x0007:    // Audio visualizer (G560 factory default)
                {
                    /*-------------------------------------*\
                    | Pulses brightness with audio. A       |
                    | painted zone pulses its own           |
                    | color; a black zone self-cycles       |
                    | like the factory default.             |
                    \*-------------------------------------*/
                    mode Visualizer;
                    Visualizer.name       = "Audio Visualizer";
                    Visualizer.value      = fx.index;
                    Visualizer.speed_min  = HIDPP20_SPEED_SLIDER_MIN;
                    Visualizer.speed_max  = HIDPP20_SPEED_SLIDER_MAX;
                    Visualizer.speed      = 80;     /* ~4.9s, near the 5s firmware default */

                    if(per_led_colors)
                    {
                        Visualizer.flags      = MODE_FLAG_HAS_PER_LED_COLOR
                                              | MODE_FLAG_HAS_SPEED;
                        Visualizer.color_mode = MODE_COLORS_PER_LED;
                    }
                    else
                    {
                        Visualizer.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                              | MODE_FLAG_HAS_SPEED;
                        Visualizer.colors_min = 1;
                        Visualizer.colors_max = 1;
                        Visualizer.color_mode = MODE_COLORS_MODE_SPECIFIC;
                        Visualizer.colors.resize(1);
                    }
                    modes.push_back(Visualizer);
                    break;
                }

                case 0x0004:    // Color Wave
                {
                    mode Wave;
                    Wave.name           = "Color Wave";
                    Wave.value          = fx.index;
                    Wave.flags          = MODE_FLAG_HAS_SPEED;
                    if(effect_brightness)
                    {
                        Wave.flags     |= MODE_FLAG_HAS_BRIGHTNESS;
                    }
                    Wave.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Wave.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Wave.speed          = 80;     /* ~4.9s, lively medium */
                    Wave.brightness_min = 1;
                    Wave.brightness_max = 100;
                    Wave.brightness     = 100;
                    Wave.color_mode     = MODE_COLORS_NONE;
                    modes.push_back(Wave);
                    break;
                }

                case 0x000B:    // Ripple
                {
                    mode Ripple;
                    Ripple.name           = "Ripple";
                    Ripple.value          = fx.index;
                    Ripple.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                          | MODE_FLAG_HAS_SPEED
                                          | MODE_FLAG_HAS_BRIGHTNESS;
                    Ripple.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Ripple.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Ripple.speed          = 70;     /* ~6.8s, calm medium */
                    Ripple.brightness_min = 1;
                    Ripple.brightness_max = 100;
                    Ripple.brightness     = 100;
                    Ripple.colors_min     = 1;
                    Ripple.colors_max     = 1;
                    Ripple.color_mode     = MODE_COLORS_MODE_SPECIFIC;
                    Ripple.colors.resize(1);
                    modes.push_back(Ripple);
                    break;
                }

                case 0x0015:    // Cycle (saturation variant)
                {
                    /*-------------------------------------*\
                    | Saturation-bearing variant of 0x0003. |
                    | Same UI (speed = period, brightness = |
                    | intensity); the saturation byte is    |
                    | hardcoded full on the wire.           |
                    \*-------------------------------------*/
                    mode Cycle;
                    Cycle.name           = "Spectrum Cycle";
                    Cycle.value          = fx.index;
                    Cycle.flags          = MODE_FLAG_HAS_SPEED
                                         | MODE_FLAG_HAS_BRIGHTNESS;
                    Cycle.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Cycle.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Cycle.speed          = 80;     /* ~4.9s, lively medium */
                    Cycle.brightness_min = 1;
                    Cycle.brightness_max = 100;
                    Cycle.brightness     = 100;
                    Cycle.color_mode     = MODE_COLORS_NONE;
                    modes.push_back(Cycle);
                    break;
                }

                case 0x0016:    // Wave (saturation variant)
                {
                    /*-------------------------------------*\
                    | Saturation-bearing variant of 0x0004. |
                    | Period is a BE16 ms value on the      |
                    | standard 1..20s range (Solaar's       |
                    | LEDEffects table has no period range  |
                    | override for Wave); saturation is     |
                    | hardcoded on the wire.                |
                    \*-------------------------------------*/
                    mode Wave;
                    Wave.name           = "Color Wave";
                    Wave.value          = fx.index;
                    Wave.flags          = MODE_FLAG_HAS_SPEED
                                        | MODE_FLAG_HAS_BRIGHTNESS
                                        | MODE_FLAG_HAS_DIRECTION_LR
                                        | MODE_FLAG_HAS_DIRECTION_UD
                                        | MODE_FLAG_HAS_DIRECTION_HV;
                    Wave.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Wave.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Wave.speed          = 80;     /* ~4.9s, lively medium */
                    Wave.brightness_min = 1;
                    Wave.brightness_max = 100;
                    Wave.brightness     = 100;
                    Wave.direction      = MODE_DIRECTION_RIGHT;
                    Wave.color_mode     = MODE_COLORS_NONE;
                    modes.push_back(Wave);
                    break;
                }

                case 0x0017:    // Ripple (saturation variant)
                {
                    /*-------------------------------------*\
                    | Saturation-bearing variant of         |
                    | 0x000B. Carries color + period        |
                    | only, no intensity param, so no       |
                    | brightness slider. Saturation         |
                    | is hardcoded full on the wire.        |
                    \*-------------------------------------*/
                    mode Ripple;
                    Ripple.name           = "Ripple";
                    Ripple.value          = fx.index;
                    Ripple.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR
                                          | MODE_FLAG_HAS_SPEED;
                    Ripple.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Ripple.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Ripple.speed          = 70;     /* mid, fast ripple range */
                    Ripple.colors_min     = 1;
                    Ripple.colors_max     = 1;
                    Ripple.color_mode     = MODE_COLORS_MODE_SPECIFIC;
                    Ripple.colors.resize(1);
                    modes.push_back(Ripple);
                    break;
                }

                case 0x000E:    // Decomposition
                case 0x000F:    // Signature1
                case 0x0010:    // Signature2
                {
                    /*-------------------------------------*\
                    | Animated, no user color (period +     |
                    | intensity only), per Solaar's         |
                    | LEDEffects table. Wire param          |
                    | offsets handled in SetZoneEffect.     |
                    \*-------------------------------------*/
                    mode Anim;
                    Anim.name           = (fx.effect_id == 0x000E) ? "Decomposition"
                                        : (fx.effect_id == 0x000F) ? "Signature 1"
                                        :                            "Signature 2";
                    Anim.value          = fx.index;
                    Anim.flags          = MODE_FLAG_HAS_SPEED
                                        | MODE_FLAG_HAS_BRIGHTNESS;
                    Anim.speed_min      = HIDPP20_SPEED_SLIDER_MIN;
                    Anim.speed_max      = HIDPP20_SPEED_SLIDER_MAX;
                    Anim.speed          = 80;
                    Anim.brightness_min = 1;
                    Anim.brightness_max = 100;
                    Anim.brightness     = 100;
                    Anim.color_mode     = MODE_COLORS_NONE;
                    modes.push_back(Anim);
                    break;
                }

                default:
                    break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | On 0x8070 devices every effect write is ephemeral     |
    | by default (see DeviceUpdateMode persist branch       |
    | below). Add a Save button on firmware-effect          |
    | modes so users can explicitly commit the active       |
    | mode to NVM. Direct is excluded because per-key       |
    | framebuffer writes don't map to a savable             |
    | firmware effect on 0x8070. 0x8071/0x0600 already      |
    | persist on every write, so no Save button is          |
    | exposed there pending further research.               |
    \*-----------------------------------------------------*/
    if(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS)
    {
        for(size_t i = 0; i < modes.size(); i++)
        {
            if(modes[i].name != "Direct")
            {
                modes[i].flags |= MODE_FLAG_MANUAL_SAVE;
            }
        }
    }

    SetupZones();

    /*-----------------------------------------------------*\
    | Register repaint callback and start power             |
    | manager. The callback triggers                        |
    | DeviceUpdateLEDs from the power thread for            |
    | dim/wake when no animation is driving updates.        |
    \*-----------------------------------------------------*/
    controller->SetRepaintCallback(
        std::bind(&RGBController_LogitechHIDPP20::OnRepaintRequest, this));

    controller->SetReapplyActiveModeCallback(
        std::bind(&RGBController_LogitechHIDPP20::ReapplyActiveMode, this));

    /*-----------------------------------------------------*\
    | Per-key Direct frames ride the controller's sender    |
    | thread so the effect engine never blocks on the wire. |
    \*-----------------------------------------------------*/
    if(controller->GetCapabilities().has_perkey)
    {
        controller->SetPerKeyFrameSender(
            std::bind(&RGBController_LogitechHIDPP20::SendPerKeyFrame, this, std::placeholders::_1));
    }
}

/*---------------------------------------------------------*\
| Repaint callback handler (request_repaint_fn).            |
| Invoked from the power thread for dim/wake                |
| when no animation is driving updates.                     |
\*---------------------------------------------------------*/
void RGBController_LogitechHIDPP20::OnRepaintRequest()
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    /*-----------------------------------------------------*\
    | If Wake() signaled a full repaint, invalidate         |
    | sent_colors so DeviceUpdateLEDs pushes every zone     |
    | regardless of delta. Uses HIDPP20_UNCOMMITTED         |
    | rather than clear() so sent_colors is non-empty;      |
    | that avoids the first_frame / prep trigger while      |
    | still forcing a full push.                            |
    \*-----------------------------------------------------*/
    if(controller->ConsumeWakeFullRepaint())
    {
        for(size_t i = 0; i < sent_colors.size(); i++)
        {
            sent_colors[i] = HIDPP20_UNCOMMITTED;
        }
    }
    DeviceUpdateLEDs();
}

RGBController_LogitechHIDPP20::~RGBController_LogitechHIDPP20()
{
    controller->StopPowerManager();

    Shutdown();

    delete controller;
}

/*---------------------------------------------------------*\
| Derive the KeyboardLayoutManager size + layout from the   |
| discovered capabilities. Shared by the 0x8081 and 0x8080  |
| per-key keyboard branches. Layout codes follow the 0x4540 |
| code table used by the official app. Legend-only variants |
| with no distinct physical frame stay on the ANSI default. |
\*---------------------------------------------------------*/
static void hidpp20_derive_keyboard_layout
    (
    const HIDPP20DeviceCapabilities&    caps,
    KEYBOARD_SIZE&                      kb_size,
    KEYBOARD_LAYOUT&                    kb_layout
    )
{
    kb_size = caps.has_numpad ? KEYBOARD_SIZE_FULL : KEYBOARD_SIZE_TKL;

    switch(caps.keyboard_layout_code)
    {
        case 0x04:  // German
        case 0x0D:  // Swiss
        case 0x14:  // Czech
        case 0x19:  // Hungarian
        case 0x41:  // Czech
            kb_layout = KEYBOARD_LAYOUT_ISO_QWERTZ;
            break;

        case 0x05:  // French
        case 0x11:  // Belgian
            kb_layout = KEYBOARD_LAYOUT_ISO_AZERTY;
            break;

        case 0x02:  // International
        case 0x03:  // UK
        case 0x08:  // Nordic
        case 0x0E:  // Turkish
        case 0x0F:  // Spanish
        case 0x16:  // Nordic
        case 0x1A:  // Italian
        case 0x1D:  // Nordic
        case 0x1F:  // Portuguese
        case 0x21:  // Nordic
        case 0x24:  // Turkish
        case 0x37:  // International 2
            kb_layout = KEYBOARD_LAYOUT_ISO_QWERTY;
            break;

        case 0x0A:  // Japanese
            kb_layout = KEYBOARD_LAYOUT_JIS;
            break;

        case 0x38:  // Brazilian Portuguese
            kb_layout = KEYBOARD_LAYOUT_ABNT2;
            break;

        case 0x01:  // US
        default:
            kb_layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
            break;
    }
}

void RGBController_LogitechHIDPP20::AddLayoutZone
    (
    const std::string&                                   zone_name,
    zone_type                                            type,
    unsigned int                                         rows,
    unsigned int                                         cols,
    const unsigned int*                                  map,
    const std::vector<std::pair<std::string, uint16_t>>& zone_leds
    )
{
    zone new_zone;
    new_zone.name       = zone_name;
    new_zone.type       = type;
    new_zone.leds_min   = (unsigned int)zone_leds.size();
    new_zone.leds_max   = (unsigned int)zone_leds.size();
    new_zone.leds_count = (unsigned int)zone_leds.size();

    if(type == ZONE_TYPE_MATRIX && map != nullptr)
    {
        new_zone.matrix_map.Set(rows, cols, (unsigned int*)map);
    }

    zones.push_back(new_zone);

    for(size_t i = 0; i < zone_leds.size(); i++)
    {
        led new_led;
        new_led.name  = zone_leds[i].first;
        new_led.value = zone_leds[i].second;
        leds.push_back(new_led);

        led_to_zone_id.push_back(zone_leds[i].second);
    }
}

void RGBController_LogitechHIDPP20::SetupZones()
{
    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    led_to_zone_id.clear();
    led_to_keytype_keyid_8080.clear();
    sent_colors.clear();

    if(PerKey8080Capable())
    {
        SetupZones8080();
    }
    else if(caps.has_perkey)
    {
        if(caps.device_type == LOGITECH_DEVICE_TYPE_KEYBOARD)
        {
            /*---------------------------------------------*\
            | Keyboard: use KeyboardLayoutManager for       |
            | matrix layout. Size from numpad presence,     |
            | layout from the 0x4540 feature.               |
            \*---------------------------------------------*/
            KEYBOARD_SIZE   kb_size;
            KEYBOARD_LAYOUT kb_layout;
            hidpp20_derive_keyboard_layout(caps, kb_size, kb_layout);

            KeyboardLayoutManager klm(kb_layout, kb_size);

            /*---------------------------------------------*\
            | Apply any per-board layout edit (e.g.         |
            | a top strip of media/logo keys above          |
            | the F-row). KLM inserts the keys and          |
            | re-derives dimensions, so the matrix          |
            | build below needs no change.                  |
            \*---------------------------------------------*/
            key_set kb_layout_edit = FindKbLayoutEdit(caps.pid_wired, caps.pid_wireless);
            if(!kb_layout_edit.empty())
            {
                klm.ChangeKeys(kb_layout_edit);
            }

            zone perkey_zone;
            perkey_zone.name       = ZONE_EN_KEYBOARD;
            perkey_zone.type       = ZONE_TYPE_MATRIX;
            perkey_zone.leds_min   = klm.GetKeyCount();
            perkey_zone.leds_max   = klm.GetKeyCount();
            perkey_zone.leds_count = klm.GetKeyCount();
            perkey_zone.matrix_map = klm.GetKeyMap(KEYBOARD_MAP_FILL_TYPE_COUNT);
            zones.push_back(perkey_zone);

            for(unsigned int i = 0; i < klm.GetKeyCount(); i++)
            {
                led new_led;
                std::string key_name = klm.GetKeyNameAt(i);
                std::string alt_name = klm.GetKeyAltNameAt(i);

                /*-----------------------------------------*\
                | Display the regional legend (KLM alt      |
                | name) when the layout defines one; the    |
                | positional name below still drives the    |
                | wire lookup.                              |
                \*-----------------------------------------*/
                new_led.name = (alt_name != KEY_EN_UNUSED) ? alt_name : key_name;

                /*-----------------------------------------*\
                | Zone ID: a nonzero KLM value is a         |
                | board- specific wire address from a       |
                | layout edit (e.g. renumbered top-         |
                | strip keys); otherwise look it up by      |
                | key name from the standard table.         |
                \*-----------------------------------------*/
                unsigned int zone_id = klm.GetKeyValueAt(i);
                if(zone_id == 0)
                {
                    std::map<std::string, unsigned int>::const_iterator it = hidpp20_key_name_to_zone.find(key_name);
                    zone_id = (it != hidpp20_key_name_to_zone.end()) ? it->second : 0;
                }
                new_led.value = zone_id;
                leds.push_back(new_led);

                led_to_zone_id.push_back((uint16_t)zone_id);
            }

            /*---------------------------------------------*\
            | Classify every reported zone the matrix did   |
            | not claim. Zone ids index the firmware's      |
            | fixed enumeration (superset reported, subset  |
            | stuffed). An unclaimed zone is one of:        |
            | - a reserved protocol id (>= 0xFE)            |
            | - a phantom this board misreports (per-PID)   |
            | - a named extra no KLM layout places          |
            |   (media, G-keys, logo)                       |
            | - a defined position this board does not      |
            |   stuff (ISO on ANSI, numpad on TKL, F13-F15) |
            | - unknown: surfaced in an Unmapped zone and   |
            |   logged, so an unstudied board stays         |
            |   controllable and a tester can report which  |
            |   key lights; a report becomes a name or a    |
            |   layout edit.                                |
            \*---------------------------------------------*/
            std::set<uint16_t> klm_claimed_zones;
            for(uint16_t zid : led_to_zone_id)
            {
                if(zid != 0)
                {
                    klm_claimed_zones.insert(zid);
                }
            }

            const KbZonePolicy* zone_policy = FindKbZonePolicy(caps.pid_wired, caps.pid_wireless);

            /*---------------------------------------------*\
            | Plugin override: surface every                |
            | suppressed zone, for boards that stuff a      |
            | position no layout places (F13, Keypad        |
            | =) and for auditing policy entries.           |
            | Reserved ids stay hidden: 0xFF is the         |
            | all-keys broadcast, not an LED.               |
            \*---------------------------------------------*/
            LogitechHIDPP20IdleSettings::instance()->load();
            bool show_unmapped = LogitechHIDPP20IdleSettings::instance()->showUnmapped();

            std::vector<std::pair<uint16_t, std::string>> extras;
            std::vector<std::pair<uint16_t, std::string>> unmapped;

            for(uint16_t zid : caps.perkey_zone_ids)
            {
                if(klm_claimed_zones.count(zid) != 0)
                {
                    continue;
                }

                if(zid >= 0xFE)
                {
                    LOG_DEBUG("[LogitechHID++2.0 %s] zone %u is a reserved protocol id",
                              name.c_str(), (unsigned)zid);
                    continue;
                }

                if(zone_policy != nullptr &&
                   (zone_policy->suppress_unclaimed || zone_policy->excluded.count(zid) != 0))
                {
                    if(!show_unmapped)
                    {
                        LOG_DEBUG("[LogitechHID++2.0 %s] zone %u is a known phantom on this board",
                                  name.c_str(), (unsigned)zid);
                        continue;
                    }

                    unmapped.emplace_back(zid, "LED " + std::to_string(zid));
                    continue;
                }

                std::map<uint16_t, std::string>::const_iterator xn = hidpp20_extras_names.find(zid);
                if(xn != hidpp20_extras_names.end())
                {
                    extras.emplace_back(zid, xn->second);
                    continue;
                }

                std::string position = HIDPP20ZonePositionName(zid);
                if(!position.empty())
                {
                    if(!show_unmapped)
                    {
                        LOG_DEBUG("[LogitechHID++2.0 %s] zone %u (%s) is a defined position this board does not stuff",
                                  name.c_str(), (unsigned)zid, position.c_str());
                        continue;
                    }

                    unmapped.emplace_back(zid, position);
                    continue;
                }

                unmapped.emplace_back(zid, "LED " + std::to_string(zid));
            }

            if(!extras.empty())
            {
                zone extras_zone;
                extras_zone.name       = "Extras";
                extras_zone.type       = ZONE_TYPE_LINEAR;
                extras_zone.leds_min   = (unsigned int)extras.size();
                extras_zone.leds_max   = (unsigned int)extras.size();
                extras_zone.leds_count = (unsigned int)extras.size();
                zones.push_back(extras_zone);

                for(size_t i = 0; i < extras.size(); i++)
                {
                    led new_led;
                    new_led.name  = extras[i].second;
                    new_led.value = extras[i].first;
                    leds.push_back(new_led);
                    led_to_zone_id.push_back(extras[i].first);
                }
            }

            if(!unmapped.empty())
            {
                std::string id_list;
                for(const std::pair<uint16_t, std::string>& um : unmapped)
                {
                    if(!id_list.empty())
                    {
                        id_list += " ";
                    }
                    id_list += std::to_string(um.first);
                }

                LOG_INFO("[LogitechHID++2.0 %s] %zu unmapped per-key zone(s): %s, if coloring one "
                         "of these lights a key, please report which key it is (with PID %04X)",
                         name.c_str(), unmapped.size(), id_list.c_str(), caps.pid_wired);

                zone unmapped_zone;
                unmapped_zone.name       = "Unmapped";
                unmapped_zone.type       = ZONE_TYPE_LINEAR;
                unmapped_zone.leds_min   = (unsigned int)unmapped.size();
                unmapped_zone.leds_max   = (unsigned int)unmapped.size();
                unmapped_zone.leds_count = (unsigned int)unmapped.size();
                zones.push_back(unmapped_zone);

                for(const std::pair<uint16_t, std::string>& um : unmapped)
                {
                    led new_led;
                    new_led.name  = um.second;
                    new_led.value = um.first;
                    leds.push_back(new_led);
                    led_to_zone_id.push_back(um.first);
                }
            }
        }
        else if(const MouseLayout* ml = FindMouseLayout(caps.device_name))
        {
            /*---------------------------------------------*\
            | Known mouse: use table-defined matrix layout  |
            \*---------------------------------------------*/
            std::vector<std::pair<std::string, uint16_t>> mouse_leds;
            for(unsigned int i = 0; i < ml->led_count && i < caps.perkey_zone_ids.size(); i++)
            {
                mouse_leds.push_back(std::make_pair(std::string(ml->led_names[i]),
                                                    (uint16_t)caps.perkey_zone_ids[i]));
            }
            AddLayoutZone("Mouse LEDs", ZONE_TYPE_MATRIX, ml->rows, ml->cols, ml->map, mouse_leds);
        }
        else
        {
            /*---------------------------------------------*\
            | Other devices: linear zone, auto-named LEDs   |
            \*---------------------------------------------*/
            std::vector<std::pair<std::string, uint16_t>> other_leds;
            for(size_t i = 0; i < caps.perkey_zone_ids.size(); i++)
            {
                other_leds.push_back(std::make_pair("LED " + std::to_string(caps.perkey_zone_ids[i]),
                                                    (uint16_t)caps.perkey_zone_ids[i]));
            }
            AddLayoutZone("LEDs", ZONE_TYPE_LINEAR, 0, 0, nullptr, other_leds);
        }
    }
    else if(caps.is_headset_rgb_hostmode)
    {
        const HeadsetLayout* hl = FindHeadsetLayout(caps.pid_wireless, caps.pid_wired);

        unsigned int layout_leds = 0;
        if(hl != nullptr)
        {
            for(unsigned int e = 0; e < hl->earcup_count; e++)
            {
                layout_leds += hl->earcups[e].led_count;
            }
        }

        if(hl != nullptr && caps.headset_rgb_hostmode_zone_ids.size() == layout_leds)
        {
            /*---------------------------------------------*\
            | Known headset: one matrix zone per earcup     |
            | on the physical 2x2 grid; LED value = the     |
            | firmware zone ID the write path addresses.    |
            | A zone-count mismatch (firmware variant)      |
            | falls through to the generic path.            |
            \*---------------------------------------------*/
            for(unsigned int e = 0; e < hl->earcup_count; e++)
            {
                const HeadsetEarcup& ec = hl->earcups[e];

                std::vector<std::pair<std::string, uint16_t>> earcup_leds;
                for(unsigned int i = 0; i < ec.led_count; i++)
                {
                    earcup_leds.push_back(std::make_pair("LED " + std::to_string(ec.zone_ids[i]),
                                                         (uint16_t)ec.zone_ids[i]));
                }
                AddLayoutZone(ec.name, ZONE_TYPE_MATRIX, ec.rows, ec.cols, ec.map, earcup_leds);
            }
        }
        else
        {
            /*---------------------------------------------*\
            | Unknown 0x0620 headset: no geometry           |
            | table, so a single linear zone with           |
            | one LED per discovered zone ID.               |
            \*---------------------------------------------*/
            std::vector<std::pair<std::string, uint16_t>> headset_leds;
            for(size_t i = 0; i < caps.headset_rgb_hostmode_zone_ids.size(); i++)
            {
                headset_leds.push_back(std::make_pair("LED " + std::to_string(caps.headset_rgb_hostmode_zone_ids[i]),
                                                      (uint16_t)caps.headset_rgb_hostmode_zone_ids[i]));
            }
            AddLayoutZone("Headset", ZONE_TYPE_LINEAR, 0, 0, nullptr, headset_leds);
        }
    }
    else if(caps.has_zone_effects)
    {
        /*-------------------------------------------------*\
        | No per-key: create one zone per cluster           |
        \*-------------------------------------------------*/
        for(size_t i = 0; i < caps.zone_clusters.size(); i++)
        {
            const HIDPP20ZoneCluster& cluster = caps.zone_clusters[i];

            zone new_zone;
            new_zone.name       = (caps.device_type == LOGITECH_DEVICE_TYPE_SPEAKER)
                                ? speaker_zone_location_name(cluster.location)
                                : zone_location_name(cluster.location);
            new_zone.type       = ZONE_TYPE_SINGLE;
            new_zone.leds_min   = 1;
            new_zone.leds_max   = 1;
            new_zone.leds_count = 1;
            zones.push_back(new_zone);

            led new_led;
            new_led.name  = new_zone.name;
            new_led.value = cluster.index;
            leds.push_back(new_led);

            led_to_zone_id.push_back(cluster.index);
        }
    }

    /*-----------------------------------------------------*\
    | Build the zone_id -> LED index reverse map.           |
    | Indexed 0..255 (zone IDs are bytes); -1 marks         |
    | "no LED for this zone". Used by the FrameEnd          |
    | commit step to translate the acked_zones list         |
    | back into LED indices for sent_colors.                |
    \*-----------------------------------------------------*/
    zone_id_to_led_idx.assign(256, -1);

    for(size_t i = 0; i < led_to_zone_id.size(); i++)
    {
        uint16_t zid = led_to_zone_id[i];
        if(zid > 0 && zid < 256)
        {
            zone_id_to_led_idx[zid] = (int)i;
        }
    }

    SetupColors();
}

/*---------------------------------------------------------*\
| Feature 0x8080 zone setup, fed entirely by                |
| DiscoverPerKey8080(); nothing about the key set is        |
| hardcoded. Keyboards get the KLM matrix with each key     |
| resolved to (keyType, keyId) by its HID usage; the        |
| keyboard keyType is the enumerated one whose keyIds best  |
| match the HID keyboard page, not a hardcoded value.       |
| Enumerated pairs the matrix does not claim go in an       |
| Extras zone, named by HID usage where standard.           |
\*---------------------------------------------------------*/
bool RGBController_LogitechHIDPP20::PerKey8080Capable() const
{
    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    /*-----------------------------------------------------*\
    | 0x8080 per-key is usable if fn2 enumeration           |
    | confirmed keyTypes, OR the device is a known          |
    | keyboard model (its strip supplies the wire           |
    | addressing itself, so it works even when fn2 reads    |
    | are unavailable).                                     |
    \*-----------------------------------------------------*/
    return caps.has_perkey_8080
        || (caps.device_type == LOGITECH_DEVICE_TYPE_KEYBOARD
         && caps.idx_perkey_8080 != 0
         && FindKb8080Strip(caps.pid_wired, caps.pid_wireless) != nullptr);
}

void RGBController_LogitechHIDPP20::SetupZones8080()
{
    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    /*-----------------------------------------------------*\
    | Identify the keyboard keyType: the enumerated keyType |
    | whose keyIds overlap the standard HID keyboard page   |
    | the most. Ties / no keyboard device -> none.          |
    \*-----------------------------------------------------*/
    int      kb_type_index   = -1;
    size_t   kb_best_overlap = 0;

    if(caps.device_type == LOGITECH_DEVICE_TYPE_KEYBOARD)
    {
        for(size_t t = 0; t < caps.perkey_8080_types.size(); t++)
        {
            size_t overlap = 0;

            for(uint8_t key_id : caps.perkey_8080_types[t].key_ids)
            {
                if(hidpp20_key_name_for_usage(key_id) != nullptr)
                {
                    overlap++;
                }
            }

            if(overlap > kb_best_overlap)
            {
                kb_best_overlap = overlap;
                kb_type_index   = (int)t;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Track which (keyType, keyId) pairs the matrix claims  |
    | so the Extras zone can pick up everything else.       |
    \*-----------------------------------------------------*/
    std::set<uint32_t> claimed;        /* (keyType << 8) | keyId */

    size_t matrix_total  = 0;          /* KLM keys in the matrix       */
    size_t matrix_mapped = 0;          /* matrix keys with a live keyId */

    const Kb8080Strip* strip = FindKb8080Strip(caps.pid_wired, caps.pid_wireless);

    /*-----------------------------------------------------*\
    | Keyboard matrix: built when enumeration found a       |
    | keyboard keyType, or on a known-strip model, where    |
    | the main block claims unconditionally on keyType      |
    | 0x01 (the legacy controllers wrote every listed key   |
    | for years, absent keys just stay dark).               |
    \*-----------------------------------------------------*/
    bool have_enum = (kb_type_index >= 0);

    if(have_enum || strip != nullptr)
    {
        uint16_t          kb_key_type = 0x0001;
        std::set<uint8_t> kb_key_ids;

        if(have_enum)
        {
            const HIDPP20PerKey8080Type& kb_type = caps.perkey_8080_types[kb_type_index];

            kb_key_type = kb_type.key_type;
            kb_key_ids.insert(kb_type.key_ids.begin(), kb_type.key_ids.end());
        }

        KEYBOARD_SIZE   kb_size;
        KEYBOARD_LAYOUT kb_layout;
        hidpp20_derive_keyboard_layout(caps, kb_size, kb_layout);

        /*-------------------------------------------------*\
        | has_numpad comes from enumeration; without it     |
        | the strip entry supplies the frame size.          |
        \*-------------------------------------------------*/
        if(!have_enum && strip != nullptr)
        {
            kb_size = strip->full_size ? KEYBOARD_SIZE_FULL : KEYBOARD_SIZE_TKL;
        }

        KeyboardLayoutManager klm(kb_layout, kb_size, hidpp20_kb_8080_values);

        if(strip != nullptr && strip->keys != nullptr)
        {
            klm.ChangeKeys(key_set(strip->keys, strip->keys + strip->key_count));
        }

        zone perkey_zone;
        perkey_zone.name       = ZONE_EN_KEYBOARD;
        perkey_zone.type       = ZONE_TYPE_MATRIX;
        perkey_zone.leds_min   = klm.GetKeyCount();
        perkey_zone.leds_max   = klm.GetKeyCount();
        perkey_zone.leds_count = klm.GetKeyCount();

        perkey_zone.matrix_map = klm.GetKeyMap(KEYBOARD_MAP_FILL_TYPE_COUNT,
                                               (uint8_t)klm.GetRowCount(),
                                               (uint8_t)klm.GetColumnCount());
        zones.push_back(perkey_zone);

        matrix_total = klm.GetKeyCount();

        for(unsigned int i = 0; i < klm.GetKeyCount(); i++)
        {
            std::string key_name = klm.GetKeyNameAt(i);
            std::string alt_name = klm.GetKeyAltNameAt(i);

            led new_led;

            /*---------------------------------------------*\
            | Display the regional legend (KLM alt name)    |
            | when the layout defines one; key_name stays   |
            | the positional name and drives the wire       |
            | lookup below.                                 |
            \*---------------------------------------------*/
            new_led.name = (alt_name != KEY_EN_UNUSED) ? alt_name : key_name;

            uint16_t key_type = 0;
            uint8_t  key_id   = 0;

            /*---------------------------------------------*\
            | A nonzero KLM value is a strip key carrying   |
            | its wire address (keyType << 8) | keyId.      |
            | Main-block keys resolve by name to their HID  |
            | usage; with enumeration the keyId must be     |
            | one the device reported, on a known strip     |
            | model without enumeration every resolved key  |
            | is claimed.                                   |
            \*---------------------------------------------*/
            unsigned int strip_value = klm.GetKeyValueAt(i);

            if(strip_value != 0)
            {
                key_type = (uint16_t)(strip_value >> 8);
                key_id   = (uint8_t)(strip_value & 0xFF);
                claimed.insert(strip_value);
                matrix_mapped++;
            }
            else
            {
                std::map<std::string, unsigned int>::const_iterator it = hidpp20_key_name_to_usage.find(key_name);

                if(it != hidpp20_key_name_to_usage.end()
                && (have_enum ? kb_key_ids.count((uint8_t)it->second) != 0 : true))
                {
                    key_type = kb_key_type;
                    key_id   = (uint8_t)it->second;
                    claimed.insert(((uint32_t)key_type << 8) | key_id);
                    matrix_mapped++;
                }
            }

            new_led.value = ((uint32_t)key_type << 8) | key_id;
            leds.push_back(new_led);
            led_to_keytype_keyid_8080.emplace_back(key_type, key_id);
        }
    }

    /*-----------------------------------------------------*\
    | Extras: every enumerated (keyType, keyId) the matrix  |
    | did not claim. Named by HID usage where the keyId is  |
    | a standard keyboard code, generically otherwise.      |
    \*-----------------------------------------------------*/
    std::vector<std::pair<uint16_t, uint8_t>> extras;

    /*-----------------------------------------------------*\
    | Known model without enumeration: keep the off-frame   |
    | backslash keys addressable (the ported maps listed    |
    | ANSI backslash, POUND and ISO backslash together and  |
    | let the absent ones sit dark). With enumeration the   |
    | loop below picks these up from the device instead.    |
    \*-----------------------------------------------------*/
    if(!have_enum && strip != nullptr)
    {
        static const uint8_t frame_usages[] = { 0x31, 0x32, 0x64 };

        for(uint8_t usage : frame_usages)
        {
            if(claimed.count(((uint32_t)0x0001 << 8) | usage) == 0)
            {
                extras.emplace_back(0x0001, usage);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Studied-board phantom mask (G512: the firmware        |
    | reports all five international ids 0x87-0x8B and      |
    | the full G810 indicator set while only the ABNT2      |
    | slash and two indicators are wired). Plugin           |
    | override surfaces them anyway for auditing new        |
    | variants (a JIS board may need id probing like the    |
    | ABNT2 slash, which lights on 0x88 not 0x87).          |
    \*-----------------------------------------------------*/
    LogitechHIDPP20IdleSettings::instance()->load();
    bool suppress_unclaimed = (strip != nullptr)
                            && strip->suppress_unclaimed
                            && !LogitechHIDPP20IdleSettings::instance()->showUnmapped();

    for(size_t t = 0; t < caps.perkey_8080_types.size(); t++)
    {
        uint16_t key_type = caps.perkey_8080_types[t].key_type;

        for(uint8_t key_id : caps.perkey_8080_types[t].key_ids)
        {
            uint32_t pair_key = ((uint32_t)key_type << 8) | key_id;

            if(claimed.count(pair_key) != 0)
            {
                continue;
            }

            if(suppress_unclaimed)
            {
                LOG_DEBUG("[LogitechHID++2.0 %s] 0x8080 keyType 0x%04X keyId 0x%02X "
                          "is a known phantom on this board",
                          name.c_str(), key_type, key_id);
                continue;
            }

            extras.emplace_back(key_type, key_id);
        }
    }

    if(!extras.empty())
    {
        zone extras_zone;
        extras_zone.name       = "Extras";
        extras_zone.type       = ZONE_TYPE_LINEAR;
        extras_zone.leds_min   = (unsigned int)extras.size();
        extras_zone.leds_max   = (unsigned int)extras.size();
        extras_zone.leds_count = (unsigned int)extras.size();
        extras_zone.matrix_map.Reset();
        zones.push_back(extras_zone);

        for(size_t i = 0; i < extras.size(); i++)
        {
            uint16_t    key_type = extras[i].first;
            uint8_t     key_id   = extras[i].second;
            const char* std_name = hidpp20_key_name_for_usage(key_id);

            led new_led;

            if(std_name != nullptr)
            {
                new_led.name = std_name;
            }
            else
            {
                char label[32];
                snprintf(label, sizeof(label), "Zone 0x%04X Key 0x%02X",
                         key_type, key_id);
                new_led.name = label;
            }

            new_led.value = ((uint32_t)key_type << 8) | key_id;
            leds.push_back(new_led);
            led_to_keytype_keyid_8080.emplace_back(key_type, key_id);
        }
    }

    /*-----------------------------------------------------*\
    | One-shot summary of how the build mapped onto the     |
    | UI. If the structure dump looked right but nothing    |
    | lights, this line localises the break: no keyboard    |
    | keyType, zero matrix keys mapped, or everything in    |
    | Extras all point at distinct faults.                  |
    \*-----------------------------------------------------*/
    if(have_enum || strip != nullptr)
    {
        LOG_DEBUG("[LogitechHID++2.0 %s] 0x8080 zones: keyboard keyType 0x%04X%s%s, "
                  "%zu/%zu matrix keys mapped, %zu extra LED(s)",
                  name.c_str(),
                  have_enum ? caps.perkey_8080_types[kb_type_index].key_type : 0x0001,
                  have_enum ? "" : " (no enumeration, known-model claim)",
                  (strip != nullptr) ? ", strip applied" : "",
                  matrix_mapped, matrix_total, extras.size());
    }
    else
    {
        LOG_DEBUG("[LogitechHID++2.0 %s] 0x8080 zones: no keyboard keyType "
                  "identified, %zu extra LED(s) only",
                  name.c_str(), extras.size());
    }
}

void RGBController_LogitechHIDPP20::DeviceUpdateLEDs()
{
    /*-----------------------------------------------------*\
    | Don't paint while the controller is                   |
    | rediscovering its map: has_perkey and the             |
    | zone/effect data flap false, so a paint here          |
    | claims into an empty per-key layer. The effect        |
    | engine repaints next tick once discovery ends.        |
    \*-----------------------------------------------------*/
    if(controller->DiscoveryInProgress())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Per-key Direct rides the sender thread: snapshot,     |
    | hand over, return, the effect engine owns             |
    | colors[] and is never throttled by the wire.          |
    | Checked before the transaction guard so a frame       |
    | in flight cannot block this thread either.            |
    \*-----------------------------------------------------*/
    {
        const HIDPP20DeviceCapabilities& caps_pre = controller->GetCapabilities();

        if(caps_pre.has_perkey && !caps_pre.is_headset_rgb_hostmode && !PerKey8080Capable() &&
           (unsigned int)active_mode < modes.size() &&
           modes[active_mode].color_mode == MODE_COLORS_PER_LED &&
           controller->HasPerKeyFrameSender())
        {
            controller->SubmitPerKeyFrame(colors);
            return;
        }
    }

    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    if(!controller->IsOnline())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Ensure SW control is claimed on the first real        |
    | color push, safe here because the buffer holds        |
    | actual colors.                                        |
    \*-----------------------------------------------------*/
    controller->ClaimSWControlIfNeeded();

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    /*-----------------------------------------------------*\
    | Frame handling during SLEEPING:                       |
    |                                                       |
    | Default, suppress frames. A suppressed frame          |
    | cannot wake a device that treats writes as            |
    | activity, so this is the safe choice when we          |
    | don't know how a particular firmware handles          |
    | host traffic during its fade.                         |
    |                                                       |
    | Quirk-gated, devices flagged FADE_ACCEPTS_WRITES      |
    | opt out of suppression because their firmware         |
    | accepts writes without cancelling sleep. Frames       |
    | flow through SLEEPING until deep sleep starts BUSY-   |
    | NACKing every FrameEnd; consecutive-failure           |
    | tracking then sets deep_sleep and the top             |
    | IsDeepSleep() check takes over.                       |
    |                                                       |
    | Both paths suppress until Wake() clears the state.    |
    \*-----------------------------------------------------*/
    if(controller->IsDeepSleep())
    {
        return;
    }

    if(controller->GetPowerState() == HIDPP20_POWER_SLEEPING
       && !(caps.quirks & HIDPP20_QUIRK_FADE_ACCEPTS_WRITES))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | 0x0620 headset (G522 / PRO X 2): static color         |
    | only, two earcup zones; bypasses the per-key,         |
    | zone-effect and effect-card paths (0x0620 has         |
    | none). Host mode is already claimed (sticky);         |
    | just colors + FrameEnd[0x01].                         |
    \*-----------------------------------------------------*/
    if(caps.is_headset_rgb_hostmode)
    {
        controller->SetHeadsetRGBHostmodeColors(colors);
        return;
    }

    /*-----------------------------------------------------*\
    | Feature 0x8080 (Per Key Lighting). Full repaint       |
    | grouped by keyType, then one FlushLEDS.               |
    | led_to_keytype_keyid_8080 (built in SetupZones8080)   |
    | maps each LED to its wire (keyType, keyId).           |
    \*-----------------------------------------------------*/
    if(PerKey8080Capable() && (unsigned int)active_mode < modes.size() &&
       modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        std::vector<RGBColor> snapshot(colors.begin(), colors.end());

        int brightness = controller->GetDimBrightness();

        /*-------------------------------------------------*\
        | Group painted LEDs by keyType, preserving         |
        | keyType order of first appearance. Skip           |
        | unaddressable LEDs (keyType 0 + keyId 0, present  |
        | in the matrix but not enumerated on the device).  |
        \*-------------------------------------------------*/
        std::vector<std::pair<uint16_t, std::vector<std::pair<uint8_t, RGBColor>>>> by_type;
        std::map<uint16_t, size_t> type_slot;

        for(size_t i = 0; i < snapshot.size() && i < led_to_keytype_keyid_8080.size(); i++)
        {
            uint16_t key_type = led_to_keytype_keyid_8080[i].first;
            uint8_t  key_id   = led_to_keytype_keyid_8080[i].second;

            if(key_type == 0 && key_id == 0)
            {
                continue;
            }

            RGBColor color = snapshot[i];

            if(brightness < 100)
            {
                uint8_t r = RGBGetRValue(color) * brightness / 100;
                uint8_t g = RGBGetGValue(color) * brightness / 100;
                uint8_t b = RGBGetBValue(color) * brightness / 100;
                color = ToRGBColor(r, g, b);
            }

            std::map<uint16_t, size_t>::iterator slot = type_slot.find(key_type);

            if(slot == type_slot.end())
            {
                type_slot[key_type] = by_type.size();
                by_type.emplace_back(key_type, std::vector<std::pair<uint8_t, RGBColor>>());
                slot = type_slot.find(key_type);
            }

            by_type[slot->second].second.emplace_back(key_id, color);
        }

        if(!by_type.empty())
        {
            controller->SetPerKey8080(by_type);
            controller->PerKeyCommit8080();
        }

        return;
    }

    if(caps.has_perkey && (unsigned int)active_mode < modes.size() &&
       modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        /*-------------------------------------------------*\
        | Sender thread not registered, send inline.        |
        \*-------------------------------------------------*/
        std::vector<RGBColor> snapshot(colors.begin(), colors.end());
        SendPerKeyFrame(snapshot);
        return;
    }

    /*-----------------------------------------------------*\
    | Zone-effect devices without per-key (G560,            |
    | G810 family): one LED per cluster. Re-issue           |
    | the active effect on each changed zone with           |
    | that LED's color; Direct rides Static.                |
    \*-----------------------------------------------------*/
    if(caps.has_zone_effects && !caps.has_perkey && !PerKey8080Capable() &&
       (unsigned int)active_mode < modes.size() &&
       modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        const mode& current = modes[active_mode];

        bool          full_update = (sent_colors.size() != colors.size());
        bool          is_direct   = (current.name == "Direct");
        uint16_t      period      = SpeedSliderToPeriodMs(current.speed);
        unsigned char brightness  = (current.flags & MODE_FLAG_HAS_BRIGHTNESS)
                                  ? (unsigned char)current.brightness
                                  : 100;

        for(size_t i = 0; i < caps.zone_clusters.size() && i < colors.size(); i++)
        {
            if(!full_update && sent_colors[i] == colors[i])
            {
                continue;
            }

            const HIDPP20ZoneCluster& cluster = caps.zone_clusters[i];

            uint8_t  eff_idx = 0;
            uint16_t eff_id  = 0;

            for(size_t j = 0; j < cluster.effects.size(); j++)
            {
                bool match = is_direct ? (cluster.effects[j].effect_id == 0x0001)
                                       : (cluster.effects[j].index == (uint8_t)current.value);

                if(match)
                {
                    eff_idx = cluster.effects[j].index;
                    eff_id  = cluster.effects[j].effect_id;
                    break;
                }
            }

            if(eff_id == 0)
            {
                continue;
            }

            controller->SetZoneEffect(cluster.index, eff_idx, eff_id,
                                      RGBGetRValue(colors[i]),
                                      RGBGetGValue(colors[i]),
                                      RGBGetBValue(colors[i]),
                                      period, brightness, 0, false);
        }

        sent_colors.assign(colors.begin(), colors.end());
        controller->UpgradeSwControlAfterFirstPaint();
        return;
    }
}

/*---------------------------------------------------------*\
| One per-key frame: diff the snapshot against sent_colors, |
| put the differences on the wire, settle the ACKs. Runs on |
| the controller's sender thread (or inline as a fallback   |
| when no sender is registered), never on the thread that   |
| animates the color buffer.                                |
\*---------------------------------------------------------*/
void RGBController_LogitechHIDPP20::SendPerKeyFrame(std::vector<RGBColor>& snapshot)
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    if(!controller->IsOnline())
    {
        return;
    }

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    if(controller->IsDeepSleep())
    {
        return;
    }

    if(controller->GetPowerState() == HIDPP20_POWER_SLEEPING
       && !(caps.quirks & HIDPP20_QUIRK_FADE_ACCEPTS_WRITES))
    {
        return;
    }

    controller->ClaimSWControlIfNeeded();

    if(!caps.has_perkey || (unsigned int)active_mode >= modes.size() ||
       modes[active_mode].color_mode != MODE_COLORS_PER_LED)
    {
        return;
    }

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    /*-----------------------------------------------------*\
    | Detect re-initialization (reconnect, wake from        |
    | sleep). Device state is unknown, force full resend.   |
    \*-----------------------------------------------------*/
    uint32_t gen = controller->GetInitGeneration();

    if(gen != last_init_gen)
    {
        sent_colors.clear();
        resync_cursor = 0;
        last_init_gen = gen;
    }

    /*-----------------------------------------------------*\
    | Per-key prep, selected by a capability probe at       |
    | discovery: (A) Observed prep (DoObservedPerKeyPrep):  |
    | two SetEffectByIndex calls on 0x8071 cloned from the  |
    | official app, parameterized from discovery (template  |
    | bytes, effect count) so it fits any device with       |
    | effect cards. Gated on caps.has_effect_cards (set iff |
    | GetEffectSpecificInfo answers). (B) Static-pass-      |
    | through (doc-verified on G515), for devices without   |
    | effect cards or on 0x8070/0x0600: SetEffect           |
    | cluster=0xFF, Static, RGB 0, persist=1.               |
    \*-----------------------------------------------------*/
    bool needs_prep = controller->NeedsPrepSequence();
    bool prep_ran   = false;

    if(needs_prep && caps.has_zone_effects)
    {
        prep_ran = true;

        /*-------------------------------------------------*\
        | Once per claim. The prep wipes the per-key        |
        | buffer, so running it again on a later            |
        | frame throws away everything painted since.       |
        \*-------------------------------------------------*/
        controller->MarkPrepApplied();

        bool shape_matches_keyboard_family =
            caps.idx_disable_keys_by_usage != 0
            && caps.idx_perkey_v2 != 0
            && caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS;

        bool shape_matches_observed_prep =
            caps.has_effect_cards
            && caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS;

        if(shape_matches_keyboard_family)
        {
            /*---------------------------------------------*\
            | G815 / G915 / G Pro: per-cluster Off + primer |
            | key + FrameEnd. Matches their legacy          |
            | InitializeDirect wire sequence.               |
            \*---------------------------------------------*/
            controller->DoKeyboardFamilyPerKeyPrep();
        }
        else if(shape_matches_observed_prep)
        {
            controller->DoObservedPerKeyPrep();
        }
        else
        {
            /*---------------------------------------------*\
            | Per-cluster static-black primer. 0x8070       |
            | rejects the cluster=0xFF broadcast (seen on   |
            | the G810), so address each cluster by its     |
            | own index, matching the Off-mode path.        |
            \*---------------------------------------------*/
            for(size_t i = 0; i < caps.zone_clusters.size(); i++)
            {
                for(size_t j = 0; j < caps.zone_clusters[i].effects.size(); j++)
                {
                    if(caps.zone_clusters[i].effects[j].effect_id == 0x0001)
                    {
                        controller->SetZoneEffect(
                            caps.zone_clusters[i].index,
                            caps.zone_clusters[i].effects[j].index,
                            0x0001,              /* static effect */
                            0, 0, 0,             /* black, no fixed-color marker */
                            0,
                            100,                 /* brightness, unused for static */
                            0,                   /* direction, unused for static */
                            true                 /* persist=true */);
                        break;
                    }
                }
            }
        }
    }

    /*-----------------------------------------------------*\
    | Apply dim brightness scaling if not at                |
    | full brightness. This modifies the OUTPUT             |
    | only: the internal colors[] buffer stays              |
    | at full brightness for the animation.                 |
    \*-----------------------------------------------------*/
    int brightness = controller->GetDimBrightness();

    if(brightness < 100)
    {
        for(size_t i = 0; i < snapshot.size(); i++)
        {
            uint8_t r = RGBGetRValue(snapshot[i]) * brightness / 100;
            uint8_t g = RGBGetGValue(snapshot[i]) * brightness / 100;
            uint8_t b = RGBGetBValue(snapshot[i]) * brightness / 100;
            snapshot[i] = ToRGBColor(r, g, b);
        }
    }

    /*-----------------------------------------------------*\
    | Delta against last committed state; the first call    |
    | sends everything. A prep frame sends everything       |
    | too: the prep wipes the device buffer, so nothing     |
    | sent_colors calls committed is still there, and an    |
    | empty delta would return without painting, leaving    |
    | the device wiped, the 6->5 upgrade never firing,      |
    | and the prep re-running every frame.                  |
    \*-----------------------------------------------------*/
    bool full_update = (sent_colors.size() != snapshot.size()) || prep_ran;

    std::map<RGBColor, std::vector<uint8_t>> color_to_zones;
    std::vector<bool>                       zone_in_frame(256, false);
    unsigned int                            delta_zones = 0;

    for(size_t i = 0; i < snapshot.size() && i < led_to_zone_id.size(); i++)
    {
        if(led_to_zone_id[i] == 0 || led_to_zone_id[i] > 255)
        {
            continue;
        }

        if(full_update || snapshot[i] != sent_colors[i])
        {
            color_to_zones[snapshot[i]].push_back((uint8_t)led_to_zone_id[i]);
            zone_in_frame[led_to_zone_id[i]] = true;
            delta_zones++;
        }
    }

    /*-----------------------------------------------------*\
    | Rolling resync (see HIDPP20_RESYNC_KEYS_MIN): repaint |
    | the least recently visited keys whether or not they   |
    | look clean, so a key the device never applied heals.  |
    | The cursor walks every LED in turn, so coverage is    |
    | bounded even when one region changes every frame.     |
    \*-----------------------------------------------------*/
    if(!full_update && !led_to_zone_id.empty())
    {
        unsigned int budget = (delta_zones <= HIDPP20_RESYNC_QUIET_ZONES)
                            ? HIDPP20_RESYNC_KEYS_MAX
                            : HIDPP20_RESYNC_KEYS_MIN;
        unsigned int added  = 0;

        for(size_t scanned = 0; scanned < led_to_zone_id.size() && added < budget; scanned++)
        {
            size_t idx = resync_cursor % led_to_zone_id.size();

            resync_cursor++;

            if(idx >= snapshot.size() || led_to_zone_id[idx] == 0 || led_to_zone_id[idx] > 255)
            {
                continue;
            }

            if(zone_in_frame[led_to_zone_id[idx]])
            {
                continue;
            }

            color_to_zones[snapshot[idx]].push_back((uint8_t)led_to_zone_id[idx]);
            zone_in_frame[led_to_zone_id[idx]] = true;
            added++;
        }
    }

    if(color_to_zones.empty())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Drain stale ACKs from previous frames before sending. |
    | Without this, SendPerKeyData reads a stale ACK,       |
    | mistakes it for the current write's ACK, returns      |
    | early, and FrameEnd then races with the actual ACK.   |
    \*-----------------------------------------------------*/
    controller->FlushResponseQueue();

    /*-----------------------------------------------------*\
    | Batch changed keys for wire encoding: same-color      |
    | groups (2+): sort zones; runs of 3+ go fn5            |
    | SET_RANGE [start,end,R,G,B] x3/packet; scattered      |
    | go fn6 SET_SINGLE_VALUE [R,G,B,zid...] up to          |
    | 13/packet single-occurrence colors: fn1               |
    | SET_INDIVIDUAL [zid,R,G,B] x4/packet                  |
    \*-----------------------------------------------------*/
    std::vector<std::pair<uint16_t, RGBColor>> individual_pairs;

    for(std::pair<const RGBColor, std::vector<uint8_t>>& entry : color_to_zones)
    {
        RGBColor color = entry.first;
        std::vector<uint8_t>& zone_ids = entry.second;

        if(zone_ids.size() >= 2)
        {
            uint8_t r = RGBGetRValue(color);
            uint8_t g = RGBGetGValue(color);
            uint8_t b = RGBGetBValue(color);

            /*---------------------------------------------*\
            | Sort zone IDs into contiguous runs for fn5    |
            \*---------------------------------------------*/
            std::sort(zone_ids.begin(), zone_ids.end());

            std::vector<std::pair<uint8_t, uint8_t>> ranges;
            std::vector<uint8_t> scattered;
            size_t run_start = 0;

            for(size_t i = 1; i <= zone_ids.size(); i++)
            {
                if(i < zone_ids.size() && zone_ids[i] == zone_ids[i - 1] + 1)
                {
                    continue;
                }

                size_t run_len = i - run_start;

                if(run_len >= 3)
                {
                    ranges.push_back({zone_ids[run_start], zone_ids[i - 1]});
                }
                else
                {
                    for(size_t j = run_start; j < i; j++)
                    {
                        scattered.push_back(zone_ids[j]);
                    }
                }

                run_start = i;
            }

            /*---------------------------------------------*\
            | fn5 (SET_RANGE): 3 range entries per          |
            | packet. Track every zone in each              |
            | packet's ranges so the FrameEnd ACK           |
            | matcher can mark them committed.              |
            \*---------------------------------------------*/
            for(size_t i = 0; i < ranges.size(); i += 3)
            {
                uint8_t              data[16] = {};
                std::vector<uint8_t> packet_zones;
                size_t batch = ranges.size() - i;
                if(batch > 3) batch = 3;

                for(size_t j = 0; j < batch; j++)
                {
                    data[j * 5 + 0] = ranges[i + j].first;
                    data[j * 5 + 1] = ranges[i + j].second;
                    data[j * 5 + 2] = r;
                    data[j * 5 + 3] = g;
                    data[j * 5 + 4] = b;

                    for(uint8_t z = ranges[i + j].first;
                        z <= ranges[i + j].second; z++)
                    {
                        packet_zones.push_back(z);
                    }
                }

                controller->SendPerKeyData(perkey_idx, FN_8081_SET_RANGE,
                                           data, batch * 5, packet_zones);
            }

            /*---------------------------------------------*\
            | fn6 (SET_SINGLE_VALUE) for the scattered      |
            | remainder. Track listed zone IDs per packet.  |
            \*---------------------------------------------*/
            for(size_t i = 0; i < scattered.size(); i += 13)
            {
                uint8_t              data[16] = {};
                std::vector<uint8_t> packet_zones;
                data[0] = r;
                data[1] = g;
                data[2] = b;

                size_t batch = scattered.size() - i;
                if(batch > 13) batch = 13;

                for(size_t j = 0; j < batch; j++)
                {
                    data[3 + j] = scattered[i + j];
                    packet_zones.push_back(scattered[i + j]);
                }

                controller->SendPerKeyData(perkey_idx, FN_8081_SET_SINGLE_VALUE,
                                           data, 3 + batch, packet_zones);
            }
        }
        else
        {
            for(uint8_t zid : zone_ids)
            {
                individual_pairs.push_back({zid, color});
            }
        }
    }

    if(!individual_pairs.empty())
    {
        controller->SetPerKeyColors(individual_pairs);
    }

    PerKeyFrameResult commit = controller->PerKeyFrameEnd();

    /*-----------------------------------------------------*\
    | Size sent_colors to the snapshot before the           |
    | commit loop writes by index, empty on the             |
    | first frame (or after a reinit clear) the             |
    | writes would no-op and the prep would re-fire         |
    | every frame. Fill with HIDPP20_UNCOMMITTED so         |
    | untouched LEDs schedule for the next delta.           |
    \*-----------------------------------------------------*/
    if(sent_colors.size() != snapshot.size())
    {
        sent_colors.assign(snapshot.size(), HIDPP20_UNCOMMITTED);
    }

    /*-----------------------------------------------------*\
    | Build a fast lookup of acked zones for this frame.    |
    \*-----------------------------------------------------*/
    std::set<uint8_t> acked_set(commit.acked_zones.begin(),
                                commit.acked_zones.end());

    if(commit.frame_end_acked)
    {
        /*-------------------------------------------------*\
        | Frame end ACKed: any zone whose write packet also |
        | ACKed is now committed, advance sent_colors for   |
        | that LED. Any zone we attempted but never saw an  |
        | ACK for goes to HIDPP20_UNCOMMITTED so the next   |
        | frame's delta picks it up.                        |
        \*-------------------------------------------------*/
        for(uint8_t zid : commit.attempted_zones)
        {
            int led_idx = zone_id_to_led_idx[zid];
            if(led_idx < 0 || (size_t)led_idx >= sent_colors.size())
            {
                continue;
            }

            if(acked_set.count(zid))
            {
                sent_colors[led_idx] = snapshot[led_idx];
            }
            else
            {
                /*-----------------------------------------*\
                | A reply that did not arrive says nothing  |
                | about the position; leave it uncommitted  |
                | and let the next delta carry it again.    |
                \*-----------------------------------------*/
                sent_colors[led_idx] = HIDPP20_UNCOMMITTED;
            }
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | FrameEnd timed out: unknown what committed,       |
        | mark every attempted LED uncommitted for          |
        | re-push. ACKed writes still sit un-swapped        |
        | in staging, so per-zone ACK info is moot.         |
        \*-------------------------------------------------*/
        for(uint8_t zid : commit.attempted_zones)
        {
            int led_idx = zone_id_to_led_idx[zid];
            if(led_idx >= 0 && (size_t)led_idx < sent_colors.size())
            {
                sent_colors[led_idx] = HIDPP20_UNCOMMITTED;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Upgrade SW control 6 -> 5 only once the per-key       |
    | buffer holds a complete picture: at flags=5 the per-  |
    | key layer masks the zone layer, and unwritten keys    |
    | render black. "Every zone this frame attempted ACKed" |
    | is not enough, a clean two-key delta right after the  |
    | prep wipe would upgrade a device holding two keys and |
    | 93 blanks. sent_colors knows: when no mapped LED is   |
    | still HIDPP20_UNCOMMITTED, the device holds the whole |
    | picture; reachable across several frames, where one   |
    | perfect frame on a 95-key board is not.               |
    \*-----------------------------------------------------*/
    bool buffer_complete = commit.frame_end_acked;

    for(size_t i = 0; buffer_complete && i < sent_colors.size() && i < led_to_zone_id.size(); i++)
    {
        if(led_to_zone_id[i] == 0 || led_to_zone_id[i] > 255)
        {
            continue;
        }

        if(sent_colors[i] == HIDPP20_UNCOMMITTED)
        {
            buffer_complete = false;
        }
    }

    if(buffer_complete)
    {
        /*-------------------------------------------------*\
        | Now, and not one frame before. At flags=5         |
        | the per-key layer masks the zone layer, so        |
        | whatever is in that buffer is what the            |
        | device shows, hand it the screen while it is      |
        | half painted and the keys we have not             |
        | written render as the prep left them.             |
        \*-------------------------------------------------*/
        controller->UpgradeSwControlAfterFirstPaint();

        controller->CancelRetryPaint();
    }
    else if(controller->AwaitingSWControlUpgrade())
    {
        /*-------------------------------------------------*\
        | First paint after a claim: nothing guarantees a   |
        | next frame (a static color never calls again), so |
        | retry until the buffer completes. Only in this    |
        | window, streaming frames partial-commit routinely |
        | and their stragglers ride the next tick's delta;  |
        | retrying those stalls the animation.              |
        \*-------------------------------------------------*/
        if(!controller->ScheduleRetryPaint())
        {
            /*---------------------------------------------*\
            | Ladder spent: hand the screen over anyway. A  |
            | zone that never confirms must not hold the    |
            | upgrade forever, and the zone layer beneath   |
            | is the worse picture of the two.              |
            \*---------------------------------------------*/
            LOG_INFO("[LogitechHID++2.0 %s] buffer never fully confirmed, upgrading anyway",
                     name.c_str());

            controller->UpgradeSwControlAfterFirstPaint();
            controller->CancelRetryPaint();
        }
    }
    else
    {
        controller->CancelRetryPaint();
    }
}

void RGBController_LogitechHIDPP20::DeviceUpdateZoneLEDs(int /*zone*/)
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    DeviceUpdateLEDs();
}

void RGBController_LogitechHIDPP20::DeviceUpdateSingleLED(int /*led*/)
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    DeviceUpdateLEDs();
}

void RGBController_LogitechHIDPP20::DeviceUpdateMode()
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    if(!controller->IsOnline())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Drop mode changes while the firmware is fading        |
    | to off. The device owns its own power state; we       |
    | don't force-wake it from software. active_mode        |
    | stays tracked framework- side, and the next           |
    | wake (firmware onUserActivity) or reconnect           |
    | will re-apply it through the reinit callback.         |
    \*-----------------------------------------------------*/
    if(controller->GetPowerState() == HIDPP20_POWER_SLEEPING)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Claim SW control on the first mode set,               |
    | deferred from init.                                   |
    \*-----------------------------------------------------*/
    controller->ClaimSWControlIfNeeded();

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    if((unsigned int)active_mode >= modes.size())
    {
        return;
    }

    const mode& current = modes[active_mode];

    /*-----------------------------------------------------*\
    | Direct mode: invalidate delta tracking so the         |
    | next DeviceUpdateLEDs sends a full frame with         |
    | actual colors.                                        |
    \*-----------------------------------------------------*/
    if(current.name == "Direct")
    {
        sent_colors.clear();
        DeviceUpdateLEDs();

        /*-------------------------------------------------*\
        | Start power manager (reader + power               |
        | threads) if not already running.                  |
        \*-------------------------------------------------*/
        controller->StartPowerManager();

        if(caps.idx_wireless_status != 0 && !caps.has_power_mgmt)
        {
            controller->StartEventWatcher();
        }

        return;
    }

    sent_colors.clear();

    /*-----------------------------------------------------*\
    | Per-key devices: single-color non-animated modes      |
    | (Off, Static) go through the per-key path so LEDs     |
    | track the mode color; animated modes (HAS_SPEED)      |
    | fall through to zone effects, which render fine       |
    | alongside per-key on the devices we have data for.    |
    |                                                       |
    | 0x8080: Direct is handled above and Off               |
    | blacks every enumerated key via per-key. Any          |
    | other mode is a 0x8070/0x8071 zone effect             |
    | and falls through; those modes only exist             |
    | when a zone-effect feature is advertised.             |
    \*-----------------------------------------------------*/
    if(PerKey8080Capable())
    {
        if(current.value == 0xFF)
        {
            std::vector<std::pair<uint16_t, std::vector<std::pair<uint8_t, RGBColor>>>> by_type;

            for(size_t t = 0; t < caps.perkey_8080_types.size(); t++)
            {
                std::vector<std::pair<uint8_t, RGBColor>> keys;

                for(uint8_t key_id : caps.perkey_8080_types[t].key_ids)
                {
                    keys.emplace_back(key_id, ToRGBColor(0, 0, 0));
                }

                by_type.emplace_back(caps.perkey_8080_types[t].key_type, keys);
            }

            if(!by_type.empty())
            {
                controller->SetPerKey8080(by_type);
                controller->PerKeyCommit8080();
            }

            return;
        }

        /*-------------------------------------------------*\
        | Any other mode is a zone effect, fall             |
        | through to the SetZoneEffect path below.          |
        \*-------------------------------------------------*/
    }

    if(caps.has_perkey)
    {
        /*-------------------------------------------------*\
        | Off mode via per-key: set all LEDs to black       |
        \*-------------------------------------------------*/
        if(current.value == 0xFF)
        {
            controller->SetAllPerKeyColor(ToRGBColor(0, 0, 0));
            controller->PerKeyFrameEnd();
            return;
        }

        /*-------------------------------------------------*\
        | Static mode via per-key: only used as a fallback  |
        | when the device exposes per-key but no zone       |
        | effects. When both are available we prefer the    |
        | zone-effect path because per-key writes alone     |
        | don't fully claim against the firmware effect     |
        | engine on some devices (G502), leaving the        |
        | firmware fade fighting our per-key colors until   |
        | something else (e.g. Cycle) force-claims. Gated   |
        | on !HAS_SPEED so animated colored effects like    |
        | Breathing don't get clipped to a static color.    |
        \*-------------------------------------------------*/
        if(!caps.has_zone_effects
           && current.color_mode == MODE_COLORS_MODE_SPECIFIC
           && current.colors.size() > 0
           && !(current.flags & MODE_FLAG_HAS_SPEED))
        {
            controller->SetAllPerKeyColor(current.colors[0]);
            controller->PerKeyFrameEnd();
            return;
        }

        /*-------------------------------------------------*\
        | Animated effects (Breathing, Cycle,               |
        | Wave, Ripple) on per-key devices fall             |
        | through to the zone effect path.                  |
        \*-------------------------------------------------*/
    }

    /*-----------------------------------------------------*\
    | Zone effect modes (devices without per-key, or        |
    | animated effects that can't be done via per-key)      |
    |                                                       |
    | persist branching: 0x8070: ephemeral by default;      |
    | becomes persist=true only when DeviceSaveMode has     |
    | set save_pending. 0x8071/0x0600: keeps the pre-       |
    | existing per-branch hardcoded values (Off=false,      |
    | Effect=true) pending 0x8071 save research.            |
    \*-----------------------------------------------------*/
    const bool is_8070 = (caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS);

    /*-----------------------------------------------------*\
    | Off mode: set static black on all clusters            |
    \*-----------------------------------------------------*/
    if(current.value == 0xFF)
    {
        const bool off_persist = is_8070 ? save_pending : false;

        for(size_t i = 0; i < caps.zone_clusters.size(); i++)
        {
            for(size_t j = 0; j < caps.zone_clusters[i].effects.size(); j++)
            {
                if(caps.zone_clusters[i].effects[j].effect_id == 0x0001)
                {
                    controller->SetZoneEffect(
                        caps.zone_clusters[i].index,
                        caps.zone_clusters[i].effects[j].index,
                        0x0001, 0, 0, 0, 0, 100, 0, off_persist);
                    break;
                }
            }
        }
        controller->UpgradeSwControlAfterFirstPaint();
        return;
    }

    /*-----------------------------------------------------*\
    | Effect mode: apply to all clusters.                   |
    |                                                       |
    | Color source per cluster:                             |
    | MODE_COLORS_PER_LED: current.colors[i] maps to        |
    | caps.zone_clusters[i]                                 |
    | (one LED per cluster on the                           |
    | 0x8070 zone path)                                     |
    | MODE_COLORS_MODE_SPECIFIC, current.colors[0] for all  |
    | MODE_COLORS_NONE: zero (effect ignores RGB)           |
    \*-----------------------------------------------------*/
    uint16_t period = SpeedSliderToPeriodMs(current.speed);

    /*-----------------------------------------------------*\
    | Brightness defaults to 100 for modes that             |
    | don't expose a brightness slider; those               |
    | modes ignore the value at the wire level              |
    | anyway. Modes flagged HAS_BRIGHTNESS take             |
    | the user-set value from current.brightness.           |
    \*-----------------------------------------------------*/
    unsigned char brightness = (current.flags & MODE_FLAG_HAS_BRIGHTNESS)
                             ? (unsigned char)current.brightness
                             : 100;

    for(size_t i = 0; i < caps.zone_clusters.size(); i++)
    {
        uint16_t eff_id = 0;

        const std::vector<HIDPP20Effect>& cluster_effects = caps.zone_clusters[i].effects;
        for(size_t j = 0; j < cluster_effects.size(); j++)
        {
            if(cluster_effects[j].index == (uint8_t)current.value)
            {
                eff_id = cluster_effects[j].effect_id;
                break;
            }
        }

        unsigned char r = 0, g = 0, b = 0;

        /*-------------------------------------------------*\
        | Per-LED modes carry their colors in               |
        | the LED buffer, not the mode color                |
        | list, the UI paints leds/colors.                  |
        \*-------------------------------------------------*/
        if(current.color_mode == MODE_COLORS_PER_LED && i < colors.size())
        {
            r = RGBGetRValue(colors[i]);
            g = RGBGetGValue(colors[i]);
            b = RGBGetBValue(colors[i]);
        }
        else if(current.color_mode == MODE_COLORS_MODE_SPECIFIC && !current.colors.empty())
        {
            r = RGBGetRValue(current.colors[0]);
            g = RGBGetGValue(current.colors[0]);
            b = RGBGetBValue(current.colors[0]);
        }

        /*-------------------------------------------------*\
        | Ripple wants a narrower, much faster period       |
        | range (2..200ms) than the breathing/wave          |
        | baseline of 1..20s. Both Ripple variants, 0x000B  |
        | and the saturation 0x0017: use the fast range;    |
        | this mirrors Solaar's LEDEffects table, where     |
        | only Ripple carries a period range override.      |
        | Cycle (0x0003/0x0015) and Wave (0x0004/0x0016)    |
        | stay on the standard 1..20s range.                |
        \*-------------------------------------------------*/
        uint16_t cluster_period = (eff_id == 0x000B
                                || eff_id == 0x0017)
                                ? RippleSpeedSliderToPeriodMs(current.speed)
                                : period;

        /*-------------------------------------------------*\
        | 0x8071/0x0600: persist=true matches what the      |
        | observed vendor-app wire capture does for every   |
        | mode-set on these devices. With persist=false     |
        | the firmware appears to accept the command        |
        | without actually committing the new effect,       |
        | which is consistent with Static (which gets       |
        | prepped with persist=true at startup) being the   |
        | only effect that visibly works.                   |
        |                                                   |
        | 0x8070: ephemeral (persist=false) on live         |
        | writes; DeviceSaveMode flips save_pending         |
        | true to replay the active mode with               |
        | persist=true and commit to NVM.                   |
        \*-------------------------------------------------*/
        const bool effect_persist = is_8070 ? save_pending : true;

        controller->SetZoneEffect(
            caps.zone_clusters[i].index,
            current.value,
            eff_id, r, g, b, cluster_period, brightness,
            WaveDirectionToWire(current.direction), effect_persist);
    }

    /*-----------------------------------------------------*\
    | The zone effects are now committed, safe to upgrade   |
    | from flags=6 to flags=5. Without this, devices that   |
    | only use zone effects (no per-key Direct path) would  |
    | stay at flags=6 forever and the firmware would never  |
    | send onUserActivity events for idle/sleep.            |
    \*-----------------------------------------------------*/
    controller->UpgradeSwControlAfterFirstPaint();
}

void RGBController_LogitechHIDPP20::DeviceSaveMode()
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    /*-----------------------------------------------------*\
    | 0x8071/0x0600 already write persist=true on every     |
    | mode change, so the Save button isn't exposed on      |
    | those pages (MODE_FLAG_MANUAL_SAVE is only set for    |
    | 0x8070 modes in the constructor). If a save ever      |
    | lands here from those pages anyway, nothing needs     |
    | doing, the active mode is already committed to NVM.   |
    |                                                       |
    | 0x8070: replay the active mode through                |
    | DeviceUpdateMode with save_pending true so the        |
    | zone effect writes go out with persist=true,          |
    | committing the currently-live effect to flash.        |
    \*-----------------------------------------------------*/
    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();
    if(caps.rgb_feature_page != HIDPP20_FEAT_COLOR_LED_EFFECTS)
    {
        return;
    }

    save_pending = true;
    DeviceUpdateMode();
    save_pending = false;
}

bool RGBController_LogitechHIDPP20::ReapplyActiveMode()
{
    std::lock_guard<std::recursive_mutex> frame_guard(controller->TransactionMutex());

    /*-----------------------------------------------------*\
    | Re-establish the current active_mode on the device.   |
    | Used by the wake path (after SetRgbPowerMode(1)       |
    | cancels the firmware fade) and by the reconnect       |
    | path (after a wireless or USB reconnect). Handles     |
    | both per-key Direct and zone effect modes:            |
    |                                                       |
    | 1. Claim SW control (host mode + flags + power        |
    |    mode). Retried internally; returns true iff        |
    |    the final claim ACKed. ReconnectDevice's           |
    |    fast-backoff loop uses this as the accept          |
    |    signal.                                            |
    | 2. Clear sent_colors so the next frame is a           |
    |    full push. Per the 0x8071 lifecycle the            |
    |    device's LED buffer may not survive mode           |
    |    3->1 or a full reconnect, so we do not             |
    |    trust it to remember prior state.                  |
    | 3. Route through DeviceUpdateMode so both             |
    |    per-key Direct (full per-key frame via             |
    |    DeviceUpdateLEDs) and zone effects (SetEffect      |
    |    per cluster) re-establish correctly. Covers        |
    |    the case where active_mode changed in the GUI      |
    |    while the device was fading; that change was       |
    |    dropped then and lands here on wake.               |
    \*-----------------------------------------------------*/
    bool claimed = controller->ClaimSWControlIfNeeded();
    sent_colors.clear();
    DeviceUpdateMode();
    return claimed;
}
