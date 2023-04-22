/*-----------------------------------------*\
|  RGBController_AOCMouse.cpp               |
|                                           |
|  Generic RGB Interface for AOC mouse      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/20/2023   |
\*-----------------------------------------*/

#include "RGBController_AOCMouse.h"

/**------------------------------------------------------------------*\
    @name AOC Mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAOCMouseControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCMouse::RGBController_AOCMouse(AOCMouseController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "AOC Mouse Device";
    vendor      = "AOC";
    type        = DEVICE_TYPE_MOUSE;
    description = "AOC Mouse Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
};

RGBController_AOCMouse::~RGBController_AOCMouse()
{
    delete controller;
}

void RGBController_AOCMouse::SetupZones()
{
    zone logo_zone;
    logo_zone.name                  = "Logo";
    logo_zone.type                  = ZONE_TYPE_SINGLE;
    logo_zone.leds_min              = 1;
    logo_zone.leds_max              = 1;
    logo_zone.leds_count            = 1;
    logo_zone.matrix_map            = NULL;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name                   = "Logo";
    leds.push_back(logo_led);

    zone scroll_wheel_zone;
    scroll_wheel_zone.name          = "Scroll Wheel";
    scroll_wheel_zone.type          = ZONE_TYPE_SINGLE;
    scroll_wheel_zone.leds_min      = 1;
    scroll_wheel_zone.leds_max      = 1;
    scroll_wheel_zone.leds_count    = 1;
    scroll_wheel_zone.matrix_map    = NULL;
    zones.push_back(scroll_wheel_zone);

    led scroll_wheel_led;
    scroll_wheel_led.name           = "Scroll Wheel";
    leds.push_back(scroll_wheel_led);

    SetupColors();
}

void RGBController_AOCMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCMouse::DeviceUpdateLEDs()
{
    controller->SendDirect(&colors[0]);
}

void RGBController_AOCMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMouse::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
