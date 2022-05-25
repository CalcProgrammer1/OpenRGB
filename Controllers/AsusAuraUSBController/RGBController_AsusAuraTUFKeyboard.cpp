/*-----------------------------------------*\
|  RGBController_AsusAuraTUFKeyboard.cpp    |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 03/03/2020                        |
\*-----------------------------------------*/

#include "RGBController_AsusAuraTUFKeyboard.h"
#include <vector>
#include <cmath>

/**------------------------------------------------------------------*\
    @name Asus Aura TUF Keyboard
    @category Keyboard
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraTUFUSBKeyboard
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraTUFKeyboard::RGBController_AuraTUFKeyboard(AuraTUFKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "ASUS Aura Keyboard";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "ASUS Aura Keyboard Device";
    version     = controller->GetVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = AURA_KEYBOARD_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = AURA_KEYBOARD_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min       = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness_max       = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness           = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = AURA_KEYBOARD_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.speed_min         = AURA_KEYBOARD_SPEED_MIN;
    Breathing.speed_max         = AURA_KEYBOARD_SPEED_MAX;
    Breathing.speed             = AURA_KEYBOARD_SPEED_DEFAULT;
    Breathing.brightness_min    = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness_max    = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness        = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Color_Cycle;
    Color_Cycle.name            = "Spectrum Cycle";
    Color_Cycle.value           = AURA_KEYBOARD_MODE_COLOR_CYCLE;
    Color_Cycle.flags           = MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Color_Cycle.speed_min       = AURA_KEYBOARD_SPEED_MIN;
    Color_Cycle.speed_max       = AURA_KEYBOARD_SPEED_MAX;
    Color_Cycle.speed           = AURA_KEYBOARD_SPEED_DEFAULT;
    Color_Cycle.brightness_min  = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Color_Cycle.brightness_max  = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Color_Cycle.brightness      = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Color_Cycle.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Color_Cycle);

    mode Wave;
    Wave.name                   = "Rainbow Wave";
    Wave.value                  = AURA_KEYBOARD_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_HAS_DIRECTION_HV | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.speed_min              = AURA_KEYBOARD_SPEED_MIN;
    Wave.speed_max              = AURA_KEYBOARD_SPEED_MAX;
    Wave.speed                  = AURA_KEYBOARD_SPEED_DEFAULT;
    Wave.brightness_min         = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Wave.brightness_max         = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Wave.brightness             = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Wave.direction              = MODE_DIRECTION_LEFT;
    Wave.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors_min             = 1;
    Wave.colors_max             = 7;
    Wave.colors.resize(7);
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name                 = "Ripple";
    Ripple.value                = AURA_KEYBOARD_MODE_RIPPLE;
    Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Ripple.speed_min            = AURA_KEYBOARD_SPEED_MIN;
    Ripple.speed_max            = AURA_KEYBOARD_SPEED_MAX;
    Ripple.speed                = AURA_KEYBOARD_SPEED_DEFAULT;
    Ripple.brightness_min       = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Ripple.brightness_max       = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Ripple.brightness           = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Ripple.colors_min           = 1;
    Ripple.colors_max           = 8;
    Ripple.colors.resize(7);
    modes.push_back(Ripple);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = AURA_KEYBOARD_MODE_REACTIVE;
    Reactive.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.speed_min          = AURA_KEYBOARD_SPEED_MIN;
    Reactive.speed_max          = AURA_KEYBOARD_SPEED_MAX;
    Reactive.speed              = AURA_KEYBOARD_SPEED_DEFAULT;
    Reactive.brightness_min     = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Reactive.brightness_max     = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Reactive.brightness         = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min         = 1;
    Reactive.colors_max         = 2;
    Reactive.colors.resize(1);
    modes.push_back(Reactive);

    mode Starry_Night;
    Starry_Night.name           = "Starry Night";
    Starry_Night.value          = AURA_KEYBOARD_MODE_STARRY_NIGHT;
    Starry_Night.flags          = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Starry_Night.speed_min      = AURA_KEYBOARD_SPEED_MIN;
    Starry_Night.speed_max      = AURA_KEYBOARD_SPEED_MAX;
    Starry_Night.speed          = AURA_KEYBOARD_SPEED_DEFAULT;
    Starry_Night.brightness_min = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Starry_Night.brightness_max = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Starry_Night.brightness     = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Starry_Night.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Starry_Night.colors_min     = 1;
    Starry_Night.colors_max     = 3;
    Starry_Night.colors.resize(1);
    modes.push_back(Starry_Night);

    mode Quicksand;
    Quicksand.name              = "Quicksand";
    Quicksand.value             = AURA_KEYBOARD_MODE_QUICKSAND;
    Quicksand.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_UD | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Quicksand.direction         = MODE_DIRECTION_DOWN;
    Quicksand.speed_min         = AURA_KEYBOARD_SPEED_MIN;
    Quicksand.speed_max         = AURA_KEYBOARD_SPEED_MAX;
    Quicksand.speed             = AURA_KEYBOARD_SPEED_DEFAULT;
    Quicksand.brightness_min    = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Quicksand.brightness_max    = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Quicksand.brightness        = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Quicksand.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Quicksand.colors_min        = 6;
    Quicksand.colors_max        = 6;
    Quicksand.colors.resize(6);
    modes.push_back(Quicksand);

    mode Current;
    Current.name                = "Current";
    Current.value               = AURA_KEYBOARD_MODE_CURRENT;
    Current.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Current.speed_min           = AURA_KEYBOARD_SPEED_MIN;
    Current.speed_max           = AURA_KEYBOARD_SPEED_MAX;
    Current.speed               = AURA_KEYBOARD_SPEED_DEFAULT;
    Current.brightness_min      = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Current.brightness_max      = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Current.brightness          = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Current.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Current.colors_min          = 1;
    Current.colors_max          = 3;
    Current.colors.resize(1);
    modes.push_back(Current);

    mode Rain_Drop;
    Rain_Drop.name              = "Rain Drop";
    Rain_Drop.value             = AURA_KEYBOARD_MODE_RAIN_DROP;
    Rain_Drop.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Rain_Drop.speed_min         = AURA_KEYBOARD_SPEED_MIN;
    Rain_Drop.speed_max         = AURA_KEYBOARD_SPEED_MAX;
    Rain_Drop.speed             = AURA_KEYBOARD_SPEED_DEFAULT;
    Rain_Drop.brightness_min    = AURA_KEYBOARD_BRIGHTNESS_MIN;
    Rain_Drop.brightness_max    = AURA_KEYBOARD_BRIGHTNESS_MAX;
    Rain_Drop.brightness        = AURA_KEYBOARD_BRIGHTNESS_DEFAULT;
    Rain_Drop.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Rain_Drop.colors_min        = 1;
    Rain_Drop.colors_max        = 3;
    Rain_Drop.colors.resize(1);
    modes.push_back(Rain_Drop);

    SetupZones();
}

RGBController_AuraTUFKeyboard::~RGBController_AuraTUFKeyboard()
{
    delete controller;
}

void RGBController_AuraTUFKeyboard::SetupZones()
{
    int layout = controller->GetLayout();

    if(AsusTUFK7Layouts.find(layout % 100) == AsusTUFK7Layouts.end())
    {
        layout = std::floor(layout/100) == 2 ? ASUS_TUF_K7_LAYOUT_UK : ASUS_TUF_K7_LAYOUT_US;
    }
    else
    {
        layout = layout % 100;
    }

    unsigned int total_led_count = 0;

    int zone_size = 138;

    zone keyboard;
    keyboard.name               = "Keyboard";
    keyboard.type               = ZONE_TYPE_MATRIX;
    keyboard.leds_min           = zone_size;
    keyboard.leds_max           = zone_size;
    keyboard.leds_count         = zone_size;
    keyboard.matrix_map         = new matrix_map_type;
    keyboard.matrix_map->height = 6;
    keyboard.matrix_map->width  = 23;
    keyboard.matrix_map->map    = AsusTUFK7Layouts[layout].matrix_map;
    zones.push_back(keyboard);

    total_led_count += zone_size;


    for(unsigned int led_idx = 0; led_idx < total_led_count; led_idx++)
    {
        led new_led;
        new_led.name = AsusTUFK7Layouts[layout].led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_AuraTUFKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraTUFKeyboard::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AuraTUFKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    controller->UpdateLeds(colors);
}

void RGBController_AuraTUFKeyboard::UpdateSingleLED(int led)
{
    unsigned char red   = RGBGetRValue(colors[led]);
    unsigned char green = RGBGetGValue(colors[led]);
    unsigned char blue  = RGBGetBValue(colors[led]);

    controller->UpdateSingleLed(led, red, green, blue);
}

void RGBController_AuraTUFKeyboard::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraTUFKeyboard::DeviceUpdateMode()
{
    unsigned char color_mode;
    std::vector<RGBColor> mode_colors;

    if(modes[active_mode].value == AURA_KEYBOARD_MODE_DIRECT)
    {
        controller->UpdateLeds(colors);
    }
    else
    {
        bool mode_with_double = (modes[active_mode].value == 1 || modes[active_mode].value == 3 || modes[active_mode].value == 6 || modes[active_mode].value == 8 || modes[active_mode].value == 9);

        if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC && mode_with_double && modes[active_mode].colors.size() > 1)
        {
            if(modes[active_mode].colors[1] == 000)
            {
                color_mode = 0;
            }
            else
            {
                color_mode = 16;
            }
        }
        else
        {
            color_mode = modes[active_mode].color_mode == MODE_COLORS_RANDOM ? 1 : 0;
        }

        int direction = 0;

        switch (modes[active_mode].direction)
        {
            case MODE_DIRECTION_LEFT:
                direction = 0;
                break;
            case MODE_DIRECTION_RIGHT:
                direction = 4;
                break;
            case MODE_DIRECTION_UP:
                direction = 6;
                break;
            case MODE_DIRECTION_DOWN:
                direction = 2;
                break;
            case MODE_DIRECTION_HORIZONTAL:
                direction = 8;
                break;
            case MODE_DIRECTION_VERTICAL:
                direction = 1;
                break;
        }

        mode_colors = modes[active_mode].color_mode == MODE_COLORS_PER_LED ? std::vector<RGBColor>(colors) : std::vector<RGBColor>(modes[active_mode].colors);

        controller->UpdateDevice(modes[active_mode].value, mode_colors, direction, color_mode, (15 - modes[active_mode].speed), modes[active_mode].brightness * 25);
    }
}

void RGBController_AuraTUFKeyboard::DeviceSaveMode()
{
    controller->ClearResponses();
    DeviceUpdateMode();
    controller->AwaitResponse();
    controller->SaveMode();
}
