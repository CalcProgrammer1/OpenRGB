/*-----------------------------------------*\
|  RGBController_LogitechX56.cpp            |
|                                           |
|  Generic RGB Interface Logitech X56       |
|  Class                                    |
|                                           |
|  Edbgon 11/06/2021                        |
\*-----------------------------------------*/

#include "RGBController_LogitechX56.h"

RGBController_LogitechX56::RGBController_LogitechX56(LogitechX56Controller* controller_ptr)
{
    controller  = controller_ptr;
    
    name        = controller->GetDeviceName();
    vendor      = "Logitech";
    type        = DEVICE_TYPE_GAMEPAD; 
    description = "Logitech X56 Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_LogitechX56::~RGBController_LogitechX56()
{
    delete controller;
}

void RGBController_LogitechX56::SetupZones()
{
    /*---------------------------------------------------------*\
    | Each device has only one zone and LED                     |
    \*---------------------------------------------------------*/
    zone x56_zone;
    x56_zone.name           = "X56";
    x56_zone.type           = ZONE_TYPE_SINGLE;
    x56_zone.leds_min       = 1;
    x56_zone.leds_max       = 1;
    x56_zone.leds_count     = 1;
    x56_zone.matrix_map     = NULL;
    zones.push_back(x56_zone);

    led x56_led;
    x56_led.name            = "X56";
    leds.push_back(x56_led);

    SetupColors();
}

void RGBController_LogitechX56::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechX56::DeviceUpdateLEDs()
{
    controller->SetColor(colors[0]);
}

void RGBController_LogitechX56::UpdateZoneLEDs(int /*zone*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_LogitechX56::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_LogitechX56::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_LogitechX56::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
