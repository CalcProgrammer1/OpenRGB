/*-----------------------------------------*\
|  RGBController_Network.cpp                |
|                                           |
|  Generic RGB Interface Network Class      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/11/2020   |
\*-----------------------------------------*/

#include "RGBController_Network.h"

RGBController_Network::RGBController_Network()
{
    //Don't need to set up anything, this class should be initialized by network manager
}

void RGBController_Network::SetupZones()
{
    //Don't send anything, this function should only process on host
}

void RGBController_Network::ResizeZone(int /*zone*/, int /*new_size*/)
{
    //Send NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE
}

void RGBController_Network::UpdateLEDs()
{
    //Send NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS
}

void RGBController_Network::UpdateZoneLEDs(int /*zone*/)
{
    //Send NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS
}

void RGBController_Network::UpdateSingleLED(int /*led*/)
{
    //Send NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED
}

void RGBController_Network::SetCustomMode()
{
    //Send NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE
}

void RGBController_Network::UpdateMode()
{
    //Send NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE
}
