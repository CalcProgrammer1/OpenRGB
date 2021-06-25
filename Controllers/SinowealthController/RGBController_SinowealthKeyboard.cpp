/*------------------------------------------*\
|  RGBController_SinowealthKeyboard.cpp      |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include "RGBController_SinowealthKeyboard.h"

#define NA              0xFFFFFFFF

static unsigned int tkl_matrix_map[6][17] =
    { {   8,  NA,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23, 24},
      {  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44, 45},
      {  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65, 66},
      {  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  84,  NA,  NA,  NA, NA},
      {  93,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 106,  NA,  NA,  NA, 107, NA},
      { 113, 114, 115,  NA,  NA,  NA, 118,  NA,  NA,  NA,  NA, 121, 122, 123, 127, 128, 129}};


static const char *led_names_tkl[] =
{
    "Key: Escape",
    "Key: F1",
    "Key: F2",
    "Key: F3",
    "Key: F4",
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
    "Key: Pause",

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
    "Key: Backspace",
    "Key: Insert",
    "Key: Home",
    "Key: Page Up",

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
    "Key: \\",
    "Key: Delete",
    "Key: End",
    "Key: Page Down",

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
    "Key: Enter",

    "Key: Left Shift",
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
    "Key: Right Shift",
    "Key: Up Arrow",

    "Key: Left Control",
    "Key: Left Windows",
    "Key: Left Alt",
    "Key: Space",
    "Key: Right Control",
    "Key: Right Alt",
    "Key: Right Windows",
    "Key: Right Fn",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Right Arrow",
};

RGBController_SinowealthKeyboard::RGBController_SinowealthKeyboard(SinowealthKeyboardController* sinowealth_ptr)
{
    sinowealth  = sinowealth_ptr;

    name        = "Sinowealth Keyboard";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Sinowealth Keyboard";
    location    = sinowealth->GetLocation();
    serial      = sinowealth->GetSerialString();

    mode Static;
    Static.name       = "Static";
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.value      = MODE_STATIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Custom;
    Custom.name       = "Custom";
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    Custom.value      = MODE_PER_KEY;
    modes.push_back(Custom);

    mode Off;
    Off.name       = "Off";
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    Off.value      = MODE_OFF;
    modes.push_back(Off);

    mode Respire;
    Respire.name       = "Respire";
    Respire.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Respire.speed_min  = SPEED_SLOW;
    Respire.speed      = SPEED_NORMAL;
    Respire.speed_max  = SPEED_FASTEST;
    Respire.color_mode = MODE_COLORS_RANDOM;
    Respire.value      = MODE_RESPIRE;
    Respire.colors_min = 1;
    Respire.colors_max = 1;
    Respire.colors.resize(1);
    modes.push_back(Respire);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min  = SPEED_SLOW;
    Rainbow.speed      = SPEED_NORMAL;
    Rainbow.speed_max  = SPEED_FASTEST;
    Rainbow.color_mode = MODE_COLORS_NONE;
    Rainbow.value      = MODE_RAINBOW;
    modes.push_back(Rainbow);

    mode FlashAway;
    FlashAway.name       = "Flash Away";
    FlashAway.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    FlashAway.speed_min  = SPEED_SLOW;
    FlashAway.speed      = SPEED_NORMAL;
    FlashAway.speed_max  = SPEED_FASTEST;
    FlashAway.color_mode = MODE_COLORS_RANDOM;
    FlashAway.value      = MODE_FLASH_AWAY;
    FlashAway.colors_min = 1;
    FlashAway.colors_max = 1;
    FlashAway.colors.resize(1);
    modes.push_back(FlashAway);

    mode Raindrops;
    Raindrops.name       = "Raindrops";
    Raindrops.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Raindrops.speed_min  = SPEED_SLOW;
    Raindrops.speed      = SPEED_NORMAL;
    Raindrops.speed_max  = SPEED_FASTEST;
    Raindrops.color_mode = MODE_COLORS_RANDOM;
    Raindrops.value      = MODE_RAINDROPS;
    Raindrops.colors_min = 1;
    Raindrops.colors_max = 1;
    Raindrops.colors.resize(1);
    modes.push_back(Raindrops);

    mode RainbowWheel;
    RainbowWheel.name       = "Rainbow Wheel";
    RainbowWheel.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RainbowWheel.speed_min  = SPEED_SLOW;
    RainbowWheel.speed      = SPEED_NORMAL;
    RainbowWheel.speed_max  = SPEED_FASTEST;
    RainbowWheel.color_mode = MODE_COLORS_RANDOM;
    RainbowWheel.value      = MODE_RAINBOW_WHEEL;
    RainbowWheel.colors_min = 1;
    RainbowWheel.colors_max = 1;
    RainbowWheel.colors.resize(1);
    modes.push_back(RainbowWheel);

    mode RipplesShining;
    RipplesShining.name       = "Ripples Shining";
    RipplesShining.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RipplesShining.speed_min  = SPEED_SLOW;
    RipplesShining.speed      = SPEED_NORMAL;
    RipplesShining.speed_max  = SPEED_FASTEST;
    RipplesShining.color_mode = MODE_COLORS_RANDOM;
    RipplesShining.value      = MODE_RIPPLES_SHINING;
    RipplesShining.colors_min = 1;
    RipplesShining.colors_max = 1;
    RipplesShining.colors.resize(1);
    modes.push_back(RipplesShining);

    mode StarsTwinkle;
    StarsTwinkle.name       = "Stars Twinkle";
    StarsTwinkle.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    StarsTwinkle.speed_min  = SPEED_SLOW;
    StarsTwinkle.speed      = SPEED_NORMAL;
    StarsTwinkle.speed_max  = SPEED_FASTEST;
    StarsTwinkle.color_mode = MODE_COLORS_RANDOM;
    StarsTwinkle.value      = MODE_STARS_TWINKLE;
    StarsTwinkle.colors_min = 1;
    StarsTwinkle.colors_max = 1;
    StarsTwinkle.colors.resize(1);
    modes.push_back(StarsTwinkle);

    mode ShadowDisappear;
    ShadowDisappear.name       = "Shadow Disappear";
    ShadowDisappear.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ShadowDisappear.speed_min  = SPEED_SLOW;
    ShadowDisappear.speed      = SPEED_NORMAL;
    ShadowDisappear.speed_max  = SPEED_FASTEST;
    ShadowDisappear.color_mode = MODE_COLORS_RANDOM;
    ShadowDisappear.value      = MODE_SHADOW_DISAPPEAR;
    ShadowDisappear.colors_min = 1;
    ShadowDisappear.colors_max = 1;
    ShadowDisappear.colors.resize(1);
    modes.push_back(ShadowDisappear);

    mode RetroSnake;
    RetroSnake.name       = "Retro Snake";
    RetroSnake.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RetroSnake.speed_min  = SPEED_SLOW;
    RetroSnake.speed      = SPEED_NORMAL;
    RetroSnake.speed_max  = SPEED_FASTEST;
    RetroSnake.color_mode = MODE_COLORS_RANDOM;
    RetroSnake.value      = MODE_RETRO_SNAKE;
    RetroSnake.colors_min = 1;
    RetroSnake.colors_max = 1;
    RetroSnake.colors.resize(1);
    modes.push_back(RetroSnake);

    mode NeonStream;
    NeonStream.name       = "Neon Stream";
    NeonStream.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    NeonStream.speed_min  = SPEED_SLOW;
    NeonStream.speed      = SPEED_NORMAL;
    NeonStream.speed_max  = SPEED_FASTEST;
    NeonStream.color_mode = MODE_COLORS_RANDOM;
    NeonStream.value      = MODE_NEON_STREAM;
    NeonStream.colors_min = 1;
    NeonStream.colors_max = 1;
    NeonStream.colors.resize(1);
    modes.push_back(NeonStream);

    mode Reaction;
    Reaction.name       = "Reaction";
    Reaction.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Reaction.speed_min  = SPEED_SLOW;
    Reaction.speed      = SPEED_NORMAL;
    Reaction.speed_max  = SPEED_FASTEST;
    Reaction.color_mode = MODE_COLORS_RANDOM;
    Reaction.value      = MODE_REACTION;
    Reaction.colors_min = 1;
    Reaction.colors_max = 1;
    Reaction.colors.resize(1);
    modes.push_back(Reaction);

    mode SineWave;
    SineWave.name       = "Sine Wave";
    SineWave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    SineWave.speed_min  = SPEED_SLOW;
    SineWave.speed      = SPEED_NORMAL;
    SineWave.speed_max  = SPEED_FASTEST;
    SineWave.color_mode = MODE_COLORS_RANDOM;
    SineWave.value      = MODE_SINE_WAVE;
    SineWave.colors_min = 1;
    SineWave.colors_max = 1;
    SineWave.colors.resize(1);
    modes.push_back(SineWave);

    mode RetinueScanning;
    RetinueScanning.name       = "Retinue Scanning";
    RetinueScanning.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RetinueScanning.speed_min  = SPEED_SLOW;
    RetinueScanning.speed      = SPEED_NORMAL;
    RetinueScanning.speed_max  = SPEED_FASTEST;
    RetinueScanning.color_mode = MODE_COLORS_RANDOM;
    RetinueScanning.value      = MODE_RETINUE_SCANNING;
    RetinueScanning.colors_min = 1;
    RetinueScanning.colors_max = 1;
    RetinueScanning.colors.resize(1);
    modes.push_back(RetinueScanning);

    mode RotatingWindmill;
    RotatingWindmill.name       = "Rotating Windmill";
    RotatingWindmill.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RotatingWindmill.speed_min  = SPEED_SLOW;
    RotatingWindmill.speed      = SPEED_NORMAL;
    RotatingWindmill.speed_max  = SPEED_FASTEST;
    RotatingWindmill.color_mode = MODE_COLORS_RANDOM;
    RotatingWindmill.value      = MODE_ROTATING_WINDMILL;
    RotatingWindmill.colors_min = 1;
    RotatingWindmill.colors_max = 1;
    RotatingWindmill.colors.resize(1);
    modes.push_back(RotatingWindmill);

    mode ColorfulWaterfall;
    ColorfulWaterfall.name       = "Colorful Waterfall";
    ColorfulWaterfall.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    ColorfulWaterfall.speed_min  = SPEED_SLOW;
    ColorfulWaterfall.speed      = SPEED_NORMAL;
    ColorfulWaterfall.speed_max  = SPEED_FASTEST;
    ColorfulWaterfall.color_mode = MODE_COLORS_NONE;
    ColorfulWaterfall.value      = MODE_COLORFUL_WATERFALL;
    modes.push_back(ColorfulWaterfall);

    mode Blossoming;
    Blossoming.name       = "Blossoming";
    Blossoming.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Blossoming.speed_min  = SPEED_SLOW;
    Blossoming.speed      = SPEED_NORMAL;
    Blossoming.speed_max  = SPEED_FASTEST;
    Blossoming.color_mode = MODE_COLORS_NONE;
    Blossoming.value      = MODE_BLOSSOMING;
    modes.push_back(Blossoming);

    mode RotatingStorm;
    RotatingStorm.name       = "Rotating Storm";
    RotatingStorm.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RotatingStorm.speed_min  = SPEED_SLOW;
    RotatingStorm.speed      = SPEED_NORMAL;
    RotatingStorm.speed_max  = SPEED_FASTEST;
    RotatingStorm.color_mode = MODE_COLORS_RANDOM;
    RotatingStorm.value      = MODE_ROTATING_STORM;
    RotatingStorm.colors_min = 1;
    RotatingStorm.colors_max = 1;
    RotatingStorm.colors.resize(1);
    modes.push_back(RotatingStorm);

    mode Collision;
    Collision.name       = "Collision";
    Collision.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Collision.speed_min  = SPEED_SLOW;
    Collision.speed      = SPEED_NORMAL;
    Collision.speed_max  = SPEED_FASTEST;
    Collision.color_mode = MODE_COLORS_RANDOM;
    Collision.value      = MODE_COLLISION;
    Collision.colors_min = 1;
    Collision.colors_max = 1;
    Collision.colors.resize(1);
    modes.push_back(Collision);

    mode Perfect;
    Perfect.name       = "Perfect";
    Perfect.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Perfect.speed_min  = SPEED_SLOW;
    Perfect.speed      = SPEED_NORMAL;
    Perfect.speed_max  = SPEED_FASTEST;
    Perfect.color_mode = MODE_COLORS_RANDOM;
    Perfect.value      = MODE_PERFECT;
    Perfect.colors_min = 1;
    Perfect.colors_max = 1;
    Perfect.colors.resize(1);
    modes.push_back(Perfect);

    SetupZones();
}

RGBController_SinowealthKeyboard::~RGBController_SinowealthKeyboard()
{
    delete sinowealth;
}

void RGBController_SinowealthKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;

    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_min               = 86;
    new_zone.leds_max               = 86;
    new_zone.leds_count             = 86;
    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = 6;
    new_zone.matrix_map->width      = 17;
    new_zone.matrix_map->map        = (unsigned int *)&tkl_matrix_map;

    zones.push_back(new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < 86; led_idx++)
    {
        led new_led;
        new_led.name = led_names_tkl[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SinowealthKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SinowealthKeyboard::DeviceUpdateLEDs()
{
    sinowealth->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SinowealthKeyboard::DeviceUpdateMode()
{
    unsigned int brightness  = BRIGHTNESS_FULL;
    RGBColor* selected_color = (modes[active_mode].color_mode == MODE_COLORS_NONE) ? 0 : &modes[active_mode].colors[0];

    if(modes[active_mode].value == MODE_STATIC)
    {
        sinowealth->SetStaticColor(selected_color);
    }
    else
    {
        sinowealth->SetMode(modes[active_mode].value, brightness, modes[active_mode].speed, modes[active_mode].color_mode, selected_color);
    }
}
