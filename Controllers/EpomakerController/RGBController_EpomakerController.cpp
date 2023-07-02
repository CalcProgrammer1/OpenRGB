/*-------------------------------------------------------------------*\
|  RGBController_EpomakerController.cpp                               |
|                                                                     |
|  Driver for Epomaker Keyboard                                       |
|                                                                     |
|  Alvaro Munoz (alvaromunoz)          2023-06-05                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_EpomakerController.h"

/**------------------------------------------------------------------*\
    @name Epomaker TH80 Pro
    @category Keyboard
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors EpomakerControllerDetect
    @comment
\*-------------------------------------------------------------------*/

RGBController_EpomakerController::RGBController_EpomakerController(EpomakerController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Epomaker";
    type        = DEVICE_TYPE_KEYBOARD;
    description = controller->GetDeviceName();
    serial      = controller->GetSerial();
    location    = controller->GetLocation();

    mode Off;
    Off.name                                    = "Off";
    Off.value                                   = 0;
    Off.flags                                   = 0;
    Off.color_mode                              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Always_on;
    Always_on.name                              = "Static";
    Always_on.value                             = Epomaker_MODE_ALWAYS_ON;
    Always_on.flags                             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Always_on.color_mode                        = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Always_on.brightness_min                    = Epomaker_BRIGHTNESS_MIN;
    Always_on.brightness_max                    = Epomaker_BRIGHTNESS_MAX;
    Always_on.brightness                        = Epomaker_BRIGHTNESS_DEFAULT;
    modes.push_back(Always_on);

    mode Dynamic_breathing;
    Dynamic_breathing.name                      = "Breathing";
    Dynamic_breathing.value                     = Epomaker_MODE_DYNAMIC_BREATHING;
    Dynamic_breathing.flags                     = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Dynamic_breathing.color_mode                = MODE_COLORS_PER_LED;
    Dynamic_breathing.brightness_min            = Epomaker_BRIGHTNESS_MIN;
    Dynamic_breathing.brightness_max            = Epomaker_BRIGHTNESS_MAX;
    Dynamic_breathing.brightness                = Epomaker_BRIGHTNESS_DEFAULT;
    Dynamic_breathing.speed_min                 = Epomaker_SPEED_MIN;
    Dynamic_breathing.speed_max                 = Epomaker_SPEED_MAX;
    Dynamic_breathing.speed                     = Epomaker_SPEED_DEFAULT;
    modes.push_back(Dynamic_breathing);

    mode Spectrum_cycle;
    Spectrum_cycle.name                         = "Spectrum Cycle";
    Spectrum_cycle.value                        = Epomaker_MODE_SPECTRUM_CYCLE;
    Spectrum_cycle.flags                        = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spectrum_cycle.color_mode                   = MODE_COLORS_NONE;
    Spectrum_cycle.brightness_min               = Epomaker_BRIGHTNESS_MIN;
    Spectrum_cycle.brightness_max               = Epomaker_BRIGHTNESS_MAX;
    Spectrum_cycle.brightness                   = Epomaker_BRIGHTNESS_DEFAULT;
    Spectrum_cycle.speed_min                    = Epomaker_SPEED_MIN;
    Spectrum_cycle.speed_max                    = Epomaker_SPEED_MAX_SPECIAL;
    Spectrum_cycle.speed                        = Epomaker_SPEED_DEFAULT;
    modes.push_back(Spectrum_cycle);

    mode Drift;
    Drift.name                                  = "Drift";
    Drift.value                                 = Epomaker_MODE_DRIFT;
    Drift.flags                                 = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Drift.color_mode                            = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Drift.brightness_min                        = Epomaker_BRIGHTNESS_MIN;
    Drift.brightness_max                        = Epomaker_BRIGHTNESS_MAX;
    Drift.brightness                            = Epomaker_BRIGHTNESS_DEFAULT;
    Drift.speed_min                             = Epomaker_SPEED_MIN;
    Drift.speed_max                             = Epomaker_SPEED_MAX;
    Drift.speed                                 = Epomaker_SPEED_DEFAULT;
    modes.push_back(Drift);

    mode Waves_ripple;
    Waves_ripple.name                           = "Waves ripple";
    Waves_ripple.value                          = Epomaker_MODE_WAVES_RIPPLE;
    Waves_ripple.flags                          = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Waves_ripple.color_mode                     = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Waves_ripple.brightness_min                 = Epomaker_BRIGHTNESS_MIN;
    Waves_ripple.brightness_max                 = Epomaker_BRIGHTNESS_MAX;
    Waves_ripple.brightness                     = Epomaker_BRIGHTNESS_DEFAULT;
    modes.push_back(Waves_ripple);

    mode Stars_twinkle;
    Stars_twinkle.name                          = "Stars twinkle";
    Stars_twinkle.value                         = Epomaker_MODE_STARS_TWINKLE;
    Stars_twinkle.flags                         = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Stars_twinkle.color_mode                    = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Stars_twinkle.brightness_min                = Epomaker_BRIGHTNESS_MIN;
    Stars_twinkle.brightness_max                = Epomaker_BRIGHTNESS_MAX;
    Stars_twinkle.brightness                    = Epomaker_BRIGHTNESS_DEFAULT;
    Stars_twinkle.speed_min                     = Epomaker_SPEED_MIN;
    Stars_twinkle.speed_max                     = Epomaker_SPEED_MAX;
    Stars_twinkle.speed                         = Epomaker_SPEED_DEFAULT;
    modes.push_back(Stars_twinkle);

    mode Steady_stream;
    Steady_stream.name                          = "Steady stream";
    Steady_stream.value                         = Epomaker_MODE_STEADY_STREAM;
    Steady_stream.flags                         = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Steady_stream.color_mode                    = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Steady_stream.brightness_min                = Epomaker_BRIGHTNESS_MIN;
    Steady_stream.brightness_max                = Epomaker_BRIGHTNESS_MAX;
    Steady_stream.brightness                    = Epomaker_BRIGHTNESS_DEFAULT;
    Steady_stream.speed_min                     = Epomaker_SPEED_MIN;
    Steady_stream.speed_max                     = Epomaker_SPEED_MAX;
    Steady_stream.speed                         = Epomaker_SPEED_DEFAULT;
    modes.push_back(Steady_stream);

    mode Shadowing;
    Shadowing.name                              = "Reactive";
    Shadowing.value                             = Epomaker_MODE_SHADOWING;
    Shadowing.flags                             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Shadowing.color_mode                        = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Shadowing.brightness_min                    = Epomaker_BRIGHTNESS_MIN;
    Shadowing.brightness_max                    = Epomaker_BRIGHTNESS_MAX;
    Shadowing.brightness                        = Epomaker_BRIGHTNESS_DEFAULT;
    Shadowing.speed_min                         = Epomaker_SPEED_MIN;
    Shadowing.speed_max                         = Epomaker_SPEED_MAX;
    Shadowing.speed                             = Epomaker_SPEED_DEFAULT;
    modes.push_back(Shadowing);

    mode Peaks_rising;
    Peaks_rising.name                           = "Peaks rising one after another";
    Peaks_rising.value                          = Epomaker_MODE_PEAKS_RISING_ONE_AFTER_ANOTHER;
    Peaks_rising.flags                          = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Peaks_rising.color_mode                     = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Peaks_rising.brightness_min                 = Epomaker_BRIGHTNESS_MIN;
    Peaks_rising.brightness_max                 = Epomaker_BRIGHTNESS_MAX;
    Peaks_rising.brightness                     = Epomaker_BRIGHTNESS_DEFAULT;
    Peaks_rising.speed_min                      = Epomaker_SPEED_MIN;
    Peaks_rising.speed_max                      = Epomaker_SPEED_MAX;
    Peaks_rising.speed                          = Epomaker_SPEED_DEFAULT;
    modes.push_back(Peaks_rising);

    mode Sine_wave;
    Sine_wave.name                              = "Sine wave";
    Sine_wave.value                             = Epomaker_MODE_SINE_WAVE;
    Sine_wave.flags                             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Sine_wave.color_mode                        = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Sine_wave.brightness_min                    = Epomaker_BRIGHTNESS_MIN;
    Sine_wave.brightness_max                    = Epomaker_BRIGHTNESS_MAX;
    Sine_wave.brightness                        = Epomaker_BRIGHTNESS_DEFAULT;
    Sine_wave.speed_min                         = Epomaker_SPEED_MIN;
    Sine_wave.speed_max                         = Epomaker_SPEED_MAX;
    Sine_wave.speed                             = Epomaker_SPEED_DEFAULT;
    modes.push_back(Sine_wave);

    mode Caispring;
    Caispring.name                              = "Caispring Surging";
    Caispring.value                             = Epomaker_MODE_CAISPRING_SURGING;
    Caispring.flags                             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Caispring.color_mode                        = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Caispring.brightness_min                    = Epomaker_BRIGHTNESS_MIN;
    Caispring.brightness_max                    = Epomaker_BRIGHTNESS_MAX;
    Caispring.brightness                        = Epomaker_BRIGHTNESS_DEFAULT;
    Caispring.speed_min                         = Epomaker_SPEED_MIN;
    Caispring.speed_max                         = Epomaker_SPEED_MAX;
    Caispring.speed                             = Epomaker_SPEED_DEFAULT;
    modes.push_back(Caispring);

    mode Flowers_blooming;
    Flowers_blooming.name                       = "Flowers blooming";
    Flowers_blooming.value                      = Epomaker_MODE_FLOWERS_BLOOMING;
    Flowers_blooming.flags                      = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Flowers_blooming.color_mode                 = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Flowers_blooming.brightness_min             = Epomaker_BRIGHTNESS_MIN;
    Flowers_blooming.brightness_max             = Epomaker_BRIGHTNESS_MAX;
    Flowers_blooming.brightness                 = Epomaker_BRIGHTNESS_DEFAULT;
    Flowers_blooming.speed_min                  = Epomaker_SPEED_MIN;
    Flowers_blooming.speed_max                  = Epomaker_SPEED_MAX;
    Flowers_blooming.speed                      = Epomaker_SPEED_DEFAULT;
    modes.push_back(Flowers_blooming);

    mode Laser;
    Laser.name                                  = "Laser";
    Laser.value                                 = Epomaker_MODE_LASER;
    Laser.flags                                 = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Laser.color_mode                            = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Laser.brightness_min                        = Epomaker_BRIGHTNESS_MIN;
    Laser.brightness_max                        = Epomaker_BRIGHTNESS_MAX;
    Laser.brightness                            = Epomaker_BRIGHTNESS_DEFAULT;
    Laser.speed_min                             = Epomaker_SPEED_MIN;
    Laser.speed_max                             = Epomaker_SPEED_MAX;
    Laser.speed                                 = Epomaker_SPEED_DEFAULT;
    modes.push_back(Laser);

    mode Peak_turn;
    Peak_turn.name                              = "Peak turn";
    Peak_turn.value                             = Epomaker_MODE_PEAK_TURN;
    Peak_turn.flags                             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    Peak_turn.color_mode                        = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Peak_turn.brightness_min                    = Epomaker_BRIGHTNESS_MIN;
    Peak_turn.brightness_max                    = Epomaker_BRIGHTNESS_MAX;
    Peak_turn.brightness                        = Epomaker_BRIGHTNESS_DEFAULT;
    Peak_turn.speed_min                         = Epomaker_SPEED_MIN;
    Peak_turn.speed_max                         = Epomaker_SPEED_MAX_SPECIAL;
    Peak_turn.speed                             = Epomaker_SPEED_DEFAULT;
    modes.push_back(Peak_turn);

    mode Inclined_rain;
    Inclined_rain.name                          = "Inclined Rain";
    Inclined_rain.value                         = Epomaker_MODE_INCLINED_RAIN;
    Inclined_rain.flags                         = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Inclined_rain.color_mode                    = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Inclined_rain.brightness_min                = Epomaker_BRIGHTNESS_MIN;
    Inclined_rain.brightness_max                = Epomaker_BRIGHTNESS_MAX;
    Inclined_rain.brightness                    = Epomaker_BRIGHTNESS_DEFAULT;
    Inclined_rain.speed_min                     = Epomaker_SPEED_MIN;
    Inclined_rain.speed_max                     = Epomaker_SPEED_MAX;
    Inclined_rain.speed                         = Epomaker_SPEED_DEFAULT;
    modes.push_back(Inclined_rain);

    mode Snow;
    Snow.name                                   = "Snow";
    Snow.value                                  = Epomaker_MODE_SNOW;
    Snow.flags                                  = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Snow.color_mode                             = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Snow.brightness_min                         = Epomaker_BRIGHTNESS_MIN;
    Snow.brightness_max                         = Epomaker_BRIGHTNESS_MAX;
    Snow.brightness                             = Epomaker_BRIGHTNESS_DEFAULT;
    Snow.speed_min                              = Epomaker_SPEED_MIN;
    Snow.speed_max                              = Epomaker_SPEED_MAX_SPECIAL;
    Snow.speed                                  = Epomaker_SPEED_DEFAULT;
    modes.push_back(Snow);

    mode Meteor;
    Meteor.name                                 = "Meteor";
    Meteor.value                                = Epomaker_MODE_METEOR;
    Meteor.flags                                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Meteor.color_mode                           = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Meteor.brightness_min                       = Epomaker_BRIGHTNESS_MIN;
    Meteor.brightness_max                       = Epomaker_BRIGHTNESS_MAX;
    Meteor.brightness                           = Epomaker_BRIGHTNESS_DEFAULT;
    Meteor.speed_min                            = Epomaker_SPEED_MIN;
    Meteor.speed_max                            = Epomaker_SPEED_MAX_SPECIAL;
    Meteor.speed                                = Epomaker_SPEED_DEFAULT;
    modes.push_back(Meteor);

    mode Through_the_snow;
    Through_the_snow.name                       = "Through the snow (non trace)";
    Through_the_snow.value                      = Epomaker_MODE_THROUGH_THE_SNOW_NON_TRACE;
    Through_the_snow.flags                      = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Through_the_snow.color_mode                 = MODE_COLORS_PER_LED | MODE_COLORS_RANDOM;
    Through_the_snow.brightness_min             = Epomaker_BRIGHTNESS_MIN;
    Through_the_snow.brightness_max             = Epomaker_BRIGHTNESS_MAX;
    Through_the_snow.brightness                 = Epomaker_BRIGHTNESS_DEFAULT;
    Through_the_snow.speed_min                  = Epomaker_SPEED_MIN;
    Through_the_snow.speed_max                  = Epomaker_SPEED_MAX_SPECIAL;
    Through_the_snow.speed                      = Epomaker_SPEED_DEFAULT;
    modes.push_back(Through_the_snow);

    SetupZones();
}

