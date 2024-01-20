/*-----------------------------------------*\
|  RGBController_CherryKeyboard.h           |
|                                           |
|  Generic RGB Interface for Cherry RGB     |
|  Keyboard                                 |
|                                           |
|  Sebastian Kraus 12/25/2021               |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CherryKeyboardController.h"

class RGBController_CherryKeyboard : public RGBController
{
public:
    RGBController_CherryKeyboard(CherryKeyboardController* controller_ptr, uint16_t product_id);
    ~RGBController_CherryKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CherryKeyboardController*  controller;

    static bool hasUnofficialModeSupport(uint16_t product_id);
};
