/*-----------------------------------------*\
|  RGBController_ASRockPolychromeSMBus.h    |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockASRRGBSMBusController.h"

class RGBController_ASRockASRRGBSMBus : public RGBController
{
public:
    RGBController_ASRockASRRGBSMBus(ASRockASRRGBSMBusController* controller_ptr);
    ~RGBController_ASRockASRRGBSMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ASRockASRRGBSMBusController* controller;
};
