/*------------------------------------------*\
|  RGBController_SinowealthKeyboard90.cpp    |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0090,                   |
|  made spefically for Genesis Thor 300      |
|                                            |
|  Jan Baier 30/06/2022                      |
\*-----------------------------------------=*/

#include "RGBController_SinowealthKeyboard90.h"
#include "LogManager.h"

using namespace thor300;

/**------------------------------------------------------------------*\
    @name Genesis Thor 300
    @category Keyboard
    @type USB
    @save :robot:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectSinowealthGenesisKeyboard
    @comment Direct mode is not supported by the keyboard
\*-------------------------------------------------------------------*/

RGBController_SinowealthKeyboard90::RGBController_SinowealthKeyboard90(SinowealthKeyboard90Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetNameString();
    vendor                          = "Sinowealth";
    type                            = DEVICE_TYPE_KEYBOARD;
    description                     = "Generic Sinowealth Keyboard";
    location                        = controller->GetDeviceLocation();
    serial                          = controller->GetSerialString();

    AddMode("Breathing",            MODE_BREATHING,         true    );
    AddMode("CCW Rotation",         MODE_CCW_ROTATION,      false   );
    AddMode("CW Rotation",          MODE_CW_ROTATION,       false   );
    AddMode("Flowers Blossom",      MODE_FLOWERS_BLOSSOM,   false   );
    AddMode("Neon",                 MODE_NEON,              true    );
    AddMode("Prismo",               MODE_PRISMO,            false   );
    AddMode("Rainbow Wave",         MODE_RAINBOW,           false   );
    AddMode("Raindrops",            MODE_RAINDROPS,         true    );
    AddMode("Reactive",             MODE_RESPONSE,          false   );
    AddMode("Single Key Reactive",  MODE_RESPONSE_SINGLE,   true    );
    AddMode("Snake",                MODE_SNAKE,             true    );
    AddMode("Stars Twinkling",      MODE_TWINKLING,         true    );
    AddMode("Static",               MODE_STATIC,            true    );
    AddMode("Tornado",              MODE_TORNADO,           true    );
    AddMode("Wave 1",               MODE_WAVE_1,            true    );
    AddMode("Wave 2",               MODE_WAVE_2,            false   );
    AddMode("Wave 3",               MODE_WAVE_3,            true    );
    AddMode("Wave 4",               MODE_WAVE_4,            true    );
    AddMode("Wave 5",               MODE_WAVE_5,            false   );

    mode Custom;
    Custom.name                     = "Custom";
    Custom.value                    = MODE_CUSTOM;
    Custom.flags                    = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Custom.brightness_min           = BRIGHTNESS_OFF;
    Custom.brightness_max           = BRIGHTNESS_FULL;
    Custom.brightness               = BRIGHTNESS_FULL;
    Custom.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = MODE_STATIC;
    Off.flags                       = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    active_mode                     = (int)modes.size() - 1;

    SetupZones();
}

RGBController_SinowealthKeyboard90::~RGBController_SinowealthKeyboard90()
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

void RGBController_SinowealthKeyboard90::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name                   = "Keyboard";
    new_zone.type                   = ZONE_TYPE_MATRIX;
    new_zone.leds_count             = 104;
    new_zone.leds_min               = new_zone.leds_count;
    new_zone.leds_max               = new_zone.leds_count;

    new_zone.matrix_map             = new matrix_map_type;
    new_zone.matrix_map->height     = 6;
    new_zone.matrix_map->width      = 23;
    new_zone.matrix_map->map        = (unsigned int *)&matrix_map;

    zones.push_back(new_zone);

    for(unsigned int led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;
        new_led.name                = led_names[led_idx].name;
        new_led.value               = led_names[led_idx].idx;
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_SinowealthKeyboard90::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SinowealthKeyboard90::DeviceUpdateLEDs()
{
    controller->SendMode(modes[active_mode].value, modes[active_mode].brightness);
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char key           = leds[led_idx].value;
        unsigned char red           = RGBGetRValue(colors[led_idx]);
        unsigned char green         = RGBGetGValue(colors[led_idx]);
        unsigned char blue          = RGBGetBValue(colors[led_idx]);

        controller->SendSingleLED(key, red, green, blue);
    }

    controller->SendCommit();
}

void RGBController_SinowealthKeyboard90::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard90::UpdateSingleLED(int /*key*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SinowealthKeyboard90::DeviceUpdateMode()
{
    if (modes[active_mode].value == MODE_CUSTOM)
    {
        return;
    }

    unsigned char mode_color    = COLOR_RAINBOW;
    if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        mode_color              = MapRGBToColorEnum(modes[active_mode].colors.at(0));
    }
    controller->SendMode
        (
        modes[active_mode].value,
        modes[active_mode].brightness,
        modes[active_mode].speed,
        mode_color
        );
}

unsigned char RGBController_SinowealthKeyboard90::MapRGBToColorEnum(RGBColor color)
{
    unsigned char red   = RGBGetRValue(color);
    unsigned char green = RGBGetGValue(color);
    unsigned char blue  = RGBGetBValue(color);

    if (red & green & blue)
    {
        return COLOR_WHITE;
    }
    if (red & green)
    {
        return COLOR_YELLOW;
    }
    if (red & blue)
    {
        return COLOR_VIOLET;
    }
    if (green & blue)
    {
        return COLOR_CYAN;
    }
    if (red)
    {
        return COLOR_RED;
    }
    if (green)
    {
        return COLOR_GREEN;
    }
    if (blue)
    {
        return COLOR_BLUE;
    }
    return COLOR_RAINBOW;
}

void RGBController_SinowealthKeyboard90::AddMode
    (
    std::string name,
    unsigned char value,
    bool color_support
    )
{
    mode Mode;
    Mode.name                   = name;
    Mode.value                  = value;
    if (color_support)
    {
        Mode.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS |
                                    MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR |
                                    MODE_FLAG_AUTOMATIC_SAVE;
        Mode.colors_min         = 1;
        Mode.colors_max         = 1;
        Mode.color_mode         = MODE_COLORS_RANDOM;
        Mode.colors.resize(1);
    }
    else
    {
        Mode.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
        Mode.color_mode         = MODE_COLORS_NONE;
    }
    Mode.brightness_min         = BRIGHTNESS_OFF;
    Mode.brightness_max         = BRIGHTNESS_FULL;
    Mode.brightness             = BRIGHTNESS_FULL;
    Mode.speed_min              = SPEED_SLOWEST;
    Mode.speed_max              = SPEED_FASTEST;
    Mode.speed                  = SPEED_NORMAL;
    modes.push_back(Mode);
}
