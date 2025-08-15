/*---------------------------------------------------------*\
| RGBController_ZETBladeOptical.cpp                         |
|                                                           |
|   RGBController for ZET Blade                             |
|                                                           |
|   Based on HyperX Alloy Elite2 implementation by          |
|   KundaPanda                                              |
|                                                           |
|   Moon_darker (Vaker)                         23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_ZETBladeOptical.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[ZET_BLADE_OPTICAL_ROWS][ZET_BLADE_OPTICAL_COLUMNS] =
{
    {   0,  NA,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  NA,  13,  14,  15,  NA,  NA,  NA,  NA }, // Skipped: 1, 17, 18, 19, 20
    {  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  NA,  30,  31,  32,  33,  34,  35,  36 },
    {  37,  NA,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57 },
    {  58,  NA,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  NA,  NA,  NA,  NA,  71,  72,  73,  NA }, // Skipped: 75, 77, 78, 79, 83
    {  74,  NA,  NA,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  NA,  NA,  86,  NA,  87,  88,  89,  90 }, // Skipped: 85, 96, 98, 100
    {  91,  92,  93,  NA,  NA,  NA,  NA,  94,  NA,  NA,  NA,  95,  96,  97,  98,  99, 100, 101, 102,  NA, 103,  NA }  // Skipped: 108, 109, 111, 112, 113, 116, 123, 125
};

static const char* zone_names[] =
{
    ZONE_EN_KEYBOARD,
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
};

static const unsigned int zone_sizes[] =
{
    104,
};

static const char *led_names[] =
{
    KEY_EN_ESCAPE,
    // Skip index 1
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
    KEY_EN_PAUSE_BREAK,
    // Skip index 17
    // Skip index 18
    // Skip index 19
    // Skip index 20
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
    // Skip index 75
    KEY_EN_ANSI_ENTER,
    // Skip index 77
    // Skip index 78
    // Skip index 79
    KEY_EN_NUMPAD_4,
    KEY_EN_NUMPAD_5,
    KEY_EN_NUMPAD_6,
    // Skip index 83
    KEY_EN_LEFT_SHIFT,
    // Skip index 85
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
    // Skip index 96
    KEY_EN_RIGHT_SHIFT,
    // Skip index 98
    KEY_EN_UP_ARROW,
    // Skip index 100
    KEY_EN_NUMPAD_1,
    KEY_EN_NUMPAD_2,
    KEY_EN_NUMPAD_3,
    KEY_EN_NUMPAD_ENTER,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    // Skip index 108
    // Skip index 109
    KEY_EN_SPACE,
    // Skip index 111
    // Skip index 112
    // Skip index 113
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    // Skip index 116
    KEY_EN_MENU,
    KEY_EN_RIGHT_CONTROL,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
    KEY_EN_NUMPAD_0,
    // Skip index 123
    KEY_EN_NUMPAD_PERIOD,
};

/**------------------------------------------------------------------*\
    @name Zet Blade Optical
    @category Keyboard
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectZETBladeOptical
    @comment
\*-------------------------------------------------------------------*/

