/*-------------------------------------------------------------------*\
|  CMMKController.h                                                   |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)       28th Jun 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <array>

#include <hidapi/hidapi.h>

#include <libcmmk/libcmmk.h>

#pragma once

class CMMKController
{
public:
    CMMKController(hid_device_info* _dev_info, wchar_t *_vendor, wchar_t *_device_name, char *_path);
    virtual ~CMMKController();

    std::string GetDeviceName() const;
    std::string GetLocation() const;
    std::string GetFirmwareVersion() const;

    void SetFirmwareControl();
    void SetManualControl();

    void SetSingle(int row, int col, struct rgb color);
    void SetAll(struct cmmk_color_matrix const& colors);
    void SetAllSingle(struct rgb color);

    void SetMode(cmmk_effect_fully_lit eff);
    void SetMode(cmmk_effect_breathe eff);
    void SetMode(cmmk_effect_cycle eff);
    void SetMode(cmmk_effect_single eff);
    void SetMode(cmmk_effect_wave eff);
    void SetMode(cmmk_effect_ripple eff);
    void SetMode(cmmk_effect_cross eff);
    void SetMode(cmmk_effect_raindrops eff);
    void SetMode(cmmk_effect_stars eff);

    bool PositionValid(int y, int x) const;

private:
    void ActivateMode(int mode);
    void ActivateEffect(int effect);

    int         current_mode = -1;
    int         current_effect = -1;

    std::string device_name;
    std::string location;
    std::string firmware_version;

    mutable struct cmmk cmmk_handle;
};
