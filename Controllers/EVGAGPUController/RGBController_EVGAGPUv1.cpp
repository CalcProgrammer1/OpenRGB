/*-----------------------------------------*\
|  RGBController_EVGAGPUv1.cpp              |
|                                           |
|  Generic RGB Interface for OpenRGB EVGA   |
|  GPU V1 (Pascal) Driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/11/2020   |
\*-----------------------------------------*/

#include "RGBController_EVGAGPUv1.h"

RGBController_EVGAGPUv1::RGBController_EVGAGPUv1(EVGAGPUv1Controller* evga_ptr)
{
    evga = evga_ptr;

    name        = "EVGA GPU";
    vendor      = "EVGA";
    description = "EVGA RGB v1 GPU Device";
    location    = evga->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Off;
    Off.name       = "Off";
    Off.value      = EVGA_GPU_V1_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = EVGA_GPU_V1_MODE_CUSTOM;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = EVGA_GPU_V1_MODE_RAINBOW;
    Rainbow.flags      = 0;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = EVGA_GPU_V1_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    SetupZones();

    // Initialize active mode and stored color

    unsigned char raw_active_mode = evga->GetMode();

    active_mode = 0;
    for(unsigned int i = 0; i < modes.size(); i++)
    {
        if (modes[i].value == raw_active_mode)
        {
            active_mode = i;
            break;
        }
    }

    unsigned char r = evga->GetRed();
    unsigned char g = evga->GetGreen();
    unsigned char b = evga->GetBlue();

    RGBColor color = ToRGBColor(r, g, b);
    colors[0] = color;
}

RGBController_EVGAGPUv1::~RGBController_EVGAGPUv1()
{
    delete evga;
}

void RGBController_EVGAGPUv1::SetupZones()
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

void RGBController_EVGAGPUv1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_EVGAGPUv1::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    evga->SetColor(red, grn, blu);
}

void RGBController_EVGAGPUv1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_EVGAGPUv1::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_EVGAGPUv1::DeviceUpdateMode()
{
    evga->SetMode((unsigned char)modes[(unsigned int)active_mode].value);
}
