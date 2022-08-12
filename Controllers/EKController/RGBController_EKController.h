/*-------------------------------------------------------------------*\
|  RGBController_EKController.h                                       |
|                                                                     |
|  Driver for EK Loop Connect                                         |
|                                                                     |
|  Chris M (Dr_No)          16th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef RGBCONTROLLER_EKCONTROLLER_H
#define RGBCONTROLLER_EKCONTROLLER_H

#include "RGBController.h"
#include "Controllers/EKController/EKController.h"

class RGBController_EKController : public RGBController
{
public:
    RGBController_EKController(EKController* controller_ptr);
    ~RGBController_EKController();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EKController* controller;
};

#endif // RGBCONTROLLER_EKCONTROLLER_H