RGBController_ZETBladeOptical::RGBController_ZETBladeOptical(ZETBladeOpticalController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "ZET";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "ZET Blade Optical Keyboard Device";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    mode Custom;
    Custom.name                     = "Custom";
    Custom.value                    = ZET_BLADE_OPTICAL_MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    Custom.brightness_min           = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Custom.brightness_max           = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Custom.brightness               = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    modes.push_back(Custom);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = ZET_BLADE_OPTICAL_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min               = 1;
    Static.colors_max               = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min           = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Static.brightness_max           = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Static.brightness               = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Static.speed                    = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = ZET_BLADE_OPTICAL_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode            = MODE_COLORS_RANDOM;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness_min        = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Breathing.brightness_max        = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Breathing.brightness            = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Breathing.speed_min             = ZET_BLADE_OPTICAL_SPEED_MIN;
    Breathing.speed_max             = ZET_BLADE_OPTICAL_SPEED_MAX;
    Breathing.speed                 = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Breathing);

    mode OnPress;
    OnPress.name                    = "Reactive";
    OnPress.value                   = ZET_BLADE_OPTICAL_MODE_ON_PRESS;
    OnPress.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    OnPress.color_mode              = MODE_COLORS_RANDOM;
    OnPress.colors_min              = 1;
    OnPress.colors_max              = 1;
    OnPress.colors.resize(OnPress.colors_max);
    OnPress.brightness_min          = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    OnPress.brightness_max          = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    OnPress.brightness              = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    OnPress.speed_min               = ZET_BLADE_OPTICAL_SPEED_MIN;
    OnPress.speed_max               = ZET_BLADE_OPTICAL_SPEED_MAX;
    OnPress.speed                   = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(OnPress);

    mode Raindrop;
    Raindrop.name                   = "Reactive Raindrop";
    Raindrop.value                  = ZET_BLADE_OPTICAL_MODE_RAINDROP;
    Raindrop.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Raindrop.color_mode             = MODE_COLORS_RANDOM;
    Raindrop.colors_min             = 1;
    Raindrop.colors_max             = 1;
    Raindrop.colors.resize(Raindrop.colors_max);
    Raindrop.brightness_min         = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Raindrop.brightness_max         = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Raindrop.brightness             = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Raindrop.speed_min              = ZET_BLADE_OPTICAL_SPEED_MIN;
    Raindrop.speed_max              = ZET_BLADE_OPTICAL_SPEED_MAX;
    Raindrop.speed                  = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Raindrop);

    mode Ripple;
    Ripple.name                     = "Reactive Ripple";
    Ripple.value                    = ZET_BLADE_OPTICAL_MODE_RIPPLE;
    Ripple.flags                    = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode               = MODE_COLORS_RANDOM;
    Ripple.colors_min               = 1;
    Ripple.colors_max               = 1;
    Ripple.colors.resize(Ripple.colors_max);
    Ripple.brightness_min           = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Ripple.brightness_max           = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Ripple.brightness               = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Ripple.speed_min                = ZET_BLADE_OPTICAL_SPEED_MIN;
    Ripple.speed_max                = ZET_BLADE_OPTICAL_SPEED_MAX;
    Ripple.speed                    = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Ripple);

    mode Laser;
    Laser.name                      = "Reactive Laser";
    Laser.value                     = ZET_BLADE_OPTICAL_MODE_LASER;
    Laser.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Laser.color_mode                = MODE_COLORS_RANDOM;
    Laser.colors_min                = 1;
    Laser.colors_max                = 1;
    Laser.colors.resize(Laser.colors_max);
    Laser.brightness_min            = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Laser.brightness_max            = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Laser.brightness                = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Laser.speed_min                 = ZET_BLADE_OPTICAL_SPEED_MIN;
    Laser.speed_max                 = ZET_BLADE_OPTICAL_SPEED_MAX;
    Laser.speed                     = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Laser);

    mode Waves;
    Waves.name                      = "Waves";
    Waves.value                     = ZET_BLADE_OPTICAL_MODE_WAVES;
    Waves.flags                     = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Waves.color_mode                = MODE_COLORS_RANDOM;
    Waves.colors_min                = 1;
    Waves.colors_max                = 1;
    Waves.colors.resize(Waves.colors_max);
    Waves.brightness_min            = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Waves.brightness_max            = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Waves.brightness                = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Waves.speed_min                 = ZET_BLADE_OPTICAL_SPEED_MIN;
    Waves.speed_max                 = ZET_BLADE_OPTICAL_SPEED_MAX;
    Waves.speed                     = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Waves);

    mode Rain;
    Rain.name                       = "Rain";
    Rain.value                      = ZET_BLADE_OPTICAL_MODE_RAIN;
    Rain.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.color_mode                 = MODE_COLORS_NONE;
    Rain.brightness_min             = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Rain.brightness_max             = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Rain.brightness                 = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Rain.speed_min                  = ZET_BLADE_OPTICAL_SPEED_MIN;
    Rain.speed_max                  = ZET_BLADE_OPTICAL_SPEED_MAX;
    Rain.speed                      = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Rain);

    mode Spectrum;
    Spectrum.name                   = "Spectrum Cycle";
    Spectrum.value                  = ZET_BLADE_OPTICAL_MODE_SPECTRUM;
    Spectrum.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.color_mode             = MODE_COLORS_NONE;
    Spectrum.brightness_min         = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Spectrum.brightness_max         = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Spectrum.brightness             = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Spectrum.speed_min              = ZET_BLADE_OPTICAL_SPEED_MIN;
    Spectrum.speed_max              = ZET_BLADE_OPTICAL_SPEED_MAX;
    Spectrum.speed                  = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Spectrum);

    mode SurfingRight; // Now, you might be thinking... it'd be better to use .direction here, right? WRONG! There is no Surfing Left /._.
    SurfingRight.name               = "Rainbow Wave";
    SurfingRight.value              = ZET_BLADE_OPTICAL_MODE_SURFING_RIGHT;
    SurfingRight.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingRight.color_mode         = MODE_COLORS_NONE;
    SurfingRight.brightness_min     = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingRight.brightness_max     = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingRight.brightness         = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingRight.speed_min          = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingRight.speed_max          = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingRight.speed              = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingRight);

    mode SurfingCenter;
    SurfingCenter.name              = "Rainbow Wave Center";
    SurfingCenter.value             = ZET_BLADE_OPTICAL_MODE_SURFING_CENTER;
    SurfingCenter.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingCenter.color_mode        = MODE_COLORS_NONE;
    SurfingCenter.brightness_min    = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingCenter.brightness_max    = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingCenter.brightness        = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingCenter.speed_min         = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingCenter.speed_max         = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingCenter.speed             = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingCenter);

    mode SurfingCross;
    SurfingCross.name               = "Rainbow Wave Cross";
    SurfingCross.value              = ZET_BLADE_OPTICAL_MODE_SURFING_CROSS;
    SurfingCross.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingCross.color_mode         = MODE_COLORS_NONE;
    SurfingCross.brightness_min     = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingCross.brightness_max     = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingCross.brightness         = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingCross.speed_min          = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingCross.speed_max          = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingCross.speed              = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingCross);

    mode RotateMarquee;
    RotateMarquee.name              = "Vortex";
    RotateMarquee.value             = ZET_BLADE_OPTICAL_MODE_ROTATE_MARQUEE;
    RotateMarquee.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RotateMarquee.color_mode        = MODE_COLORS_NONE;
    RotateMarquee.brightness_min    = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    RotateMarquee.brightness_max    = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    RotateMarquee.brightness        = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    RotateMarquee.speed_min         = ZET_BLADE_OPTICAL_SPEED_MIN;
    RotateMarquee.speed_max         = ZET_BLADE_OPTICAL_SPEED_MAX;
    RotateMarquee.speed             = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(RotateMarquee);

    mode Traffic;
    Traffic.name                    = "Traffic";
    Traffic.value                   = ZET_BLADE_OPTICAL_MODE_TRAFFIC;
    Traffic.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Traffic.color_mode              = MODE_COLORS_NONE;
    Traffic.brightness_min          = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Traffic.brightness_max          = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Traffic.brightness              = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Traffic.speed_min               = ZET_BLADE_OPTICAL_SPEED_MIN;
    Traffic.speed_max               = ZET_BLADE_OPTICAL_SPEED_MAX;
    Traffic.speed                   = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Traffic);

    mode Gradient;
    Gradient.name                   = "Gradient";
    Gradient.value                  = ZET_BLADE_OPTICAL_MODE_GRADIENT;
    Gradient.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Gradient.color_mode             = MODE_COLORS_NONE;
    Gradient.brightness_min         = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Gradient.brightness_max         = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Gradient.brightness             = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Gradient.speed_min              = ZET_BLADE_OPTICAL_SPEED_MIN;
    Gradient.speed_max              = ZET_BLADE_OPTICAL_SPEED_MAX;
    Gradient.speed                  = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Gradient);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = ZET_BLADE_OPTICAL_MODE_OFF;
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    Off.speed                       = ZET_BLADE_OPTICAL_SPEED_DEF;
    Off.brightness                  = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    modes.push_back(Off);

    SetupZones();
}

