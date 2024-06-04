/*---------------------------------------------------------*\
| QMKOpenRGBRevBController.h                                |
|                                                           |
|   Driver for OpenRGB QMK Keyboard Protocol Revision B     |
|                                                           |
|   Kasper                                      28 Jun 2021 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

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
