/*---------------------------------------------------------------------*\
|  RGBController_AsusAuraCoreLaptop.cpp                                 |
|                                                                       |
|  Driver for AsusAuraCoreLaptop USB Controller                         |
|                                                                       |
|  Chris M (Dr_No)          28 Jul 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include "RGBController_AsusAuraCoreLaptop.h"

static unsigned int matrix_map[ASUSAURACORELAPTOP_KEY_HEIGHT][ASUSAURACORELAPTOP_KEY_WIDTH] =
{
    {  NA,  NA,  86,  87,  88,  89,  90,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
    {   0,  NA,   1,   2,   3,   4,  NA,   5,   6,   7,   8,  NA,   9,  10,  11,  12,  13,  NA },
    {  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  NA },
    {  31,  NA,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  NA,  45,  NA },
    {  46,  NA,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  NA,  58,  NA,  59,  NA },
    {  60,  NA,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  NA,  72,  NA,  73,  NA },
    {  74,  75,  76,  77,  NA,  NA,  NA,  78,  NA,  NA,  NA,  79,  80,  81,  82,  83,  84,  85 }
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,              //00
    KEY_EN_F1,
    KEY_EN_F2,
    KEY_EN_F3,
    KEY_EN_F4,
    KEY_EN_F5,
    KEY_EN_F6,
    KEY_EN_F7,
    KEY_EN_F8,
    KEY_EN_F9,
    KEY_EN_F10,                 //10
    KEY_EN_F11,
    KEY_EN_F12,
    KEY_EN_DELETE,

    KEY_EN_BACK_TICK,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,                   //20
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_BACKSPACE,
    KEY_EN_BACKSPACE,
    KEY_EN_MEDIA_PLAY_PAUSE,    //30

    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,                   //40
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_MEDIA_STOP,

    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,                   //50
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_MEDIA_PREVIOUS,

    KEY_EN_LEFT_SHIFT,          //60
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,       //70
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_MEDIA_NEXT,

    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_FUNCTION,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_CONTROL,       //80
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_PRINT_SCREEN,
    "Asus Keystone",

    KEY_EN_MEDIA_VOLUME_DOWN,
    KEY_EN_MEDIA_VOLUME_UP,
    "Key: Mic On/Off",
    "Key: HyperFan",
    "Key: Armoury Crate",       //90

    "Lightbar LED 1",
    "Lightbar LED 2",
    "Lightbar LED 3",
    "Lightbar LED 4",
    "Lightbar LED 5",
    "Lightbar LED 6",

    "Logo",
    "Lid Left",
    "Lid Right",                //99
};

/**------------------------------------------------------------------*\
    @name AsusAuraCoreLaptop
    @category DEVICE_TYPE_KEYBOARD
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraCoreLaptopControllers
    @comment Power profiles for this controller are set to `On` for all power
        state and scan be adjusted in the JSON config file.

        For each zone available LEDs can be set as `On = true` or `Off = false` when
        * Booting
        * Awake (Normal Usage)
        * Sleeping
        * Shutdown / Power Off
\*-------------------------------------------------------------------*/

