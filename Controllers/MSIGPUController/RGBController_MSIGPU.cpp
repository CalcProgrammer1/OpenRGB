/*-----------------------------------------*\
|  RGBController_MSIGPU.h                   |
|                                           |
|  Generic RGB Interface for MSI GPU        |
|                                           |
\*-----------------------------------------*/

#include "RGBController_MSIGPU.h"
#include <array>

static const std::array<unsigned char, 5> brightness_values = { 0x14, 0x28, 0x3c, 0x50, 0x64 };
static const std::array<unsigned char, 3> speed_values      = { 0x04, 0x02, 0x01 };

int RGBController_MSIGPU::GetDeviceMode()
{
    unsigned char dev_mode = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_MODE);

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if (modes[mode].value == dev_mode)
        {
            active_mode = mode;
            break;
        }
    }

    return(active_mode);
}

int RGBController_MSIGPU::GetModeSpeed()
{
    unsigned char mode_speed = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_SPEED);

    for(std::size_t speed = 0; speed < speed_values.size(); speed++)
    {
        if(speed_values[speed] == mode_speed)
        {
            return speed;
        }
    }

    return 0;
}

int RGBController_MSIGPU::GetModeBrightness()
{
    unsigned char mode_brightness = msi_gpu->MSIGPURegisterRead(MSI_GPU_REG_BRIGHTNESS);

    for(std::size_t brightness = 0; brightness < brightness_values.size(); brightness++)
    {
        if (brightness_values[brightness] == mode_brightness)
        {
            return brightness;
        }
    }

    return 0;
}

