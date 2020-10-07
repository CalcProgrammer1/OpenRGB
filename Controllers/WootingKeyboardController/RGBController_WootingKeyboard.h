/*-----------------------------------------*\
|  RGBController_WootingKeyboard.h          |
|                                           |
|  Generic RGB Interface for Wooting RGB    |
|  keyboard devices                         |
|                                           |
|  Diogo Trindade (diogotr7)    3/4/2021    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "WootingKeyboardController.h"

class RGBController_WootingKeyboard : public RGBController
{
public:
    RGBController_WootingKeyboard(WootingKeyboardController* wooting_ptr);
    ~RGBController_WootingKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    WootingKeyboardController*  wooting;
};
