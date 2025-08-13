/*---------------------------------------------------------*\
| RGBController_MSIGPU.cpp                                  |
|                                                           |
|   RGBController for MSI GPU                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include "RGBController_MSIGPU.h"

static const std::array<unsigned char, 3> speed_values      = { 0x04, 0x02, 0x01 };

int RGBController_MSIGPU::GetDeviceMode()
{
    unsigned char dev_mode = controller->MSIGPURegisterRead(MSI_GPU_REG_MODE);

    for(std::size_t mode = 0; mode < modes.size(); mode++)
    {
        if(modes[mode].value == dev_mode)
        {
            active_mode = (int)mode;
            break;
        }
    }

    return(active_mode);
}

int RGBController_MSIGPU::GetModeSpeed()
{
    unsigned char mode_speed = controller->MSIGPURegisterRead(MSI_GPU_REG_SPEED);

    for(std::size_t speed = 0; speed < speed_values.size(); speed++)
    {
        if(speed_values[speed] == mode_speed)
        {
            return((int)speed);
        }
    }

    return(0);
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

RGBController_MSIGPU::RGBController_MSIGPU(MSIGPUController * controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "MSI";
    type                            = DEVICE_TYPE_GPU;
    description                     = name;
    location                        = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = MSI_GPU_MODE_STATIC;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.brightness_min           = MSI_GPU_BRIGHTNESS_MIN;
    Direct.brightness               = MSI_GPU_BRIGHTNESS_MAX;
    Direct.brightness_max           = MSI_GPU_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Rainbow;
    Rainbow.name                    = "Rainbow";
    Rainbow.value                   = MSI_GPU_MODE_RAINBOW;
    Rainbow.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Rainbow.speed_min               = MSI_GPU_SPEED_MIN;
    Rainbow.speed                   = MSI_GPU_SPEED_MID;
    Rainbow.speed_max               = MSI_GPU_SPEED_MAX;
    Rainbow.brightness_min          = MSI_GPU_BRIGHTNESS_MIN;
    Rainbow.brightness              = MSI_GPU_BRIGHTNESS_MAX;
    Rainbow.brightness_max          = MSI_GPU_BRIGHTNESS_MAX;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Raindrop;
    Raindrop.name                   = "Raindrop";
    Raindrop.value                  = MSI_GPU_MODE_RAINDROP;
    Raindrop.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Raindrop.speed_min              = MSI_GPU_SPEED_MIN;
    Raindrop.speed                  = MSI_GPU_SPEED_MID;
    Raindrop.speed_max              = MSI_GPU_SPEED_MAX;
    Raindrop.brightness_min         = MSI_GPU_BRIGHTNESS_MIN;
    Raindrop.brightness             = MSI_GPU_BRIGHTNESS_MAX;
    Raindrop.brightness_max         = MSI_GPU_BRIGHTNESS_MAX;
    Raindrop.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Raindrop);

    mode Magic;
    Magic.name                      = "Magic";
    Magic.value                     = MSI_GPU_MODE_MAGIC;
    Magic.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Magic.speed_min                 = MSI_GPU_SPEED_MIN;
    Magic.speed                     = MSI_GPU_SPEED_MID;
    Magic.speed_max                 = MSI_GPU_SPEED_MAX;
    Magic.brightness_min            = MSI_GPU_BRIGHTNESS_MIN;
    Magic.brightness                = MSI_GPU_BRIGHTNESS_MAX;
    Magic.brightness_max            = MSI_GPU_BRIGHTNESS_MAX;
    Magic.color_mode                = MODE_COLORS_NONE;
    modes.push_back(Magic);

    mode Patrolling;
    Patrolling.name                 = "Patrolling";
    Patrolling.value                = MSI_GPU_MODE_PATROLLING;
    Patrolling.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Patrolling.speed_min            = MSI_GPU_SPEED_MIN;
    Patrolling.speed                = MSI_GPU_SPEED_MID;
    Patrolling.speed_max            = MSI_GPU_SPEED_MAX;
    Patrolling.brightness_min       = MSI_GPU_BRIGHTNESS_MIN;
    Patrolling.brightness           = MSI_GPU_BRIGHTNESS_MAX;
    Patrolling.brightness_max       = MSI_GPU_BRIGHTNESS_MAX;
    Patrolling.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Patrolling);

    mode Streaming;
    Streaming.name                  = "Streaming";
    Streaming.value                 = MSI_GPU_MODE_STREAMING;
    Streaming.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Streaming.speed_min             = MSI_GPU_SPEED_MIN;
    Streaming.speed                 = MSI_GPU_SPEED_MID;
    Streaming.speed_max             = MSI_GPU_SPEED_MAX;
    Streaming.brightness_min        = MSI_GPU_BRIGHTNESS_MIN;
    Streaming.brightness            = MSI_GPU_BRIGHTNESS_MAX;
    Streaming.brightness_max        = MSI_GPU_BRIGHTNESS_MAX;
    Streaming.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Streaming);

    mode Lightning;
    Lightning.name                  = "Lightning";
    Lightning.value                 = MSI_GPU_MODE_LIGHTNING;
    Lightning.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Lightning.speed_min             = MSI_GPU_SPEED_MIN;
    Lightning.speed                 = MSI_GPU_SPEED_MID;
    Lightning.speed_max             = MSI_GPU_SPEED_MAX;
    Lightning.brightness_min        = MSI_GPU_BRIGHTNESS_MIN;
    Lightning.brightness            = MSI_GPU_BRIGHTNESS_MAX;
    Lightning.brightness_max        = MSI_GPU_BRIGHTNESS_MAX;
    Lightning.color_mode            = MODE_COLORS_NONE;
    modes.push_back(Lightning);

    mode Wave;
    Wave.name                       = "Wave";
    Wave.value                      = MSI_GPU_MODE_WAVE;
    Wave.flags                      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Wave.speed_min                  = MSI_GPU_SPEED_MIN;
    Wave.speed                      = MSI_GPU_SPEED_MID;
    Wave.speed_max                  = MSI_GPU_SPEED_MAX;
    Wave.brightness_min             = MSI_GPU_BRIGHTNESS_MIN;
    Wave.brightness                 = MSI_GPU_BRIGHTNESS_MAX;
    Wave.brightness_max             = MSI_GPU_BRIGHTNESS_MAX;
    Wave.color_mode                 = MODE_COLORS_PER_LED;
    modes.push_back(Wave);

    mode Meteor;
    Meteor.name                     = "Meteor";
    Meteor.value                    = MSI_GPU_MODE_METEOR;
    Meteor.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Meteor.speed_min                = MSI_GPU_SPEED_MIN;
    Meteor.speed                    = MSI_GPU_SPEED_MID;
    Meteor.speed_max                = MSI_GPU_SPEED_MAX;
    Meteor.brightness_min           = MSI_GPU_BRIGHTNESS_MIN;
    Meteor.brightness               = MSI_GPU_BRIGHTNESS_MAX;
    Meteor.brightness_max           = MSI_GPU_BRIGHTNESS_MAX;
    Meteor.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Meteor);

    mode Stack;
    Stack.name                      = "Stack";
    Stack.value                     = MSI_GPU_MODE_STACK;
    Stack.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Stack.speed_min                 = MSI_GPU_SPEED_MIN;
    Stack.speed                     = MSI_GPU_SPEED_MID;
    Stack.speed_max                 = MSI_GPU_SPEED_MAX;
    Stack.brightness_min            = MSI_GPU_BRIGHTNESS_MIN;
    Stack.brightness                = MSI_GPU_BRIGHTNESS_MAX;
    Stack.brightness_max            = MSI_GPU_BRIGHTNESS_MAX;
    Stack.color_mode                = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Rhythm;
    Rhythm.name                     = "Rhythm";
    Rhythm.value                    = MSI_GPU_MODE_RHYTHM;
    Rhythm.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Rhythm.speed_min                = MSI_GPU_SPEED_MIN;
    Rhythm.speed                    = MSI_GPU_SPEED_MID;
    Rhythm.speed_max                = MSI_GPU_SPEED_MAX;
    Rhythm.brightness_min           = MSI_GPU_BRIGHTNESS_MIN;
    Rhythm.brightness               = MSI_GPU_BRIGHTNESS_MAX;
    Rhythm.brightness_max           = MSI_GPU_BRIGHTNESS_MAX;
    Rhythm.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Rhythm);

    mode Flowing;
    Flowing.name                    = "Flowing";
    Flowing.value                   = MSI_GPU_MODE_FLOWING;
    Flowing.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Flowing.speed_min               = MSI_GPU_SPEED_MIN;
    Flowing.speed                   = MSI_GPU_SPEED_MID;
    Flowing.speed_max               = MSI_GPU_SPEED_MAX;
    Flowing.brightness_min          = MSI_GPU_BRIGHTNESS_MIN;
    Flowing.brightness              = MSI_GPU_BRIGHTNESS_MAX;
    Flowing.brightness_max          = MSI_GPU_BRIGHTNESS_MAX;
    Flowing.color_mode              = MODE_COLORS_PER_LED;
    modes.push_back(Flowing);

    mode Whirling;
    Whirling.name                   = "Whirling";
    Whirling.value                  = MSI_GPU_MODE_WHIRLING;
    Whirling.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Whirling.speed_min              = MSI_GPU_SPEED_MIN;
    Whirling.speed                  = MSI_GPU_SPEED_MID;
    Whirling.speed_max              = MSI_GPU_SPEED_MAX;
    Whirling.brightness_min         = MSI_GPU_BRIGHTNESS_MIN;
    Whirling.brightness             = MSI_GPU_BRIGHTNESS_MAX;
    Whirling.brightness_max         = MSI_GPU_BRIGHTNESS_MAX;
    Whirling.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Whirling);

    mode Twisting;
    Twisting.name                   = "Twisting";
    Twisting.value                  = MSI_GPU_MODE_TWISTING;
    Twisting.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Twisting.speed_min              = MSI_GPU_SPEED_MIN;
    Twisting.speed                  = MSI_GPU_SPEED_MID;
    Twisting.speed_max              = MSI_GPU_SPEED_MAX;
    Twisting.brightness_min         = MSI_GPU_BRIGHTNESS_MIN;
    Twisting.brightness             = MSI_GPU_BRIGHTNESS_MAX;
    Twisting.brightness_max         = MSI_GPU_BRIGHTNESS_MAX;
    Twisting.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Twisting);

    mode Laminating;
    Laminating.name                 = "Laminating";
    Laminating.value                = MSI_GPU_MODE_LAMINATING;
    Laminating.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Laminating.speed_min            = MSI_GPU_SPEED_MIN;
    Laminating.speed                = MSI_GPU_SPEED_MID;
    Laminating.speed_max            = MSI_GPU_SPEED_MAX;
    Laminating.brightness_min       = MSI_GPU_BRIGHTNESS_MIN;
    Laminating.brightness           = MSI_GPU_BRIGHTNESS_MAX;
    Laminating.brightness_max       = MSI_GPU_BRIGHTNESS_MAX;
    Laminating.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Laminating);

    mode Fadein;
    Fadein.name                     = "Fadein";
    Fadein.value                    = MSI_GPU_MODE_FADEIN;
    Fadein.flags                    = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Fadein.speed_min                = MSI_GPU_SPEED_MIN;
    Fadein.speed                    = MSI_GPU_SPEED_MID;
    Fadein.speed_max                = MSI_GPU_SPEED_MAX;
    Fadein.brightness_min           = MSI_GPU_BRIGHTNESS_MIN;
    Fadein.brightness               = MSI_GPU_BRIGHTNESS_MAX;
    Fadein.brightness_max           = MSI_GPU_BRIGHTNESS_MAX;
    Fadein.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Fadein);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = MSI_GPU_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Breathing.speed_min             = MSI_GPU_SPEED_MIN;
    Breathing.speed                 = MSI_GPU_SPEED_MID;
    Breathing.speed_max             = MSI_GPU_SPEED_MAX;
    Breathing.brightness_min        = MSI_GPU_BRIGHTNESS_MIN;
    Breathing.brightness            = MSI_GPU_BRIGHTNESS_MAX;
    Breathing.brightness_max        = MSI_GPU_BRIGHTNESS_MAX;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode flashing;
    flashing.name                   = "Flashing";
    flashing.value                  = MSI_GPU_MODE_FLASHING;
    flashing.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    flashing.speed_min              = MSI_GPU_SPEED_MIN;
    flashing.speed                  = MSI_GPU_SPEED_MID;
    flashing.speed_max              = MSI_GPU_SPEED_MAX;
    flashing.brightness_min         = MSI_GPU_BRIGHTNESS_MIN;
    flashing.brightness             = MSI_GPU_BRIGHTNESS_MAX;
    flashing.brightness_max         = MSI_GPU_BRIGHTNESS_MAX;
    flashing.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(flashing);

    mode doubleflashing;
    doubleflashing.name             = "Doubleflashing";
    doubleflashing.value            = MSI_GPU_MODE_DOUBLEFLASHING;
    doubleflashing.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    doubleflashing.speed_min        = 0;
    doubleflashing.speed            = 0;
    doubleflashing.speed_max        = 2;
    doubleflashing.brightness_min   = MSI_GPU_BRIGHTNESS_MIN;
    doubleflashing.brightness       = MSI_GPU_BRIGHTNESS_MAX;
    doubleflashing.brightness_max   = MSI_GPU_BRIGHTNESS_MAX;
    doubleflashing.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(doubleflashing);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = MSI_GPU_MODE_OFF;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    active_mode                     = GetDeviceMode();
    modes[active_mode].speed        = GetModeSpeed();
    modes[active_mode].brightness   = controller->MSIGPURegisterRead(MSI_GPU_REG_BRIGHTNESS) / MSI_GPU_BRIGHTNESS_MULTI;
}

RGBController_MSIGPU::~RGBController_MSIGPU()
{
    delete controller;
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
    unsigned char r1 = controller->MSIGPURegisterRead(MSI_GPU_REG_R1);
    unsigned char g1 = controller->MSIGPURegisterRead(MSI_GPU_REG_G1);
    unsigned char b1 = controller->MSIGPURegisterRead(MSI_GPU_REG_B1);
    unsigned char r2 = controller->MSIGPURegisterRead(MSI_GPU_REG_R2);
    unsigned char g2 = controller->MSIGPURegisterRead(MSI_GPU_REG_G2);
    unsigned char b2 = controller->MSIGPURegisterRead(MSI_GPU_REG_B2);
    unsigned char r3 = controller->MSIGPURegisterRead(MSI_GPU_REG_R3);
    unsigned char g3 = controller->MSIGPURegisterRead(MSI_GPU_REG_G3);
    unsigned char b3 = controller->MSIGPURegisterRead(MSI_GPU_REG_B3);

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

bool RGBController_MSIGPU::TimeToSend()
{
    /*-----------------------------------------------------*\
    | Rate limit is 1000(ms) / wait_time in Frames Per Sec  |
    \*-----------------------------------------------------*/
    const uint8_t wait_time     = 33;
    return (std::chrono::steady_clock::now() - last_commit_time) > std::chrono::milliseconds(wait_time);
}

