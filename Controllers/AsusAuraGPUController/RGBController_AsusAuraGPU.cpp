/*-----------------------------------------*\
|  RGBController_AsusAuraGPU.h              |
|                                           |
|  Generic RGB Interface for Asus Aura GPU  |
|                                           |
|  Jan Rettig (Klapstuhl) 14.02.2020        |
\*-----------------------------------------*/

#include "RGBController_AsusAuraGPU.h"

int RGBController_AuraGPU::GetDeviceMode()
{
    int dev_mode = aura_gpu->AuraGPURegisterRead(AURA_GPU_REG_MODE);
    int color_mode = MODE_COLORS_PER_LED;

    if(dev_mode == AURA_GPU_MODE_STATIC)
    {
        if (aura_gpu->direct)
        {
            dev_mode = AURA_GPU_MODE_DIRECT;
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

RGBController_AuraGPU::RGBController_AuraGPU(AuraGPUController * aura_gpu_ptr)
{
    aura_gpu = aura_gpu_ptr;


    name        = aura_gpu->GetDeviceName();
    vendor      = "ASUS";
    type        = DEVICE_TYPE_GPU;
    description = "ASUS Aura GPU Device";
    version     = "0.00.1";
    location    = aura_gpu->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_GPU_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

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

    SetupZones();

    active_mode = GetDeviceMode();
}

RGBController_AuraGPU::~RGBController_AuraGPU()
{
    delete aura_gpu;
}

void RGBController_AuraGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone aura_gpu_zone;
    aura_gpu_zone.name          = "GPU";
    aura_gpu_zone.type          = ZONE_TYPE_SINGLE;
    aura_gpu_zone.leds_min      = 1;
    aura_gpu_zone.leds_max      = 1;
    aura_gpu_zone.leds_count    = 1;
    aura_gpu_zone.matrix_map    = NULL;
    zones.push_back(aura_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led aura_gpu_led;
    aura_gpu_led.name = "GPU";
    leds.push_back(aura_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = aura_gpu->GetLEDRed();
    unsigned char grn = aura_gpu->GetLEDGreen();
    unsigned char blu = aura_gpu->GetLEDBlue();

    colors[0] =  ToRGBColor(red, grn, blu);
}

void RGBController_AuraGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraGPU::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        if (GetMode() == 0)
        {
            aura_gpu->SetLEDColorsDirect(red, grn, blu);
        }
        else 
        {
            aura_gpu->SetLEDColorsEffect(red, grn, blu);
        }
    }
}

void RGBController_AuraGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraGPU::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraGPU::DeviceUpdateMode()
{
    int new_mode = modes[active_mode].value;
    aura_gpu->direct = false;
    
    switch(new_mode)
    {
        
    // Set all LEDs to 0 and Mode to static as a workaround for the non existing Off Mode
    case AURA_GPU_MODE_OFF:
        aura_gpu->SetLEDColorsEffect(0, 0, 0);
        new_mode = AURA_GPU_MODE_STATIC;
        break;
    
    // Direct mode is done by switching to Static and not applying color changes
    case AURA_GPU_MODE_DIRECT:
        aura_gpu->direct = true;
        new_mode = AURA_GPU_MODE_STATIC;
        break;
    }

    aura_gpu->SetMode(new_mode);
}
