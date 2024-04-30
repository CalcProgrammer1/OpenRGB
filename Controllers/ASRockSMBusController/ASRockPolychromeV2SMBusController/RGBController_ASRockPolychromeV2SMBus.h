/*-----------------------------------------*\
|  RGBController_ASRockPolychromev2SMBus.h  |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeV2SMBusController.h"

class RGBController_ASRockPolychromeV2SMBus : public RGBController
{
public:
    RGBController_ASRockPolychromeV2SMBus(ASRockPolychromeV2SMBusController* controller_ptr);
    ~RGBController_ASRockPolychromeV2SMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ASRockPolychromeV2SMBusController* controller;
};
