/*-----------------------------------------*\
|  RGBController_Network.h                  |
|                                           |
|  Generic RGB Interface Network Class      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/11/2020   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NetworkClient.h"

class RGBController_Network : public RGBController
{
public:
    RGBController_Network(NetworkClient * client_ptr, unsigned int dev_idx_val);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    void        UpdateLEDs();

private:
    NetworkClient *     client;
    unsigned int        dev_idx;
};
