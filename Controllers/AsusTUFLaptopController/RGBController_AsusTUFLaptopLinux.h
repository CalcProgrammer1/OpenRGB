#ifndef RGBCONTROLLER_ASUSTUFLAPTOPLINUX_H
#define RGBCONTROLLER_ASUSTUFLAPTOPLINUX_H

#include "RGBController.h"
#include "AsusTUFLaptopLinuxController.h"


class RGBController_AsusTUFLaptopLinux : public RGBController
{
public:
    RGBController_AsusTUFLaptopLinux(AsusTUFLaptopLinuxController* controller_ptr);

    void SetupZones() override;

    void ResizeZone(int zone, int new_size) override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;

private:
    AsusTUFLaptopLinuxController* controller;
};

#endif // RGBCONTROLLER_ASUSTUFLAPTOPLINUX_H
