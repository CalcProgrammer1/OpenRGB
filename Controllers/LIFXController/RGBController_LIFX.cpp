/*-----------------------------------------*\
|  RGBController_LIFX.cpp                   |
|                                           |
|  Generic RGB Interface for LIFX           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/5/2022    |
\*-----------------------------------------*/

#include "RGBController_LIFX.h"

/**------------------------------------------------------------------*\
    @name LIFX Globes
    @category Light
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectLIFXControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_LIFX::RGBController_LIFX(LIFXController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "LIFX Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LIFX::~RGBController_LIFX()
{
    delete controller;
}

void RGBController_LIFX::SetupZones()
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

void RGBController_LIFX::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LIFX::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_LIFX::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LIFX::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_LIFX::DeviceUpdateMode()
{

}
