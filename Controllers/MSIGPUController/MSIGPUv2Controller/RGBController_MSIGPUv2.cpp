/*---------------------------------------------------------*\
| RGBController_MSIGPUv2.cpp                                |
|                                                           |
|   RGBController for MSI V2 GPU (ITE9)                     |
|                                                           |
|   Wojciech Lazarski                           03 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_MSIGPUv2.h"

static const unsigned char speed_values[3]      = { 0x04, 0x02, 0x01 };

/**------------------------------------------------------------------*\
    @name MSI GPU v2
    @category GPU
    @type I2C
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectMSIGPUv2Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSIGPUv2::RGBController_MSIGPUv2(MSIGPUv2Controller * controller_ptr, int msi_gpu_id)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "MSI";
    type                            = DEVICE_TYPE_GPU;
    description                     = "MSI GPU V2 Device";
    location                        = controller->GetDeviceLocation();

    mode Off;
    Off.name                        = "Off";
    Off.value                       = MSI_GPU_V2_MODE_OFF;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = MSI_GPU_V2_MODE_STATIC;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.brightness_min           = MSI_GPU_V2_BRIGHTNESS_MIN;
    Direct.brightness               = MSI_GPU_V2_BRIGHTNESS_MAX;
    Direct.brightness_max           = MSI_GPU_V2_BRIGHTNESS_MAX;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name                    = "Rainbow Wave";
    Rainbow.value                   = MSI_GPU_V2_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min               = MSI_GPU_V2_SPEED_MIN;
    Rainbow.speed                   = MSI_GPU_V2_SPEED_MID;
    Rainbow.speed_max               = MSI_GPU_V2_SPEED_MAX;
    Rainbow.brightness_min          = MSI_GPU_V2_BRIGHTNESS_MIN;
    Rainbow.brightness              = MSI_GPU_V2_BRIGHTNESS_MAX;
    Rainbow.brightness_max          = MSI_GPU_V2_BRIGHTNESS_MAX;
    Rainbow.color_mode              = MODE_COLORS_RANDOM;
    modes.push_back(Rainbow);

    mode Magic;
    Magic.name                      = "Magic";
    Magic.value                     = MSI_GPU_V2_MODE_MAGIC;
    Magic.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Magic.speed_min                 = MSI_GPU_V2_SPEED_MIN;
    Magic.speed                     = MSI_GPU_V2_SPEED_MID;
    Magic.speed_max                 = MSI_GPU_V2_SPEED_MAX;
    Magic.brightness_min            = MSI_GPU_V2_BRIGHTNESS_MIN;
    Magic.brightness                = MSI_GPU_V2_BRIGHTNESS_MAX;
    Magic.brightness_max            = MSI_GPU_V2_BRIGHTNESS_MAX;
    Magic.color_mode                = MODE_COLORS_RANDOM;
    modes.push_back(Magic);

    mode ColorCycle;
    ColorCycle.name                 = "Color Cycle";
    ColorCycle.value                = MSI_GPU_V2_MODE_MAGIC;
    ColorCycle.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.speed_min            = MSI_GPU_V2_SPEED_MIN;
    ColorCycle.speed                = MSI_GPU_V2_SPEED_MID;
    ColorCycle.speed_max            = MSI_GPU_V2_SPEED_MAX;
    ColorCycle.brightness_min       = MSI_GPU_V2_BRIGHTNESS_MIN;
    ColorCycle.brightness           = MSI_GPU_V2_BRIGHTNESS_MAX;
    ColorCycle.brightness_max       = MSI_GPU_V2_BRIGHTNESS_MAX;
    ColorCycle.colors_min           = 1;
    ColorCycle.colors_max           = 3;
    ColorCycle.colors.resize(3);
    ColorCycle.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(ColorCycle);


    mode Patrolling;
    Patrolling.name                 = "Patrolling";
    Patrolling.value                = MSI_GPU_V2_MODE_PATROLLING;
    Patrolling.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Patrolling.speed_min            = MSI_GPU_V2_SPEED_MIN;
    Patrolling.speed                = MSI_GPU_V2_SPEED_MID;
    Patrolling.speed_max            = MSI_GPU_V2_SPEED_MAX;
    Patrolling.brightness_min       = MSI_GPU_V2_BRIGHTNESS_MIN;
    Patrolling.brightness           = MSI_GPU_V2_BRIGHTNESS_MAX;
    Patrolling.brightness_max       = MSI_GPU_V2_BRIGHTNESS_MAX;
    Patrolling.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Patrolling);

    mode Streaming;
    Streaming.name                  = "Streaming";
    Streaming.value                 = MSI_GPU_V2_MODE_STREAMING;
    Streaming.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Streaming.speed_min             = MSI_GPU_V2_SPEED_MIN;
    Streaming.speed                 = MSI_GPU_V2_SPEED_MID;
    Streaming.speed_max             = MSI_GPU_V2_SPEED_MAX;
    Streaming.brightness_min        = MSI_GPU_V2_BRIGHTNESS_MIN;
    Streaming.brightness            = MSI_GPU_V2_BRIGHTNESS_MAX;
    Streaming.brightness_max        = MSI_GPU_V2_BRIGHTNESS_MAX;
    Streaming.color_mode            = MODE_COLORS_RANDOM;
    modes.push_back(Streaming);

    mode Lightning;
    Lightning.name                  = "Lightning";
    Lightning.value                 = MSI_GPU_V2_MODE_LIGHTNING;
    Lightning.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_MANUAL_SAVE;
    Lightning.speed_min             = MSI_GPU_V2_SPEED_MIN;
    Lightning.speed                 = MSI_GPU_V2_SPEED_MID;
    Lightning.speed_max             = MSI_GPU_V2_SPEED_MAX;
    Lightning.brightness_min        = MSI_GPU_V2_BRIGHTNESS_MIN;
    Lightning.brightness            = MSI_GPU_V2_BRIGHTNESS_MAX;
    Lightning.brightness_max        = MSI_GPU_V2_BRIGHTNESS_MAX;
    Lightning.color_mode            = MODE_COLORS_RANDOM;
    modes.push_back(Lightning);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = MSI_GPU_V2_MODE_RAINBOW;      //Rainbow has two modes now
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE;
    Wave.speed_min                  = MSI_GPU_V2_SPEED_MIN;
    Wave.speed                      = MSI_GPU_V2_SPEED_MID;
    Wave.speed_max                  = MSI_GPU_V2_SPEED_MAX;
    Wave.brightness_min             = MSI_GPU_V2_BRIGHTNESS_MIN;
    Wave.brightness                 = MSI_GPU_V2_BRIGHTNESS_MAX;
    Wave.brightness_max             = MSI_GPU_V2_BRIGHTNESS_MAX;
    Wave.colors_min                 = 1;
    Wave.colors_max                 = 3;
    Wave.colors.resize(3);
    Wave.color_mode                 = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Wave);

    mode Meteor;
    Meteor.name                     = "Meteor";
    Meteor.value                    = MSI_GPU_V2_MODE_METEOR;
    Meteor.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Meteor.speed_min                = MSI_GPU_V2_SPEED_MIN;
    Meteor.speed                    = MSI_GPU_V2_SPEED_MID;
    Meteor.speed_max                = MSI_GPU_V2_SPEED_MAX;
    Meteor.brightness_min           = MSI_GPU_V2_BRIGHTNESS_MIN;
    Meteor.brightness               = MSI_GPU_V2_BRIGHTNESS_MAX;
    Meteor.brightness_max           = MSI_GPU_V2_BRIGHTNESS_MAX;
    Meteor.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Meteor);

    switch(msi_gpu_id)
    {
        case MSI_RTX4060TI_GAMING_X_16G_SLIM_WHITE_SUB_DEV | NVIDIA_RTX4060TI_16G_DEV << 16:
        case MSI_RTX4070S_GAMING_X_SLIM_SUB_DEV | NVIDIA_RTX4070S_DEV << 16:
        case MSI_RTX4070TI_GAMING_X_TRIO_WHITE_SUB_DEV | NVIDIA_RTX4070TIS_DEV << 16:
        case MSI_RTX4080S_GAMING_X_TRIO_SUB_DEV | NVIDIA_RTX4080S_DEV << 16:
        case MSI_RTX4080S_GAMING_X_SLIM_WHITE_SUB_DEV | NVIDIA_RTX4080_DEV << 16:
        case MSI_RTX4080S_GAMING_X_SLIM_WHITE_SUB_DEV | NVIDIA_RTX4080S_DEV << 16:
            break;

        default:
            mode Stack;
            Stack.name                      = "Stack";
            Stack.value                     = MSI_GPU_V2_MODE_STACK;
            Stack.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
            Stack.speed_min                 = MSI_GPU_V2_SPEED_MIN;
            Stack.speed                     = MSI_GPU_V2_SPEED_MID;
            Stack.speed_max                 = MSI_GPU_V2_SPEED_MAX;
            Stack.brightness_min            = MSI_GPU_V2_BRIGHTNESS_MIN;
            Stack.brightness                = MSI_GPU_V2_BRIGHTNESS_MAX;
            Stack.brightness_max            = MSI_GPU_V2_BRIGHTNESS_MAX;
            Stack.color_mode                = MODE_COLORS_PER_LED;
            modes.push_back(Stack);

            mode Rhythm;
            Rhythm.name                     = "Rhythm";
            Rhythm.value                    = MSI_GPU_V2_MODE_RHYTHM;
            Rhythm.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
            Rhythm.speed_min                = MSI_GPU_V2_SPEED_MIN;
            Rhythm.speed                    = MSI_GPU_V2_SPEED_MID;
            Rhythm.speed_max                = MSI_GPU_V2_SPEED_MAX;
            Rhythm.brightness_min           = MSI_GPU_V2_BRIGHTNESS_MIN;
            Rhythm.brightness               = MSI_GPU_V2_BRIGHTNESS_MAX;
            Rhythm.brightness_max           = MSI_GPU_V2_BRIGHTNESS_MAX;
            Rhythm.color_mode               = MODE_COLORS_PER_LED;
            modes.push_back(Rhythm);
    }

    mode Flowing;
    Flowing.name                    = "Flowing";
    Flowing.value                   = MSI_GPU_V2_MODE_FLOWING;
    Flowing.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Flowing.speed_min               = MSI_GPU_V2_SPEED_MIN;
    Flowing.speed                   = MSI_GPU_V2_SPEED_MID;
    Flowing.speed_max               = MSI_GPU_V2_SPEED_MAX;
    Flowing.brightness_min          = MSI_GPU_V2_BRIGHTNESS_MIN;
    Flowing.brightness              = MSI_GPU_V2_BRIGHTNESS_MAX;
    Flowing.brightness_max          = MSI_GPU_V2_BRIGHTNESS_MAX;
    Flowing.color_mode              = MODE_COLORS_PER_LED;
    modes.push_back(Flowing);

    mode Whirling;
    Whirling.name                   = "Whirling";
    Whirling.value                  = MSI_GPU_V2_MODE_WHIRLING;
    Whirling.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Whirling.speed_min              = MSI_GPU_V2_SPEED_MIN;
    Whirling.speed                  = MSI_GPU_V2_SPEED_MID;
    Whirling.speed_max              = MSI_GPU_V2_SPEED_MAX;
    Whirling.brightness_min         = MSI_GPU_V2_BRIGHTNESS_MIN;
    Whirling.brightness             = MSI_GPU_V2_BRIGHTNESS_MAX;
    Whirling.brightness_max         = MSI_GPU_V2_BRIGHTNESS_MAX;
    Whirling.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Whirling);

    mode Fadein;
    Fadein.name                     = "Fade In";
    Fadein.value                    = MSI_GPU_V2_MODE_FADEIN;
    Fadein.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Fadein.speed_min                = MSI_GPU_V2_SPEED_MIN;
    Fadein.speed                    = MSI_GPU_V2_SPEED_MID;
    Fadein.speed_max                = MSI_GPU_V2_SPEED_MAX;
    Fadein.brightness_min           = MSI_GPU_V2_BRIGHTNESS_MIN;
    Fadein.brightness               = MSI_GPU_V2_BRIGHTNESS_MAX;
    Fadein.brightness_max           = MSI_GPU_V2_BRIGHTNESS_MAX;
    Fadein.colors_min               = 1;
    Fadein.colors_max               = 2;
    Fadein.colors.resize(2);
    Fadein.color_mode               = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Fadein);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = MSI_GPU_V2_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min             = MSI_GPU_V2_SPEED_MIN;
    Breathing.speed                 = MSI_GPU_V2_SPEED_MID;
    Breathing.speed_max             = MSI_GPU_V2_SPEED_MAX;
    Breathing.brightness_min        = MSI_GPU_V2_BRIGHTNESS_MIN;
    Breathing.brightness            = MSI_GPU_V2_BRIGHTNESS_MAX;
    Breathing.brightness_max        = MSI_GPU_V2_BRIGHTNESS_MAX;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 2;
    Breathing.colors.resize(2);
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    modes.push_back(Breathing);

    mode flashing;
    flashing.name                   = "Flashing";
    flashing.value                  = MSI_GPU_V2_MODE_FLASHING;
    flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    flashing.speed_min              = MSI_GPU_V2_SPEED_MIN;
    flashing.speed                  = MSI_GPU_V2_SPEED_MID;
    flashing.speed_max              = MSI_GPU_V2_SPEED_MAX;
    flashing.brightness_min         = MSI_GPU_V2_BRIGHTNESS_MIN;
    flashing.brightness             = MSI_GPU_V2_BRIGHTNESS_MAX;
    flashing.brightness_max         = MSI_GPU_V2_BRIGHTNESS_MAX;
    flashing.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(flashing);

    mode doubleflashing;
    doubleflashing.name             = "Double Flashing";
    doubleflashing.value            = MSI_GPU_V2_MODE_DOUBLEFLASHING;
    doubleflashing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    doubleflashing.speed_min        = MSI_GPU_V2_SPEED_MIN;
    doubleflashing.speed            = MSI_GPU_V2_SPEED_MID;
    doubleflashing.speed_max        = MSI_GPU_V2_SPEED_MAX;
    doubleflashing.brightness_min   = MSI_GPU_V2_BRIGHTNESS_MIN;
    doubleflashing.brightness       = MSI_GPU_V2_BRIGHTNESS_MAX;
    doubleflashing.brightness_max   = MSI_GPU_V2_BRIGHTNESS_MAX;
    doubleflashing.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(doubleflashing);

    SetupZones();

    modes[active_mode].speed        = MSI_GPU_V2_SPEED_MID;
    modes[active_mode].brightness   = MSI_GPU_V2_BRIGHTNESS_MAX;
}

RGBController_MSIGPUv2::~RGBController_MSIGPUv2()
{
    delete controller;
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
    msi_gpu_zone.leds_count    = 1;
    msi_gpu_zone.matrix_map    = NULL;
    zones.push_back(msi_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led led1;
    led1.name = "Color";
    leds.push_back(led1);

    SetupColors();

    /*-------------------------------------------------------------*\
    | Initialize colors                                             |
    | This controller doesn't support reading colors from device    |
    \*-------------------------------------------------------------*/
    colors[0] =  ToRGBColor(0xFF, 0, 0);

    for(unsigned int mode_idx = 0; mode_idx < modes.size(); mode_idx++)
    {
        if(modes[mode_idx].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            if(modes[mode_idx].colors.size()>2)
            {
               modes[mode_idx].colors[2] = ToRGBColor(0, 0, 0xFF);
            }
            if(modes[mode_idx].colors.size()>1)
            {
               modes[mode_idx].colors[1] = ToRGBColor(0, 0xFF, 0);
            }
            modes[mode_idx].colors[0] = ToRGBColor(0xFF, 0, 0);
        }
    }

}

