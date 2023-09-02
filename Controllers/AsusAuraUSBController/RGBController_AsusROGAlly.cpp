/*-----------------------------------------*\
|  RGBController_AsusROGAlly.cpp            |
|                                           |
|  Generic RGB Interface for Asus ROG Ally  |
|  controller driver                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2023   |
\*-----------------------------------------*/

#include "RGBController_AsusROGAlly.h"

/**------------------------------------------------------------------*\
    @name Asus ROG Ally
    @category Gamepad
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusROGAlly
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusROGAlly::RGBController_AsusROGAlly(ROGAllyController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "ASUS ROG Ally";
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_GAMEPAD;
    description                 = "ASUS ROG Ally Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = ROG_ALLY_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ROG_ALLY_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ROG_ALLY_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min         = ROG_ALLY_SPEED_MIN;
    Breathing.speed_max         = ROG_ALLY_SPEED_MAX;
    Breathing.speed             = ROG_ALLY_SPEED_MED;
    Breathing.colors_min        = 2;
    Breathing.colors_max        = 2;
    Breathing.colors.resize(2);
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = ROG_ALLY_MODE_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED;
    ColorCycle.color_mode       = MODE_COLORS_RANDOM;
    ColorCycle.speed_min        = ROG_ALLY_SPEED_MIN;
    ColorCycle.speed_max        = ROG_ALLY_SPEED_MAX;
    ColorCycle.speed            = ROG_ALLY_SPEED_MED;
    modes.push_back(ColorCycle);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = ROG_ALLY_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.color_mode          = MODE_COLORS_RANDOM;
    Rainbow.speed_min           = ROG_ALLY_SPEED_MIN;
    Rainbow.speed_max           = ROG_ALLY_SPEED_MAX;
    Rainbow.speed               = ROG_ALLY_SPEED_MED;
    modes.push_back(Rainbow);

    mode Strobing;
    Strobing.name               = "Strobing";
    Strobing.value              = ROG_ALLY_MODE_STROBING;
    Strobing.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Strobing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Strobing.colors_min         = 1;
    Strobing.colors_max         = 1;
    Strobing.colors.resize(1);
    modes.push_back(Strobing);

    SetupZones();
}

RGBController_AsusROGAlly::~RGBController_AsusROGAlly()
{
    delete controller;
}

void RGBController_AsusROGAlly::SetupZones()
{
    zone left_stick_zone;

    left_stick_zone.name         = "Left Stick";
    left_stick_zone.type         = ZONE_TYPE_SINGLE;
    left_stick_zone.leds_min     = 2;
    left_stick_zone.leds_max     = 2;
    left_stick_zone.leds_count   = 2;
    left_stick_zone.matrix_map   = NULL;

    zones.push_back(left_stick_zone);

    for(unsigned int i = 0; i < 2; i++)
    {
        led left_stick_led;

        left_stick_led.name      = "Left Stick LED " + std::to_string(i);

        leds.push_back(left_stick_led);
    }

    zone right_stick_zone;

    right_stick_zone.name         = "Right Stick";
    right_stick_zone.type         = ZONE_TYPE_SINGLE;
    right_stick_zone.leds_min     = 2;
    right_stick_zone.leds_max     = 2;
    right_stick_zone.leds_count   = 2;
    right_stick_zone.matrix_map   = NULL;

    zones.push_back(right_stick_zone);

    for(unsigned int i = 0; i < 2; i++)
    {
        led right_stick_led;

        right_stick_led.name      = "Right Stick LED " + std::to_string(i);

        leds.push_back(right_stick_led);
    }

    SetupColors();
}

void RGBController_AsusROGAlly::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusROGAlly::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == ROG_ALLY_MODE_DIRECT)
    {
        controller->UpdateLeds(std::vector<RGBColor>(colors));
    }
}

void RGBController_AsusROGAlly::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusROGAlly::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusROGAlly::DeviceUpdateMode()
{
    if(modes[active_mode].value == ROG_ALLY_MODE_DIRECT)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        unsigned int rog_ally_direction = ROG_ALLY_DIRECTION_RIGHT;

        if((modes[active_mode].flags & MODE_FLAG_HAS_DIRECTION_LR) && (modes[active_mode].direction == MODE_DIRECTION_LEFT))
        {
            rog_ally_direction = ROG_ALLY_DIRECTION_LEFT;
        }

        controller->UpdateDevice(modes[active_mode].value, modes[active_mode].colors, modes[active_mode].speed, modes[active_mode].brightness, rog_ally_direction);
    }
}

void RGBController_AsusROGAlly::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
