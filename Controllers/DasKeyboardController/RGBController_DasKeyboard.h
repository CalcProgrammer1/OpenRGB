/*-----------------------------------------*\
|  RGBController_DasKeyboard.h              |
|                                           |
|  Generic RGB Interface for Das Keyboard   |
|  RGB keyboard devices                     |
|                                           |
|  Frank Niessen (denk_mal) 12/16/2020      |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "DasKeyboardController.h"

enum
{
    DAS_KEYBOARD_MODE_DIRECT         = 0x01,
    DAS_KEYBOARD_MODE_FLASHING       = 0x1F,
    DAS_KEYBOARD_MODE_BREATHING      = 0x08,
    DAS_KEYBOARD_MODE_SPECTRUM_CYCLE = 0x14
};


class RGBController_DasKeyboard : public RGBController
{
public:
    RGBController_DasKeyboard(DasKeyboardController *das_ptr);

    ~RGBController_DasKeyboard();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();

    void UpdateZoneLEDs(int zone);

    void UpdateSingleLED(int led);

    void SetCustomMode();

    void DeviceUpdateMode();

private:
    DasKeyboardController *das;

    std::vector<RGBColor> double_buffer;
    bool                  updateDevice;
};
