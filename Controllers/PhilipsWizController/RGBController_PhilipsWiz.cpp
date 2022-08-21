/*-----------------------------------------*\
|  RGBController_PhilipsWiz.cpp             |
|                                           |
|  Generic RGB Interface for Philips Wiz    |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/3/2020   |
\*-----------------------------------------*/

#include "RGBController_PhilipsWiz.h"

/**------------------------------------------------------------------*\
    @name Philips Wiz
    @category Light
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPhilipsWizControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PhilipsWiz::RGBController_PhilipsWiz(PhilipsWizController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "Philips Wiz Device";
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

RGBController_PhilipsWiz::~RGBController_PhilipsWiz()
{
    delete controller;
}

void RGBController_PhilipsWiz::SetupZones()
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

void RGBController_PhilipsWiz::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PhilipsWiz::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_PhilipsWiz::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsWiz::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsWiz::DeviceUpdateMode()
{

}
