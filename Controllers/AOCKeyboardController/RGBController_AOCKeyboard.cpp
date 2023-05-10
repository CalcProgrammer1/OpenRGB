/*-----------------------------------------*\
|  RGBController_AOCKeyboard.cpp            |
|                                           |
|  Generic RGB Interface for AOC keyboard   |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/10/2023   |
\*-----------------------------------------*/

#include "RGBController_AOCKeyboard.h"

/**------------------------------------------------------------------*\
    @name AOC Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAOCKeyboardControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCKeyboard::RGBController_AOCKeyboard(AOCKeyboardController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "AOC Keyboard Device";
    vendor      = "AOC";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "AOC Keyboard Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = AOC_KEYBOARD_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_PER_LED;
    Static.brightness_min           = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Static.brightness_max           = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Static.brightness               = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    modes.push_back(Static);

    mode SpectrumCycle;
    SpectrumCycle.name              = "Spectrum Cycle";
    SpectrumCycle.value             = AOC_KEYBOARD_MODE_STATIC;
    SpectrumCycle.flags             = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    SpectrumCycle.color_mode        = MODE_COLORS_RANDOM;
    SpectrumCycle.brightness_min    = AOC_KEYBOARD_BRIGHTNESS_OFF;
    SpectrumCycle.brightness_max    = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    SpectrumCycle.brightness        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    SpectrumCycle.speed_min         = AOC_KEYBOARD_SPEED_SLOW;
    SpectrumCycle.speed_max         = AOC_KEYBOARD_SPEED_FAST;
    SpectrumCycle.speed             = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(SpectrumCycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = AOC_KEYBOARD_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.brightness_min        = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Breathing.brightness_max        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Breathing.brightness            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Breathing.speed_min             = AOC_KEYBOARD_SPEED_SLOW;
    Breathing.speed_max             = AOC_KEYBOARD_SPEED_FAST;
    Breathing.speed                 = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Breathing);

    mode React;
    React.name                      = "React";
    React.value                     = AOC_KEYBOARD_MODE_REACT;
    React.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    React.color_mode                = MODE_COLORS_PER_LED;
    React.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    React.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    React.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    React.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    React.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    React.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(React);

    mode Ripple;
    Ripple.name                     = "Ripple";
    Ripple.value                    = AOC_KEYBOARD_MODE_RIPPLE;
    Ripple.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Ripple.color_mode               = MODE_COLORS_PER_LED;
    Ripple.brightness_min           = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Ripple.brightness_max           = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Ripple.brightness               = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Ripple.speed_min                = AOC_KEYBOARD_SPEED_SLOW;
    Ripple.speed_max                = AOC_KEYBOARD_SPEED_FAST;
    Ripple.speed                    = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Ripple);

    mode Radar;
    Radar.name                      = "Radar";
    Radar.value                     = AOC_KEYBOARD_MODE_RADAR;
    Radar.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Radar.color_mode                = MODE_COLORS_PER_LED;
    Radar.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Radar.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Radar.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Radar.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    Radar.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    Radar.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Radar);

    mode Fireworks;
    Fireworks.name                  = "Fireworks";
    Fireworks.value                 = AOC_KEYBOARD_MODE_FIREWORKS;
    Fireworks.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Fireworks.color_mode            = MODE_COLORS_PER_LED;
    Fireworks.brightness_min        = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Fireworks.brightness_max        = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Fireworks.brightness            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Fireworks.speed_min             = AOC_KEYBOARD_SPEED_SLOW;
    Fireworks.speed_max             = AOC_KEYBOARD_SPEED_FAST;
    Fireworks.speed                 = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Fireworks);

    mode Blink;
    Blink.name                      = "Flashing";
    Blink.value                     = AOC_KEYBOARD_MODE_BLINK;
    Blink.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.color_mode                = MODE_COLORS_PER_LED;
    Blink.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Blink.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Blink.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Blink.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    Blink.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    Blink.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Blink);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = AOC_KEYBOARD_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.color_mode                 = MODE_COLORS_PER_LED;
    Wave.brightness_min             = AOC_KEYBOARD_BRIGHTNESS_OFF;
    Wave.brightness_max             = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Wave.brightness                 = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    Wave.speed_min                  = AOC_KEYBOARD_SPEED_SLOW;
    Wave.speed_max                  = AOC_KEYBOARD_SPEED_FAST;
    Wave.speed                      = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(Wave);

    mode RainbowWave;
    RainbowWave.name                = "Rainbow Wave";
    RainbowWave.value               = AOC_KEYBOARD_MODE_WAVE;
    RainbowWave.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    RainbowWave.color_mode          = MODE_COLORS_RANDOM;
    RainbowWave.brightness_min      = AOC_KEYBOARD_BRIGHTNESS_OFF;
    RainbowWave.brightness_max      = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    RainbowWave.brightness          = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    RainbowWave.speed_min           = AOC_KEYBOARD_SPEED_SLOW;
    RainbowWave.speed_max           = AOC_KEYBOARD_SPEED_FAST;
    RainbowWave.speed               = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(RainbowWave);

    mode ConcentricCircles;
    ConcentricCircles.name          = "Concentric Circles";
    ConcentricCircles.value         = AOC_KEYBOARD_MODE_CONCENTRIC_CIRCLES;
    ConcentricCircles.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    ConcentricCircles.color_mode    = MODE_COLORS_PER_LED;
    ConcentricCircles.brightness_min= AOC_KEYBOARD_BRIGHTNESS_OFF;
    ConcentricCircles.brightness_max= AOC_KEYBOARD_BRIGHTNESS_HIGH;
    ConcentricCircles.brightness    = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    ConcentricCircles.speed_min     = AOC_KEYBOARD_SPEED_SLOW;
    ConcentricCircles.speed_max     = AOC_KEYBOARD_SPEED_FAST;
    ConcentricCircles.speed         = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(ConcentricCircles);

    mode WWave;
    WWave.name                      = "W Wave";
    WWave.value                     = AOC_KEYBOARD_MODE_W_WAVE;
    WWave.flags                     = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    WWave.color_mode                = MODE_COLORS_PER_LED;
    WWave.brightness_min            = AOC_KEYBOARD_BRIGHTNESS_OFF;
    WWave.brightness_max            = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    WWave.brightness                = AOC_KEYBOARD_BRIGHTNESS_HIGH;
    WWave.speed_min                 = AOC_KEYBOARD_SPEED_SLOW;
    WWave.speed_max                 = AOC_KEYBOARD_SPEED_FAST;
    WWave.speed                     = AOC_KEYBOARD_SPEED_MEDIUM;
    modes.push_back(WWave);

    SetupZones();
};

RGBController_AOCKeyboard::~RGBController_AOCKeyboard()
{
    delete controller;
}

void RGBController_AOCKeyboard::SetupZones()
{
    zone keyboard_zone;
    keyboard_zone.name              = "Keyboard";
    keyboard_zone.type              = ZONE_TYPE_SINGLE;
    keyboard_zone.leds_min          = 1;
    keyboard_zone.leds_max          = 1;
    keyboard_zone.leds_count        = 1;
    keyboard_zone.matrix_map        = NULL;
    zones.push_back(keyboard_zone);

    led keyboard_led;
    keyboard_led.name               = "Keyboard";
    leds.push_back(keyboard_led);

    SetupColors();
}

void RGBController_AOCKeyboard::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCKeyboard::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AOCKeyboard::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCKeyboard::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCKeyboard::DeviceUpdateMode()
{
    unsigned char   aoc_direction   = AOC_KEYBOARD_DIRECTION_CLOCKWISE;
    unsigned char   aoc_random      = AOC_KEYBOARD_SINGLE_COLOR;

    if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
    {
        aoc_direction = AOC_KEYBOARD_DIRECTION_COUNTERCLOCKWISE;
    }

    if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
    {
        aoc_random = AOC_KEYBOARD_RANDOM;
    }

    controller->SendPacket(modes[active_mode].value,
                            aoc_random,
                            modes[active_mode].brightness,
                            modes[active_mode].speed,
                            aoc_direction,
                            &colors[0]);
}
