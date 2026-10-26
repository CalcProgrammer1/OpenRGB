/*---------------------------------------------------------*\
| RGBController_KlipschLumina.cpp                           |
|                                                           |
|   RGBController for Klipsch ProMedia Lumina speakers      |
|                                                           |
|   wlof                                        26 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_KlipschLumina.h"

/**------------------------------------------------------------------*\
    @name Klipsch ProMedia Lumina
    @category Speaker
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectKlipschLuminaControllers
    @comment Direct mode uses the same per-LED stream as Screen React
        in Klipsch's app.  The HID interfaces have no interrupt
        endpoint, so on Linux hidapi-libusb must be installed.
\*-------------------------------------------------------------------*/

/*---------------------------------------------------------*\
| Each satellite is a 2 x 3 matrix.  The device orders the  |
| LEDs top 1-3 then bottom 1-3, but the bottom row is wired |
| left, right, center when viewed from the front.           |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_MATRIX_HEIGHT    2
#define KLIPSCH_LUMINA_MATRIX_WIDTH     3

static unsigned int satellite_matrix_map[KLIPSCH_LUMINA_MATRIX_HEIGHT][KLIPSCH_LUMINA_MATRIX_WIDTH] =
{
    { 0, 1, 2 },
    { 3, 5, 4 },
};

static const char* satellite_led_names[KLIPSCH_LUMINA_LEDS_PER_SATELLITE] =
{
    "Top Left",
    "Top Center",
    "Top Right",
    "Bottom Left",
    "Bottom Right",
    "Bottom Center",
};

static const char* satellite_names[KLIPSCH_LUMINA_NUM_SATELLITES] =
{
    "Left Satellite",
    "Right Satellite",
};

/*---------------------------------------------------------*\
| Firmware presets.  Aurora and Music React only render     |
| these color pairs, all zero colors select the default     |
| Music React rotation.                                     |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_PRESET_BASE      0x100

typedef struct
{
    const char*     name;
    unsigned char   light_mode;
    RGBColor        primary;
    RGBColor        secondary;
} klipsch_lumina_preset;

static const klipsch_lumina_preset presets[] =
{
    { "Aurora Cool",                KLIPSCH_LUMINA_MODE_AURORA,         ToRGBColor(0x00, 0x00, 0xFF), ToRGBColor(0x00, 0xFF, 0xFF) },
    { "Aurora Warm",                KLIPSCH_LUMINA_MODE_AURORA,         ToRGBColor(0xFF, 0x00, 0x00), ToRGBColor(0xFF, 0x7F, 0x00) },
    { "Music React",                KLIPSCH_LUMINA_MODE_MUSIC_REACT,    ToRGBColor(0x00, 0x00, 0x00), ToRGBColor(0x00, 0x00, 0x00) },
    { "Music React Blue Purple",    KLIPSCH_LUMINA_MODE_MUSIC_REACT,    ToRGBColor(0x00, 0x00, 0xFF), ToRGBColor(0xFF, 0x00, 0xFF) },
    { "Music React Cyan Blue",      KLIPSCH_LUMINA_MODE_MUSIC_REACT,    ToRGBColor(0x00, 0xFF, 0xFF), ToRGBColor(0x00, 0x00, 0xFF) },
    { "Music React Red Pink",       KLIPSCH_LUMINA_MODE_MUSIC_REACT,    ToRGBColor(0xFF, 0x00, 0x00), ToRGBColor(0xFF, 0x00, 0x7F) },
    { "Music React Yellow Orange",  KLIPSCH_LUMINA_MODE_MUSIC_REACT,    ToRGBColor(0xFF, 0xFF, 0x00), ToRGBColor(0xFF, 0x7F, 0x00) },
};

RGBController_KlipschLumina::RGBController_KlipschLumina(KlipschLuminaController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = "Klipsch ProMedia Lumina";
    vendor                  = "Klipsch";
    type                    = DEVICE_TYPE_SPEAKER;
    description             = "Klipsch ProMedia Lumina Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    version                 = controller->GetFirmwareVersion();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = KLIPSCH_LUMINA_MODE_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = KLIPSCH_LUMINA_BRIGHTNESS_MIN;
    Direct.brightness_max   = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
    Direct.brightness       = controller->GetBrightness();
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = KLIPSCH_LUMINA_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min       = 1;
    Static.colors_max       = 1;
    Static.colors.resize(1);
    Static.brightness_min   = KLIPSCH_LUMINA_BRIGHTNESS_MIN;
    Static.brightness_max   = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
    Static.brightness       = controller->GetBrightness();
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = KLIPSCH_LUMINA_MODE_BREATHE;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    Breathing.brightness_min    = KLIPSCH_LUMINA_BRIGHTNESS_MIN;
    Breathing.brightness_max    = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
    Breathing.brightness        = controller->GetBrightness();
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = KLIPSCH_LUMINA_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    Rainbow.brightness_min  = KLIPSCH_LUMINA_BRIGHTNESS_MIN;
    Rainbow.brightness_max  = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
    Rainbow.brightness      = controller->GetBrightness();
    modes.push_back(Rainbow);

    for(unsigned int preset_idx = 0; preset_idx < (sizeof(presets) / sizeof(presets[0])); preset_idx++)
    {
        mode Preset;
        Preset.name             = presets[preset_idx].name;
        Preset.value            = KLIPSCH_LUMINA_PRESET_BASE + preset_idx;
        Preset.flags            = MODE_FLAG_HAS_BRIGHTNESS;
        Preset.color_mode       = MODE_COLORS_NONE;
        Preset.brightness_min   = KLIPSCH_LUMINA_BRIGHTNESS_MIN;
        Preset.brightness_max   = KLIPSCH_LUMINA_BRIGHTNESS_MAX;
        Preset.brightness       = controller->GetBrightness();
        modes.push_back(Preset);
    }

    mode Off;
    Off.name                = "Off";
    Off.value               = KLIPSCH_LUMINA_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    /*-----------------------------------------------------*\
    | Start on whatever hardware mode the speakers are in,  |
    | on the preset with the stored colors if there is one  |
    \*-----------------------------------------------------*/
    bool mode_found = false;

    for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        unsigned int    light_mode      = modes[mode_idx].value;
        bool            colors_match    = true;

        if(light_mode >= KLIPSCH_LUMINA_PRESET_BASE)
        {
            const klipsch_lumina_preset* preset = &presets[light_mode - KLIPSCH_LUMINA_PRESET_BASE];

            light_mode      = preset->light_mode;
            colors_match    = (preset->primary == controller->GetPrimaryColor()) && (preset->secondary == controller->GetSecondaryColor());
        }

        if(light_mode == controller->GetLightMode())
        {
            if(!mode_found || colors_match)
            {
                active_mode = mode_idx;
                mode_found  = true;
            }

            if(colors_match)
            {
                break;
            }
        }
    }

    SetupZones();
}

