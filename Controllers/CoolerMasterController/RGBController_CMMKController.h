/*-------------------------------------------------------------------*\
|  RGBController_CMMKController.h                                     |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)          28th Jun 2020                          |
|  Tam D (too.manyhobbies)     25th Apr 2021                          |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CMMKController.h"

class RGBController_CMMKController : public RGBController
{
public:
    RGBController_CMMKController(CMMKController* cmmk_ptr);
    ~RGBController_CMMKController();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void SetCustomMode();
    void DeviceUpdateMode();

private:
    void SetupMatrixMap();

    CMMKController* cmmk;

    int matrix_map[CMMK_ROWS_MAX][CMMK_COLS_MAX];
    struct cmmk_color_matrix current_matrix;

    std::atomic<bool> dirty;
    std::atomic<bool> force_update;
};