RGBController_ZETBladeOptical::~RGBController_ZETBladeOptical()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for (unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if (zones[zone_index].matrix_map != nullptr)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_ZETBladeOptical::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    unsigned int total_led_count = 0;
    for (unsigned int zone_idx = 0; zone_idx < 1; zone_idx++)
    {
        zone new_zone;
        new_zone.name                   = zone_names[zone_idx];
        new_zone.type                   = zone_types[zone_idx];
        new_zone.leds_min               = zone_sizes[zone_idx];
        new_zone.leds_max               = zone_sizes[zone_idx];
        new_zone.leds_count             = zone_sizes[zone_idx];

        if (zone_types[zone_idx] == ZONE_TYPE_MATRIX)
        {
            new_zone.matrix_map         = new matrix_map_type;
            new_zone.matrix_map->height = ZET_BLADE_OPTICAL_ROWS;
            new_zone.matrix_map->width  = ZET_BLADE_OPTICAL_COLUMNS;
            new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
        }
        else
        {
            new_zone.matrix_map         = nullptr;
        }

        zones.push_back(new_zone);

        total_led_count += zone_sizes[zone_idx];
    }

    for (unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name                    = led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_ZETBladeOptical::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ZETBladeOptical::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();

    if(active_mode == 0)
    {
        controller->SetLEDDirect(colors, modes[active_mode].brightness);
    }
}

void RGBController_ZETBladeOptical::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ZETBladeOptical::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ZETBladeOptical::DeviceUpdateMode()
{
    bool          random            = (modes[active_mode].color_mode == MODE_COLORS_RANDOM || modes[active_mode].color_mode == MODE_COLORS_NONE);
    unsigned char mode_colors[3];

    mode_colors[0] = 0;
    mode_colors[1] = 0;
    mode_colors[2] = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        mode_colors[0] = RGBGetRValue(modes[active_mode].colors[0]);
        mode_colors[1] = RGBGetGValue(modes[active_mode].colors[0]);
        mode_colors[2] = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->SetEffect(modes[active_mode].value,
                          modes[active_mode].speed,
                          modes[active_mode].brightness,
                          random,
                          mode_colors[0],
                          mode_colors[1],
                          mode_colors[2]);

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
}

