/*-----------------------------------------*\
|  RGBController_CorsairPeripheral.h        |
|                                           |
|  Generic RGB Interface for Corsair RGB    |
|  keyboard, mouse, and mousemat devices    |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairPeripheralController.h"

class RGBController_CorsairPeripheral : public RGBController
{
public:
    RGBController_CorsairPeripheral(CorsairPeripheralController* controller_ptr, bool supports_hardware_modes);
    ~RGBController_CorsairPeripheral();

    int         physical_layout;
    int         logical_layout;

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    CorsairPeripheralController* controller;
};
