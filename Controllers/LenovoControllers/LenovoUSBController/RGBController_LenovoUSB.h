/*-------------------------------------------------------------------*\
|  RGBController_LenovoUSB.h                                          |
|                                                                     |
|  interface for various Lenovo Legion Devices                        |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "LenovoDevices.h"
#include "LenovoUSBController.h"
#include "RGBController.h"

#include <vector>

#ifndef RGBCONTROLLER_LENOVOUSB_H
#define RGBCONTROLLER_LENOVOUSB_H

#define NA 0xFFFFFFFF

class RGBController_LenovoUSB : public RGBController
{
public:
    RGBController_LenovoUSB(LenovoUSBController* controller_ptr);
    ~RGBController_LenovoUSB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    std::string ConvertBytesToHex(const std::vector<uint8_t> &input);
    std::string keyboardToString(LENOVO_KEYBOARD kb);
    std::string sizeToString(LENOVO_SIZE size);
    void        DumpControllerInformation();

    LENOVO_KEYBOARD keyboard_type;
    LENOVO_SIZE chasis_size;

    LenovoUSBController *controller;
};

#endif
