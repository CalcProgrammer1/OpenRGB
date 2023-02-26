/*-----------------------------------------*\
|  RGBController_KasaSmart.cpp              |
|                                           |
|  Generic RGB Interface for KasaSmart      |
|                                           |
|  Devin Wendt (umbreon222) 2/16/2023       |
\*-----------------------------------------*/

#include "RGBController_KasaSmart.h"

/**------------------------------------------------------------------*\
    @name Kasa Smart Bulbs
    @category Light
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectKasaSmartControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_KasaSmart::RGBController_KasaSmart(KasaSmartController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "Kasa Smart Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = KASA_SMART_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Off;
    Off.name       = "Off";
    Off.value      = KASA_SMART_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_KasaSmart::~RGBController_KasaSmart()
{
    delete controller;
}

void RGBController_KasaSmart::SetupZones()
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

void RGBController_KasaSmart::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*-------------------------------------------*\
    | This device does not support resizing zones |
    \*-------------------------------------------*/
}

void RGBController_KasaSmart::DeviceUpdateLEDs()
{
    if(modes[active_mode].value != KASA_SMART_MODE_DIRECT)
    {
        return;
    }

    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_KasaSmart::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KasaSmart::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_KasaSmart::DeviceUpdateMode()
{
    if(modes[active_mode].value == KASA_SMART_MODE_OFF)
    {
        controller->TurnOff();
        return;
    }
}
