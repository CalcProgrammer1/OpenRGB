/*---------------------------------------------------------*\
| RGBController_AsusTUFLaptop_Windows.cpp                   |
|                                                           |
|   RGBController for ASUS TUF laptop                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusTUFLaptop_Windows.h"

using namespace std::chrono_literals;

/**------------------------------------------------------------------*\
    @name Asus TUF Laptop
    @category Keyboard
    @type WMI
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusTUFLaptopWMIControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusTUFLaptopWMI::RGBController_AsusTUFLaptopWMI(AsusTUFLaptopController* controller_ptr)
{
    name                        = "ASUS TUF Laptop Keyboard";
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_LAPTOP;
    description                 = "WMI Device";
    location                    = "\\\\.\\ATKACPI";

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASUS_WMI_KEYBOARD_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    Static.brightness_max       = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    Static.brightness_min       = ASUS_WMI_KEYBOARD_BRIGHTNESS_MIN;
    Static.brightness           = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASUS_WMI_KEYBOARD_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    Breathing.speed_min         = ASUS_WMI_KEYBOARD_SPEED_MIN;
    Breathing.speed_max         = ASUS_WMI_KEYBOARD_SPEED_MAX;
    Breathing.speed             = 2;
    Breathing.brightness_max    = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    Breathing.brightness_min    = ASUS_WMI_KEYBOARD_BRIGHTNESS_MIN;
    Breathing.brightness        = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = ASUS_WMI_KEYBOARD_MODE_COLORCYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    ColorCycle.speed_min        = ASUS_WMI_KEYBOARD_SPEED_MIN;
    ColorCycle.speed_max        = ASUS_WMI_KEYBOARD_SPEED_MAX;
    ColorCycle.speed            = 2;
    ColorCycle.brightness_max   = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    ColorCycle.brightness_min   = ASUS_WMI_KEYBOARD_BRIGHTNESS_MIN;
    ColorCycle.brightness       = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    modes.push_back(ColorCycle);

    mode Strobing;
    Strobing.name               = "Strobing";
    Strobing.value              = ASUS_WMI_KEYBOARD_MODE_STROBING;
    Strobing.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Strobing.color_mode         = MODE_COLORS_PER_LED;
    Strobing.brightness_max     = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    Strobing.brightness_min     = ASUS_WMI_KEYBOARD_BRIGHTNESS_MIN;
    Strobing.brightness         = ASUS_WMI_KEYBOARD_BRIGHTNESS_MAX;
    modes.push_back(Strobing);

    SetupZones();

    controller = controller_ptr;

    ReadConfiguration();
}

RGBController_AsusTUFLaptopWMI::~RGBController_AsusTUFLaptopWMI()
{
    delete controller;
}

void RGBController_AsusTUFLaptopWMI::SetupZones()
{
    /*---------------------------------------------------------*\
    | Device only has one zone and one led                      |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->type           = ZONE_TYPE_SINGLE;
    new_zone->name           = "Keyboard Backlight zone";
    new_zone->leds_min       = 1;
    new_zone->leds_max       = 1;
    new_zone->leds_count     = 1;
    new_zone->matrix_map     = NULL;

    new_led->name            = "Keyboard Backlight LED";

    zones.push_back(*new_zone);
    leds.push_back(*new_led);

    SetupColors();
}

void RGBController_AsusTUFLaptopWMI::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

/*---------------------------------------------------------*\
| Break this function off since we have to call save in the |
  same operation as doing everything else.                  |
\*---------------------------------------------------------*/
void RGBController_AsusTUFLaptopWMI::ControllerSetMode(bool save)
{
    unsigned char red   = RGBGetRValue(colors[0]);
    unsigned char green = RGBGetGValue(colors[0]);
    unsigned char blue  = RGBGetBValue(colors[0]);

    unsigned char mode  = (unsigned char)modes[(unsigned int)active_mode].value;

    /*------------------------------------------------------------*\
    | Use speed only if the mode supports it. Otherwise set normal |
    \*------------------------------------------------------------*/
    unsigned char speed = ASUS_WMI_KEYBOARD_SPEED_NORMAL;

    if (modes[(unsigned int)active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        speed = (unsigned char)modes[(unsigned int)active_mode].speed;
    }

    controller->setMode(red, green, blue, mode, speed, save);
}

void RGBController_AsusTUFLaptopWMI::DeviceUpdateLEDs()
{
    ControllerSetMode(false);
}

void RGBController_AsusTUFLaptopWMI::UpdateZoneLEDs(int /*zone*/)
{
    ControllerSetMode(false);
}

void RGBController_AsusTUFLaptopWMI::UpdateSingleLED(int /*led*/)
{
    ControllerSetMode(false);
}

void RGBController_AsusTUFLaptopWMI::DeviceUpdateMode()
{
    if (modes[(unsigned int)active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->setBrightness((unsigned char)modes[(unsigned int)active_mode].brightness);
    }
    ControllerSetMode(false);
}

void RGBController_AsusTUFLaptopWMI::ReadConfiguration()
{
    if (modes[(unsigned int)active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        modes[(unsigned int)active_mode].brightness = controller->getBrightness();
    }
}

void RGBController_AsusTUFLaptopWMI::DeviceSaveMode()
{
    ControllerSetMode(true);
}
