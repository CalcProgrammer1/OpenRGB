/*---------------------------------------------------------*\
| RGBController_CherryKeyboard.cpp                          |
|                                                           |
|   RGBController for Cherry keyboard                       |
|                                                           |
|   Sebastian Kraus                             25 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CherryKeyboard.h"

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF
#define CHERRY_MATRIX_MAP_HEIGHT 6
#define CHERRY_MATRIX_MAP_WIDTH 21
#define CHERRY_MATRIX_CELL_COUNT ( CHERRY_MATRIX_MAP_HEIGHT * CHERRY_MATRIX_MAP_WIDTH )

/* The total byte count for all colors is 'number of matrix cells' times '3 color components' */
#define CUSTOM_COLOR_ARRAY_BYTE_COUNT ( CHERRY_MATRIX_CELL_COUNT * 3 )

static unsigned int matrix_map[CHERRY_MATRIX_MAP_HEIGHT][CHERRY_MATRIX_MAP_WIDTH] =
    { {  0,  NA,  12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90,  96,  102, 108, 114, 120 },
      {  1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97,  103, 109, 115, 121 },
      {  2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  NA,  86,  92,  98,  104, 110, 116,  NA },
      {  3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,  NA,  NA,  NA,  105, 111, 117, 122 },
      {  4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,  NA,  82,  NA,  94,  NA,  106, 112, 118,  NA },
      {  5,  11,  17,  NA,  NA,  NA,  41,  NA,  NA,  NA,  65,  71,  77,  83,  89,  95, 101,  113,  NA, 119, 124 } };

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

