/*------------------------------------------*\
|  RGBController_SinowealthKeyboard16.cpp    |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0016,                   |
|  Hopefully generic for this PID,           |
|  this was made spefically for ZUOYA X51    |
|                                            |
|  Zagorodnikov Aleksey (glooom) 26.07.2021  |
|  based on initial implementation from      |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include "RGBController_SinowealthKeyboard16.h"
#include <algorithm>

#define NA              0xFFFFFFFF

using namespace kbd16;

static unsigned int matrix_map[6][22] =
    { {   0,  NA,  2,   3,   4,   5,   NA,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  NA,  NA,  NA,  NA },
      {  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  NA,  36,  37,  38,  39,  40,  41,  42,  43 },
      {  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  NA,  59,  60,  61,  62,  63,  64,  65 },
      {  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  NA,  79,  NA,  NA,  NA,  NA,  84,  85,  86,  87 },
      {  88,  NA,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  NA,  NA, 102,  NA, 104,  NA, 106, 107, 108, 109 },
      { 110, 111, 112,  NA,  NA, 115,  NA,  NA, 118, 119, 120,  NA, 122,  NA,  NA, 125, 126, 127, 128, 129, 130, 131 }
    };

static const char *led_names_tkl[] =
{
    "Key: Escape",
    "",
    "Key: F1",
    "Key: F2",
    "Key: F3",
    "Key: F4",
    "",
    "Key: F5",
    "Key: F6",
    "Key: F7",
    "Key: F8",
    "Key: F9",
    "Key: F10",
    "Key: F11",
    "Key: F12",
    "Key: Print Screen",
    "Key: Scroll Lock",
    "Key: Pause/Break",
    "",
    "",
    "",
    "",

    "Key: `",
    "Key: 1",
    "Key: 2",
    "Key: 3",
    "Key: 4",
    "Key: 5",
    "Key: 6",
    "Key: 7",
    "Key: 8",
    "Key: 9",
    "Key: 0",
    "Key: -",
    "Key: =",
    "Undefined",
    "Key: Backspace",
    "Key: Insert",
    "Key: Home",
    "Key: Page Up",
    "Key: Num Lock",
    "Key: Number Pad /",
    "Key: Number Pad *",
    "Key: Number Pad -",

    "Key: Tab",
    "Key: Q",
    "Key: W",
    "Key: E",
    "Key: R",
    "Key: T",
    "Key: Y",
    "Key: U",
    "Key: I",
    "Key: O",
    "Key: P",
    "Key: [",
    "Key: ]",
    "Key: \\ (ANSI)",
    "",
    "Key: Delete",
    "Key: End",
    "Key: Page Down",
    "Key: Number Pad 7",
    "Key: Number Pad 8",
    "Key: Number Pad 9",
    "Key: Number Pad +",

    "Key: Caps Lock",
    "Key: A",
    "Key: S",
    "Key: D",
    "Key: F",
    "Key: G",
    "Key: H",
    "Key: J",
    "Key: K",
    "Key: L",
    "Key: ;",
    "Key: '",
    "",
    "Key: Enter",
    "",
    "",
    "",
    "",
    "Key: Number Pad 4",
    "Key: Number Pad 5",
    "Key: Number Pad 6",
    "Key: Number Pad +",

    "Key: Left Shift",
    "",
    "Key: Z",
    "Key: X",
    "Key: C",
    "Key: V",
    "Key: B",
    "Key: N",
    "Key: M",
    "Key: ,",
    "Key: .",
    "Key: /",
    "",
    "",
    "Key: Right Shift",
    "",
    "Key: Up Arrow",
    "",
    "Key: Number Pad 1",
    "Key: Number Pad 2",
    "Key: Number Pad 3",
    "Key: Number Pad Enter",

    "Key: Left Control",
    "Key: Left Windows",
    "Key: Left Alt",
    "",
    "",
    "Key: Space",
    "","",
    "Key: Right Alt",
    "Key: Right Fn",
    "Key: Right Windows",
    "",
    "Key: Right Control",
    "",
    "",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Right Arrow",
    "Key: Number Pad 0",
    "Key: Number Pad 0",
    "Key: Number Pad .",
    "Key: Number Pad Enter"
};

RGBController_SinowealthKeyboard16::RGBController_SinowealthKeyboard16(SinowealthKeyboard16Controller* sinowealth_ptr)
{
    sinowealth  = sinowealth_ptr;

    name        = "Sinowealth Keyboard";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Generic Sinowealth Keyboard";
    location    = sinowealth->GetLocation();
    serial      = sinowealth->GetSerialString();

    std::vector<ModeCfg>      modes_cfg     = sinowealth->GetDeviceModes();
    std::vector<ModeColorCfg> color_presets = sinowealth->GetDeviceColors();

    int mode_id         = 0;

    for(const ModeCfg &cfg : modes_cfg)
    {
        mode Mode       = getModeItem(mode_id);

        Mode.brightness = cfg.brightness;
        Mode.speed      = cfg.speed;
        Mode.direction  = cfg.direction_left;

        if(cfg.color == COLOR_PRESET_RANDOM)
        {
            Mode.color_mode = MODE_COLORS_RANDOM;
        }
        else
        {
            Mode.color_mode = MODE_COLORS_MODE_SPECIFIC;
        }

        ModeColorCfg mode_color = color_presets[mode_id];

        for(unsigned int i = Mode.colors_min; i < Mode.colors_max; i++)
        {
            RGBColor color = (mode_color.preset[i].blue << 16) | (mode_color.preset[i].green << 8) | (mode_color.preset[i].red);
            Mode.colors.push_back(color);
        }

        modes.push_back(Mode);
        mode_id++;
    }

    /*-----------------------------------------------------------------*\
    | This keyboard supports 5 configurable custom profiles.            |
    | The first one is named "Custom" and the others are "Custom 2" to  |
    | "Custom 5".  Leaving the first one just "Custom" to adhere to     |
    | common mode names scheme.                                         |
    \*-----------------------------------------------------------------*/
    for(unsigned int i = 0; i < 5; i++)
    {
        mode PerLed;

        PerLed.name         = "Custom";
        
        if(i > 0)
        {
            PerLed.name    += " " + std::to_string(i + 1);
        }

        PerLed.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
        PerLed.color_mode   = MODE_COLORS_PER_LED;
        PerLed.value        = MODE_PER_KEY1+i;

        modes.push_back(PerLed);
    }

    SetupZones();

    int device_mode = sinowealth->GetCurrentMode();

    if(device_mode >= MODE_PER_KEY1)
    {
        device_mode = mode_id+(device_mode & 0x0F);
        colors = sinowealth->GetPerLedColors();
    }

    active_mode = device_mode;
}

