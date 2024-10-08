/*-----------------------------------------*\
|  RGBController_MintakaKeyboard.h          |
|                                           |
|  Driver for Mintaka Keyboard lighting     |
|  controller - header file                 |
|                                           |
|  Based on KeychronKeyboardController      |
|                                           |
|  Federico Scodelaro (pudymody) 8/10/2024  |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include "MintakaKeyboardController.h"

class RGBController_MintakaKeyboard : public RGBController
{
public:
    RGBController_MintakaKeyboard(MintakaKeyboardController* controller_ptr);
    ~RGBController_MintakaKeyboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MintakaKeyboardController* controller;
};
