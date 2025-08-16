/*------------------------------------------*\
|  RGBController_SinowealthKeyboard.cpp      |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include "RGBControllerKeyNames.h"
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
    KEY_EN_ESCAPE,
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
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
    "Key: Pause",

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
    KEY_EN_BACKSPACE,
    KEY_EN_INSERT,
    KEY_EN_HOME,
    KEY_EN_PAGE_UP,

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
    KEY_EN_DELETE,
    KEY_EN_END,
    KEY_EN_PAGE_DOWN,

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
    KEY_EN_ANSI_ENTER,

    KEY_EN_LEFT_SHIFT,
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
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_WINDOWS,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
};

/**------------------------------------------------------------------*\
    @name Sinowealth Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_SinowealthKeyboard::RGBController_SinowealthKeyboard(SinowealthKeyboardController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetName();
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Sinowealth Keyboard Device";
    location                        = controller->GetLocation();
    serial                          = controller->GetSerialString();

    mode Static;
    Static.name                     = "Static";
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.value                    = MODE_STATIC;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Custom;
    Custom.name                     = "Custom";
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    Custom.value                    = MODE_PER_KEY;
    modes.push_back(Custom);

    mode Off;
    Off.name                        = "Off";
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    Off.value                       = MODE_OFF;
    modes.push_back(Off);

    mode Respire;
    Respire.name                    = "Respire";
    Respire.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Respire.speed_min               = SPEED_SLOW;
    Respire.speed                   = SPEED_NORMAL;
    Respire.speed_max               = SPEED_FASTEST;
    Respire.color_mode              = MODE_COLORS_RANDOM;
    Respire.value                   = MODE_RESPIRE;
    Respire.colors_min              = 1;
    Respire.colors_max              = 1;
    Respire.colors.resize(1);
    modes.push_back(Respire);

    mode Rainbow;
    Rainbow.name                    = "Rainbow";
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min               = SPEED_SLOW;
    Rainbow.speed                   = SPEED_NORMAL;
    Rainbow.speed_max               = SPEED_FASTEST;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    Rainbow.value                   = MODE_RAINBOW;
    modes.push_back(Rainbow);

    mode FlashAway;
    FlashAway.name                  = "Flash Away";
    FlashAway.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    FlashAway.speed_min             = SPEED_SLOW;
    FlashAway.speed                 = SPEED_NORMAL;
    FlashAway.speed_max             = SPEED_FASTEST;
    FlashAway.color_mode            = MODE_COLORS_RANDOM;
    FlashAway.value                 = MODE_FLASH_AWAY;
    FlashAway.colors_min            = 1;
    FlashAway.colors_max            = 1;
    FlashAway.colors.resize(1);
    modes.push_back(FlashAway);

    mode Raindrops;
    Raindrops.name                  = "Raindrops";
    Raindrops.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Raindrops.speed_min             = SPEED_SLOW;
    Raindrops.speed                 = SPEED_NORMAL;
    Raindrops.speed_max             = SPEED_FASTEST;
    Raindrops.color_mode            = MODE_COLORS_RANDOM;
    Raindrops.value                 = MODE_RAINDROPS;
    Raindrops.colors_min            = 1;
    Raindrops.colors_max            = 1;
    Raindrops.colors.resize(1);
    modes.push_back(Raindrops);

    mode RainbowWheel;
    RainbowWheel.name               = "Rainbow Wheel";
    RainbowWheel.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RainbowWheel.speed_min          = SPEED_SLOW;
    RainbowWheel.speed              = SPEED_NORMAL;
    RainbowWheel.speed_max          = SPEED_FASTEST;
    RainbowWheel.color_mode         = MODE_COLORS_RANDOM;
    RainbowWheel.value              = MODE_RAINBOW_WHEEL;
    RainbowWheel.colors_min         = 1;
    RainbowWheel.colors_max         = 1;
    RainbowWheel.colors.resize(1);
    modes.push_back(RainbowWheel);

    mode RipplesShining;
    RipplesShining.name             = "Ripples Shining";
    RipplesShining.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RipplesShining.speed_min        = SPEED_SLOW;
    RipplesShining.speed            = SPEED_NORMAL;
    RipplesShining.speed_max        = SPEED_FASTEST;
    RipplesShining.color_mode       = MODE_COLORS_RANDOM;
    RipplesShining.value            = MODE_RIPPLES_SHINING;
    RipplesShining.colors_min       = 1;
    RipplesShining.colors_max       = 1;
    RipplesShining.colors.resize(1);
    modes.push_back(RipplesShining);

    mode StarsTwinkle;
    StarsTwinkle.name               = "Stars Twinkle";
    StarsTwinkle.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    StarsTwinkle.speed_min          = SPEED_SLOW;
    StarsTwinkle.speed              = SPEED_NORMAL;
    StarsTwinkle.speed_max          = SPEED_FASTEST;
    StarsTwinkle.color_mode         = MODE_COLORS_RANDOM;
    StarsTwinkle.value              = MODE_STARS_TWINKLE;
    StarsTwinkle.colors_min         = 1;
    StarsTwinkle.colors_max         = 1;
    StarsTwinkle.colors.resize(1);
    modes.push_back(StarsTwinkle);

    mode ShadowDisappear;
    ShadowDisappear.name            = "Shadow Disappear";
    ShadowDisappear.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ShadowDisappear.speed_min       = SPEED_SLOW;
    ShadowDisappear.speed           = SPEED_NORMAL;
    ShadowDisappear.speed_max       = SPEED_FASTEST;
    ShadowDisappear.color_mode      = MODE_COLORS_RANDOM;
    ShadowDisappear.value           = MODE_SHADOW_DISAPPEAR;
    ShadowDisappear.colors_min      = 1;
    ShadowDisappear.colors_max      = 1;
    ShadowDisappear.colors.resize(1);
    modes.push_back(ShadowDisappear);

    mode RetroSnake;
    RetroSnake.name                 = "Retro Snake";
    RetroSnake.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RetroSnake.speed_min            = SPEED_SLOW;
    RetroSnake.speed                = SPEED_NORMAL;
    RetroSnake.speed_max            = SPEED_FASTEST;
    RetroSnake.color_mode           = MODE_COLORS_RANDOM;
    RetroSnake.value                = MODE_RETRO_SNAKE;
    RetroSnake.colors_min           = 1;
    RetroSnake.colors_max           = 1;
    RetroSnake.colors.resize(1);
    modes.push_back(RetroSnake);

    mode NeonStream;
    NeonStream.name                 = "Neon Stream";
    NeonStream.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    NeonStream.speed_min            = SPEED_SLOW;
    NeonStream.speed                = SPEED_NORMAL;
    NeonStream.speed_max            = SPEED_FASTEST;
    NeonStream.color_mode           = MODE_COLORS_RANDOM;
    NeonStream.value                = MODE_NEON_STREAM;
    NeonStream.colors_min           = 1;
    NeonStream.colors_max           = 1;
    NeonStream.colors.resize(1);
    modes.push_back(NeonStream);

    mode Reaction;
    Reaction.name                   = "Reaction";
    Reaction.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Reaction.speed_min              = SPEED_SLOW;
    Reaction.speed                  = SPEED_NORMAL;
    Reaction.speed_max              = SPEED_FASTEST;
    Reaction.color_mode             = MODE_COLORS_RANDOM;
    Reaction.value                  = MODE_REACTION;
    Reaction.colors_min             = 1;
    Reaction.colors_max             = 1;
    Reaction.colors.resize(1);
    modes.push_back(Reaction);

    mode SineWave;
    SineWave.name                   = "Sine Wave";
    SineWave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    SineWave.speed_min              = SPEED_SLOW;
    SineWave.speed                  = SPEED_NORMAL;
    SineWave.speed_max              = SPEED_FASTEST;
    SineWave.color_mode             = MODE_COLORS_RANDOM;
    SineWave.value                  = MODE_SINE_WAVE;
    SineWave.colors_min             = 1;
    SineWave.colors_max             = 1;
    SineWave.colors.resize(1);
    modes.push_back(SineWave);

    mode RetinueScanning;
    RetinueScanning.name            = "Retinue Scanning";
    RetinueScanning.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RetinueScanning.speed_min       = SPEED_SLOW;
    RetinueScanning.speed           = SPEED_NORMAL;
    RetinueScanning.speed_max       = SPEED_FASTEST;
    RetinueScanning.color_mode      = MODE_COLORS_RANDOM;
    RetinueScanning.value           = MODE_RETINUE_SCANNING;
    RetinueScanning.colors_min      = 1;
    RetinueScanning.colors_max      = 1;
    RetinueScanning.colors.resize(1);
    modes.push_back(RetinueScanning);

    mode RotatingWindmill;
    RotatingWindmill.name           = "Rotating Windmill";
    RotatingWindmill.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RotatingWindmill.speed_min      = SPEED_SLOW;
    RotatingWindmill.speed          = SPEED_NORMAL;
    RotatingWindmill.speed_max      = SPEED_FASTEST;
    RotatingWindmill.color_mode     = MODE_COLORS_RANDOM;
    RotatingWindmill.value          = MODE_ROTATING_WINDMILL;
    RotatingWindmill.colors_min     = 1;
    RotatingWindmill.colors_max     = 1;
    RotatingWindmill.colors.resize(1);
    modes.push_back(RotatingWindmill);

    mode ColorfulWaterfall;
    ColorfulWaterfall.name          = "Colorful Waterfall";
    ColorfulWaterfall.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    ColorfulWaterfall.speed_min     = SPEED_SLOW;
    ColorfulWaterfall.speed         = SPEED_NORMAL;
    ColorfulWaterfall.speed_max     = SPEED_FASTEST;
    ColorfulWaterfall.color_mode    = MODE_COLORS_NONE;
    ColorfulWaterfall.value         = MODE_COLORFUL_WATERFALL;
    modes.push_back(ColorfulWaterfall);

    mode Blossoming;
    Blossoming.name                 = "Blossoming";
    Blossoming.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Blossoming.speed_min            = SPEED_SLOW;
    Blossoming.speed                = SPEED_NORMAL;
    Blossoming.speed_max            = SPEED_FASTEST;
    Blossoming.color_mode           = MODE_COLORS_NONE;
    Blossoming.value                = MODE_BLOSSOMING;
    modes.push_back(Blossoming);

    mode RotatingStorm;
    RotatingStorm.name              = "Rotating Storm";
    RotatingStorm.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    RotatingStorm.speed_min         = SPEED_SLOW;
    RotatingStorm.speed             = SPEED_NORMAL;
    RotatingStorm.speed_max         = SPEED_FASTEST;
    RotatingStorm.color_mode        = MODE_COLORS_RANDOM;
    RotatingStorm.value             = MODE_ROTATING_STORM;
    RotatingStorm.colors_min        = 1;
    RotatingStorm.colors_max        = 1;
    RotatingStorm.colors.resize(1);
    modes.push_back(RotatingStorm);

    mode Collision;
    Collision.name                  = "Collision";
    Collision.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Collision.speed_min             = SPEED_SLOW;
    Collision.speed                 = SPEED_NORMAL;
    Collision.speed_max             = SPEED_FASTEST;
    Collision.color_mode            = MODE_COLORS_RANDOM;
    Collision.value                 = MODE_COLLISION;
    Collision.colors_min            = 1;
    Collision.colors_max            = 1;
    Collision.colors.resize(1);
    modes.push_back(Collision);

    mode Perfect;
    Perfect.name                    = "Perfect";
    Perfect.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Perfect.speed_min               = SPEED_SLOW;
    Perfect.speed                   = SPEED_NORMAL;
    Perfect.speed_max               = SPEED_FASTEST;
    Perfect.color_mode              = MODE_COLORS_RANDOM;
    Perfect.value                   = MODE_PERFECT;
    Perfect.colors_min              = 1;
    Perfect.colors_max              = 1;
    Perfect.colors.resize(1);
    modes.push_back(Perfect);

    SetupZones();
}

RGBController_SinowealthKeyboard::~RGBController_SinowealthKeyboard()
{
    delete controller;
}

void RGBController_SinowealthKeyboard::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone new_zone;

    new_zone.name                   = ZONE_EN_KEYBOARD;
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
    controller->SetLEDsDirect(colors);
}

void RGBController_SinowealthKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard::DeviceUpdateMode()
{
    unsigned int brightness  = BRIGHTNESS_FULL;
    RGBColor* selected_color = (modes[active_mode].color_mode == MODE_COLORS_NONE) ? 0 : &modes[active_mode].colors[0];

    if(modes[active_mode].value == MODE_STATIC)
    {
        controller->SetStaticColor(selected_color);
    }
    else
    {
        controller->SetMode(modes[active_mode].value, brightness, modes[active_mode].speed, modes[active_mode].color_mode);
    }
}
