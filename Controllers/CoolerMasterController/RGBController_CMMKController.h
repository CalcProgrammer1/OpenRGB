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

#include <cstring>
#include "RGBController.h"
#include "CMMKController.h"
#include "RGBControllerKeyNames.h"
#include "KeyboardLayoutManager.h"

class RGBController_CMMKController : public RGBController
{
public:
    RGBController_CMMKController(CMMKController* controller_ptr);
    ~RGBController_CMMKController();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateSingleLED(int led, RGBColor color);
    void UpdateSingleLED(int led);
    void UpdateZoneLEDs(int zone_idx);

    void SetCustomMode();
    void DeviceUpdateMode();

private:
    void copy_buffers(std::vector<RGBColor> &in_colors, struct cmmk_color_matrix& mat, std::atomic<bool>& dirty);

    CMMKController*             controller;
    KeyboardLayoutManager*      layoutManager;

    struct cmmk_color_matrix current_matrix;

    std::atomic<bool>           dirty;
    std::atomic<bool>           force_update;
};
