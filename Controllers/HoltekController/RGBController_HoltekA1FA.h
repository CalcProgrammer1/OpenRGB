/*--------------------------------------------------------------*\
|  RGBController_HoltekA1FA.h                                    |
|                                                                |
|  Generic RGB Interface for Holtek based Mousemat [04d9:a1fa]   |
|                                                                |
|  Edoardo Ridolfi (edo2313) 26/12/2020                          |
\*--------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "HoltekA1FAController.h"

class RGBController_HoltekA1FA : public RGBController
{
public:
    RGBController_HoltekA1FA(HoltekA1FAController* holtek_ptr);
    ~RGBController_HoltekA1FA();

    int         previous_mode = 0;  /* previous  mode              */

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    HoltekA1FAController*   holtek;
};
