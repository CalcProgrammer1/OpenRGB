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
#include <chrono>
#include <algorithm>
#include <functional>
#include "RGBController_LogitechHIDPP20.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"
#include "LogManager.h"

/*----------------------------------------------------------*\
| Sentinel for "this LED's last write didn't ACK".           |
| Stored in sent_colors[i] to force the next frame to        |
| re-push the LED regardless of color delta. The high byte   |
| 0xFF is unreachable from any ToRGBColor(r,g,b) value       |
| (those have high byte 0), so the sentinel never collides   |
| with a real color including black (0x00000000) or          |
| white (0x00FFFFFF).                                        |
\*----------------------------------------------------------*/
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
| constants: 2ms..200ms. A ripple feels right when quick —  |
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
        case 0x0001: return "All";
        case 0x0002: return "Primary";
        case 0x0003: return "Combined";
        case 0x0004: return "Logo";
        case 0x0005: return "Left";
        case 0x0006: return "Right";
        case 0x0007: return "Group 1";
        case 0x0008: return "Group 2";
        case 0x0009: return "Group 3";
        case 0x000A: return "Group 4";
        case 0x000B: return "Group 5";
        case 0x2000: return "Top";
        case 0x4000: return "Bottom";
        default:
        {
            static char buf[16];
            snprintf(buf, sizeof(buf), "Zone 0x%04X", location);
            return buf;
        }
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
    { KEY_EN_ANSI_BACK_SLASH, 46  },
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

    /*------------------------------------------------------*\
    | Numpad zones (Solaar KEYCODES 80-96).                  |
    | Required for any full-size HID++ keyboard.             |
    \*------------------------------------------------------*/
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

    /*------------------------------------------------------*\
    | G915 (and similar) out-of-KLM LEDs.                    |
    | Zone IDs from Solaar KEYCODES. Names match the legacy  |
    | G915 controller so existing users don't see their LED  |
    | labels change when they move onto the unified driver.  |
    \*------------------------------------------------------*/
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
    /*-------------------------------------------------------*\
    | Add new mice here:                                      |
    | { "G PRO X", rows, cols, count, map_ptr, names_ptr },   |
    \*-------------------------------------------------------*/
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
        default:
            type = DEVICE_TYPE_UNKNOWN;
            break;
    }

    /*----------------------------------------------------------*\
    | Build mode list from discovered capabilities               |
    \*----------------------------------------------------------*/

    /*----------------------------------------------------------*\
    | Direct mode: per-key control via 0x8081                    |
    \*----------------------------------------------------------*/
    if(caps.has_perkey)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*----------------------------------------------------------*\
    | Off mode: always available                                 |
    \*----------------------------------------------------------*/
    {
        mode Off;
        Off.name       = "Off";
        Off.value      = 0xFF;
        Off.flags      = 0;
        Off.color_mode = MODE_COLORS_NONE;
        modes.push_back(Off);
    }

    /*----------------------------------------------------------*\
    | 0x0620 Headset RGB Hostmode has no effect cards. Provide   |
    | a single Direct mode that maps every LED to the frame      |
    | buffer; SetHeadsetRGBHostmodeColors writes them straight   |
    | to the earcup zones.                                       |
    \*----------------------------------------------------------*/
    if(caps.is_headset_rgb_hostmode)
    {
        mode Direct;
        Direct.name       = "Direct";
        Direct.value      = 0;
        Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode = MODE_COLORS_PER_LED;
        modes.push_back(Direct);
    }

    /*----------------------------------------------------------*\
    | Effect modes from zone cluster discovery                   |
    | Scan effects from the first cluster (effects are usually   |
    | the same across clusters)                                  |
    \*----------------------------------------------------------*/
    if(caps.has_zone_effects && !caps.zone_clusters.empty() && !caps.is_headset_rgb_hostmode)
    {
        const HIDPP20ZoneCluster& cluster = caps.zone_clusters[0];

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

                    /*-----------------------------------------------------*\
                    | Multi-cluster devices (mice with logo/scroll/DPI)     |
                    | get per-LED colors so each zone can be painted        |
                    | independently in Static. Single-cluster devices       |
                    | (keyboards, single-zone mice) keep the single-color   |
                    | MODE_COLORS_MODE_SPECIFIC UX.                         |
                    \*-----------------------------------------------------*/
                    if(caps.zone_clusters.size() > 1)
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

                    /*-----------------------------------------------------*\
                    | See Static above — multi-cluster gets per-LED colors. |
                    \*-----------------------------------------------------*/
                    if(caps.zone_clusters.size() > 1)
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
                    modes.push_back(Breathing);
                    break;
                }

                case 0x0004:    // Color Wave
                {
                    mode Wave;
                    Wave.name           = "Color Wave";
                    Wave.value          = fx.index;
                    Wave.flags          = MODE_FLAG_HAS_SPEED
                                        | MODE_FLAG_HAS_BRIGHTNESS;
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
                    /*-----------------------------------------------*\
                    | Saturation-bearing variant of 0x0003. Same UI   |
                    | (speed = period, brightness = intensity); the   |
                    | saturation byte is hardcoded full on the wire.  |
                    \*-----------------------------------------------*/
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
                    /*-----------------------------------------------*\
                    | Saturation-bearing variant of 0x0004. Period    |
                    | is a BE16 ms value on the standard 1..20s range |
                    | (Solaar's LEDEffects table has no period range  |
                    | override for Wave); saturation is hardcoded on  |
                    | the wire.                                       |
                    \*-----------------------------------------------*/
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
                    /*-----------------------------------------------*\
                    | Saturation-bearing variant of 0x000B. Carries   |
                    | color + period only — no intensity param, so    |
                    | no brightness slider. Saturation is hardcoded   |
                    | full on the wire.                               |
                    \*-----------------------------------------------*/
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

                default:
                    break;
            }
        }
    }

    /*---------------------------------------------------------*\
    | On 0x8070 devices every effect write is ephemeral by      |
    | default (see DeviceUpdateMode persist branch below). Add  |
    | a Save button on firmware-effect modes so users can       |
    | explicitly commit the active mode to NVM. Direct is       |
    | excluded because per-key framebuffer writes don't map to  |
    | a savable firmware effect on 0x8070. 0x8071/0x0600        |
    | already persist on every write, so no Save button is      |
    | exposed there pending further research.                   |
    \*---------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Register repaint callback and start power manager.         |
    | The callback triggers DeviceUpdateLEDs from the power      |
    | thread for dim/wake when no animation is driving updates.  |
    \*----------------------------------------------------------*/
    controller->SetRepaintCallback(
        std::bind(&RGBController_LogitechHIDPP20::OnRepaintRequest, this));

    controller->SetReapplyActiveModeCallback(
        std::bind(&RGBController_LogitechHIDPP20::ReapplyActiveMode, this));
}

