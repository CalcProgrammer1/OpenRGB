/*-----------------------------------------*\
|  RGBController_ASRockPolychromeV1SMBus.h  |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ASRockPolychromeV1SMBusController.h"

class RGBController_ASRockPolychromeV1SMBus : public RGBController
{
public:
    RGBController_ASRockPolychromeV1SMBus(ASRockPolychromeV1SMBusController* controller_ptr);
    ~RGBController_ASRockPolychromeV1SMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    ASRockPolychromeV1SMBusController* controller;
    uint8_t                 getModeIndex(uint8_t mode_value);
    std::vector<uint8_t>    zoneIndexMap;
};
