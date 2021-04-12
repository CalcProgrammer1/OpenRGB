/*-----------------------------------------*\
|  RGBController_MSIGPU.h                   |
|                                           |
|  Generic RGB Interface for MSI GPU        |
|                                           |
\*-----------------------------------------*/

#include "RGBController_MSIGPU.h"

static unsigned char brightness_values[] = { 0x14, 0x28, 0x3c, 0x50, 0x64 };
static unsigned char speed_values[]      = { 0x04, 0x02, 0x01 };

int RGBController_MSIGPU::GetDeviceMode()
{
    int dev_mode = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_MODE);

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if (modes[mode].value == dev_mode)
        {
            active_mode            = mode;
            break;
        }
    }

    return(active_mode);
}

RGBController_MSIGPU::RGBController_MSIGPU(MSIGPUController * msi_gpu_ptr)
{
    msi_gpu = msi_gpu_ptr;

    name        = "MSI GPU Device";
    vendor      = "MSI";
    type        = DEVICE_TYPE_GPU;
    description = "MSI GPU Device";
    location    = msi_gpu->GetDeviceLocation();

    mode Off;
    Off.name       = "Off";
    Off.value      = MSI_GPU_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = MSI_GPU_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min  = 0;
    Rainbow.speed_max  = 2;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Static;
    Static.name       = "Static";
    Static.value      = MSI_GPU_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Raindrop;
    Raindrop.name       = "Raindrop";
    Raindrop.value      = MSI_GPU_MODE_RAINDROP;
    Raindrop.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Raindrop.speed_min  = 0;
    Raindrop.speed_max  = 2;
    Raindrop.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Raindrop);

    mode Magic;
    Magic.name       = "Magic";
    Magic.value      = MSI_GPU_MODE_MAGIC;
    Magic.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Magic.speed_min  = 0;
    Magic.speed_max  = 2;
    Magic.color_mode = MODE_COLORS_NONE;
    modes.push_back(Magic);

    mode Patrolling;
    Patrolling.name       = "Patrolling";
    Patrolling.value      = MSI_GPU_MODE_PATROLLING;
    Patrolling.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Patrolling.speed_min  = 0;
    Patrolling.speed_max  = 2;
    Patrolling.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Patrolling);

    mode Streaming;
    Streaming.name       = "Streaming";
    Streaming.value      = MSI_GPU_MODE_STREAMING;
    Streaming.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Streaming.speed_min  = 0;
    Streaming.speed_max  = 2;
    Streaming.color_mode = MODE_COLORS_NONE;
    modes.push_back(Streaming);

    mode Lightning;
    Lightning.name       = "Lightning";
    Lightning.value      = MSI_GPU_MODE_LIGHTNING;
    Lightning.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Lightning.speed_min  = 0;
    Lightning.speed_max  = 2;
    Lightning.color_mode = MODE_COLORS_NONE;
    modes.push_back(Lightning);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = MSI_GPU_MODE_WAVE;
    Wave.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Wave.speed_min  = 0;
    Wave.speed_max  = 2;
    Wave.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Meteor;
    Meteor.name       = "Meteor";
    Meteor.value      = MSI_GPU_MODE_METEOR;
    Meteor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Meteor.speed_min  = 0;
    Meteor.speed_max  = 2;
    Meteor.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Meteor);

    mode Stack;
    Stack.name       = "Stack";
    Stack.value      = MSI_GPU_MODE_STACK;
    Stack.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Stack.speed_min  = 0;
    Stack.speed_max  = 2;
    Stack.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Rhythm;
    Rhythm.name       = "Rhythm";
    Rhythm.value      = MSI_GPU_MODE_RHYTHM;
    Rhythm.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Rhythm.speed_min  = 0;
    Rhythm.speed_max  = 2;
    Rhythm.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Rhythm);

    mode Flowing;
    Flowing.name       = "Flowing";
    Flowing.value      = MSI_GPU_MODE_FLOWING;
    Flowing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Flowing.speed_min  = 0;
    Flowing.speed_max  = 2;
    Flowing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flowing);

    mode Whirling;
    Whirling.name       = "Whirling";
    Whirling.value      = MSI_GPU_MODE_WHIRLING;
    Whirling.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Whirling.speed_min  = 0;
    Whirling.speed_max  = 2;
    Whirling.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Whirling);

    mode Twisting;
    Twisting.name       = "Twisting";
    Twisting.value      = MSI_GPU_MODE_TWISTING;
    Twisting.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Twisting.speed_min  = 0;
    Twisting.speed_max  = 2;
    Twisting.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Twisting);

    mode Laminating;
    Laminating.name       = "Laminating";
    Laminating.value      = MSI_GPU_MODE_LAMINATING;
    Laminating.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Laminating.speed_min  = 0;
    Laminating.speed_max  = 2;
    Laminating.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Laminating);

    mode Fadein;
    Fadein.name       = "Fadein";
    Fadein.value      = MSI_GPU_MODE_FADEIN;
    Fadein.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Fadein.speed_min  = 0;
    Fadein.speed_max  = 2;
    Fadein.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Fadein);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = MSI_GPU_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = 0;
    Breathing.speed_max  = 2;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode flashing;
    flashing.name       = "Flashing";
    flashing.value      = MSI_GPU_MODE_FLASHING;
    flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    flashing.speed_min  = 0;
    flashing.speed_max  = 2;
    flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(flashing);

    mode doubleflashing;
    doubleflashing.name       = "Doubleflashing";
    doubleflashing.value      = MSI_GPU_MODE_DOUBLEFLASHING;
    doubleflashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    doubleflashing.speed_min  = 0;
    doubleflashing.speed_max  = 2;
    doubleflashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(doubleflashing);

    SetupZones();

    active_mode = GetDeviceMode();
}

