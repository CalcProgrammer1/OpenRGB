/*-----------------------------------------*\
|  RGBController_LinuxLED.cpp               |
|                                           |
|  Generic RGB Interface for Linux LED      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/25/2020   |
\*-----------------------------------------*/

#include "RGBController_LinuxLED.h"

RGBController_LinuxLED::RGBController_LinuxLED(LinuxLEDController* controller_ptr)
{
    controller = controller_ptr;

    name        = "Linux LED";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "Linux Sysfs LED Device";

    location    = "R: " + controller->GetRedPath() + "\r\n" +
                  "G: " + controller->GetGreenPath() + "\r\n" +
                  "B: " + controller->GetBluePath();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LinuxLED::~RGBController_LinuxLED()
{
    delete controller;
}

void RGBController_LinuxLED::SetupZones()
{
    zone led_zone;
    led_zone.name       = "RGB Light";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "RGB Light";

    leds.push_back(new_led);

    SetupColors();
}

void RGBController_LinuxLED::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LinuxLED::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetRGB(red, grn, blu);
}

void RGBController_LinuxLED::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LinuxLED::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LinuxLED::SetCustomMode()
{

}

void RGBController_LinuxLED::DeviceUpdateMode()
{

}