/*---------------------------------------------------------------*\
| Repaint callback handler (request_repaint_fn). Invoked from the |
| power thread for dim/wake when no animation is driving updates. |
\*---------------------------------------------------------------*/
void RGBController_LogitechHIDPP20::OnRepaintRequest()
{
    /*-------------------------------------------------*\
    | If Wake() signaled a full repaint, invalidate     |
    | sent_colors so DeviceUpdateLEDs pushes every zone |
    | regardless of delta. Uses HIDPP20_UNCOMMITTED     |
    | rather than clear() so sent_colors is non-empty — |
    | that avoids the first_frame / prep trigger while  |
    | still forcing a full push.                        |
    \*-------------------------------------------------*/
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
    delete controller;
}

void RGBController_LogitechHIDPP20::SetupZones()
{
    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    led_to_zone_id.clear();
    sent_colors.clear();

    if(caps.has_perkey)
    {
        if(caps.device_type == LOGITECH_DEVICE_TYPE_KEYBOARD)
        {
            /*--------------------------------------------------*\
            | Keyboard: use KeyboardLayoutManager for matrix     |
            | layout. Derive size from numpad presence, layout   |
            | from 0x4540 KeyboardLayout feature.                |
            \*--------------------------------------------------*/
            KEYBOARD_SIZE kb_size = caps.has_numpad
                                  ? KEYBOARD_SIZE_FULL
                                  : KEYBOARD_SIZE_TKL;

            KEYBOARD_LAYOUT kb_layout;

            switch(caps.keyboard_layout_code)
            {
                case 3:     // German
                case 7:     // Swiss
                    kb_layout = KEYBOARD_LAYOUT_ISO_QWERTZ;
                    break;

                case 4:     // French
                    kb_layout = KEYBOARD_LAYOUT_ISO_AZERTY;
                    break;

                case 2:     // UK
                case 5:     // Spanish
                case 0x0B:  // Italian
                case 0x0D:  // Portuguese
                case 0x0E:  // Belgian
                case 0x0F:  // Scandinavian
                case 8:     // Nordic
                case 0x16:  // Nordic
                case 0x1D:  // Nordic
                case 0x21:  // Nordic
                case 0x24:  // Belgian
                    kb_layout = KEYBOARD_LAYOUT_ISO_QWERTY;
                    break;

                case 9:     // Japanese
                case 0x3E:  // Japanese
                    kb_layout = KEYBOARD_LAYOUT_JIS;
                    break;

                case 1:     // US
                default:
                    kb_layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
                    break;
            }

            KeyboardLayoutManager klm(kb_layout, kb_size);

            zone perkey_zone;
            perkey_zone.name       = ZONE_EN_KEYBOARD;
            perkey_zone.type       = ZONE_TYPE_MATRIX;
            perkey_zone.leds_min   = klm.GetKeyCount();
            perkey_zone.leds_max   = klm.GetKeyCount();
            perkey_zone.leds_count = klm.GetKeyCount();

            matrix_map_type* new_map = new matrix_map_type;
            new_map->height          = klm.GetRowCount();
            new_map->width           = klm.GetColumnCount();
            new_map->map             = new unsigned int[new_map->height * new_map->width];
            klm.GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT,
                          new_map->height, new_map->width);
            perkey_zone.matrix_map = new_map;
            zones.push_back(perkey_zone);

            for(unsigned int i = 0; i < klm.GetKeyCount(); i++)
            {
                led new_led;
                std::string key_name = klm.GetKeyNameAt(i);
                new_led.name = key_name;

                /*---------------------------------------------*\
                | Look up zone ID by key name                   |
                \*---------------------------------------------*/
                std::map<std::string, unsigned int>::const_iterator it = hidpp20_key_name_to_zone.find(key_name);
                unsigned int zone_id = (it != hidpp20_key_name_to_zone.end()) ? it->second : 0;
                new_led.value = zone_id;
                leds.push_back(new_led);

                led_to_zone_id.push_back((uint16_t)zone_id);
            }

            /*-------------------------------------------------*\
            | Extras: zones the device reported via paginated   |
            | 0x8081 GetInfo that aren't covered by KLM. On a   |
            | G915 this is media keys, G1-G5, brightness, and   |
            | logo. Append them as a separate linear zone so    |
            | users can still address them.                     |
            \*-------------------------------------------------*/
            std::set<uint16_t> klm_claimed_zones;
            for(uint16_t zid : led_to_zone_id)
            {
                if(zid != 0)
                {
                    klm_claimed_zones.insert(zid);
                }
            }

            /*---------------------------------------------------*\
            | Extras candidates: zones reported by the device     |
            | that aren't claimed by KLM AND have a known name    |
            | in hidpp20_key_name_to_zone. We deliberately DROP   |
            | unnamed zones — firmware-side GetInfo bitmaps       |
            | enumerate phantom/reserved slots (G515 reports      |
            | 47, 97, 99-103, 254 among others) that aren't       |
            | wired to physical LEDs. Exposing them as "LED N"    |
            | created ghost entries in the GUI. Treat             |
            | hidpp20_key_name_to_zone as the curated allowlist.  |
            \*---------------------------------------------------*/
            std::vector<std::pair<uint16_t, std::string>> extras;
            for(uint16_t zid : caps.perkey_zone_ids)
            {
                if(klm_claimed_zones.count(zid) != 0)
                {
                    continue;
                }

                std::string label;
                for(const std::pair<const std::string, unsigned int>& kv : hidpp20_key_name_to_zone)
                {
                    if(kv.second == zid)
                    {
                        label = kv.first;
                        break;
                    }
                }

                if(label.empty())
                {
                    LOG_DEBUG("[LogitechHID++2.0 %s] Dropping unnamed per-key zone %u "
                              "(not in hidpp20_key_name_to_zone)",
                              name.c_str(), (unsigned)zid);
                    continue;
                }

                extras.emplace_back(zid, label);
            }

            if(!extras.empty())
            {
                zone extras_zone;
                extras_zone.name       = "Extras";
                extras_zone.type       = ZONE_TYPE_LINEAR;
                extras_zone.leds_min   = (unsigned int)extras.size();
                extras_zone.leds_max   = (unsigned int)extras.size();
                extras_zone.leds_count = (unsigned int)extras.size();
                extras_zone.matrix_map = nullptr;
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
        }
        else if(const MouseLayout* ml = FindMouseLayout(caps.device_name))
        {
            /*--------------------------------------------------*\
            | Known mouse: use table-defined matrix layout       |
            \*--------------------------------------------------*/
            zone perkey_zone;
            perkey_zone.name       = "Mouse LEDs";
            perkey_zone.type       = ZONE_TYPE_MATRIX;
            perkey_zone.leds_min   = ml->led_count;
            perkey_zone.leds_max   = ml->led_count;
            perkey_zone.leds_count = ml->led_count;

            matrix_map_type* new_map = new matrix_map_type;
            new_map->height          = ml->rows;
            new_map->width           = ml->cols;
            new_map->map             = new unsigned int[ml->rows * ml->cols];
            memcpy(new_map->map, ml->map, ml->rows * ml->cols * sizeof(unsigned int));
            perkey_zone.matrix_map   = new_map;
            zones.push_back(perkey_zone);

            for(unsigned int i = 0; i < ml->led_count && i < caps.perkey_zone_ids.size(); i++)
            {
                led new_led;
                new_led.name  = ml->led_names[i];
                new_led.value = caps.perkey_zone_ids[i];
                leds.push_back(new_led);

                led_to_zone_id.push_back(caps.perkey_zone_ids[i]);
            }
        }
        else
        {
            /*-------------------------------------------------*\
            | Other devices: linear zone with auto-named LEDs   |
            \*-------------------------------------------------*/
            zone perkey_zone;
            perkey_zone.name       = "LEDs";
            perkey_zone.type       = ZONE_TYPE_LINEAR;
            perkey_zone.leds_min   = (unsigned int)caps.perkey_zone_ids.size();
            perkey_zone.leds_max   = (unsigned int)caps.perkey_zone_ids.size();
            perkey_zone.leds_count = (unsigned int)caps.perkey_zone_ids.size();
            perkey_zone.matrix_map = nullptr;
            zones.push_back(perkey_zone);

            for(size_t i = 0; i < caps.perkey_zone_ids.size(); i++)
            {
                led new_led;
                new_led.name  = "LED " + std::to_string(caps.perkey_zone_ids[i]);
                new_led.value = caps.perkey_zone_ids[i];
                leds.push_back(new_led);

                led_to_zone_id.push_back(caps.perkey_zone_ids[i]);
            }
        }
    }
    else if(caps.is_headset_rgb_hostmode)
    {
        /*------------------------------------------------------*\
        | Headset RGB hostmode (0x0620): single linear zone with |
        | one LED per discovered earcup zone ID.                 |
        \*------------------------------------------------------*/
        size_t led_count = caps.headset_rgb_hostmode_zone_ids.size();

        zone headset_zone;
        headset_zone.name       = "Headset";
        headset_zone.type       = ZONE_TYPE_LINEAR;
        headset_zone.leds_min   = (unsigned int)led_count;
        headset_zone.leds_max   = (unsigned int)led_count;
        headset_zone.leds_count = (unsigned int)led_count;
        headset_zone.matrix_map = nullptr;
        zones.push_back(headset_zone);

        for(size_t i = 0; i < led_count; i++)
        {
            led new_led;
            new_led.name  = (i == 0) ? "Left Earcup"
                          : (i == 1) ? "Right Earcup"
                          : "Zone " + std::to_string(i);
            new_led.value = caps.headset_rgb_hostmode_zone_ids[i];
            leds.push_back(new_led);

            led_to_zone_id.push_back(caps.headset_rgb_hostmode_zone_ids[i]);
        }
    }
    else if(caps.has_zone_effects)
    {
        /*------------------------------------------------------*\
        | No per-key: create one zone per cluster                |
        \*------------------------------------------------------*/
        for(size_t i = 0; i < caps.zone_clusters.size(); i++)
        {
            const HIDPP20ZoneCluster& cluster = caps.zone_clusters[i];

            zone new_zone;
            new_zone.name       = zone_location_name(cluster.location);
            new_zone.type       = ZONE_TYPE_SINGLE;
            new_zone.leds_min   = 1;
            new_zone.leds_max   = 1;
            new_zone.leds_count = 1;
            new_zone.matrix_map = nullptr;
            zones.push_back(new_zone);

            led new_led;
            new_led.name  = new_zone.name;
            new_led.value = cluster.index;
            leds.push_back(new_led);

            led_to_zone_id.push_back(cluster.index);
        }
    }

    /*---------------------------------------------------------*\
    | Build the zone_id -> LED index reverse map. Indexed       |
    | 0..255 (zone IDs are bytes); -1 marks "no LED for this    |
    | zone". Used by the FrameEnd commit step to translate the  |
    | acked_zones list back into LED indices for sent_colors.   |
    \*---------------------------------------------------------*/
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

void RGBController_LogitechHIDPP20::ResizeZone(int /*zone*/, int /*new_size*/)
{
}

void RGBController_LogitechHIDPP20::DeviceUpdateLEDs()
{
    if(!controller->IsOnline())
    {
        return;
    }

    /*----------------------------------------------------------*\
    | Ensure SW control is claimed on first actual color push.   |
    | Safe here because we have real colors in the buffer.       |
    \*----------------------------------------------------------*/
    controller->ClaimSWControlIfNeeded();

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    /*----------------------------------------------------------*\
    | Frame handling during SLEEPING:                            |
    |                                                            |
    | Default — suppress frames. A suppressed frame cannot       |
    | wake a device that treats writes as activity, so this is   |
    | the safe choice when we don't know how a particular        |
    | firmware handles host traffic during its fade.             |
    |                                                            |
    | Quirk-gated — devices flagged FADE_ACCEPTS_WRITES opt out  |
    | of suppression because their firmware accepts writes       |
    | without cancelling sleep. Frames flow through SLEEPING     |
    | until deep sleep starts BUSY-NACKing every FrameEnd;       |
    | consecutive-failure tracking then sets deep_sleep and the  |
    | top IsDeepSleep() check takes over.                        |
    |                                                            |
    | Both paths suppress until Wake() clears the state.         |
    \*----------------------------------------------------------*/
    if(controller->IsDeepSleep())
    {
        return;
    }

    if(controller->GetPowerState() == HIDPP20_POWER_SLEEPING
       && !(caps.quirks & HIDPP20_QUIRK_FADE_ACCEPTS_WRITES))
    {
        return;
    }

    /*----------------------------------------------------------*\
    | Feature 0x0620 Headset RGB Hostmode (Centurion G522 /      |
    | PRO X 2). Static-color only, two earcup zones. Bypasses    |
    | per-key, SetZoneEffect, and effect-card paths entirely —   |
    | 0x0620 has none of that. Claim was made once in            |
    | SetHostMode(); we just write colors + FrameEnd[0x01].      |
    \*----------------------------------------------------------*/
    if(caps.is_headset_rgb_hostmode)
    {
        controller->SetHeadsetRGBHostmodeColors(colors);
        return;
    }

    if(caps.has_perkey && (unsigned int)active_mode < modes.size() &&
       modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

        /*------------------------------------------------------*\
        | Detect re-initialization (reconnect, wake from sleep). |
        | Device state is unknown — force full resend.           |
        \*------------------------------------------------------*/
        uint32_t gen = controller->GetInitGeneration();

        if(gen != last_init_gen)
        {
            sent_colors.clear();
            last_init_gen = gen;
        }

        /*-------------------------------------------------------*\
        | Per-key prep call. Two paths, selected by a runtime     |
        | capability probe at feature-discovery time:             |
        |                                                         |
        | (A) Observed prep via DoObservedPerKeyPrep — two        |
        |     SetEffectByIndex calls on 0x8071 cloned from        |
        |     the observed vendor-app wire behavior. The          |
        |     template bytes at                                   |
        |     prep1 params[6..7] and the effectIdx at prep2 are   |
        |     parameterized from device-discovery results         |
        |     (caps.effect_card_template[], and                   |
        |     caps.zone_clusters[0].effects.size() respectively)  |
        |     so the same code adapts to any device that shares   |
        |     the G502-family prep pattern.                       |
        |                                                         |
        |     Gated on caps.has_effect_cards, which is set by     |
        |     DiscoverEffectCards iff the device responds         |
        |     successfully to GetEffectSpecificInfo. Devices      |
        |     without firmware effect cards leave this false      |
        |     and fall through to path (B).                       |
        |                                                         |
        | (B) Static-pass-through prep (original fork behavior,   |
        |     doc-verified on G515) — applied when the device     |
        |     has no effect cards or uses 0x8070 / 0x0600         |
        |     instead of 0x8071. SetEffect cluster=0xFF,          |
        |     effect=Static, RGB=(0,0,0), no fixed-color marker,  |
        |     persist=1.                                          |
        |                                                         |
        | An earlier revision used `effects.size() < 5` as a      |
        | heuristic proxy for "G502-shaped" devices. The proxy    |
        | accidentally correlated with "has effect cards" on      |
        | the two devices we knew about but had no principled     |
        | meaning — it's been replaced with the direct capability |
        | probe.                                                  |
        \*-------------------------------------------------------*/
        bool needs_prep = controller->NeedsPrepSequence();

        if(needs_prep && caps.has_zone_effects)
        {
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
                uint8_t static_effect_idx = 0;

                for(size_t j = 0; j < caps.zone_clusters[0].effects.size(); j++)
                {
                    if(caps.zone_clusters[0].effects[j].effect_id == 0x0001)
                    {
                        static_effect_idx = caps.zone_clusters[0].effects[j].index;
                        break;
                    }
                }

                controller->SetZoneEffect(
                    0xFF,                /* all clusters */
                    static_effect_idx,
                    0x0001,              /* static effect */
                    0, 0, 0,             /* black — no fixed-color marker */
                    0,
                    100,                 /* brightness — unused for static */
                    0,                   /* direction — unused for static */
                    true                 /* persist=true */);
            }
        }

        /*------------------------------------------------------*\
        | Snapshot colors to avoid races with effects updating   |
        | the colors array while we're sending.                  |
        \*------------------------------------------------------*/
        std::vector<RGBColor> snapshot(colors.begin(), colors.end());

        /*-------------------------------------------------------*\
        | Apply dim brightness scaling if not at full brightness. |
        | This modifies the OUTPUT only — the internal colors[]   |
        | buffer stays at full brightness for the animation.      |
        \*-------------------------------------------------------*/
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

        /*------------------------------------------------------*\
        | Compute delta against last committed state.            |
        | First call (sent_colors empty) sends everything.       |
        \*------------------------------------------------------*/
        bool full_update = (sent_colors.size() != snapshot.size());

        std::map<RGBColor, std::vector<uint8_t>> color_to_zones;

        for(size_t i = 0; i < snapshot.size() && i < led_to_zone_id.size(); i++)
        {
            if(led_to_zone_id[i] == 0 || led_to_zone_id[i] > 255)
            {
                continue;
            }

            if(full_update || snapshot[i] != sent_colors[i])
            {
                color_to_zones[snapshot[i]].push_back((uint8_t)led_to_zone_id[i]);
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

        /*------------------------------------------------------*\
        | Batch changed keys for efficient wire encoding.        |
        |                                                        |
        | For same-color groups (>= 2 keys):                     |
        |   Sort zone IDs and find contiguous runs.              |
        |   fn5 (SET_RANGE) for runs of 3+:                      |
        |     [start, end, R, G, B] × 3 per packet               |
        |   fn6 (SET_SINGLE_VALUE) for scattered remainder:      |
        |     [R, G, B, zid, zid, ...] up to 13 per packet       |
        |                                                        |
        | For single-occurrence colors:                          |
        |   fn1 (SET_INDIVIDUAL): [zid,R,G,B] × 4 per packet     |
        \*------------------------------------------------------*/
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

                /*--------------------------------------------------*\
                | Sort zone IDs and extract contiguous runs for fn5  |
                \*--------------------------------------------------*/
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

                /*--------------------------------------------------*\
                | fn5 (SET_RANGE): 3 range entries per packet.       |
                | Track every zone in each packet's ranges so the    |
                | FrameEnd ACK matcher can mark them committed.      |
                \*--------------------------------------------------*/
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

                /*--------------------------------------------------*\
                | fn6 (SET_SINGLE_VALUE) for remaining scattered.    |
                | Track the listed zone IDs in each packet.          |
                \*--------------------------------------------------*/
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
        | A frame is "fully committed" only if FrameEnd ACKed   |
        | AND every attempted zone also ACKed. FrameEnd alone   |
        | is not enough — the firmware happily ACKs FrameEnd    |
        | even when prior per-key writes were silently dropped  |
        | (observed on G502 X PLUS during wireless reconnect    |
        | transients, where the Set* writes return no response  |
        | but FrameEnd still lands cleanly).                    |
        \*-----------------------------------------------------*/
        bool full_commit = commit.frame_end_acked
                         && (commit.acked_zones.size()
                             == commit.attempted_zones.size());

        /*-----------------------------------------------------*\
        | Upgrade SW control flags from 6 → 5 once the per-key  |
        | layer is populated. ClaimSWControlIfNeeded leaves the |
        | device at flags=6 (effect engine still autonomous) to |
        | avoid the onboard→host transition flash; now that the |
        | per-key layer is masking zone output, it's safe (and  |
        | required for idle/wake event generation) to claim the |
        | effect bit. No-op if the upgrade has already happened |
        | or if claim itself hasn't occurred.                   |
        |                                                       |
        | Gated on full_commit: upgrading into flags=5 with an  |
        | empty per-key buffer would leave the firmware with    |
        | nothing to render and expose its default LED buffer   |
        | (warm-white on the G502 X PLUS).                      |
        \*-----------------------------------------------------*/
        if(full_commit)
        {
            controller->UpgradeSwControlAfterFirstPaint();
        }

        /*------------------------------------------------------*\
        | Retry scheduling — ONLY on the critical first paint    |
        | after a fresh claim (needs_prep == true). Streaming    |
        | animation frames regularly partial-commit due to       |
        | fire-and-forget timing, and the delta carry-over       |
        | (HIDPP20_UNCOMMITTED) already handles missed zones on  |
        | the next animation tick. Scheduling retries on every   |
        | partial streaming frame causes the power thread's      |
        | TickRetryPaintIfPending to fire request_repaint_fn     |
        | between animation frames, colliding with the animation |
        | loop and producing visible stalls.                     |
        |                                                        |
        | For the first-paint-after-claim case (needs_prep),     |
        | there IS no "next animation frame" guaranteed, so the  |
        | retry is the only mechanism to recover from a partial  |
        | commit during the reconnect-transient window.          |
        \*------------------------------------------------------*/
        if(full_commit || !needs_prep)
        {
            controller->CancelRetryPaint();
        }
        else
        {
            controller->ScheduleRetryPaint();
        }

        /*-------------------------------------------------------*\
        | Ensure sent_colors is sized to the snapshot before      |
        | the commit loop writes by index. On the first frame     |
        | (or after a reinit clear) sent_colors is empty, and     |
        | the per-zone writes below would silently no-op,         |
        | leaving sent_colors empty and re-firing the prep call   |
        | on every subsequent frame.                              |
        |                                                         |
        | Initial fill is HIDPP20_UNCOMMITTED so any LED that we  |
        | did not touch this frame stays "uncommitted" and gets   |
        | scheduled for the next delta.                           |
        \*-------------------------------------------------------*/
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
            /*----------------------------------------------------*\
            | Frame end ACKed: any zone whose write packet also    |
            | ACKed is now committed — advance sent_colors for     |
            | that LED. Any zone we attempted but never saw an     |
            | ACK for goes to HIDPP20_UNCOMMITTED so the next      |
            | frame's delta picks it up.                           |
            \*----------------------------------------------------*/
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
                    sent_colors[led_idx] = HIDPP20_UNCOMMITTED;
                }
            }
        }
        else
        {
            /*---------------------------------------------------*\
            | Frame end timed out: we don't know what the device  |
            | committed. Mark every attempted LED uncommitted so  |
            | the next frame re-pushes them all. Don't bother     |
            | with the per-zone ACK info here — if FrameEnd       |
            | didn't land, the per-key writes that did ACK still  |
            | sit in the staging buffer un-swapped.               |
            \*---------------------------------------------------*/
            for(uint8_t zid : commit.attempted_zones)
            {
                int led_idx = zone_id_to_led_idx[zid];
                if(led_idx >= 0 && (size_t)led_idx < sent_colors.size())
                {
                    sent_colors[led_idx] = HIDPP20_UNCOMMITTED;
                }
            }
        }

    }
}