RGBController_MSIGPU::~RGBController_MSIGPU()
{
    delete msi_gpu;
}

void RGBController_MSIGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone msi_gpu_zone;
    msi_gpu_zone.name          = "GPU";
    msi_gpu_zone.type          = ZONE_TYPE_SINGLE;
    msi_gpu_zone.leds_min      = 1;
    msi_gpu_zone.leds_max      = 1;
    msi_gpu_zone.leds_count    = 3;
    msi_gpu_zone.matrix_map    = NULL;
    zones.push_back(msi_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led led1;
    led1.name = "Color 1";
    leds.push_back(led1);
    led led2;
    led2.name = "Color 2";
    leds.push_back(led2);
    led led3;
    led3.name = "Color 3";
    leds.push_back(led3);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char r1 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_R1);
    unsigned char g1 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_G1);
    unsigned char b1 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_B1);
    unsigned char r2 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_R2);
    unsigned char g2 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_G2);
    unsigned char b2 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_B2);
    unsigned char r3 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_R3);
    unsigned char g3 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_G3);
    unsigned char b3 = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_B3);

    colors[0] =  ToRGBColor(r1, g1, b1);
    colors[1] =  ToRGBColor(r2, g2, b2);
    colors[2] =  ToRGBColor(r3, g3, b3);
}

void RGBController_MSIGPU::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSIGPU::DeviceUpdateLEDs()
{
    msi_gpu->MSIGPURegisterWrite(MSI_GPU_REG_UNKNOWN, 0x00);

    if (modes[active_mode].value == MSI_GPU_MODE_FADEIN)
    {
        msi_gpu->SetRGB2(RGBGetRValue(colors[1]), RGBGetGValue(colors[1]), RGBGetBValue(colors[1]));
        msi_gpu->SetRGB3(RGBGetRValue(colors[2]), RGBGetGValue(colors[2]), RGBGetBValue(colors[2]));
    }
    else
    {
        msi_gpu->SetRGB1(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
    }
}

void RGBController_MSIGPU::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIGPU::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIGPU::SetCustomMode()
{
    active_mode = 2;  //Set to MSI_GPU_MODE_STATIC
}

void RGBController_MSIGPU::DeviceUpdateMode()
{
    if (modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        msi_gpu->MSIGPURegisterWrite(MSI_GPU_REG_BRIGHTNESS, brightness_values[4]); // how to access brightness value?
    }

    if (modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        msi_gpu->MSIGPURegisterWrite(MSI_GPU_REG_SPEED, speed_values[modes[active_mode].speed]);
    }

    msi_gpu->SetMode(modes[active_mode].value);
}