/**------------------------------------------------------------------*\
    @name MSI GPU
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMSIGPUControllers
    @comment
\*-------------------------------------------------------------------*/

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
    Off.flags      = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Rainbow;
    Rainbow.name           = "Rainbow";
    Rainbow.value          = MSI_GPU_MODE_RAINBOW;
    Rainbow.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min      = 0;
    Rainbow.speed          = 0;
    Rainbow.speed_max      = 2;
    Rainbow.brightness_min = 0;
    Rainbow.brightness     = 2;
    Rainbow.brightness_max = 4;
    Rainbow.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = MSI_GPU_MODE_STATIC;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness     = 2;
    Direct.brightness_max = 4;
    modes.push_back(Direct);

    mode Raindrop;
    Raindrop.name           = "Raindrop";
    Raindrop.value          = MSI_GPU_MODE_RAINDROP;
    Raindrop.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Raindrop.speed_min      = 0;
    Raindrop.speed          = 0;
    Raindrop.speed_max      = 2;
    Raindrop.brightness_min = 0;
    Raindrop.brightness     = 2;
    Raindrop.brightness_max = 4;
    Raindrop.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Raindrop);

    mode Magic;
    Magic.name           = "Magic";
    Magic.value          = MSI_GPU_MODE_MAGIC;
    Magic.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Magic.speed_min      = 0;
    Magic.speed          = 0;
    Magic.speed_max      = 2;
    Magic.brightness_min = 0;
    Magic.brightness     = 2;
    Magic.brightness_max = 4;
    Magic.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Magic);

    mode Patrolling;
    Patrolling.name           = "Patrolling";
    Patrolling.value          = MSI_GPU_MODE_PATROLLING;
    Patrolling.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Patrolling.speed_min      = 0;
    Patrolling.speed          = 0;
    Patrolling.speed_max      = 2;
    Patrolling.brightness_min = 0;
    Patrolling.brightness     = 2;
    Patrolling.brightness_max = 4;
    Patrolling.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Patrolling);

    mode Streaming;
    Streaming.name           = "Streaming";
    Streaming.value          = MSI_GPU_MODE_STREAMING;
    Streaming.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Streaming.speed_min      = 0;
    Streaming.speed          = 0;
    Streaming.speed_max      = 2;
    Streaming.brightness_min = 0;
    Streaming.brightness     = 2;
    Streaming.brightness_max = 4;
    Streaming.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Streaming);

    mode Lightning;
    Lightning.name           = "Lightning";
    Lightning.value          = MSI_GPU_MODE_LIGHTNING;
    Lightning.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Lightning.speed_min      = 0;
    Lightning.speed          = 0;
    Lightning.speed_max      = 2;
    Lightning.brightness_min = 0;
    Lightning.brightness     = 2;
    Lightning.brightness_max = 4;
    Lightning.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Lightning);

    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = MSI_GPU_MODE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Wave.speed_min      = 0;
    Wave.speed          = 0;
    Wave.speed_max      = 2;
    Wave.brightness_min = 0;
    Wave.brightness     = 2;
    Wave.brightness_max = 4;
    Wave.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Meteor;
    Meteor.name           = "Meteor";
    Meteor.value          = MSI_GPU_MODE_METEOR;
    Meteor.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Meteor.speed_min      = 0;
    Meteor.speed          = 0;
    Meteor.speed_max      = 2;
    Meteor.brightness_min = 0;
    Meteor.brightness     = 2;
    Meteor.brightness_max = 4;
    Meteor.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Meteor);

    mode Stack;
    Stack.name           = "Stack";
    Stack.value          = MSI_GPU_MODE_STACK;
    Stack.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Stack.speed_min      = 0;
    Stack.speed          = 0;
    Stack.speed_max      = 2;
    Stack.brightness_min = 0;
    Stack.brightness     = 2;
    Stack.brightness_max = 4;
    Stack.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Rhythm;
    Rhythm.name           = "Rhythm";
    Rhythm.value          = MSI_GPU_MODE_RHYTHM;
    Rhythm.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Rhythm.speed_min      = 0;
    Rhythm.speed          = 0;
    Rhythm.speed_max      = 2;
    Rhythm.brightness_min = 0;
    Rhythm.brightness     = 2;
    Rhythm.brightness_max = 4;
    Rhythm.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Rhythm);

    mode Flowing;
    Flowing.name           = "Flowing";
    Flowing.value          = MSI_GPU_MODE_FLOWING;
    Flowing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Flowing.speed_min      = 0;
    Flowing.speed          = 0;
    Flowing.speed_max      = 2;
    Flowing.brightness_min = 0;
    Flowing.brightness     = 2;
    Flowing.brightness_max = 4;
    Flowing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Flowing);

    mode Whirling;
    Whirling.name           = "Whirling";
    Whirling.value          = MSI_GPU_MODE_WHIRLING;
    Whirling.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Whirling.speed_min      = 0;
    Whirling.speed          = 0;
    Whirling.speed_max      = 2;
    Whirling.brightness_min = 0;
    Whirling.brightness     = 2;
    Whirling.brightness_max = 4;
    Whirling.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Whirling);

    mode Twisting;
    Twisting.name           = "Twisting";
    Twisting.value          = MSI_GPU_MODE_TWISTING;
    Twisting.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Twisting.speed_min      = 0;
    Twisting.speed          = 0;
    Twisting.speed_max      = 2;
    Twisting.brightness_min = 0;
    Twisting.brightness     = 2;
    Twisting.brightness_max = 4;
    Twisting.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Twisting);

    mode Laminating;
    Laminating.name           = "Laminating";
    Laminating.value          = MSI_GPU_MODE_LAMINATING;
    Laminating.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Laminating.speed_min      = 0;
    Laminating.speed          = 0;
    Laminating.speed_max      = 2;
    Laminating.brightness_min = 0;
    Laminating.brightness     = 2;
    Laminating.brightness_max = 4;
    Laminating.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Laminating);

    mode Fadein;
    Fadein.name           = "Fadein";
    Fadein.value          = MSI_GPU_MODE_FADEIN;
    Fadein.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Fadein.speed_min      = 0;
    Fadein.speed          = 0;
    Fadein.speed_max      = 2;
    Fadein.brightness_min = 0;
    Fadein.brightness     = 2;
    Fadein.brightness_max = 4;
    Fadein.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Fadein);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = MSI_GPU_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min      = 0;
    Breathing.speed          = 0;
    Breathing.speed_max      = 2;
    Breathing.brightness_min = 0;
    Breathing.brightness     = 2;
    Breathing.brightness_max = 4;
    Breathing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode flashing;
    flashing.name           = "Flashing";
    flashing.value          = MSI_GPU_MODE_FLASHING;
    flashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    flashing.speed_min      = 0;
    flashing.speed          = 0;
    flashing.speed_max      = 2;
    flashing.brightness_min = 0;
    flashing.brightness     = 2;
    flashing.brightness_max = 4;
    flashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(flashing);

    mode doubleflashing;
    doubleflashing.name           = "Doubleflashing";
    doubleflashing.value          = MSI_GPU_MODE_DOUBLEFLASHING;
    doubleflashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    doubleflashing.speed_min      = 0;
    doubleflashing.speed          = 0;
    doubleflashing.speed_max      = 2;
    doubleflashing.brightness_min = 0;
    doubleflashing.brightness     = 2;
    doubleflashing.brightness_max = 4;
    doubleflashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(doubleflashing);

    SetupZones();

    active_mode                   = GetDeviceMode();
    modes[active_mode].speed      = GetModeSpeed();
    modes[active_mode].brightness = GetModeBrightness();
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

void RGBController_MSIGPU::DeviceUpdateMode()
{
    if (modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        msi_gpu->MSIGPURegisterWrite(MSI_GPU_REG_BRIGHTNESS, brightness_values[modes[active_mode].brightness]);
    }

    if (modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        msi_gpu->MSIGPURegisterWrite(MSI_GPU_REG_SPEED, speed_values[modes[active_mode].speed]);
    }

    msi_gpu->SetMode(modes[active_mode].value);
}

void RGBController_MSIGPU::DeviceSaveMode()
{
    msi_gpu->Save();
}
