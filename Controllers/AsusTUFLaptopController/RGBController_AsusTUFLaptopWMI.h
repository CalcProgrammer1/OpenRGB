#ifndef RGBCONTROLLER_ASUSTUFLAPTOPWMI_H
#define RGBCONTROLLER_ASUSTUFLAPTOPWMI_H

#ifdef _WIN32

#include "RGBController.h"

class RGBController_AsusTUFLaptopWMI : public RGBController
{
public:
    RGBController_AsusTUFLaptopWMI();
    virtual ~RGBController_AsusTUFLaptopWMI();

    void SetupZones() override;

    void ResizeZone(int zone, int new_size) override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;
};

#endif

#endif // RGBCONTROLLER_ASUSTUFLAPTOPWMI_H