void RGBController_LogitechHIDPP20::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechHIDPP20::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LogitechHIDPP20::DeviceUpdateMode()
{
    if(!controller->IsOnline())
    {
        return;
    }

    /*----------------------------------------------------------*\
    | Drop mode changes while the firmware is fading to off.     |
    | The device owns its own power state — we don't force-wake  |
    | it from software. active_mode stays tracked framework-     |
    | side, and the next wake (firmware onUserActivity) or       |
    | reconnect will re-apply it through the reinit callback.    |
    \*----------------------------------------------------------*/
    if(controller->GetPowerState() == HIDPP20_POWER_SLEEPING)
    {
        return;
    }

    /*----------------------------------------------------------*\
    | Claim SW control on first mode set (deferred from init).   |
    \*----------------------------------------------------------*/
    controller->ClaimSWControlIfNeeded();

    const HIDPP20DeviceCapabilities& caps = controller->GetCapabilities();

    if((unsigned int)active_mode >= modes.size())
    {
        return;
    }

    const mode& current = modes[active_mode];

    /*----------------------------------------------------------*\
    | Direct mode: invalidate delta tracking so the next         |
    | DeviceUpdateLEDs sends a full frame with actual colors.    |
    \*----------------------------------------------------------*/
    if(current.name == "Direct")
    {
        sent_colors.clear();
        DeviceUpdateLEDs();

        /*------------------------------------------------------*\
        | Start power manager (reader + power threads) if not    |
        | already running.                                       |
        \*------------------------------------------------------*/
        controller->StartPowerManager();

        if(caps.idx_wireless_status != 0 && !caps.has_power_mgmt)
        {
            controller->StartEventWatcher();
        }

        return;
    }

    sent_colors.clear();

    /*----------------------------------------------------------*\
    | On per-key devices, single-color non-animated modes (Off,  |
    | Static) are applied through the per-key path so the LEDs   |
    | track the mode color cleanly. Animated effects (anything   |
    | with HAS_SPEED — Breathing, Cycle, Wave, Ripple) fall      |
    | through to the zone-effect path below; in practice zone    |
    | effects render correctly alongside per-key on the devices  |
    | we have data for.                                          |
    \*----------------------------------------------------------*/
    if(caps.has_perkey)
    {
        /*------------------------------------------------------*\
        | Off mode via per-key: set all LEDs to black            |
        \*------------------------------------------------------*/
        if(current.value == 0xFF)
        {
            controller->SetAllPerKeyColor(ToRGBColor(0, 0, 0));
            controller->PerKeyFrameEnd();
            return;
        }

        /*------------------------------------------------------*\
        | Static mode via per-key: only used as a fallback when  |
        | the device exposes per-key but no zone effects. When   |
        | both are available we prefer the zone-effect path      |
        | because per-key writes alone don't fully claim against |
        | the firmware effect engine on some devices (G502),     |
        | leaving the firmware fade fighting our per-key colors  |
        | until something else (e.g. Cycle) force-claims.        |
        | Gated on !HAS_SPEED so animated colored effects like   |
        | Breathing don't get clipped to a static color.         |
        \*------------------------------------------------------*/
        if(!caps.has_zone_effects
           && current.color_mode == MODE_COLORS_MODE_SPECIFIC
           && current.colors.size() > 0
           && !(current.flags & MODE_FLAG_HAS_SPEED))
        {
            controller->SetAllPerKeyColor(current.colors[0]);
            controller->PerKeyFrameEnd();
            return;
        }

        /*------------------------------------------------------*\
        | Animated effects (Breathing, Cycle, Wave, Ripple) on   |
        | per-key devices fall through to the zone effect path.  |
        \*------------------------------------------------------*/
    }

    /*----------------------------------------------------------*\
    | Zone effect modes (devices without per-key, or animated    |
    | effects that can't be done via per-key)                    |
    |                                                            |
    | persist branching:                                         |
    |   0x8070: ephemeral by default; becomes persist=true       |
    |           only when DeviceSaveMode has set save_pending.   |
    |   0x8071/0x0600: keeps the pre-existing per-branch         |
    |           hardcoded values (Off=false, Effect=true)        |
    |           pending 0x8071 save research.                    |
    \*----------------------------------------------------------*/
    const bool is_8070 = (caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS);

    /*----------------------------------------------------------*\
    | Off mode: set static black on all clusters                 |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Effect mode: apply to all clusters.                        |
    |                                                            |
    | Color source per cluster:                                  |
    |   MODE_COLORS_PER_LED        — current.colors[i] maps to   |
    |                                caps.zone_clusters[i]       |
    |                                (one LED per cluster on the |
    |                                0x8070 zone path)           |
    |   MODE_COLORS_MODE_SPECIFIC  — current.colors[0] for all   |
    |   MODE_COLORS_NONE           — zero (effect ignores RGB)   |
    \*----------------------------------------------------------*/
    uint16_t period = SpeedSliderToPeriodMs(current.speed);

    /*---------------------------------------------------------*\
    | Brightness defaults to 100 for modes that don't expose    |
    | a brightness slider — those modes ignore the value at the |
    | wire level anyway. Modes flagged HAS_BRIGHTNESS take the  |
    | user-set value from current.brightness.                   |
    \*---------------------------------------------------------*/
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

        if(current.color_mode == MODE_COLORS_PER_LED && i < current.colors.size())
        {
            r = RGBGetRValue(current.colors[i]);
            g = RGBGetGValue(current.colors[i]);
            b = RGBGetBValue(current.colors[i]);
        }
        else if(current.color_mode == MODE_COLORS_MODE_SPECIFIC && !current.colors.empty())
        {
            r = RGBGetRValue(current.colors[0]);
            g = RGBGetGValue(current.colors[0]);
            b = RGBGetBValue(current.colors[0]);
        }

        /*------------------------------------------------------*\
        | Ripple wants a narrower, much faster period range      |
        | (2..200ms) than the breathing/wave baseline of 1..20s. |
        | Both Ripple variants — 0x000B and the saturation       |
        | 0x0017 — use the fast range; this mirrors Solaar's     |
        | LEDEffects table, where only Ripple carries a period   |
        | range override. Cycle (0x0003/0x0015) and Wave         |
        | (0x0004/0x0016) stay on the standard 1..20s range.     |
        \*------------------------------------------------------*/
        uint16_t cluster_period = (eff_id == 0x000B
                                || eff_id == 0x0017)
                                ? RippleSpeedSliderToPeriodMs(current.speed)
                                : period;

        /*-----------------------------------------------------*\
        | 0x8071/0x0600: persist=true matches what the observed |
        | vendor-app wire capture does for every mode-set on    |
        | these                                                 |
        | devices. With persist=false the firmware appears to   |
        | accept the command without actually committing the    |
        | new effect, which is consistent with Static (which    |
        | gets prepped with persist=true at startup) being the  |
        | only effect that visibly works.                       |
        |                                                       |
        | 0x8070: ephemeral (persist=false) on live writes;     |
        | DeviceSaveMode flips save_pending true to replay the  |
        | active mode with persist=true and commit to NVM.      |
        \*-----------------------------------------------------*/
        const bool effect_persist = is_8070 ? save_pending : true;

        controller->SetZoneEffect(
            caps.zone_clusters[i].index,
            current.value,
            eff_id, r, g, b, cluster_period, brightness,
            WaveDirectionToWire(current.direction), effect_persist);
    }

    /*-----------------------------------------------------------*\
    | The zone effects are now committed — safe to upgrade from   |
    | flags=6 to flags=5. Without this, devices that only use     |
    | zone effects (no per-key Direct path) would stay at         |
    | flags=6 forever and the firmware would never send           |
    | onUserActivity events for idle/sleep.                       |
    \*-----------------------------------------------------------*/
    controller->UpgradeSwControlAfterFirstPaint();
}