RGBController_AsusAuraCoreLaptop::RGBController_AsusAuraCoreLaptop(AsusAuraCoreLaptopController *controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Asus Aura Core Laptop";
    vendor                      = "Asus";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = controller->GetDeviceName();
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ASUSAURACORELAPTOP_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASUSAURACORELAPTOP_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min       = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Static.brightness_max       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Static.brightness           = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Static.speed_min            = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Static.speed_max            = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.speed                = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASUSAURACORELAPTOP_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(Breathing.colors_min);
    Breathing.brightness_min    = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Breathing.brightness_max    = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Breathing.brightness        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Breathing.speed_min         = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Breathing.speed_max         = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = ASUSAURACORELAPTOP_MODE_FLASHING;
    Flashing.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.colors.resize(Flashing.colors_max);
    Flashing.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Flashing.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Flashing.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Flashing.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Flashing.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Flashing);

    mode Spectrum;
    Spectrum.name               = "Spectrum Cycle";
    Spectrum.value              = ASUSAURACORELAPTOP_MODE_SPECTRUM;
    Spectrum.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Spectrum.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Spectrum.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Spectrum.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Spectrum);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = ASUSAURACORELAPTOP_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Rainbow.brightness_min      = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rainbow.brightness          = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rainbow.speed_min           = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Rainbow.speed_max           = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed               = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Rainbow);

    mode Starry;
    Starry.name                 = "Starry Night";
    Starry.value                = ASUSAURACORELAPTOP_MODE_STARRY_NIGHT;
    Starry.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Starry.colors_min           = 1;
    Starry.colors_max           = 2;
    Starry.colors.resize(Starry.colors_min);
    Starry.brightness_min       = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Starry.brightness_max       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Starry.brightness           = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Starry.speed_min            = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Starry.speed_max            = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Starry.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Starry.speed                = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Starry);

    mode Rain;
    Rain.name                   = "Rain";
    Rain.value                  = ASUSAURACORELAPTOP_MODE_RAIN;
    Rain.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rain.brightness_min         = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Rain.brightness_max         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rain.brightness             = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Rain.speed_min              = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Rain.speed_max              = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Rain.color_mode             = MODE_COLORS_NONE;
    Rain.speed                  = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Rain);

    mode ReactFade;
    ReactFade.name              = "Reactive - Fade";
    ReactFade.value             = ASUSAURACORELAPTOP_MODE_REACT_FADE;
    ReactFade.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactFade.colors_min        = 1;
    ReactFade.colors_max        = 1;
    ReactFade.colors.resize(ReactFade.colors_max);
    ReactFade.brightness_min    = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactFade.brightness_max    = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactFade.brightness        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactFade.speed_min         = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactFade.speed_max         = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactFade.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    ReactFade.speed             = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactFade);

    mode ReactLaser;
    ReactLaser.name             = "Reactive - Laser";
    ReactLaser.value            = ASUSAURACORELAPTOP_MODE_REACT_LASER;
    ReactLaser.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactLaser.colors_min       = 1;
    ReactLaser.colors_max       = 1;
    ReactLaser.colors.resize(ReactLaser.colors_max);
    ReactLaser.brightness_min   = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactLaser.brightness_max   = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactLaser.brightness       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactLaser.speed_min        = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactLaser.speed_max        = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactLaser.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    ReactLaser.speed            = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactLaser);

    mode ReactRipple;
    ReactRipple.name            = "Reactive - Ripple";
    ReactRipple.value           = ASUSAURACORELAPTOP_MODE_REACT_RIPPLE;
    ReactRipple.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ReactRipple.colors_min      = 1;
    ReactRipple.colors_max      = 1;
    ReactRipple.colors.resize(ReactRipple.colors_max);
    ReactRipple.brightness_min  = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    ReactRipple.brightness_max  = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactRipple.brightness      = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    ReactRipple.speed_min       = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    ReactRipple.speed_max       = ASUSAURACORELAPTOP_SPEED_FASTEST;
    ReactRipple.color_mode      = MODE_COLORS_MODE_SPECIFIC;
    ReactRipple.speed           = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(ReactRipple);

    mode Comet;
    Comet.name                  = "Comet";
    Comet.value                 = ASUSAURACORELAPTOP_MODE_COMET;
    Comet.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    Comet.colors_min            = 1;
    Comet.colors_max            = 1;
    Comet.colors.resize(Comet.colors_max);
    Comet.brightness_min        = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Comet.brightness_max        = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Comet.brightness            = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Comet.speed_min             = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Comet.speed_max             = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Comet.speed                 = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Comet);

    mode FlashNDash;
    FlashNDash.name             = "Flash N Dash";
    FlashNDash.value            = ASUSAURACORELAPTOP_MODE_FLASHNDASH;
    FlashNDash.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    FlashNDash.colors_min       = 1;
    FlashNDash.colors_max       = 1;
    FlashNDash.colors.resize(FlashNDash.colors_max);
    FlashNDash.brightness_min   = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    FlashNDash.brightness_max   = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    FlashNDash.brightness       = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    FlashNDash.speed_min        = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    FlashNDash.speed_max        = ASUSAURACORELAPTOP_SPEED_FASTEST;
    FlashNDash.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    FlashNDash.speed            = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(FlashNDash);

    mode Keystone;
    Keystone.name               = "Keystone";
    Keystone.value              = ASUSAURACORELAPTOP_MODE_KEYSTONE;
    Keystone.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Keystone.colors_min         = 1;
    Keystone.colors_max         = 1;
    Keystone.colors.resize(Keystone.colors_max);
    Keystone.brightness_min     = ASUSAURACORELAPTOP_BRIGHTNESS_MIN;
    Keystone.brightness_max     = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Keystone.brightness         = ASUSAURACORELAPTOP_BRIGHTNESS_MAX;
    Keystone.speed_min          = ASUSAURACORELAPTOP_SPEED_SLOWEST;
    Keystone.speed_max          = ASUSAURACORELAPTOP_SPEED_FASTEST;
    Keystone.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Keystone.speed              = ASUSAURACORELAPTOP_SPEED_NORMAL;
    modes.push_back(Keystone);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ASUSAURACORELAPTOP_MODE_OFF;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    Init_Controller();
    SetupZones();
}

