/*-----------------------------------------*\
|  RGBController_CorsairWireless.cpp        |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  wireless keyboard devices                |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/8/2021    |
\*-----------------------------------------*/

#include "RGBController_CorsairWireless.h"

RGBController_CorsairWireless::RGBController_CorsairWireless(CorsairWirelessController* corsair_ptr)
{
    corsair = corsair_ptr;

    name        = corsair->GetName();
    vendor      = "Corsair";
    description = "Corsair RGB Peripheral Device";
    type        = corsair->GetDeviceType();
    version     = corsair->GetFirmwareString();
    location    = corsair->GetDeviceLocation();
    serial      = corsair->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_CorsairWireless::~RGBController_CorsairWireless()
{
    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete corsair;
}

void RGBController_CorsairWireless::SetupZones()
{
    SetupColors();
}

void RGBController_CorsairWireless::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CorsairWireless::DeviceUpdateLEDs()
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateZoneLEDs(int /*zone*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::UpdateSingleLED(int /*led*/)
{
    corsair->SetLEDs(colors);
}

void RGBController_CorsairWireless::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CorsairWireless::DeviceUpdateMode()
{

}