void RGBController_LogitechHIDPP20::DeviceSaveMode()
{
    /*----------------------------------------------------------*\
    | 0x8071/0x0600 already write persist=true on every mode     |
    | change, so the Save button isn't exposed on those pages    |
    | (MODE_FLAG_MANUAL_SAVE is only set for 0x8070 modes in     |
    | the constructor). If a save ever lands here from those     |
    | pages anyway, nothing needs doing — the active mode is     |
    | already committed to NVM.                                  |
    |                                                            |
    | 0x8070: replay the active mode through DeviceUpdateMode    |
    | with save_pending true so the zone effect writes go out    |
    | with persist=true, committing the currently-live effect    |
    | to flash.                                                  |
    \*----------------------------------------------------------*/
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
    /*-----------------------------------------------------------*\
    | Re-establish the current active_mode on the device. Used    |
    | by the wake path (after SetRgbPowerMode(1) cancels the      |
    | firmware fade) and by the reconnect path (after a wireless  |
    | or USB reconnect). Handles both per-key Direct and zone     |
    | effect modes:                                               |
    |                                                             |
    |   1. Claim SW control (host mode + flags + power mode).     |
    |      Retried internally; returns true iff the final claim   |
    |      ACKed. ReconnectDevice's fast-backoff loop uses this   |
    |      as the accept signal.                                  |
    |   2. Clear sent_colors so the next frame is full-push.      |
    |      Per the 0x8071 lifecycle, the device's LED buffer may  |
    |      not survive mode 3→1 or a full reconnect, so we don't  |
    |      trust it to remember any prior state.                  |
    |   3. Route through DeviceUpdateMode so both per-key Direct  |
    |      (full per-key frame via DeviceUpdateLEDs) and zone     |
    |      effects (SetEffect per cluster) re-establish           |
    |      correctly. Covers the case where the active_mode was   |
    |      changed in the GUI while the device was fading — that  |
    |      mode change was dropped at the time and needs to land  |
    |      here on wake.                                          |
    \*-----------------------------------------------------------*/
    bool claimed = controller->ClaimSWControlIfNeeded();
    sent_colors.clear();
    DeviceUpdateMode();
    return claimed;
}
