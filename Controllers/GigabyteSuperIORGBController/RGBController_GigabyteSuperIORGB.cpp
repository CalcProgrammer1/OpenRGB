/*-------------------------------------------*\
|  RGBController_GigabyteSuperIORGB.cpp       |
|                                             |
|  Generic RGB Interface for Gigabyte RGB     |
|                                             |
|  Ryan Frankcombe (422gRdHuX5uk ) 9/11/2022  |
\*-------------------------------------------*/

#include "RGBController_GigabyteSuperIORGB.h"

/**------------------------------------------------------------------*\
    @name Gigabyte SuperIO RGB
    @category Motherboard
    @type SuperIO
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteSuperIORGBControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GigabyteSuperIORGB::RGBController_GigabyteSuperIORGB(GigabyteSuperIORGBController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Gigabyte SuperIO Compatible Motherboard";
    vendor      = "Gigabyte";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "Gigabyte SuperIO RGB Device";

    mode Direct;
    Direct.name          = "Direct";
    Direct.value         = 0;
    Direct.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name          = "Static";
    Static.value         = GIGABYTE_MODE1_STATIC;
    Direct.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name         = "Rainbow";
    Rainbow.value        = GIGABYTE_MODE1_RAINBOW;
    Direct.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = GIGABYTE_MODE1_BREATHING;
    Direct.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name        = "Flashing";
    Flashing.value       = GIGABYTE_MODE1_FLASHING;
    Direct.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    SetupZones();
}

RGBController_GigabyteSuperIORGB::~RGBController_GigabyteSuperIORGB()
{
    delete controller;
}

void RGBController_GigabyteSuperIORGB::SetupZones()
{
    zone gig_zone;
    gig_zone.name           = "Gigabyte Zone";
    gig_zone.type           = ZONE_TYPE_SINGLE;
    gig_zone.leds_min       = 1;
    gig_zone.leds_max       = 1;
    gig_zone.leds_count     = 1;
    gig_zone.matrix_map     = NULL;
    zones.push_back(gig_zone);

    led gig_led;
    gig_led.name = "LED_C1";
    leds.push_back(gig_led);

    SetupColors();
}

void RGBController_GigabyteSuperIORGB::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_GigabyteSuperIORGB::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_GigabyteSuperIORGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteSuperIORGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteSuperIORGB::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value);
}
