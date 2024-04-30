/*-------------------------------------------------------------------*\
|  QMKOpenRGBRev9Controller.h                                         |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol (Revision 9)       |
|                                                                     |
|  Revision 9 was the initial protocol supported by OpenRGB 0.6.      |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#pragma once

#include "QMKOpenRGBBaseController.h"

class QMKOpenRGBRev9Controller : public QMKOpenRGBBaseController
{
public:
    QMKOpenRGBRev9Controller(hid_device *dev_handle, const char *path);
    ~QMKOpenRGBRev9Controller();

    //Virtual function implementations
    void            GetLEDInfo(unsigned int led);
    void            DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int num_colors);

    //Protocol Specific functions
    bool            GetIsModeEnabled(unsigned int mode);

private:
};
