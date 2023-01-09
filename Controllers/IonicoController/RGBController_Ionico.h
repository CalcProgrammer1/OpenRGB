/*-----------------------------------------*\
|  RGBController_Ionico.h                   |
|                                           |
|  Driver for the Ionico-II-17 lighting     |
|  controller                               |
|                                           |
|  Lucas Strafe 31/12/2022                  |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include "IonicoController.h"

#define IONICO_KB_PID   0xCE00
#define IONICO_FB_PID   0x6005

class RGBController_Ionico : public RGBController
{
public:
    RGBController_Ionico(IonicoController* controller_ptr);
    ~RGBController_Ionico();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void SetSingleLED();
    void UpdateSingleLED(int led);
    void DeviceSaveMode();

    void DeviceUpdateMode();


private:
    IonicoController* controller;
};
