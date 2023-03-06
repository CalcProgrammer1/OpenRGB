#ifdef _WIN32

#ifndef RGBCONTROLLER_ASUSTUFLAPTOPWMI_H
#define RGBCONTROLLER_ASUSTUFLAPTOPWMI_H

#include "AsusTUFLaptopController.h"
#include "RGBController.h"

class RGBController_AsusTUFLaptopWMI : public RGBController
{
public:
    RGBController_AsusTUFLaptopWMI(AsusTUFLaptopController* controller_ptr);
    virtual ~RGBController_AsusTUFLaptopWMI();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void DeviceSaveMode();

private:
    AsusTUFLaptopController* controller;

    void ReadConfiguration();
    void ControllerSetMode(bool save);
};

#endif // RGBCONTROLLER_ASUSTUFLAPTOPWMI_H

#endif // _WIN32
