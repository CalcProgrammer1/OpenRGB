/*-----------------------------------------*\
|  RGBController_Network.cpp                |
|                                           |
|  Generic RGB Interface Network Class      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/11/2020   |
\*-----------------------------------------*/

#include "RGBController_Network.h"

RGBController_Network::RGBController_Network(NetworkClient * client_ptr, unsigned int dev_idx_val)
{
    client  = client_ptr;
    dev_idx = dev_idx_val;
}

void RGBController_Network::SetupZones()
{
    //Don't send anything, this function should only process on host
}

void RGBController_Network::ResizeZone(int zone, int new_size)
{
    client->SendRequest_RGBController_ResizeZone(dev_idx, zone, new_size);
}

void RGBController_Network::UpdateLEDs()
{
    client->SendRequest_RGBController_UpdateLEDs(dev_idx);
}

void RGBController_Network::UpdateZoneLEDs(int zone)
{
    client->SendRequest_RGBController_UpdateZoneLEDs(dev_idx, zone);
}

void RGBController_Network::UpdateSingleLED(int led)
{
    client->SendRequest_RGBController_UpdateSingleLED(dev_idx, led);
}

void RGBController_Network::SetCustomMode()
{
    client->SendRequest_RGBController_SetCustomMode(dev_idx);
}

void RGBController_Network::UpdateMode()
{
    client->SendRequest_RGBController_UpdateMode(dev_idx);
}
