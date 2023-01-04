/*-----------------------------------------*\
|  RGBController_MSIGPUv2.cpp               |
|                                           |
|  ITE9 RGB Interface for MSI GPU           |
|  Wojciech Lazarski 03/Jan/2023            |
|                                           |
\*-----------------------------------------*/

#include "RGBController_MSIGPUv2.h"
#include <array>

static const unsigned char brightness_values[5] = { 0x14, 0x28, 0x3c, 0x50, 0x64 };
static const unsigned char speed_values[3]      = { 0x04, 0x02, 0x01 };

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

RGBController_MSIGPUv2::RGBController_MSIGPUv2(MSIGPUv2Controller * msi_gpu_ptr)
{
    msi_gpu = msi_gpu_ptr;

    name        = "MSI GPU Device";
    vendor      = "MSI";
    type        = DEVICE_TYPE_GPU;
    description = "MSI GPU Device";
    location    = msi_gpu->GetDeviceLocation();

    mode Off;
    Off.name            = "Off";
    Off.value           = MSI_GPU_V2_MODE_OFF;
    Off.flags           = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Rainbow;
    Rainbow.name           = "Rainbow";
    Rainbow.value          = MSI_GPU_V2_MODE_RAINBOW;
    Rainbow.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
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
    Direct.value          = MSI_GPU_V2_MODE_STATIC;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = 0;
    Direct.brightness     = 2;
    Direct.brightness_max = 4;
    modes.push_back(Direct);

    mode Magic;
    Magic.name           = "Magic";
    Magic.value          = MSI_GPU_V2_MODE_MAGIC;
    Magic.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Magic.speed_min      = 0;
    Magic.speed          = 0;
    Magic.speed_max      = 2;
    Magic.brightness_min = 0;
    Magic.brightness     = 2;
    Magic.brightness_max = 4;
    Magic.color_mode     = MODE_COLORS_NONE;
    modes.push_back(Magic);

    mode ColorCycle;
    ColorCycle.name           = "Color Cycle";
    ColorCycle.value          = MSI_GPU_V2_MODE_MAGIC;
    ColorCycle.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.speed_min      = 0;
    ColorCycle.speed          = 0;
    ColorCycle.speed_max      = 2;
    ColorCycle.brightness_min = 0;
    ColorCycle.brightness     = 2;
    ColorCycle.brightness_max = 4;
    ColorCycle.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(ColorCycle);


    mode Patrolling;
    Patrolling.name           = "Patrolling";
    Patrolling.value          = MSI_GPU_V2_MODE_PATROLLING;
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
    Streaming.value          = MSI_GPU_V2_MODE_STREAMING;
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
    Lightning.value          = MSI_GPU_V2_MODE_LIGHTNING;
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
    Wave.value          = MSI_GPU_V2_MODE_RAINBOW;//Rainbow has two modes now
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_DIRECTION_LR;
    Wave.speed_min      = 0;
    Wave.speed          = 1;
    Wave.speed_max      = 2;
    Wave.brightness_min = 0;
    Wave.brightness     = 2;
    Wave.brightness_max = 4;
    Wave.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Meteor;
    Meteor.name           = "Meteor";
    Meteor.value          = MSI_GPU_V2_MODE_METEOR;
    Meteor.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Meteor.speed_min      = 0;
    Meteor.speed          = 1;
    Meteor.speed_max      = 2;
    Meteor.brightness_min = 0;
    Meteor.brightness     = 2;
    Meteor.brightness_max = 4;
    Meteor.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Meteor);

    mode Stack;
    Stack.name           = "Stack";
    Stack.value          = MSI_GPU_V2_MODE_STACK;
    Stack.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Stack.speed_min      = 0;
    Stack.speed          = 1;
    Stack.speed_max      = 2;
    Stack.brightness_min = 0;
    Stack.brightness     = 2;
    Stack.brightness_max = 4;
    Stack.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Rhythm;
    Rhythm.name           = "Rhythm";
    Rhythm.value          = MSI_GPU_V2_MODE_RHYTHM;
    Rhythm.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Rhythm.speed_min      = 0;
    Rhythm.speed          = 1;
    Rhythm.speed_max      = 2;
    Rhythm.brightness_min = 0;
    Rhythm.brightness     = 2;
    Rhythm.brightness_max = 4;
    Rhythm.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Rhythm);

    mode Flowing;
    Flowing.name           = "Flowing";
    Flowing.value          = MSI_GPU_V2_MODE_FLOWING;
    Flowing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Flowing.speed_min      = 0;
    Flowing.speed          = 1;
    Flowing.speed_max      = 2;
    Flowing.brightness_min = 0;
    Flowing.brightness     = 2;
    Flowing.brightness_max = 4;
    Flowing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Flowing);

    mode Whirling;
    Whirling.name           = "Whirling";
    Whirling.value          = MSI_GPU_V2_MODE_WHIRLING;
    Whirling.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Whirling.speed_min      = 0;
    Whirling.speed          = 1;
    Whirling.speed_max      = 2;
    Whirling.brightness_min = 0;
    Whirling.brightness     = 2;
    Whirling.brightness_max = 4;
    Whirling.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Whirling);

    mode Fadein;
    Fadein.name           = "Fadein";
    Fadein.value          = MSI_GPU_V2_MODE_FADEIN;
    Fadein.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Fadein.speed_min      = 0;
    Fadein.speed          = 1;
    Fadein.speed_max      = 2;
    Fadein.brightness_min = 0;
    Fadein.brightness     = 2;
    Fadein.brightness_max = 4;
    Fadein.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Fadein);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = MSI_GPU_V2_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min      = 0;
    Breathing.speed          = 1;
    Breathing.speed_max      = 2;
    Breathing.brightness_min = 0;
    Breathing.brightness     = 2;
    Breathing.brightness_max = 4;
    Breathing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode flashing;
    flashing.name           = "Flashing";
    flashing.value          = MSI_GPU_V2_MODE_FLASHING;
    flashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    flashing.speed_min      = 0;
    flashing.speed          = 1;
    flashing.speed_max      = 2;
    flashing.brightness_min = 0;
    flashing.brightness     = 2;
    flashing.brightness_max = 4;
    flashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(flashing);

    mode doubleflashing;
    doubleflashing.name           = "Doubleflashing";
    doubleflashing.value          = MSI_GPU_V2_MODE_DOUBLEFLASHING;
    doubleflashing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    doubleflashing.speed_min      = 0;
    doubleflashing.speed          = 1;
    doubleflashing.speed_max      = 2;
    doubleflashing.brightness_min = 0;
    doubleflashing.brightness     = 2;
    doubleflashing.brightness_max = 4;
    doubleflashing.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(doubleflashing);

    SetupZones();

    active_mode                   = 0;
    modes[active_mode].speed      = 1;
    modes[active_mode].brightness = 3;
}