RGBController_CherryKeyboard::RGBController_CherryKeyboard(CherryKeyboardController* controller_ptr, uint16_t product_id)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Cherry";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Cherry Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Custom;
    Custom.name           = "Custom";
    Custom.value          = CHERRY_KB_MODE_CUSTOM;
    Custom.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Custom.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Custom.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Custom.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = CHERRY_KB_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Wave.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Wave.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Wave.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Wave.colors_min     = 1;
    Wave.colors_max     = 1;
    Wave.speed          = CHERRY_KB_SPEED_NORMAL;
    Wave.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Wave.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode Spectrum;
    Spectrum.name           = "Spectrum";
    Spectrum.value          = CHERRY_KB_MODE_SPECTRUM;
    Spectrum.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Spectrum.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Spectrum.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Spectrum.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Spectrum.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Spectrum.speed          = CHERRY_KB_SPEED_NORMAL;
    Spectrum.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Spectrum.color_mode     = MODE_COLORS_NONE;
    Spectrum.colors.resize(1);
    modes.push_back(Spectrum);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = CHERRY_KB_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Breathing.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Breathing.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Breathing.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Breathing.colors_min     = 1;
    Breathing.colors_max     = 1;
    Breathing.speed          = CHERRY_KB_SPEED_NORMAL;
    Breathing.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Static;
    Static.name           = "Static";
    Static.value          = CHERRY_KB_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Static.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Radar;
    Radar.name           = "Radar";
    Radar.value          = CHERRY_KB_MODE_RADAR;
    Radar.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Radar.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Radar.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Radar.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Radar.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Radar.colors_min     = 1;
    Radar.colors_max     = 1;
    Radar.speed          = CHERRY_KB_SPEED_NORMAL;
    Radar.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Radar.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Radar.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(Radar);

    mode Vortex;
    Vortex.name           = "Vortex";
    Vortex.value          = CHERRY_KB_MODE_VORTEX;
    Vortex.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Vortex.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Vortex.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Vortex.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Vortex.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Vortex.colors_min     = 1;
    Vortex.colors_max     = 1;
    Vortex.speed          = CHERRY_KB_SPEED_NORMAL;
    Vortex.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Vortex.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Vortex.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(Vortex);

    mode Fire;
    Fire.name           = "Fire";
    Fire.value          = CHERRY_KB_MODE_FIRE;
    Fire.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Fire.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Fire.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Fire.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Fire.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Fire.colors_min     = 1;
    Fire.colors_max     = 1;
    Fire.speed          = CHERRY_KB_SPEED_NORMAL;
    Fire.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Fire.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Fire.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(Fire);

    mode Stars;
    Stars.name           = "Stars";
    Stars.value          = CHERRY_KB_MODE_STARS;
    Stars.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Stars.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Stars.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Stars.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Stars.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Stars.speed          = CHERRY_KB_SPEED_NORMAL;
    Stars.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Stars.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Stars.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(Stars);

    mode Rain;
    Rain.name           = "Rain";
    Rain.value          = CHERRY_KB_MODE_RAIN;
    Rain.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Rain.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Rain.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Rain.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Rain.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Rain.colors_min     = 1;
    Rain.colors_max     = 1;
    Rain.speed          = CHERRY_KB_SPEED_NORMAL;
    Rain.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Rain.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Rain.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(Rain);

    mode Rolling;
    Rolling.name           = "Rolling";
    Rolling.value          = CHERRY_KB_MODE_ROLLING;
    Rolling.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Rolling.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Rolling.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Rolling.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Rolling.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Rolling.speed          = CHERRY_KB_SPEED_NORMAL;
    Rolling.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Rolling.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Rolling);

    mode Curve;
    Curve.name           = "Curve";
    Curve.value          = CHERRY_KB_MODE_CURVE;
    Curve.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Curve.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Curve.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Curve.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Curve.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Curve.colors_min     = 1;
    Curve.colors_max     = 1;
    Curve.speed          = CHERRY_KB_SPEED_NORMAL;
    Curve.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Curve.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Curve.colors.resize(1);
    modes.push_back(Curve);

    mode WaveMid;
    WaveMid.name           = "Wave Mid";
    WaveMid.value          = CHERRY_KB_MODE_WAVE_MID;
    WaveMid.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    WaveMid.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    WaveMid.speed_max      = CHERRY_KB_SPEED_FASTEST;
    WaveMid.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    WaveMid.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    WaveMid.speed          = CHERRY_KB_SPEED_NORMAL;
    WaveMid.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    WaveMid.color_mode     = MODE_COLORS_NONE;
    WaveMid.colors.resize(1);
    if(hasUnofficialModeSupport(product_id))
        modes.push_back(WaveMid);

    mode Scan;
    Scan.name           = "Scan";
    Scan.value          = CHERRY_KB_MODE_SCAN;
    Scan.flags          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Scan.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Scan.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Scan.colors_min     = 1;
    Scan.colors_max     = 1;
    Scan.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Scan.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Scan.colors.resize(1);
    modes.push_back(Scan);

    mode Radiation;
    Radiation.name           = "Radiation";
    Radiation.value          = CHERRY_KB_MODE_RADIATION;
    Radiation.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Radiation.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Radiation.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Radiation.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Radiation.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Radiation.colors_min     = 1;
    Radiation.colors_max     = 1;
    Radiation.speed          = CHERRY_KB_SPEED_NORMAL;
    Radiation.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Radiation.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Radiation.colors.resize(1);
    modes.push_back(Radiation);

    mode Ripples;
    Ripples.name           = "Ripples";
    Ripples.value          = CHERRY_KB_MODE_RIPPLES;
    Ripples.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Ripples.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    Ripples.speed_max      = CHERRY_KB_SPEED_FASTEST;
    Ripples.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    Ripples.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Ripples.colors_min     = 1;
    Ripples.colors_max     = 1;
    Ripples.speed          = CHERRY_KB_SPEED_NORMAL;
    Ripples.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    Ripples.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Ripples.colors.resize(1);
    modes.push_back(Ripples);

    mode SingleKey;
    SingleKey.name           = "Single Key";
    SingleKey.value          = CHERRY_KB_MODE_SINGLE_KEY;
    SingleKey.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    SingleKey.speed_min      = CHERRY_KB_SPEED_SLOWEST;
    SingleKey.speed_max      = CHERRY_KB_SPEED_FASTEST;
    SingleKey.brightness_min = CHERRY_KB_BRIGHTNESS_LOWEST;
    SingleKey.brightness_max = CHERRY_KB_BRIGHTNESS_HIGHEST;
    SingleKey.colors_min     = 1;
    SingleKey.colors_max     = 1;
    SingleKey.speed          = CHERRY_KB_SPEED_NORMAL;
    SingleKey.brightness     = CHERRY_KB_BRIGHTNESS_HIGHEST;
    SingleKey.color_mode     = MODE_COLORS_MODE_SPECIFIC;
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
    new_zone.leds_min           = CHERRY_MATRIX_CELL_COUNT;
    new_zone.leds_max           = CHERRY_MATRIX_CELL_COUNT;
    new_zone.leds_count         = CHERRY_MATRIX_CELL_COUNT;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = CHERRY_MATRIX_MAP_HEIGHT;
    new_zone.matrix_map->width  = CHERRY_MATRIX_MAP_WIDTH;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;

    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < CHERRY_MATRIX_CELL_COUNT; led_idx++)
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
    unsigned char color_data[CUSTOM_COLOR_ARRAY_BYTE_COUNT];

    for(int led_idx = 0; led_idx < CHERRY_MATRIX_CELL_COUNT; led_idx++)
    {
        color_data[(3 * led_idx) + 0] = RGBGetRValue(colors[led_idx]);
        color_data[(3 * led_idx) + 1] = RGBGetGValue(colors[led_idx]);
        color_data[(3 * led_idx) + 2] = RGBGetBValue(colors[led_idx]);
    }

    controller->SetKeyboardColors
                    (
                    color_data,
                    CUSTOM_COLOR_ARRAY_BYTE_COUNT
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
                    modes[active_mode].brightness,
                    modes[active_mode].speed,
                    0,
                    random,
                    red,
                    grn,
                    blu
                    );
}

