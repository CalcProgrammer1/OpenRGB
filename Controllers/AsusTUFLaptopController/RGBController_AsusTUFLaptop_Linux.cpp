/*---------------------------------------------------------*\
| RGBController_AsusTUFLaptop_Linux.cpp                     |
|                                                           |
|   RGBController for ASUS TUF laptop                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AsusTUFLaptop_Linux.h"

/**------------------------------------------------------------------*\
    @name Asus TUF Laptop Linux WMI
    @category Keyboard
    @type File Stream
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusTUFLaptopLinuxControllers
    @comment Tested on ASUS TUF Gaming A15 2022
    PLEASE UPDATE YOUR KERNEL TO A VERSION NEWER THAN 6.1.0

    Every devices supported by asus-wmi would work technically.
\*-------------------------------------------------------------------*/

RGBController_AsusTUFLaptopLinux::RGBController_AsusTUFLaptopLinux(AsusTUFLaptopLinuxController* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = "ASUS TUF Laptop Keyboard";
    vendor                          = "ASUS";
    type                            = DEVICE_TYPE_LAPTOP;
    description                     = "Asus TUF Device";
    location                        = ASUS_KBD_BACKLIGHT_BASE_PATH;

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 4;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min           = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN;
    Direct.brightness_max           = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX;
    Direct.brightness               = ASUS_KBD_BACKLIGHT_BRIGHTNESS;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                     = "Static";
    Static.value                    = 0;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.brightness_min           = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN;
    Static.brightness_max           = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX;
    Static.brightness               = ASUS_KBD_BACKLIGHT_BRIGHTNESS;
    Static.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = 1;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.speed_min             = ASUS_KBD_BACKLIGHT_SPEED_MIN;
    Breathing.speed_max             = ASUS_KBD_BACKLIGHT_SPEED_MAX;
    Breathing.brightness_min        = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN;
    Breathing.brightness_max        = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX;
    Breathing.brightness            = ASUS_KBD_BACKLIGHT_BRIGHTNESS;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed                 = ASUS_KBD_BACKLIGHT_SPEED;
    modes.push_back(Breathing);

    mode Cycle;
    Cycle.name                      = "Spectrum Cycle";
    Cycle.value                     = 2;
    Cycle.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.speed_min                 = ASUS_KBD_BACKLIGHT_SPEED_MIN;
    Cycle.speed_max                 = ASUS_KBD_BACKLIGHT_SPEED_MAX;
    Cycle.brightness_min            = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN;
    Cycle.brightness_max            = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX;
    Cycle.brightness                = ASUS_KBD_BACKLIGHT_BRIGHTNESS;
    Cycle.color_mode                = MODE_COLORS_NONE;
    Cycle.speed                     = ASUS_KBD_BACKLIGHT_SPEED;
    modes.push_back(Cycle);

    mode Flashing;
    Flashing.name                   = "Flashing";
    Flashing.value                  = 0x0A;
    Flashing.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Flashing.brightness_min         = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MIN;
    Flashing.brightness_max         = ASUS_KBD_BACKLIGHT_BRIGHTNESS_MAX;
    Flashing.brightness             = ASUS_KBD_BACKLIGHT_BRIGHTNESS;
    Flashing.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();
}

void RGBController_AsusTUFLaptopLinux::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zones.resize(1);
    zones[0].type           = ZONE_TYPE_SINGLE;
    zones[0].name           = "Keyboard Backlight zone";
    zones[0].leds_min       = 1;
    zones[0].leds_max       = 1;
    zones[0].leds_count     = 1;
    zones[0].matrix_map     = NULL;

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    leds.resize(1);
    leds[0].name = "Keyboard Backlight LED";

    SetupColors();
}

void RGBController_AsusTUFLaptopLinux::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusTUFLaptopLinux::DeviceUpdateLEDs()
{
    uint8_t red   = RGBGetRValue(colors[0]);
    uint8_t green = RGBGetGValue(colors[0]);
    uint8_t blue  = RGBGetBValue(colors[0]);
    uint8_t speed = 0;
    uint8_t mode  = modes[active_mode].value;
    uint8_t save  = 1;
    if(mode == 4)
    {
        mode = 0;
        save = 0;
    }
    if(mode == 1 || mode == 2)
    {
        speed = modes[active_mode].speed;
    }

    controller->SendUpdate(mode, speed, save, red, green, blue);
    controller->SendBrightness(modes[active_mode].brightness);
}

void RGBController_AsusTUFLaptopLinux::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusTUFLaptopLinux::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusTUFLaptopLinux::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
