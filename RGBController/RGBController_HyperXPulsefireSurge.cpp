/*-----------------------------------------*\
|  RGBController_HyperXPulsefireSurge.cpp   |
|                                           |
|  Generic RGB Interface for HyperX         |
|  Pulsefire Surge                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#include "RGBController_HyperXPulsefireSurge.h"

RGBController_HyperXPulsefireSurge::RGBController_HyperXPulsefireSurge(HyperXPulsefireSurgeController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Pulsefire Surge Device";
    type        = DEVICE_TYPE_MOUSE;
    description = "HyperX Pulsefire Surge Device";

    SetupZones();
};

RGBController_HyperXPulsefireSurge::~RGBController_HyperXPulsefireSurge()
{

}

void RGBController_HyperXPulsefireSurge::SetupZones()
{
    SetupColors();
}

void RGBController_HyperXPulsefireSurge::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXPulsefireSurge::DeviceUpdateLEDs()
{

}

void RGBController_HyperXPulsefireSurge::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireSurge::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXPulsefireSurge::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_HyperXPulsefireSurge::DeviceUpdateMode()
{

}

void RGBController_HyperXPulsefireSurge::KeepaliveThread()
{

}