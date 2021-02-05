/*--------------------------------------------------------------*\
|  RGBController_HoltekA070.cpp                                  |
|                                                                |
|  Generic RGB Interface for Holtek USB Gaming Mouse [04d9:a070] |
|                                                                |
|  Santeri Pikarinen (santeri3700) 8/01/2020                     |
\*--------------------------------------------------------------*/

#include "RGBController_HoltekA070.h"

RGBController_HoltekA070::RGBController_HoltekA070(HoltekA070Controller* holtek_ptr)
{
    holtek = holtek_ptr;

    name        = "Holtek USB Gaming Mouse Device";
    vendor      = "Holtek";
    type        = DEVICE_TYPE_MOUSE;
    description = "Holtek USB Gaming Mouse Device";
    location    = holtek->GetDeviceLocation();
    serial      = holtek->GetSerialString();

    mode Static;
    Static.name       = "Static";
    Static.speed      = HOLTEK_A070_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed_min  = HOLTEK_A070_MODE_BREATHING_SLOW;
    Breathing.speed_max  = HOLTEK_A070_MODE_BREATHING_FAST;
    Breathing.speed      = HOLTEK_A070_MODE_BREATHING_MEDIUM;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_HoltekA070::~RGBController_HoltekA070()
{
    delete holtek;
}

void RGBController_HoltekA070::SetupZones()
{
    zone mouse_zone;
    mouse_zone.name          = "Mouse";
    mouse_zone.type          = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min      = 1;
    mouse_zone.leds_max      = 1;
    mouse_zone.leds_count    = 1;
    mouse_zone.matrix_map    = NULL;
    zones.push_back(mouse_zone);

    led mouse_led;
    mouse_led.name = "Mouse";
    leds.push_back(mouse_led);

    SetupColors();
}

void RGBController_HoltekA070::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HoltekA070::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char green = RGBGetGValue(colors[0]);
    unsigned char blue = RGBGetBValue(colors[0]);

    holtek->SendCustomColor(red, green, blue);
}

void RGBController_HoltekA070::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA070::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HoltekA070::SetCustomMode()
{

}

void RGBController_HoltekA070::DeviceUpdateMode()
{
  holtek->SendMode(modes[active_mode].speed);
}
