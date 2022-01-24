/*-----------------------------------------*\
|  RGBController_ZETBladeOptical.cpp        |
|                                           |
|  Generic RGB Interface for ZET Blade      |
|  Optical Keyboard                         |
|                                           |
|  Based on HyperX Alloy Elite2 impl by     |
|                            KundaPanda     |
|                                           |
|  Moon_darker (Vaker) 23/01/2022           |
\*-----------------------------------------*/

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
    "Keyboard",
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
    "Key: Escape",
    // Skip index 1
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
    "Key: Pause/Break",
    // Skip index 17
    // Skip index 18
    // Skip index 19
    // Skip index 20
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
    // Skip index 75
    "Key: Enter",
    // Skip index 77
    // Skip index 78
    // Skip index 79
    "Key: Number Pad 4",
    "Key: Number Pad 5",
    "Key: Number Pad 6",
    // Skip index 83
    "Key: Left Shift",
    // Skip index 85
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
    // Skip index 96
    "Key: Right Shift",
    // Skip index 98
    "Key: Up Arrow",
    // Skip index 100
    "Key: Number Pad 1",
    "Key: Number Pad 2",
    "Key: Number Pad 3",
    "Key: Number Pad Enter",
    "Key: Left Control",
    "Key: Left Windows",
    "Key: Left Alt",
    // Skip index 108
    // Skip index 109
    "Key: Space",
    // Skip index 111
    // Skip index 112
    // Skip index 113
    "Key: Right Alt",
    "Key: Right Fn",
    // Skip index 116
    "Key: Menu",
    "Key: Right Control",
    "Key: Left Arrow",
    "Key: Down Arrow",
    "Key: Right Arrow",
    "Key: Number Pad 0",
    // Skip index 123
    "Key: Number Pad .",
};

