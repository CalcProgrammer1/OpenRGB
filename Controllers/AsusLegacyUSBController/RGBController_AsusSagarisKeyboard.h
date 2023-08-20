/*-----------------------------------------*\
|  RGBController_AsusSagarisKeyboard.h     |
|                                           |
|  Generic RGB Interface for Asus Sagaris  |
|  USB controller driver                    |
|                                           |
|  Mola19 08/20/2023                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusSagarisKeyboardController.h"

class RGBController_AsusSagarisKeyboard : public RGBController
{
public:
    RGBController_AsusSagarisKeyboard(AsusSagarisKeyboardController* controller_ptr);
    ~RGBController_AsusSagarisKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AsusSagarisKeyboardController*  controller;
    std::vector<RGBColor>           current_colors;
    uint8_t                         last_mode;
};