bool RGBController_CherryKeyboard::hasUnofficialModeSupport(const uint16_t product_id)
{
    switch(product_id)
    {
    // no backlight: Why are they even listed here? (no lights, no macros)
    case MX_BOARD_3_0S_FL_NBL_PID:
    case G80_3000_TKL_NBL_PID:
    case MX_1_0_FL_NBL_PID:
    case G80_3000_TKL_NBL_KOR_PID:
    case CCF_MX_1_0_TKL_NBL_PID:
    // white backlight keyboards: very doubtful if any of those RGB modes match
    case CCF_MX_8_0_TKL_BL_PID:
    case CCF_MX_1_0_TKL_BL_PID:
    case MX_1_0_FL_BL_PID:
        return false;
    // RGB keyboards known for not supporting unofficial modes
    case MX_BOARD_3_0S_FL_RGB_PID:
    case MX_BOARD_3_0S_FL_RGB_KOR_PID:
    case MX_BOARD_2_0S_FL_RGB_US_PID:
    case MX_BOARD_2_0S_FL_NBL_PID:
    case MX_BOARD_2_0S_FL_RGB_EU_PID:
    case MV_BOARD_3_0FL_RGB_PID:
        return false;
    // RGB keyboards which (probably) support unofficial modes
    case MX_BOARD_1_0_TKL_RGB_PID: // unknown
    case MX_BOARD_8_0_TKL_RGB_PID: // unknown
    case MX_BOARD_10_0_FL_RGB_PID: // unknown (probably yes, related to 10.0N)
    case G80_3000_TKL_RGB_PID: // unknown
    case MX_1_0_FL_RGB_PID: // unkown
    case G80_3000N_TKL_RGB_EU_PID: // yes
    case G80_3000N_TKL_RGB_US_PID: // yes
    case G80_3000N_FL_RGB_EU_PID: // firmware v0102: YES, firmware v0103: NO
    case G80_3000N_FL_RGB_US_PID: // firmware v0102: YES, firmware v0103: NO
    case MX_BOARD_10_0N_FL_RGB_EU_PID: // yes
    case MX_BOARD_10_0N_FL_RGB_US_PID: // yes
    default:
        return true;
    }
}
