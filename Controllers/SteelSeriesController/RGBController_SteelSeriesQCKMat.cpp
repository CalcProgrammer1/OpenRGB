/*-----------------------------------------*\
|  RGBController_SteelSeriesQCKMat.cpp      |
|                                           |
|  Generic RGB Interface SteelSeriesQCK     |
|  Class                                    |
|                                           |
|  Edbgon 22/05/2021                        |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesQCKMat.h"

RGBController_SteelSeriesQCKMat::RGBController_SteelSeriesQCKMat(SteelSeriesQCKMatController* qck_ptr)
{
    qck         = qck_ptr;
    
    name        = qck->GetDeviceName();
    vendor      = "SteelSeries";
    type        = DEVICE_TYPE_MOUSEMAT; 
    description = "SteelSeries QCK Mat Device";
    location    = qck->GetDeviceLocation();
    serial      = qck->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_SteelSeriesQCKMat::~RGBController_SteelSeriesQCKMat()
{
    delete qck;
}

void RGBController_SteelSeriesQCKMat::SetupZones()
{
    /*---------------------------------------------------------*\
    | QCK has two zones                                         |
    \*---------------------------------------------------------*/
    zone mousemat_zone;
    mousemat_zone.name          = "Mousemat";
    mousemat_zone.type          = ZONE_TYPE_SINGLE;
    mousemat_zone.leds_min      = 2;
    mousemat_zone.leds_max      = 2;
    mousemat_zone.leds_count    = 2;
    mousemat_zone.matrix_map    = NULL;
    zones.push_back(mousemat_zone);

    led bot_led;
    bot_led.name = "Mat Bottom LED";
    leds.push_back(bot_led);

    led top_led;
    top_led.name = "Mat Top LED";
    leds.push_back(top_led);

    SetupColors();
}

void RGBController_SteelSeriesQCKMat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesQCKMat::DeviceUpdateLEDs()
{
    qck->SetColors(colors);
}

void RGBController_SteelSeriesQCKMat::UpdateZoneLEDs(int /*zone*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesQCKMat::UpdateSingleLED(int /*led*/)
{
    /*---------------------------------------------------------*\
    | Packet expects both LEDs                                  |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesQCKMat::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SteelSeriesQCKMat::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