void RGBController_MSIGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSIGPUv2::DeviceUpdateAll(const mode& current_mode)
{
    switch(current_mode.value)
    {
        case MSI_GPU_V2_MODE_RAINBOW:
            if(current_mode.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
            {
                if(current_mode.direction == MODE_DIRECTION_LEFT)
                {
                    controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_LEFT | MSI_GPU_V2_CONTROL_NON_RGBMODE);
                }
                else
                {
                    controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT | MSI_GPU_V2_CONTROL_NON_RGBMODE);
                }
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

                controller->SetRGB1V2(RGBGetRValue(current_mode.colors[0]), RGBGetGValue(current_mode.colors[0]), RGBGetBValue(current_mode.colors[0]));
                controller->SetRGB2V2(RGBGetRValue(current_mode.colors[1]), RGBGetGValue(current_mode.colors[1]), RGBGetBValue(current_mode.colors[1]));
                controller->SetRGB3V2(RGBGetRValue(current_mode.colors[2]), RGBGetGValue(current_mode.colors[2]), RGBGetBValue(current_mode.colors[2]));
            }
            else
            {
                if(current_mode.direction == MODE_DIRECTION_LEFT)
                {
                    controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_LEFT);
                }
                else
                {
                    controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT);
                }
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            }
            break;

        case MSI_GPU_V2_MODE_MAGIC:
            if(current_mode.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
            {
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_NON_RGBMODE);

                controller->SetRGB1V2(RGBGetRValue(current_mode.colors[0]), RGBGetGValue(current_mode.colors[0]), RGBGetBValue(current_mode.colors[0]));
                controller->SetRGB2V2(RGBGetRValue(current_mode.colors[1]), RGBGetGValue(current_mode.colors[1]), RGBGetBValue(current_mode.colors[1]));
                controller->SetRGB3V2(RGBGetRValue(current_mode.colors[2]), RGBGetGValue(current_mode.colors[2]), RGBGetBValue(current_mode.colors[2]));
            }
            else
            {
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

                controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_CONTROL, MSI_GPU_V2_CONTROL_DIRECTION_RIGHT);
            }
            break;


        case MSI_GPU_V2_MODE_BREATHING:
        case MSI_GPU_V2_MODE_FADEIN:
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);

            controller->SetRGB1V2(RGBGetRValue(current_mode.colors[0]), RGBGetGValue(current_mode.colors[0]), RGBGetBValue(current_mode.colors[0]));
            controller->SetRGB2V2(RGBGetRValue(current_mode.colors[1]), RGBGetGValue(current_mode.colors[1]), RGBGetBValue(current_mode.colors[1]));
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
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            controller->SetRGB1(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
            break;

        case MSI_GPU_V2_MODE_STREAMING:
        case MSI_GPU_V2_MODE_LIGHTNING:
        case MSI_GPU_V2_MODE_OFF:
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            break;

        default:
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_UNKNOWN, 0x00);
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_MODE, MSI_GPU_V2_MODE_IDLE);
            controller->SetMode(MSI_GPU_V2_MODE_OFF);
    }


    if(current_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_BRIGHTNESS, MSI_GPU_V2_BRIGHTNESS_MULTI * modes[active_mode].brightness);
        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            controller->MSIGPURegisterWrite(MSI_GPU_V2_REG_SPEED, speed_values[current_mode.speed]);
        }

    }

    controller->SetMode(current_mode.value);
}


void RGBController_MSIGPUv2::DeviceUpdateLEDs()
{
    DeviceUpdateAll(modes[active_mode]);
}

void RGBController_MSIGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateAll(modes[active_mode]);
}


void RGBController_MSIGPUv2::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | This device does not support updating single LEDs         |
    \*---------------------------------------------------------*/
}

void RGBController_MSIGPUv2::DeviceUpdateMode()
{
    DeviceUpdateAll(modes[active_mode]);
}

void RGBController_MSIGPUv2::DeviceSaveMode()
{
    controller->Save();
}