RGBController_MSIGPUv2::~RGBController_MSIGPUv2()
{
    delete msi_gpu;
}

void RGBController_MSIGPUv2::SetupZones()
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
    colors[0] =  ToRGBColor(0xff, 0, 0);
    colors[1] =  ToRGBColor(0xff, 0, 0);
    colors[2] =  ToRGBColor(0xff, 0 ,0);
}

void RGBController_MSIGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

bool RGBController_MSIGPUv2::TimeToSend()
{
    /*-----------------------------------------------------*\
    | Rate limit is 1000(ms) / wait_time in Frames Per Sec  |
    \*-----------------------------------------------------*/
    const uint8_t wait_time     = 33;
    return (std::chrono::steady_clock::now() - last_commit_time) > std::chrono::milliseconds(wait_time);
}


void RGBController_MSIGPUv2::DeviceUpdateAll()
{
    switch(modes[active_mode].value)
    {
        case MSI_GPU_V2_MODE_RAINBOW:
            if (modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
            {
                if (modes[active_mode].direction == MODE_DIRECTION_LEFT)
                {
                    msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_LEFT | MSI_GPU_V2_CONTROL_NON_RGBMODE);
                }
                else
                {
                    msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT | MSI_GPU_V2_CONTROL_NON_RGBMODE);
                }
                msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0);
                msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

                msi_gpu->SetRGB1V2(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
                msi_gpu->SetRGB2V2(RGBGetRValue(colors[1]), RGBGetGValue(colors[1]), RGBGetBValue(colors[1]));
                msi_gpu->SetRGB3V2(RGBGetRValue(colors[2]), RGBGetGValue(colors[2]), RGBGetBValue(colors[2]));
            }
            else
            {
                if(modes[active_mode].direction == MODE_DIRECTION_LEFT)
                {
                    msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_LEFT);
                }
                else
                {
                    msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT);
                }

                msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
                msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            }
            break;

    case MSI_GPU_V2_MODE_MAGIC:
        if(modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
        {
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_NON_RGBMODE);

            msi_gpu->SetRGB1V2(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
            msi_gpu->SetRGB2V2(RGBGetRValue(colors[1]), RGBGetGValue(colors[1]), RGBGetBValue(colors[1]));
            msi_gpu->SetRGB3V2(RGBGetRValue(colors[2]), RGBGetGValue(colors[2]), RGBGetBValue(colors[2]));
        }
        else
        {
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT);
        }
        break;


        case MSI_GPU_V2_MODE_BREATHING:
        case MSI_GPU_V2_MODE_FADEIN:
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            msi_gpu->SetRGB1V2(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
            msi_gpu->SetRGB2V2(RGBGetRValue(colors[1]), RGBGetGValue(colors[1]), RGBGetBValue(colors[1]));
            break;

        case MSI_GPU_V2_MODE_FLOWING:
        case MSI_GPU_V2_MODE_WHIRLING:
        case MSI_GPU_V2_MODE_PATROLLING:
        case MSI_GPU_V2_MODE_FLASHING:
        case MSI_GPU_V2_MODE_DOUBLEFLASHING:
        case MSI_GPU_V2_MODE_STATIC:
        case MSI_GPU_V2_MODE_RHYTHM:
        case MSI_GPU_V2_MODE_STACK:
        case MSI_GPU_V2_MODE_METEOR:
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            msi_gpu->SetRGB1(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
            break;

        case MSI_GPU_V2_MODE_STREAMING:
        case MSI_GPU_V2_MODE_LIGHTNING:
        case MSI_GPU_V2_MODE_OFF:
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            break;

        default:
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            msi_gpu->SetMode(MSI_GPU_V2_MODE_OFF);
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_BRIGHTNESS, brightness_values[modes[active_mode].brightness]);
        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_SPEED, speed_values[modes[active_mode].speed]);
        }

    }
    msi_gpu->SetMode(modes[active_mode].value);

    msi_gpu->MSIGPURegisterWrite(MSI_GPU_V2_REG_SAVE, 0x00);
}


void RGBController_MSIGPUv2::DeviceUpdateLEDs()
{
    if(TimeToSend())
    {
        // Comented out for now - Saving manually seems better option
        // DeviceUpdateAll();
        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();
    }
}

void RGBController_MSIGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}


void RGBController_MSIGPUv2::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | This device does not support updating single LEDs         |
    \*---------------------------------------------------------*/
}

void RGBController_MSIGPUv2::DeviceUpdateMode()
{
    if(TimeToSend())
    {    
        DeviceUpdateAll();

        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();
    }
}

void RGBController_MSIGPUv2::DeviceSaveMode()
{
    DeviceUpdateAll();
}