RGBController_AsusAuraCoreLaptop::~RGBController_AsusAuraCoreLaptop()
{
    delete controller;
}

void RGBController_AsusAuraCoreLaptop::Init_Controller()
{
    /*-------------------------------------------------*\
    | Create the keyboard zone and add the matix map    |
    \*-------------------------------------------------*/
    zone KB_zone;
    KB_zone.name                = "Keyboard Zone";
    KB_zone.type                = ZONE_TYPE_MATRIX;
    KB_zone.leds_min            = ASUSAURACORELAPTOP_KEYCOUNT;
    KB_zone.leds_max            = ASUSAURACORELAPTOP_KEYCOUNT;
    KB_zone.leds_count          = ASUSAURACORELAPTOP_KEYCOUNT;

    KB_zone.matrix_map          = new matrix_map_type;
    KB_zone.matrix_map->height  = ASUSAURACORELAPTOP_KEY_HEIGHT;
    KB_zone.matrix_map->width   = ASUSAURACORELAPTOP_KEY_WIDTH;
    KB_zone.matrix_map->map     = (unsigned int *)&matrix_map;
    zones.push_back(KB_zone);

    zone lightbar;
    lightbar.name               = "Lightbar Zone";
    lightbar.type               = ZONE_TYPE_LINEAR;
    lightbar.leds_min           = ASUSAURACORELAPTOP_LIGHTBARCOUNT;
    lightbar.leds_max           = ASUSAURACORELAPTOP_LIGHTBARCOUNT;
    lightbar.leds_count         = ASUSAURACORELAPTOP_LIGHTBARCOUNT;
    zones.push_back(lightbar);

    zone lid;
    lid.name                    = "Lid Zone";
    lid.type                    = ZONE_TYPE_LINEAR;
    lid.leds_min                = ASUSAURACORELAPTOP_LIDCOUNT;
    lid.leds_max                = ASUSAURACORELAPTOP_LIDCOUNT;
    lid.leds_count              = ASUSAURACORELAPTOP_LIDCOUNT;
    zones.push_back(lid);
}

void RGBController_AsusAuraCoreLaptop::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        int zone_offset = leds.size();

        for(unsigned int led_index = 0; led_index < zones[zone_index].leds_count; led_index++)
        {
            led new_led;
            new_led.value   = led_index + zone_offset;
            new_led.name    = led_names[new_led.value];
            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_AsusAuraCoreLaptop::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusAuraCoreLaptop::DeviceUpdateLEDs()
{
    controller->SetLedsDirect(colors);
}

void RGBController_AsusAuraCoreLaptop::UpdateZoneLEDs(int zone)
{
    std::vector<RGBColor> colour;
    for(size_t i = 0; i < zones[zone].leds_count; i++)
    {
        colour.push_back(zones[zone].colors[i]);
    }

    controller->SetLedsDirect(colour);
}

void RGBController_AsusAuraCoreLaptop::UpdateSingleLED(int led)
{
    std::vector<RGBColor> colour;
    colour.push_back(colors[led]);

    controller->SetLedsDirect(colour);
}

void RGBController_AsusAuraCoreLaptop::DeviceUpdateMode()
{
    mode set_mode       = modes[active_mode];

    if(set_mode.value == ASUSAURACORELAPTOP_MODE_DIRECT)
    {
        return;
    }
    uint8_t random      = (set_mode.color_mode == MODE_COLORS_RANDOM) ? 0xFF : 0;
    RGBColor color1     = (set_mode.colors.size() > 0) ? set_mode.colors[0] : 0;
    RGBColor color2     = (set_mode.colors.size() > 1) ? set_mode.colors[1] : 0;

    controller->SetMode(set_mode.value, set_mode.speed, set_mode.brightness, color1, color2, random, set_mode.direction );
}
