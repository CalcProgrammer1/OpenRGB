/*-----------------------------------------*\
|  RGBController_AsusROGAlly.cpp            |
|                                           |
|  Generic RGB Interface for Asus ROG Ally  |
|  controller driver                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2023   |
\*-----------------------------------------*/

#include "RGBController_AsusROGAlly.h"

/**------------------------------------------------------------------*\
    @name Asus ROG Ally
    @category Gamepad
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAsusROGAlly
    @comment
\*-------------------------------------------------------------------*/

RGBController_AsusROGAlly::RGBController_AsusROGAlly(ROGAllyController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "ASUS ROG Ally";
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_GAMEPAD;
    description                 = "ASUS ROG Ally Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0xFF;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AsusROGAlly::~RGBController_AsusROGAlly()
{
    delete controller;
}

void RGBController_AsusROGAlly::SetupZones()
{
    zone left_stick_zone;

    left_stick_zone.name         = "Left Stick";
    left_stick_zone.type         = ZONE_TYPE_SINGLE;
    left_stick_zone.leds_min     = 2;
    left_stick_zone.leds_max     = 2;
    left_stick_zone.leds_count   = 2;
    left_stick_zone.matrix_map   = NULL;

    zones.push_back(left_stick_zone);

    for(unsigned int i = 0; i < 2; i++)
    {
        led left_stick_led;

        left_stick_led.name      = "Left Stick LED " + std::to_string(i);

        leds.push_back(left_stick_led);
    }

    zone right_stick_zone;

    right_stick_zone.name         = "Left Stick";
    right_stick_zone.type         = ZONE_TYPE_SINGLE;
    right_stick_zone.leds_min     = 2;
    right_stick_zone.leds_max     = 2;
    right_stick_zone.leds_count   = 2;
    right_stick_zone.matrix_map   = NULL;

    zones.push_back(right_stick_zone);

    for(unsigned int i = 0; i < 2; i++)
    {
        led right_stick_led;

        right_stick_led.name      = "Right Stick LED " + std::to_string(i);

        leds.push_back(right_stick_led);
    }

    SetupColors();
}

void RGBController_AsusROGAlly::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AsusROGAlly::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == 0xFF)
    {
        controller->UpdateLeds(std::vector<RGBColor>(colors));
    }
}

void RGBController_AsusROGAlly::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusROGAlly::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AsusROGAlly::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFF)
    {
        DeviceUpdateLEDs();
    }
}

void RGBController_AsusROGAlly::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}
