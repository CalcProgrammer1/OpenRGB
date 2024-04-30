/*-----------------------------------------*\
|  RGBController_SonyDualSense.h            |
|                                           |
|  Controller for Sony DualSense            |
|                                           |
|  by flora             01/07/2022          |
\*-----------------------------------------*/


#include "RGBController.h"
#include "SonyDualSenseController.h"

class RGBController_SonyDualSense : public RGBController
{
public:
    RGBController_SonyDualSense(SonyDualSenseController* controller_ptr);
    ~RGBController_SonyDualSense();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SonyDualSenseController* controller;
};
