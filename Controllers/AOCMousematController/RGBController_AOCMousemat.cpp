/*-----------------------------------------*\
|  RGBController_AOCMousemat.cpp            |
|                                           |
|  Generic RGB Interface for AOC mousemat   |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/15/2023   |
\*-----------------------------------------*/

#include "RGBController_AOCMousemat.h"

/**------------------------------------------------------------------*\
    @name AOC Mousemat
    @category Mousemat
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAOCMousematControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AOCMousemat::RGBController_AOCMousemat(AOCMousematController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "AOC Mousemat Device";
    vendor      = "AOC";
    type        = DEVICE_TYPE_MOUSEMAT;
    description = "AOC Mousemat Device";
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

RGBController_AOCMousemat::~RGBController_AOCMousemat()
{
    delete controller;
}

void RGBController_AOCMousemat::SetupZones()
{
    zone mousemat_zone;
    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_SINGLE;
    mousemat_zone.leds_min      = 1;
    mousemat_zone.leds_max      = 1;
    mousemat_zone.leds_count    = 1;
    mousemat_zone.matrix_map    = NULL;
    zones.push_back(mousemat_zone);

    led mousemat_led;
    mousemat_led.name           = "Mousemat";
    leds.push_back(mousemat_led);

    SetupColors();
}

void RGBController_AOCMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AOCMousemat::DeviceUpdateLEDs()
{
    controller->SendDirect(&colors[0]);
}

void RGBController_AOCMousemat::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMousemat::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AOCMousemat::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
