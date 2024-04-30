/*-------------------------------------------------------------------*\
|  QMKOpenRGBRevBController.h                                         |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol (Revision B)       |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#pragma once

#include "QMKOpenRGBBaseController.h"

class QMKOpenRGBRevBController : public QMKOpenRGBBaseController
{
public:
    QMKOpenRGBRevBController(hid_device *dev_handle, const char *path);
    ~QMKOpenRGBRevBController();

    //Virtual function implementations
    void            GetLEDInfo(unsigned int leds_count);
    void            DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int num_colors);

    //Protocol Specific functions
    std::vector<unsigned int>   GetEnabledModes();

private:
};