RGBController_KlipschLumina::~RGBController_KlipschLumina()
{
    Shutdown();

    delete controller;
}

void RGBController_KlipschLumina::SetupZones()
{
    for(unsigned int zone_idx = 0; zone_idx < KLIPSCH_LUMINA_NUM_SATELLITES; zone_idx++)
    {
        zone satellite_zone;

        satellite_zone.name         = satellite_names[zone_idx];
        satellite_zone.type         = ZONE_TYPE_MATRIX;
        satellite_zone.leds_min     = KLIPSCH_LUMINA_LEDS_PER_SATELLITE;
        satellite_zone.leds_max     = KLIPSCH_LUMINA_LEDS_PER_SATELLITE;
        satellite_zone.leds_count   = KLIPSCH_LUMINA_LEDS_PER_SATELLITE;
        satellite_zone.matrix_map.Set(KLIPSCH_LUMINA_MATRIX_HEIGHT, KLIPSCH_LUMINA_MATRIX_WIDTH, (unsigned int *)satellite_matrix_map);

        zones.push_back(satellite_zone);

        for(unsigned int led_idx = 0; led_idx < KLIPSCH_LUMINA_LEDS_PER_SATELLITE; led_idx++)
        {
            led satellite_led;

            satellite_led.name      = std::string(satellite_names[zone_idx]) + " " + satellite_led_names[led_idx];
            satellite_led.value     = (zone_idx * KLIPSCH_LUMINA_LEDS_PER_SATELLITE) + led_idx;

            leds.push_back(satellite_led);
        }
    }

    SetupColors();
}

void RGBController_KlipschLumina::DeviceUpdateLEDs()
{
    controller->SetLEDColors(&colors[0]);
}

void RGBController_KlipschLumina::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KlipschLumina::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KlipschLumina::DeviceUpdateMode()
{
    unsigned int value = modes[active_mode].value;

    if(value >= KLIPSCH_LUMINA_PRESET_BASE)
    {
        unsigned int preset_idx = value - KLIPSCH_LUMINA_PRESET_BASE;

        controller->SetLightMode(presets[preset_idx].light_mode);
        controller->SetCustomColors(presets[preset_idx].primary, presets[preset_idx].secondary);
    }
    else
    {
        controller->SetLightMode((unsigned char)value);

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            controller->SetCustomColors(modes[active_mode].colors[0], modes[active_mode].colors[0]);
        }
        else if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
        {
            DeviceUpdateLEDs();
        }
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->SetBrightness((unsigned char)modes[active_mode].brightness);
    }
}
