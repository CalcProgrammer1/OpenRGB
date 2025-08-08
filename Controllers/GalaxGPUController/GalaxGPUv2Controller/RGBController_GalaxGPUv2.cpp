/*---------------------------------------------------------*\
| RGBController_GalaxGPUv2.cpp                              |
|                                                           |
|   RGBController for Galax GPUs (Xtreme Tuner)             |
|                                                           |
|   Daniel Stuart  (daniel.stuart14)            26 may 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GalaxGPUv2.h"

int RGBController_GalaxGPUv2::GetDeviceMode()
{
    int modereg = controller->GetMode();
    int syncreg = controller->GetSync();

    int cur_mode = 0; // Static mode by default

    if (syncreg == GALAX_V2_SYNC_ON)
    {
        cur_mode = 3; // External Sync mode
    }
    else
    {
        switch (modereg)
        {
            case GALAX_V2_MODE_BREATHING_VALUE:
                cur_mode = 1;
                break;

            case GALAX_V2_MODE_RAINBOW_VALUE:
                cur_mode = 2;
                break;

            case GALAX_V2_MODE_OFF_VALUE:
                cur_mode = 4; // Off mode
                break;
        }
    }

    return(cur_mode);
}

/**------------------------------------------------------------------*\
    @name Galax GPU v2
    @category GPU
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGalaxGPUv2Controllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_GalaxGPUv2::RGBController_GalaxGPUv2(GalaxGPUv2Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = controller->GetDeviceName();
    vendor                          = "GALAX";
    type                            = DEVICE_TYPE_GPU;
    description                     = "GALAX RTX 40+ GPU";
    location                        = controller->GetDeviceLocation();

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = 0;
    Direct.brightness_min           = 0x01;
    Direct.brightness_max           = 0x03;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = 1;
    Breathing.brightness_min        = 0x01;
    Breathing.brightness_max        = 0x03;
    Breathing.speed_min             = 0x00;
    Breathing.speed_max             = 0x09;
    Breathing.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name                    = "Rainbow";
    Rainbow.value                   = 2;
    Rainbow.flags                   = 0;
    Rainbow.brightness_min          = 0x01;
    Rainbow.brightness_max          = 0x03;
    Rainbow.speed_min               = 0x00;
    Rainbow.speed_max               = 0x09;
    Rainbow.flags                   = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Rainbow.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Sync;
    Sync.name                       = "External Sync";
    Sync.value                      = 3;
    Sync.flags                      = MODE_FLAG_MANUAL_SAVE;
    Sync.color_mode                 = MODE_COLORS_NONE;
    modes.push_back(Sync);

    mode Off;
    Off.name                        = "Off";
    Off.value                       = 4;
    Off.flags                       = MODE_FLAG_MANUAL_SAVE;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();

    active_mode                     = GetDeviceMode();
    modes[active_mode].brightness   = controller->GetBrightness();
    modes[active_mode].speed        = controller->GetSpeed();
}

RGBController_GalaxGPUv2::~RGBController_GalaxGPUv2()
{
    delete controller;
}

void RGBController_GalaxGPUv2::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zone                                               |
    \*---------------------------------------------------------*/
    zone galax_gpu_zone;
    galax_gpu_zone.name         = "GPU";
    galax_gpu_zone.type         = ZONE_TYPE_SINGLE;
    galax_gpu_zone.leds_min     = 1;
    galax_gpu_zone.leds_max     = 1;
    galax_gpu_zone.leds_count   = 1;
    galax_gpu_zone.matrix_map   = NULL;
    zones.push_back(galax_gpu_zone);

    /*---------------------------------------------------------*\
    | Set up LED                                                |
    \*---------------------------------------------------------*/
    led galax_gpu_led;
    galax_gpu_led.name          = "GPU";
    leds.push_back(galax_gpu_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize color                                          |
    \*---------------------------------------------------------*/
    unsigned char red = controller->GetLEDRed();
    unsigned char grn = controller->GetLEDGreen();
    unsigned char blu = controller->GetLEDBlue();

    colors[0] = ToRGBColor(red, grn, blu);
}

void RGBController_GalaxGPUv2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GalaxGPUv2::DeviceUpdateLEDs()
{
    for(std::size_t led = 0; led < colors.size(); led++)
    {
        unsigned char red = RGBGetRValue(colors[led]);
        unsigned char grn = RGBGetGValue(colors[led]);
        unsigned char blu = RGBGetBValue(colors[led]);

        controller->SetLEDColors(red, grn, blu);
    }
}

void RGBController_GalaxGPUv2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPUv2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GalaxGPUv2::DeviceUpdateMode()
{
    unsigned char mode_value = GALAX_V2_MODE_STATIC_VALUE; // Default to static mode
    unsigned char sync_value = GALAX_V2_SYNC_OFF; // Default to sync off

    switch(modes[active_mode].value)
    {
        case 1:
            mode_value = GALAX_V2_MODE_BREATHING_VALUE;
            break;

        case 2:
            mode_value = GALAX_V2_MODE_RAINBOW_VALUE;
            break;

        case 3:
            sync_value = GALAX_V2_SYNC_ON; // Enable sync
            break;

        case 4:
            mode_value = GALAX_V2_MODE_OFF_VALUE; // Off mode
            break;
    }

    controller->SetSync(sync_value);
    controller->SetMode(mode_value);
    controller->SetSpeed(modes[active_mode].speed);
    controller->SetBrightness(modes[active_mode].brightness);
}

void RGBController_GalaxGPUv2::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
