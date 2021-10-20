/*-----------------------------------------*\
|  RGBController_PNYGPU.cpp                 |
|                                           |
|  Generic RGB Interface for OpenRGB PNY    |
|  GPU (Turing) Driver                      |
|                                           |
|  KendallMorgan   10/17/2021               |
\*-----------------------------------------*/

#include "RGBController_PNYGPU.h"

RGBController_PNYGPU::RGBController_PNYGPU(PNYGPUController* pny_ptr)
{
    pny = pny_ptr;

    name                    = "PNY GPU";
    vendor                  = "PNY";
    description             = "PNY RGB GPU Device";
    location                = pny->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Off;
    Off.name                = "Off";
    Off.value               = PNY_GPU_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = PNY_GPU_MODE_CUSTOM;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);


    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

void RGBController_PNYGPU::SetupZones()
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

void RGBController_PNYGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PNYGPU::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    pny->SetColor(red, grn, blu);
}

void RGBController_PNYGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PNYGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PNYGPU::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_PNYGPU::DeviceUpdateMode()
{
    pny->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
}
