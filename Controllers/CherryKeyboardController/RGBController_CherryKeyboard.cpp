/*-----------------------------------------*\
|  RGBController_CherryKeyboard.cpp         |
|                                           |
|  Generic RGB Interface for Cherry RGB     |
|  Keyboard                                 |
|                                           |
|  Sebastian Kraus 12/25/2021               |
\*-----------------------------------------*/

#include "RGBController_CherryKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {  0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90,  96,  NA,  NA,  NA,  NA,  NA,  NA },
      {  1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97,  NA,  NA,  NA,  NA,  NA,  NA },
      {  2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  NA,  86,  92,  98,  NA,  NA,  NA,  NA,  NA,  NA },
      {  3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
      {  4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
      {  5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95, 101,  NA,  NA,  NA,  NA,  NA,  NA } };

/**------------------------------------------------------------------*\
    @name Cherry Keyboard
    @category Keyboard
    @type USB
    @save :robot:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectCherryKeyboards
    @comment
\*-------------------------------------------------------------------*/

RGBController_CherryKeyboard::RGBController_CherryKeyboard(CherryKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Cherry Keyboard Device";
    vendor      = "Cherry";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Cherry Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = CHERRY_KB_MODE_CUSTOM;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = CHERRY_KB_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Wave.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Wave.speed      = CHERRY_KB_SPEED_NORMAL;
    Wave.colors_min = 1;
    Wave.colors_max = 1;
    Wave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode Spectrum;
    Spectrum.name       = "Spectrum";
    Spectrum.value      = CHERRY_KB_MODE_SPECTRUM;
    Spectrum.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Spectrum.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Spectrum.speed      = CHERRY_KB_SPEED_NORMAL;
    Spectrum.color_mode = MODE_COLORS_NONE;
    Spectrum.colors.resize(1);
    modes.push_back(Spectrum);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CHERRY_KB_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Breathing.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Breathing.speed      = CHERRY_KB_SPEED_NORMAL;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Static;
    Static.name       = "Static";
    Static.value      = CHERRY_KB_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Radar;
    Radar.name       = "Radar";
    Radar.value      = CHERRY_KB_MODE_RADAR;
    Radar.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Radar.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Radar.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Radar.speed      = CHERRY_KB_SPEED_NORMAL;
    Radar.colors_min = 1;
    Radar.colors_max = 1;
    Radar.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Radar.colors.resize(1);
    modes.push_back(Radar);

    mode Vortex;
    Vortex.name       = "Vortex";
    Vortex.value      = CHERRY_KB_MODE_VORTEX;
    Vortex.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Vortex.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Vortex.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Vortex.speed      = CHERRY_KB_SPEED_NORMAL;
    Vortex.colors_min = 1;
    Vortex.colors_max = 1;
    Vortex.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Vortex.colors.resize(1);
    modes.push_back(Vortex);

    mode Fire;
    Fire.name       = "Fire";
    Fire.value      = CHERRY_KB_MODE_FIRE;
    Fire.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Fire.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Fire.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Fire.speed      = CHERRY_KB_SPEED_NORMAL;
    Fire.colors_min = 1;
    Fire.colors_max = 1;
    Fire.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Fire.colors.resize(1);
    modes.push_back(Fire);

    mode Stars;
    Stars.name       = "Stars";
    Stars.value      = CHERRY_KB_MODE_STARS;
    Stars.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Stars.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Stars.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Stars.speed      = CHERRY_KB_SPEED_NORMAL;
    Stars.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Stars.colors.resize(1);
    modes.push_back(Stars);

    mode Rain;
    Rain.name       = "Rain";
    Rain.value      = CHERRY_KB_MODE_RAIN;
    Rain.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Rain.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Rain.speed      = CHERRY_KB_SPEED_NORMAL;
    Rain.colors_min = 1;
    Rain.colors_max = 1;
    Rain.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Rain.colors.resize(1);
    modes.push_back(Rain);

    mode Rolling;
    Rolling.name       = "Rolling";
    Rolling.value      = CHERRY_KB_MODE_ROLLING;
    Rolling.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rolling.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Rolling.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Rolling.speed      = CHERRY_KB_SPEED_NORMAL;
    Rolling.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rolling);
    
    mode Curve;
    Curve.name       = "Curve";
    Curve.value      = CHERRY_KB_MODE_CURVE;
    Curve.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Curve.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Curve.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Curve.speed      = CHERRY_KB_SPEED_NORMAL;
    Curve.colors_min = 1;
    Curve.colors_max = 1;
    Curve.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Curve.colors.resize(1);
    modes.push_back(Curve);

    mode WaveMid;
    WaveMid.name       = "Wave Mid";
    WaveMid.value      = CHERRY_KB_MODE_WAVE_MID;
    WaveMid.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    WaveMid.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    WaveMid.speed_max  = CHERRY_KB_SPEED_FASTEST;
    WaveMid.speed      = CHERRY_KB_SPEED_NORMAL;
    WaveMid.color_mode = MODE_COLORS_NONE;
    WaveMid.colors.resize(1);
    modes.push_back(WaveMid);

    mode Scan;
    Scan.name       = "Scan";
    Scan.value      = CHERRY_KB_MODE_SCAN;
    Scan.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Scan.colors_min = 1;
    Scan.colors_max = 1;
    Scan.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Scan.colors.resize(1);
    modes.push_back(Scan);

    mode Radiation;
    Radiation.name       = "Radiation";
    Radiation.value      = CHERRY_KB_MODE_RADIATION;
    Radiation.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Radiation.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Radiation.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Radiation.speed      = CHERRY_KB_SPEED_NORMAL;
    Radiation.colors_min = 1;
    Radiation.colors_max = 1;
    Radiation.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Radiation.colors.resize(1);
    modes.push_back(Radiation);

    mode Ripples;
    Ripples.name       = "Ripples";
    Ripples.value      = CHERRY_KB_MODE_RIPPLES;
    Ripples.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Ripples.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    Ripples.speed_max  = CHERRY_KB_SPEED_FASTEST;
    Ripples.speed      = CHERRY_KB_SPEED_NORMAL;
    Ripples.colors_min = 1;
    Ripples.colors_max = 1;
    Ripples.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Ripples.colors.resize(1);
    modes.push_back(Ripples);

    mode SingleKey;
    SingleKey.name       = "Single Key";
    SingleKey.value      = CHERRY_KB_MODE_SINGLE_KEY;
    SingleKey.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SingleKey.speed_min  = CHERRY_KB_SPEED_SLOWEST;
    SingleKey.speed_max  = CHERRY_KB_SPEED_FASTEST;
    SingleKey.speed      = CHERRY_KB_SPEED_NORMAL;
    SingleKey.colors_min = 1;
    SingleKey.colors_max = 1;
    SingleKey.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SingleKey.colors.resize(1);
    modes.push_back(SingleKey);

    SetupZones();
}

