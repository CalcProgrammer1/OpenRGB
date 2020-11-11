/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.h                                       |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "QMKRGBMatrixController.h"

class RGBController_QMKRGBMatrix : public RGBController
{
public:
    RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr);
    ~RGBController_QMKRGBMatrix();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    QMKRGBMatrixController* qmk_rgb_matrix;
};