void RGBController_MSIGPU::DeviceUpdateLEDs()
{
    if(TimeToSend())
    {
        controller->MSIGPURegisterWrite(MSI_GPU_REG_UNKNOWN, 0x00);

        if(modes[active_mode].value == MSI_GPU_MODE_FADEIN)
        {
            controller->SetRGB2(RGBGetRValue(colors[1]), RGBGetGValue(colors[1]), RGBGetBValue(colors[1]));
            controller->SetRGB3(RGBGetRValue(colors[2]), RGBGetGValue(colors[2]), RGBGetBValue(colors[2]));
        }
        else
        {
            controller->SetRGB1(RGBGetRValue(colors[0]), RGBGetGValue(colors[0]), RGBGetBValue(colors[0]));
        }

        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();
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
    if(TimeToSend())
    {
        if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
        {
            controller->MSIGPURegisterWrite(MSI_GPU_REG_BRIGHTNESS, modes[active_mode].brightness * MSI_GPU_BRIGHTNESS_MULTI);

            if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
            {
                controller->MSIGPURegisterWrite(MSI_GPU_REG_SPEED, speed_values[modes[active_mode].speed]);
            }

            controller->SetMode(modes[active_mode].value);

            /*-----------------------------------------------------*\
            | Update last commit time                               |
            \*-----------------------------------------------------*/
            last_commit_time    = std::chrono::steady_clock::now();
        }
    }
}

void RGBController_MSIGPU::DeviceSaveMode()
{
    controller->Save();
}