RGBController_CherryKeyboard::~RGBController_CherryKeyboard()
{
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

void RGBController_CherryKeyboard::SetupZones()
{
    zone new_zone;

    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = 126;
    new_zone.leds_max           = 126;
    new_zone.leds_count         = 126;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 23;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    
    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_CherryKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CherryKeyboard::DeviceUpdateLEDs()
{
    unsigned char color_data[7*CHERRY_KB_MAX_PACKET_SIZE];

    for(int led_idx = 0; led_idx < 126; led_idx++)
    {
        color_data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        color_data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        color_data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }

    controller->SetKeyboardColors
                    (
                    color_data,
                    CHERRY_KB_MAX_PACKET_SIZE * 7
                    );
}

void RGBController_CherryKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CherryKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CherryKeyboard::SetCustomMode()
{

}

void RGBController_CherryKeyboard::DeviceUpdateMode()
{
    unsigned char red    = 0x00;
    unsigned char grn    = 0x00;
    unsigned char blu    = 0x00;
    unsigned char random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    if(modes[active_mode].colors.size() > 0)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);
    }

    controller->SendKeyboardModeEx
                    (
                    modes[active_mode].value,
                    CHERRY_KB_BRIGHTNESS_HIGHEST,
                    modes[active_mode].speed,
                    0,
                    random,
                    red,
                    grn,
                    blu
                    );
}
