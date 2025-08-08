/*---------------------------------------------------------*\
| RGBController_EVGAMouse.cpp                               |
|                                                           |
|   RGBController for EVGA mouse                            |
|                                                           |
|   Cooper Knaak                                23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_EVGAMouse.h"
#include "Colors.h"

/**------------------------------------------------------------------*\
    @name EVGA USB X20 Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectWiredEVGAMouse,DetectWirelessEVGAMouse
    @comment The EVGA USB mouse currently supports the X20 (both wired
        and wireless, but not bluetooth).
\*-------------------------------------------------------------------*/
RGBController_EVGAMouse::RGBController_EVGAMouse(EVGAMouseController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetName();
    vendor                      = "EVGA";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "EVGA Mouse Device";
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Static;
    Static.name                 = "Static";
    Static.value                = EVGA_PERIPHERAL_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Static.brightness_min       = EVGA_PERIPHERAL_BRIGHTNESS_MIN;
    Static.brightness_max       = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Static.brightness           = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(Static.colors_max);
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = EVGA_PERIPHERAL_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.brightness_min    = EVGA_PERIPHERAL_BRIGHTNESS_MIN;
    Breathing.brightness_max    = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Breathing.brightness        = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Breathing.colors_min        = 2;
    Breathing.colors_max        = 2;
    Breathing.colors            = {COLOR_GREEN, COLOR_BLUE};
    Breathing.speed_min         = EVGA_PERIPHERAL_SPEED_SLOWEST;
    Breathing.speed_max         = EVGA_PERIPHERAL_SPEED_FASTEST;
    Breathing.speed             = EVGA_PERIPHERAL_SPEED_FASTEST;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name                = "Spectrum Cycle";
    Rainbow.value               = EVGA_PERIPHERAL_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.brightness_min      = EVGA_PERIPHERAL_BRIGHTNESS_MIN;
    Rainbow.brightness_max      = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Rainbow.brightness          = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Rainbow.speed_min           = EVGA_PERIPHERAL_SPEED_SLOWEST;
    Rainbow.speed_max           = EVGA_PERIPHERAL_SPEED_FASTEST;
    Rainbow.speed               = EVGA_PERIPHERAL_SPEED_FASTEST;
    Rainbow.colors              = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_WHITE};
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Pulse;
    Pulse.name                  = "Pulse";
    Pulse.value                 = EVGA_PERIPHERAL_MODE_PULSE;
    Pulse.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Pulse.brightness_min        = EVGA_PERIPHERAL_BRIGHTNESS_MIN;
    Pulse.brightness_max        = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Pulse.brightness            = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Pulse.speed_min             = EVGA_PERIPHERAL_SPEED_SLOWEST;
    Pulse.speed_max             = EVGA_PERIPHERAL_SPEED_FASTEST;
    Pulse.speed                 = EVGA_PERIPHERAL_SPEED_FASTEST;
    Pulse.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Pulse.colors_min            = 2;
    Pulse.colors_max            = 7;
    Pulse.colors                = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_WHITE};
    modes.push_back(Pulse);

    mode Trigger;
    Trigger.name                = "Trigger";
    /*-----------------------------------*\
    | Pulse to Trigger skips from 4 to 6. |
    \*-----------------------------------*/
    Trigger.value               = EVGA_PERIPHERAL_MODE_TRIGGER;
    Trigger.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_AUTOMATIC_SAVE;
    Trigger.brightness_min      = EVGA_PERIPHERAL_BRIGHTNESS_MIN;
    Trigger.brightness_max      = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Trigger.brightness          = EVGA_PERIPHERAL_BRIGHTNESS_MAX;
    Trigger.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Trigger.colors_min          = 7;
    Trigger.colors_max          = 7;
    Trigger.colors              = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_WHITE};
    modes.push_back(Trigger);

    active_mode = EVGA_PERIPHERAL_MODE_STATIC;

    Init_Controller();
    SetupZones();

    EVGAMouseControllerDeviceState current_state = controller->GetState();
    for(unsigned int i = 0; i < modes.size(); i++)
    {
        if(modes[i].value == current_state.mode)
        {
            active_mode = i;
            break;
        }
    }
    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        modes[active_mode].colors = current_state.colors;
    }
    modes[active_mode].brightness   = current_state.brightness;
    modes[active_mode].speed        = current_state.speed;
    colors.resize(EVGA_PERIPHERAL_LED_COUNT);
    for(unsigned int i = 0; i < colors.size(); i++)
    {
        colors[i] = controller->GetColorOfLed(i);
    }
}