RGBController_SinowealthKeyboard16::~RGBController_SinowealthKeyboard16()
{
    delete sinowealth;
}

mode RGBController_SinowealthKeyboard16::getModeItem(unsigned int mode_id)
{
    mode Mode;

    Mode.value          = mode_id;
    Mode.brightness_min = BRIGHTNESS_OFF;
    Mode.brightness_max = BRIGHTNESS_FULL;
    Mode.speed_min      = SPEED_SLOW;
    Mode.speed_max      = SPEED_FASTEST;
    Mode.colors_min     = 0;
    Mode.colors_max     = COLOR_PRESETS_IN_MODE;

    switch(mode_id)
    {
    case 0:
        Mode.name       = "Off";
        Mode.flags      = 0;
        Mode.color_mode = MODE_COLORS_NONE;
        break;
    case 1:
        Mode.name       = "Spectrum Cycle";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Mode.color_mode = MODE_COLORS_NONE;
        break;
    case 2:
        Mode.name       = "Breathing";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 3:
        Mode.name       = "Static";
        Mode.flags      = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 4:
        Mode.name       = "Ripples Shining";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 5:
        Mode.name       = "Reactive";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 6:
        Mode.name       = "Flash Away";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 7:
        Mode.name       = "Sine Wave";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 8:
        Mode.name       = "Raindrops";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 9:
        Mode.name       = "Rainbow Wave";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 10:
        Mode.name       = "Rainbow Wheel";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 11:
        Mode.name       = "Adorn";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 12:
        Mode.name       = "Stars Twinkle";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 13:
        Mode.name       = "Shadow Disappear";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS |  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    case 14:
        Mode.name       = "Retro Snake";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS |  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    default:
        Mode.name       = "Unknown Mode";
        Mode.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS |  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_PER_LED_COLOR;
        Mode.color_mode = MODE_COLORS_RANDOM;
        break;
    }

    return Mode;
}

void RGBController_SinowealthKeyboard16::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;

    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_count             = sinowealth->GetLEDCount();
    new_zone.leds_min               = new_zone.leds_count;
    new_zone.leds_max               = new_zone.leds_count;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = 6;
    new_zone.matrix_map->width      = 22;
    new_zone.matrix_map->map        = (unsigned int *)&matrix_map;

    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name = led_names_tkl[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SinowealthKeyboard16::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SinowealthKeyboard16::DeviceUpdateLEDs()
{
    sinowealth->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard16::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard16::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard16::SetCustomMode()
{
    /*---------------------------------------------------------*\
    | Search the mode list and find Custom mode                 |
    \*---------------------------------------------------------*/
    for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        if(modes[mode_idx].name == "Custom")
        {
            active_mode = mode_idx;
            break;
        }
    }
}

void RGBController_SinowealthKeyboard16::DeviceUpdateMode()
{
    mode ActiveMode = modes[active_mode];

    int color_mode;

    if(ActiveMode.color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        color_mode = COLOR_PRESET_0;
    }
    else
    {
        color_mode = COLOR_PRESET_RANDOM;
    }

    sinowealth->ClearMode();

    if(ActiveMode.colors.size())
    {
        sinowealth->SetColorsForMode(ActiveMode.value, &ActiveMode.colors[0]);
    }

    sinowealth->SetMode(ActiveMode.value, ActiveMode.brightness, ActiveMode.speed, ActiveMode.direction, color_mode);

    if(ActiveMode.value >= MODE_PER_KEY1)
    {
        colors = sinowealth->GetPerLedColors();
    }
    else
    {
        if(color_mode == COLOR_PRESET_RANDOM)
        {
            std::generate(colors.begin(), colors.end(), std::rand);
        }
        else
        {
            std::fill(colors.begin(), colors.end(), ActiveMode.colors[0]);
        }
    }
    
    SignalUpdate();
}
