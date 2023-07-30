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

    name        = "Seagate Device";
    vendor      = "Seagate";
    type        = DEVICE_TYPE_STORAGE;
    description = "Seagate Device";
    location    = controller->GetLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = SEAGATE_MODE_STATIC;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Blink;
    Blink.name              = "Flashing";
    Blink.value             = SEAGATE_MODE_BLINK;
    Blink.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Blink.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = SEAGATE_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

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
    led_zone.type       = ZONE_TYPE_LINEAR;
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

    switch(modes[active_mode].value)
    {
        case SEAGATE_MODE_STATIC:
            controller->SetLEDStatic(led, red, grn, blu, false);
            break;
        
        case SEAGATE_MODE_BLINK:
            controller->SetLEDBlink(led, red, grn, blu, false);
            break;
        
        case SEAGATE_MODE_BREATHING:
            controller->SetLEDBreathing(led, red, grn, blu, false);
            break;
    }
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

        switch(modes[active_mode].value)
        {
            case SEAGATE_MODE_STATIC:
                controller->SetLEDStatic(led_idx, red, grn, blu, true);
                break;
            
            case SEAGATE_MODE_BLINK:
                controller->SetLEDBlink(led_idx, red, grn, blu, true);
                break;

            case SEAGATE_MODE_BREATHING:
                controller->SetLEDBreathing(led_idx, red, grn, blu, true);
                break;
        }
    }
}
