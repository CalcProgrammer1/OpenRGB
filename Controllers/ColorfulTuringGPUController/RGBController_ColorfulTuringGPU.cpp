#include "RGBController_ColorfulTuringGPU.h"
#include <array>

/**------------------------------------------------------------------*\
    @name Colorful GPU
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectColorfulTuringGPUControllers
    @comment This card only supports direct mode
\*-------------------------------------------------------------------*/

RGBController_ColorfulTuringGPU::RGBController_ColorfulTuringGPU(ColorfulTuringGPUController * colorful_gpu_ptr)
{
    controller  = colorful_gpu_ptr;
    name        = "Colorful GPU Device";
    vendor      = "Colorful";
    type        = DEVICE_TYPE_GPU;
    description = name;
    location    = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

}

RGBController_ColorfulTuringGPU::~RGBController_ColorfulTuringGPU()
{
    delete controller;
}

void RGBController_ColorfulTuringGPU::SetupZones()
{
    zone new_zone;

    new_zone.name       = "GPU";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);
    leds[0].name = "GPU LED";

    SetupColors();
}

void RGBController_ColorfulTuringGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_ColorfulTuringGPU::DeviceUpdateLEDs()
{
    controller->SetDirect(colors[0]);
}

void RGBController_ColorfulTuringGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulTuringGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ColorfulTuringGPU::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
