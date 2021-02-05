/*-----------------------------------------*\
|  RGBController_GigabyteRGBFusionGPU.cpp   |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion GPU Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/23/2020   |
\*-----------------------------------------*/

#include "RGBController_GigabyteRGBFusionGPU.h"

RGBController_RGBFusionGPU::RGBController_RGBFusionGPU(RGBFusionGPUController* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name        = "Gigabyte GPU";
    vendor      = "Gigabyte";
    description = "RGB Fusion GPU";
    location    = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = RGB_FUSION_GPU_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = RGB_FUSION_GPU_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = RGB_FUSION_GPU_SPEED_SLOWEST;
    Breathing.speed_max  = RGB_FUSION_GPU_SPEED_FASTEST;
    Breathing.speed      = RGB_FUSION_GPU_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = RGB_FUSION_GPU_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min  = RGB_FUSION_GPU_SPEED_SLOWEST;
    Flashing.speed_max  = RGB_FUSION_GPU_SPEED_FASTEST;
    Flashing.speed      = RGB_FUSION_GPU_SPEED_NORMAL;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode DualFlashing;
    DualFlashing.name       = "Dual Flashing";
    DualFlashing.value      = RGB_FUSION_GPU_MODE_DUAL_FLASHING;
    DualFlashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DualFlashing.speed_min  = RGB_FUSION_GPU_SPEED_SLOWEST;
    DualFlashing.speed_max  = RGB_FUSION_GPU_SPEED_FASTEST;
    DualFlashing.speed      = RGB_FUSION_GPU_SPEED_NORMAL;
    DualFlashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DualFlashing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = RGB_FUSION_GPU_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min  = RGB_FUSION_GPU_SPEED_SLOWEST;
    SpectrumCycle.speed_max  = RGB_FUSION_GPU_SPEED_FASTEST;
    SpectrumCycle.speed      = RGB_FUSION_GPU_SPEED_NORMAL;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    SetupZones();

    // Initialize active mode
    active_mode = 0;
}

RGBController_RGBFusionGPU::~RGBController_RGBFusionGPU()
{
    delete rgb_fusion;
}

void RGBController_RGBFusionGPU::SetupZones()
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

void RGBController_RGBFusionGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RGBFusionGPU::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    rgb_fusion->SetColor(red, grn, blu);
}

void RGBController_RGBFusionGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusionGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RGBFusionGPU::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RGBFusionGPU::DeviceUpdateMode()
{
    rgb_fusion->SetMode((unsigned char)modes[(unsigned int)active_mode].value, (unsigned char)modes[(unsigned int)active_mode].speed);
}
