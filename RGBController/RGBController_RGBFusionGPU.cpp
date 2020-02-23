/*-----------------------------------------*\
|  RGBController_RGBFusionGPU.cpp           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte RGB Fusion GPU Driver           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/23/2020   |
\*-----------------------------------------*/

#include "RGBController_RGBFusionGPU.h"


void RGBController_RGBFusionGPU::UpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    rgb_fusion->SetColor(red, grn, blu);
}

void RGBController_RGBFusionGPU::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_RGBFusionGPU::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

RGBController_RGBFusionGPU::RGBController_RGBFusionGPU(RGBFusionGPUController* rgb_fusion_ptr)
{
    rgb_fusion = rgb_fusion_ptr;

    name = "Gigabyte GPU";
    description = "RGB Fusion GPU";
    location = rgb_fusion->GetDeviceLocation();

    type = DEVICE_TYPE_GPU;

    mode Static;
    Static.name       = "Static";
    Static.value      = RGB_FUSION_GPU_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

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

    colors.resize(1);

    zone* new_zone = new zone();
    led*  new_led  = new led();

    std::vector<int>* zone_row = new std::vector<int>();

    // Set zone name to channel name
    new_zone->name = "GPU Zone";
    new_led->name  = "GPU LED";

    zone_row->push_back(0);

    // Aura devices can be either single or linear, never matrix
    // That means only one row is needed
    new_zone->map.push_back(*zone_row);

    // Push new LED to LEDs vector
    leds.push_back(*new_led);

    // Push new zone to zones vector
    zones.push_back(*new_zone);

    // Initialize active mode
    active_mode = 0;
}

void RGBController_RGBFusionGPU::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RGBFusionGPU::UpdateMode()
{
    rgb_fusion->SetMode(modes[active_mode].value, modes[active_mode].speed);
}
