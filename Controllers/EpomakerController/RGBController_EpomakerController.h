/*-------------------------------------------------------------------*\
|  RGBController_EpomakerController.h                                 |
|                                                                     |
|  Driver for Epomaker Keyboard                                       |
|                                                                     |
|  Alvaro Munoz (alvaromunoz)          2023-06-05                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef RGBCONTROLLER_EpomakerCONTROLLER_H
#define RGBCONTROLLER_EpomakerCONTROLLER_H

#include "RGBController.h"
#include "Controllers/EpomakerController/EpomakerController.h"

class RGBController_EpomakerController : public RGBController
{
public:
    RGBController_EpomakerController(EpomakerController* controller_ptr);
    ~RGBController_EpomakerController();


    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    EpomakerController* controller;
};

#endif // RGBCONTROLLER_EpomakerCONTROLLER_H
