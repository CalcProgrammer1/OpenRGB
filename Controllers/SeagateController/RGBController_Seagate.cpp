/*-----------------------------------------*\
|  RGBController_Seagate.cpp                |
|                                           |
|  Generic RGB Interface for Seagate        |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/8/2022   |
\*-----------------------------------------*/

#include "RGBController_Seagate.h"

/**------------------------------------------------------------------*\
    @name Seagate
    @category Storage
    @type USB
    @save :x:
    @direct :x:
    @effects :x:
    @detectors DetectSeagateControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Seagate::RGBController_Seagate(SeagateController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Seagate";
    type        = DEVICE_TYPE_STORAGE;
    description = "Seagate Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_Seagate::~RGBController_Seagate()
{
    delete controller;
}

void RGBController_Seagate::SetupZones()
{
    zone led_zone;
    led_zone.name       = "LED Strip";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 6;
    led_zone.leds_max   = 6;
    led_zone.leds_count = 6;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        new_led.name        = "LED Strip LED";

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_Seagate::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_Seagate::DeviceUpdateLEDs()
{
    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        UpdateSingleLED(led_idx);
    }
}

void RGBController_Seagate::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_Seagate::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    controller->SetLED(led, red, grn, blu, false);
}

void RGBController_Seagate::DeviceUpdateMode()
{
}

void RGBController_Seagate::DeviceSaveMode()
{
    for(unsigned int led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        controller->SetLED(led_idx, red, grn, blu, true);
    }
}