RGBController_ZETBladeOptical::RGBController_ZETBladeOptical(ZETBladeOpticalController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "ZET Blade Optical Keyboard Device";
    vendor      = "ZET";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "ZET Blade Optical Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = ZET_BLADE_OPTICAL_MODE_CUSTOM;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    Custom.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Custom.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Custom.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    modes.push_back(Custom);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ZET_BLADE_OPTICAL_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Static.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Static.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Static.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                 = "Breathing";
    Breathing.value                = ZET_BLADE_OPTICAL_MODE_BREATHING;
    Breathing.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode           = MODE_COLORS_RANDOM;
    Breathing.colors_min           = 1;
    Breathing.colors_max           = 1;
    Breathing.colors.resize(Breathing.colors_max);
    Breathing.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Breathing.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Breathing.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Breathing.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Breathing.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Breathing.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Breathing);

    mode OnPress;
    OnPress.name                 = "Reactive";
    OnPress.value                = ZET_BLADE_OPTICAL_MODE_ON_PRESS;
    OnPress.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    OnPress.color_mode           = MODE_COLORS_RANDOM;
    OnPress.colors_min           = 1;
    OnPress.colors_max           = 1;
    OnPress.colors.resize(OnPress.colors_max);
    OnPress.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    OnPress.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    OnPress.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    OnPress.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    OnPress.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    OnPress.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(OnPress);

    mode Raindrop;
    Raindrop.name                 = "Reactive Raindrop";
    Raindrop.value                = ZET_BLADE_OPTICAL_MODE_RAINDROP;
    Raindrop.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Raindrop.color_mode           = MODE_COLORS_RANDOM;
    Raindrop.colors_min           = 1;
    Raindrop.colors_max           = 1;
    Raindrop.colors.resize(Raindrop.colors_max);
    Raindrop.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Raindrop.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Raindrop.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Raindrop.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Raindrop.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Raindrop.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Raindrop);

    mode Ripple;
    Ripple.name                 = "Reactive Ripple";
    Ripple.value                = ZET_BLADE_OPTICAL_MODE_RIPPLE;
    Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Ripple.color_mode           = MODE_COLORS_RANDOM;
    Ripple.colors_min           = 1;
    Ripple.colors_max           = 1;
    Ripple.colors.resize(Ripple.colors_max);
    Ripple.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Ripple.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Ripple.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Ripple.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Ripple.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Ripple.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Ripple);

    mode Laser;
    Laser.name                 = "Reactive Laser";
    Laser.value                = ZET_BLADE_OPTICAL_MODE_LASER;
    Laser.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Laser.color_mode           = MODE_COLORS_RANDOM;
    Laser.colors_min           = 1;
    Laser.colors_max           = 1;
    Laser.colors.resize(Laser.colors_max);
    Laser.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Laser.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Laser.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Laser.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Laser.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Laser.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Laser);

    mode Waves;
    Waves.name                 = "Waves";
    Waves.value                = ZET_BLADE_OPTICAL_MODE_WAVES;
    Waves.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Waves.color_mode           = MODE_COLORS_RANDOM;
    Waves.colors_min           = 1;
    Waves.colors_max           = 1;
    Waves.colors.resize(Waves.colors_max);
    Waves.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Waves.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Waves.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Waves.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Waves.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Waves.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Waves);

    mode Rain;
    Rain.name                 = "Rain";
    Rain.value                = ZET_BLADE_OPTICAL_MODE_RAIN;
    Rain.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.color_mode           = MODE_COLORS_NONE;
    Rain.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Rain.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Rain.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Rain.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Rain.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Rain.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Rain);

    mode Spectrum;
    Spectrum.name                 = "Spectrum Cycle";
    Spectrum.value                = ZET_BLADE_OPTICAL_MODE_SPECTRUM;
    Spectrum.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.color_mode           = MODE_COLORS_NONE;
    Spectrum.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Spectrum.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Spectrum.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Spectrum.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Spectrum.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Spectrum.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Spectrum);

    mode SurfingRight; // Now, you might be thinking... it'd be better to use .direction here, right? WRONG! There is no Surfing Left /._.
    SurfingRight.name                 = "Rainbow Wave";
    SurfingRight.value                = ZET_BLADE_OPTICAL_MODE_SURFING_RIGHT;
    SurfingRight.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingRight.color_mode           = MODE_COLORS_NONE;
    SurfingRight.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingRight.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingRight.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingRight.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingRight.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingRight.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingRight);

    mode SurfingCenter;
    SurfingCenter.name                 = "Rainbow Wave Center";
    SurfingCenter.value                = ZET_BLADE_OPTICAL_MODE_SURFING_CENTER;
    SurfingCenter.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingCenter.color_mode           = MODE_COLORS_NONE;
    SurfingCenter.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingCenter.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingCenter.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingCenter.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingCenter.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingCenter.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingCenter);

    mode SurfingCross;
    SurfingCross.name                 = "Rainbow Wave Cross";
    SurfingCross.value                = ZET_BLADE_OPTICAL_MODE_SURFING_CROSS;
    SurfingCross.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    SurfingCross.color_mode           = MODE_COLORS_NONE;
    SurfingCross.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    SurfingCross.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    SurfingCross.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    SurfingCross.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    SurfingCross.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    SurfingCross.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(SurfingCross);

    mode RotateMarquee;
    RotateMarquee.name                 = "Vortex";
    RotateMarquee.value                = ZET_BLADE_OPTICAL_MODE_ROTATE_MARQUEE;
    RotateMarquee.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    RotateMarquee.color_mode           = MODE_COLORS_NONE;
    RotateMarquee.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    RotateMarquee.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    RotateMarquee.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    RotateMarquee.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    RotateMarquee.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    RotateMarquee.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(RotateMarquee);

    mode Traffic;
    Traffic.name                 = "Traffic";
    Traffic.value                = ZET_BLADE_OPTICAL_MODE_TRAFFIC;
    Traffic.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Traffic.color_mode           = MODE_COLORS_NONE;
    Traffic.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Traffic.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Traffic.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Traffic.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Traffic.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Traffic.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Traffic);

    mode Gradient;
    Gradient.name                 = "Gradient";
    Gradient.value                = ZET_BLADE_OPTICAL_MODE_GRADIENT;
    Gradient.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Gradient.color_mode           = MODE_COLORS_NONE;
    Gradient.brightness_min       = ZET_BLADE_OPTICAL_BRIGHTNESS_MIN;
    Gradient.brightness_max       = ZET_BLADE_OPTICAL_BRIGHTNESS_MAX;
    Gradient.brightness           = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
    Gradient.speed_min            = ZET_BLADE_OPTICAL_SPEED_MIN;
    Gradient.speed_max            = ZET_BLADE_OPTICAL_SPEED_MAX;
    Gradient.speed                = ZET_BLADE_OPTICAL_SPEED_DEF;
    modes.push_back(Gradient);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ZET_BLADE_OPTICAL_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    Off.speed                   = ZET_BLADE_OPTICAL_SPEED_DEF;
    Off.brightness              = ZET_BLADE_OPTICAL_BRIGHTNESS_DEF;
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
    UpdateSingleLED(-1);
}

void RGBController_ZETBladeOptical::UpdateZoneLEDs(int /*zone*/)
{
    UpdateSingleLED(-1);
}

void RGBController_ZETBladeOptical::UpdateSingleLED(int led)
{
    last_update_time = std::chrono::steady_clock::now();

    if (active_mode == 0)
    {
        controller->SetLEDDirect(colors, modes[active_mode].brightness, led);
    }
}

void RGBController_ZETBladeOptical::SetCustomMode()
{
    active_mode = 0;
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

