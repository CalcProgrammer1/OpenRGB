/*-----------------------------------------*\
|  RGBController_EVGAGPUv2.cpp              |
|                                           |
|  Generic RGB Interface for OpenRGB EVGA   |
|  GPU V2 (Pascal) Driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/13/2020   |
\*-----------------------------------------*/

#include "RGBController_EVGAGPUv2.h"

RGBController_EVGAGPUv2::RGBController_EVGAGPUv2(EVGAGPUv2Controller* evga_ptr)
{
    evga = evga_ptr;

    name        = "EVGA GPU";
    vendor      = "EVGA";
    description = "EVGA RGB v2 GPU Device";
    location    = evga->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Off;
    Off.name       = "Off";
    Off.value      = EVGA_GPU_V2_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = EVGA_GPU_V2_MODE_CUSTOM;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    // mode Rainbow;
    // Rainbow.name       = "Rainbow";
    // Rainbow.value      = EVGA_GPU_V2_MODE_RAINBOW;
    // Rainbow.flags      = 0;
    // Rainbow.color_mode = MODE_COLORS_NONE;
    // modes.push_back(Rainbow);

    // mode Breathing;
    // Breathing.name       = "Breathing";
    // Breathing.value      = EVGA_GPU_V2_MODE_BREATHING;
    // Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    // Breathing.color_mode = MODE_COLORS_PER_LED;
    // modes.push_back(Breathing);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

RGBController_EVGAGPUv2::~RGBController_EVGAGPUv2()
{
    delete evga;
}

void RGBController_EVGAGPUv2::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_EVGAGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAGPUv2::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    evga->SetColor(red, grn, blu);
}

void RGBController_EVGAGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv2::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_EVGAGPUv2::DeviceUpdateMode()
{
    evga->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
}