RGBController_EpomakerController::~RGBController_EpomakerController()
{
    delete controller;
}

void RGBController_EpomakerController::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetDazzle(modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    controller->SetColor(red, grn, blu);
}

void RGBController_EpomakerController::DeviceUpdateMode()
{
    if(modes[active_mode].value == Epomaker_MODE_DRIFT)
    {
        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            controller->SetOption(Epomaker_OPTION_DRIFT_LEFT);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
        {
            controller->SetOption(Epomaker_OPTION_DRIFT_RIGHT);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_UP)
        {
            controller->SetOption(Epomaker_OPTION_DRIFT_UP);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_DOWN)
        {
            controller->SetOption(Epomaker_OPTION_DRIFT_DOWN);
        }
    }
    else if(modes[active_mode].value == Epomaker_MODE_STEADY_STREAM)
    {
        /*---------------------------------------------------------*\
        | TODO: These OPTIONS (zig-zag, return) should not          |
        |   be DIRECTIONS (left, right)                             |
        \*---------------------------------------------------------*/
        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            controller->SetOption(Epomaker_OPTION_STEADY_STREAM_ZIG_ZAG);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
        {
            controller->SetOption(Epomaker_OPTION_STEADY_STREAM_RETURN);
        }
    }
    else if(modes[active_mode].value == Epomaker_MODE_CAISPRING_SURGING)
    {
        /*---------------------------------------------------------*\
        | TODO: These OPTIONS (in, out) should not                  |
        |   be DIRECTIONS (left, right)                             |
        \*---------------------------------------------------------*/
        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            controller->SetOption(Epomaker_OPTION_CAISPRING_SURGING_OUT);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
        {
            controller->SetOption(Epomaker_OPTION_CAISPRING_SURGING_IN);
        }
    }
    else if(modes[active_mode].value == Epomaker_MODE_FLOWERS_BLOOMING)
    {
        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            controller->SetOption(Epomaker_OPTION_FLOWERS_BLOOMING_LEFT);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
        {
            controller->SetOption(Epomaker_OPTION_FLOWERS_BLOOMING_RIGHT);
        }
    }
    else if(modes[active_mode].value == Epomaker_MODE_PEAK_TURN)
    {
        /*---------------------------------------------------------*\
        | TODO: These OPTIONS (clockwise, anti-clockwise)           |
        |   should not be DIRECTIONS (left, right)                  |
        \*---------------------------------------------------------*/
        if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
        {
            controller->SetOption(Epomaker_OPTION_PEAK_TURN_ANTI_CLOCKWISE);
        }
        else if(modes[active_mode].direction == MODE_DIRECTION_RIGHT)
        {
            controller->SetOption(Epomaker_OPTION_PEAK_TURN_CLOCKWISE);
        }
    }
    else
    {
        controller->SetOption(Epomaker_OPTION_DEFAULT);
    }

    controller->SetDazzle(modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness);
}

void RGBController_EpomakerController::SetupZones()
{
    zone new_zone;
    new_zone.name          = "Keyboard";
    new_zone.type          = ZONE_TYPE_SINGLE;
    new_zone.leds_min      = 1;
    new_zone.leds_max      = 1;
    new_zone.leds_count    = 1;
    new_zone.matrix_map    = NULL;

    zones.push_back(new_zone);

    led keyboard_led;
    keyboard_led.name = "Keyboard LEDs";
    keyboard_led.value = 0x00;
    leds.push_back(keyboard_led);

    SetupColors();
}


void RGBController_EpomakerController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | Not implemented                                           |
    \*---------------------------------------------------------*/
}

void RGBController_EpomakerController::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_EpomakerController::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}
