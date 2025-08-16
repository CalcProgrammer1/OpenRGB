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

#include "RGBControllerKeyNames.h"
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
    KEY_EN_ESCAPE,
    KEY_EN_UNUSED,
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_UNUSED,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_PRINT_SCREEN,
    KEY_EN_SCROLL_LOCK,
    KEY_EN_PAUSE_BREAK,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,

    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_UNUSED,
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,
    KEY_EN_NUMPAD_LOCK,
    KEY_EN_NUMPAD_DIVIDE,
    KEY_EN_NUMPAD_TIMES,
    KEY_EN_NUMPAD_MINUS,

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_UNUSED,
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,
    KEY_EN_NUMPAD_7,
    KEY_EN_NUMPAD_8,
    KEY_EN_NUMPAD_9,
    KEY_EN_NUMPAD_PLUS,

    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_UNUSED,
    KEY_EN_ANSI_ENTER,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    KEY_EN_NUMPAD_PLUS,

    KEY_EN_LEFT_SHIFT,
    KEY_EN_UNUSED,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UNUSED,
    KEY_EN_UP_ARROW,
    KEY_EN_UNUSED,
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_ENTER,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_SPACE,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_UNUSED,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_UNUSED,
    KEY_EN_UNUSED,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_0,
    KEY_EN_NUMPAD_PERIOD,
    KEY_EN_NUMPAD_ENTER
};

/**------------------------------------------------------------------*\
    @name Sinowealth Keyboard 16
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_SinowealthKeyboard16::RGBController_SinowealthKeyboard16(SinowealthKeyboard16Controller* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetName();
    type                = DEVICE_TYPE_KEYBOARD;
    description         = "Sinowealth Keyboard Device";
    location            = controller->GetLocation();
    serial              = controller->GetSerialString();

    std::vector<ModeCfg>      modes_cfg     = controller->GetDeviceModes();
    std::vector<ModeColorCfg> color_presets = controller->GetDeviceColors();

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

    int device_mode = controller->GetCurrentMode();

    if(device_mode >= MODE_PER_KEY1)
    {
        device_mode = mode_id+(device_mode & 0x0F);
        colors = controller->GetPerLedColors();
    }

    active_mode = device_mode;
}

RGBController_SinowealthKeyboard16::~RGBController_SinowealthKeyboard16()
{
    delete controller;
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

    new_zone.name                   = ZONE_EN_KEYBOARD;
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_count             = controller->GetLEDCount();
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
    controller->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard16::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard16::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
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

    controller->ClearMode();

    if(ActiveMode.colors.size())
    {
        controller->SetColorsForMode(ActiveMode.value, &ActiveMode.colors[0]);
    }

    controller->SetMode(ActiveMode.value, ActiveMode.brightness, ActiveMode.speed, ActiveMode.direction, color_mode);

    if(ActiveMode.value >= MODE_PER_KEY1)
    {
        colors = controller->GetPerLedColors();
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
