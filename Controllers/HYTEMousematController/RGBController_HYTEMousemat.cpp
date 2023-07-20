/*-----------------------------------------*\
|  RGBController_HYTEMousemat.cpp           |
|                                           |
|  Generic RGB Interface for HYTE CNVS RGB  |
|  mousemat                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/18/2023   |
\*-----------------------------------------*/

#include "RGBController_HYTEMousemat.h"

RGBController_HYTEMousemat::RGBController_HYTEMousemat(HYTEMousematController* controller_ptr)
{
    controller          = controller_ptr;

    name                = "HYTE Mousemat";
    description         = "HYTE Mousemat Device";
    type                = DEVICE_TYPE_MOUSEMAT;
    location            = controller->GetLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_HYTEMousemat::~RGBController_HYTEMousemat()
{

}

void RGBController_HYTEMousemat::SetupZones()
{
    zone mousemat_zone;

    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_LINEAR;
    mousemat_zone.leds_min      = 50;
    mousemat_zone.leds_max      = 50;
    mousemat_zone.leds_count    = 50;
    mousemat_zone.matrix_map    = NULL;

    zones.push_back(mousemat_zone);

    for(unsigned int led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led mousemat_led;

        mousemat_led.name       = "Mousemat LED";

        leds.push_back(mousemat_led);
    }

    SetupColors();
}

void RGBController_HYTEMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_HYTEMousemat::DeviceUpdateLEDs()
{
    printf("DeviceUpdateLEDs called\r\n");
    controller->StreamingCommand(&colors[0]);
}

void RGBController_HYTEMousemat::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_HYTEMousemat::UpdateSingleLED(int /*led*/)
{

}

void RGBController_HYTEMousemat::DeviceUpdateMode()
{

}
