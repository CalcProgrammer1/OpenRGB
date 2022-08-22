/*---------------------------------------------------------------*\
|  RGBController_GaiZhongGaiKeyboard.h                            |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
| An Yang     2022/6/12                                           |
\*---------------------------------------------------------------*/
#pragma once
#include "RGBController.h"
#include "GaiZhongGaiKeyboardController.h"

class RGBController_GaiZhongGaiKeyboard : public RGBController
{
public:
    RGBController_GaiZhongGaiKeyboard(GaiZhongGaiKeyboardController* controller_ptr);
    ~RGBController_GaiZhongGaiKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GaiZhongGaiKeyboardController*    controller;
};
