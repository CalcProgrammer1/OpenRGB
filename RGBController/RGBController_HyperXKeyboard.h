/*-----------------------------------------*\
|  RGBController_HyperXKeyboard.h           |
|                                           |
|  Generic RGB Interface for HyperX RGB     |
|  Keyboard                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/2/2020    |
\*-----------------------------------------*/

#pragma once
#include <time.h>

#include "RGBController.h"
#include "HyperXKeyboardController.h"

class RGBController_HyperXKeyboard : public RGBController
{
public:
    RGBController_HyperXKeyboard(HyperXKeyboardController* hyperx_ptr);
    ~RGBController_HyperXKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

    void        KeepaliveThread();
    
private:
    HyperXKeyboardController*   hyperx;

    clock_t                     last_update_time;
};
