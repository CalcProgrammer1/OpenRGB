/*-----------------------------------------*\
|  RGBController_GalaxGPU.cpp               |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include "RGBController_GalaxGPU.h"

/*
int RGBController_GalaxGPU::GetDeviceMode()
{

    int dev_mode = galax_gpu->GalaxGPURegisterRead(GALAX_MODE_REGISTER);
    int color_mode = MODE_COLORS_PER_LED;

    if(dev_mode == GALAX_MODE_STATIC)
    {
        if (galax_gpu->direct)
        {
            dev_mode = GALAX;
        }
    }

    switch(dev_mode)
    {
    case AURA_GPU_MODE_OFF:
    case AURA_GPU_MODE_SPECTRUM_CYCLE:
        color_mode = MODE_COLORS_NONE;
        break;
    }

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            active_mode            = mode;
            modes[mode].color_mode = color_mode;
        }
    }

    return(active_mode);

}
*/
RGBController_GalaxGPU::RGBController_GalaxGPU(GalaxGPUController * galax_gpu_ptr)
{
    galax_gpu = galax_gpu_ptr;


    name        = galax_gpu->GetDeviceName();
    type        = DEVICE_TYPE_GPU;
    description = "GALAX / KFA2 RTX GPU";
    version     = "1.0";
    location    = galax_gpu->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    //Direct.value      = GALAX_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    /*
    mode Off;
    Off.name       = "Off";
    Off.value      = AURA_GPU_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_GPU_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_GPU_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = AURA_GPU_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode Spectrum_Cycle;
    Spectrum_Cycle.name       = "Spectrum Cycle";
    Spectrum_Cycle.value      = AURA_GPU_MODE_SPECTRUM_CYCLE;
    Spectrum_Cycle.flags      = 0;
    Spectrum_Cycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(Spectrum_Cycle);
    */

    SetupZones();

    //active_mode = GetDeviceMode();
}

void RGBController_GalaxGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone galax_gpu_zone;
    galax_gpu_zone.name          = "GPU";
    galax_gpu_zone.type          = ZONE_TYPE_SINGLE;
    galax_gpu_zone.leds_min      = 1;
    galax_gpu_zone.leds_max      = 1;
    galax_gpu_zone.leds_count    = 1;
    galax_gpu_zone.matrix_map    = NULL;
    zones.push_back(galax_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led galax_gpu_led;
    galax_gpu_led.name = "GPU";
    leds.push_back(galax_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = galax_gpu->GetLEDRed();
    unsigned char grn = galax_gpu->GetLEDGreen();
    unsigned char blu = galax_gpu->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_GalaxGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GalaxGPU::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        /*
        if (GetMode() == 0)
        {
            galax_gpu->SetLEDColorsDirect(red, grn, blu);
        }
        else
        {
            galax_gpu->SetLEDColorsEffect(red, grn, blu);
        }
        */

        galax_gpu->SetLEDColorsDirect(red, grn, blu);
    }
}

void RGBController_GalaxGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPU::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_GalaxGPU::UpdateMode()
{
    /*
    int new_mode = modes[active_mode].value;
    galax_gpu->direct = false;

    switch(new_mode)
    {

    // Set all LEDs to 0 and Mode to static as a workaround for the non existing Off Mode
    case AURA_GPU_MODE_OFF:
        galax_gpu->SetLEDColorsEffect(0, 0, 0);
        new_mode = AURA_GPU_MODE_STATIC;
        break;

    // Direct mode is done by switching to Static and not applying color changes
    case AURA_GPU_MODE_DIRECT:
        galax_gpu->direct = true;
        new_mode = AURA_GPU_MODE_STATIC;
        break;
    }

    galax_gpu->SetMode(new_mode);
    */
}