RGBController_EVGAMouse::~RGBController_EVGAMouse()
{
    delete controller;
}

void RGBController_EVGAMouse::Init_Controller()
{
    /*------------------------------------------------------------------------------------------*\
    | Since each LED can have its own mode, each one needs to be its own zone with a single LED. |
    \*------------------------------------------------------------------------------------------*/
    zone front_zone;
    front_zone.name         = "Front";
    front_zone.type         = ZONE_TYPE_SINGLE;
    front_zone.leds_min     = 1;
    front_zone.leds_max     = 1;
    front_zone.leds_count   = 1;
    front_zone.matrix_map   = NULL;
    zones.push_back(front_zone);

    zone wheel_zone;
    wheel_zone.name         = "Scroll Wheel";
    wheel_zone.type         = ZONE_TYPE_SINGLE;
    wheel_zone.leds_min     = 1;
    wheel_zone.leds_max     = 1;
    wheel_zone.leds_count   = 1;
    wheel_zone.matrix_map   = NULL;
    zones.push_back(wheel_zone);

    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    led front_led;
    front_led.name          = "Front LED";
    front_led.value         = 0;
    leds.push_back(front_led);

    led wheel_led;
    wheel_led.name          = "Scroll Wheel LED";
    wheel_led.value         = 1;
    leds.push_back(wheel_led);

    led back_led;
    back_led.name           = "Back LED";
    back_led.value          = 2;
    leds.push_back(back_led);
}

void RGBController_EVGAMouse::SetupZones()
{
    SetupColors();
}

void RGBController_EVGAMouse::ResizeZone(int /* zone */, int /* new_size */)
{
    /*--------------------------------------*\
    | This device does not support resizing. |
    \*--------------------------------------*/
}

void RGBController_EVGAMouse::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < colors.size(); i++)
    {
        controller->SetLed(i, modes[active_mode].brightness, modes[active_mode].speed, colors[i]);
    }
}

void RGBController_EVGAMouse::UpdateZoneLEDs(int zone)
{
    controller->SetLed(zone, modes[active_mode].brightness, modes[active_mode].speed, colors[zone]);
}

void RGBController_EVGAMouse::UpdateSingleLED(int led)
{
    controller->SetLed(led,  modes[active_mode].brightness, modes[active_mode].speed, colors[led]);
}

void RGBController_EVGAMouse::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value);
    /*--------------------------------------------------------------------*\
    | Modes with specific colors should use their mode's colors. All other |
    | modes should use the colors stored in this controller.               |
    \*--------------------------------------------------------------------*/
    std::vector<RGBColor>* temp_colors = &colors;
    /*-------------------------------------------------------------------------------------------------*\
    | Rainbow does not have mode specific colors that can be controlled by OpenRGB, so it does not have |
    | the corresponding flag. However, to properly activate it, you still must pass the correct list of |
    | colors in the LED packet. Specifying the wrong number of colors causes the effect to restart the  |
    | cycle earlier in the sequence than it is supposed to.                                             |
    \*-------------------------------------------------------------------------------------------------*/
    if((modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR) || modes[active_mode].value == EVGA_PERIPHERAL_MODE_RAINBOW)
    {
        temp_colors = &(modes[active_mode].colors);
    }
    if(modes[active_mode].value == EVGA_PERIPHERAL_MODE_BREATHING)
    {
        controller->SetAllLedsAndActivate(modes[active_mode].brightness, modes[active_mode].speed, *temp_colors);
    }
    else
    {
        controller->SetAllLeds(modes[active_mode].brightness, modes[active_mode].speed, *temp_colors);
    }
}

void RGBController_EVGAMouse::DeviceSaveMode()
{

}

int RGBController_EVGAMouse::GetDeviceMode()
{
    return controller->GetMode();
}

