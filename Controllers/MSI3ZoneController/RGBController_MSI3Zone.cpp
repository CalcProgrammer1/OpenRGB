/*-----------------------------------------*\
|  RGBController_MSI3Zone.cpp               |
|                                           |
|  Generic RGB Interface for MSI/Steelseries|
|  3-Zone Keyboard                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_MSI3Zone.h"

RGBController_MSI3Zone::RGBController_MSI3Zone(MSI3ZoneController* msi_ptr)
{
    msi = msi_ptr;

    name        = "MSI 3-Zone Keyboard";
    vendor      = "MSI";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "MSI 3-Zone Keyboard Device";
    location    = msi->GetDeviceLocation();
    serial      = msi->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_MSI3Zone::~RGBController_MSI3Zone()
{
    delete msi;
}

void RGBController_MSI3Zone::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up Keyboard zone and Keyboard LEDs                    |
    \*---------------------------------------------------------*/
    zone keyboard_zone;
    keyboard_zone.name          = "Keyboard";
    keyboard_zone.type          = ZONE_TYPE_LINEAR;
    keyboard_zone.leds_min      = 3;
    keyboard_zone.leds_max      = 3;
    keyboard_zone.leds_count    = 3;
    keyboard_zone.matrix_map    = NULL;
    zones.push_back(keyboard_zone);

    led left_led;
    left_led.name = "Keyboard Left";
    leds.push_back(left_led);

    led mid_led;
    mid_led.name = "Keyboard Middle";
    leds.push_back(mid_led);

    led right_led;
    right_led.name = "Keyboard Right";
    leds.push_back(right_led);

    /*---------------------------------------------------------*\
    | Set up Aux zone and Aux LED                               |
    \*---------------------------------------------------------*/
    zone aux_zone;
    aux_zone.name           = "Aux";
    aux_zone.type           = ZONE_TYPE_SINGLE;
    aux_zone.leds_min       = 1;
    aux_zone.leds_max       = 1;
    aux_zone.leds_count     = 1;
    aux_zone.matrix_map     = NULL;
    zones.push_back(aux_zone);

    led aux_led;
    aux_led.name = "Aux";
    leds.push_back(aux_led);

    SetupColors();
}

void RGBController_MSI3Zone::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_MSI3Zone::DeviceUpdateLEDs()
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateZoneLEDs(int /*zone*/)
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::UpdateSingleLED(int /*led*/)
{
    msi->SetLEDs(colors);
}

void RGBController_MSI3Zone::SetCustomMode()
{

}

void RGBController_MSI3Zone::DeviceUpdateMode()
{

}
