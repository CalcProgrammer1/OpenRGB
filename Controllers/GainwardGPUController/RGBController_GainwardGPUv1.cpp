/*-----------------------------------------*\
|  RGBController_GainwardGPUv1.cpp          |
|                                           |
|  Driver for Gainward RGB v1 on GPUs       |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include "RGBController_GainwardGPUv1.h"

int RGBController_GainwardGPUv1::GetDeviceMode()
{
    active_mode = 1;
    return(active_mode);
}

RGBController_GainwardGPUv1::RGBController_GainwardGPUv1(GainwardGPUv1Controller * gainward_gpu_ptr)
{
    gainward_gpu            = gainward_gpu_ptr;

    name                    = "Gainward GPU";
    vendor                  = "Gainward";
    type                    = DEVICE_TYPE_GPU;
    description             = "Gainward GTX GPU";
    version                 = "";
    location                = gainward_gpu->GetDeviceLocation();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 1;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_GainwardGPUv1::~RGBController_GainwardGPUv1()
{
    delete gainward_gpu;
}

void RGBController_GainwardGPUv1::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone gainward_gpu_zone;
    gainward_gpu_zone.name              = "GPU";
    gainward_gpu_zone.type              = ZONE_TYPE_SINGLE;
    gainward_gpu_zone.leds_min          = 1;
    gainward_gpu_zone.leds_max          = 1;
    gainward_gpu_zone.leds_count        = 1;
    gainward_gpu_zone.matrix_map        = NULL;
    zones.push_back(gainward_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led gainward_gpu_led;
    gainward_gpu_led.name = "GPU";
    leds.push_back(gainward_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = gainward_gpu->GetLEDRed();
    unsigned char grn = gainward_gpu->GetLEDGreen();
    unsigned char blu = gainward_gpu->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_GainwardGPUv1::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GainwardGPUv1::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        gainward_gpu->SetLEDColors(red, grn, blu);
    }
}

void RGBController_GainwardGPUv1::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GainwardGPUv1::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GainwardGPUv1::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_GainwardGPUv1::DeviceUpdateMode()
{

}
