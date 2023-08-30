/*-------------------------------------------------------------------*\
|  CMMKController.h                                                   |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)       28th Jun 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <cstring>
#include <KeyboardLayoutManager.h>
#include <hidapi/hidapi.h>
#include <libcmmk/libcmmk.h>

typedef int16_t keyboard_layout[CMMK_ROWS_MAX][CMMK_COLS_MAX];

/*---------------------------------------------------------*\
| i'm not thrilled with the include path, but this will     |
| work for now. fixing this would need to happen in libcmmk |
\*---------------------------------------------------------*/
#pragma GCC push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ansi/pro_s.h>
#include <ansi/pro_l.h>
#include <ansi/mk750.h>
#include <ansi/sk630.h>
#include <ansi/sk650.h>
#include <iso/pro_s.h>
#include <iso/pro_l.h>
#include <iso/mk750.h>
#include <iso/sk630.h>
#include <iso/sk650.h>
#pragma GCC pop

#pragma once

class CMMKController
{
public:
    CMMKController(hid_device* dev_handle, hid_device_info* dev_info);
    ~CMMKController();

    std::string         GetDeviceName();
    std::string         GetDeviceVendor();
    std::string         GetLocation();
    std::string         GetFirmwareVersion();
    uint8_t             GetRowCount();
    uint8_t             GetColumnCount();

    KEYBOARD_LAYOUT     GetLayout();
    KEYBOARD_SIZE       GetSize();
    layout_values       GetLayoutValues();
    keyboard_layout*    GetTransform();

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
    void SetMode(cmmk_effect_snake eff);

    bool PositionValid(int y, int x);

private:
    void ActivateMode(int mode);
    void ActivateEffect(int effect);

    int         current_mode    = -1;
    int         current_effect  = -1;

    std::string device_name;
    std::string vendor_name;
    std::string location;
    std::string firmware_version;

    KEYBOARD_SIZE       kb_size;
    KEYBOARD_LAYOUT     kb_layout_type;
    layout_values       kb_offsets;
    keyboard_layout     kb_transform;
    keyboard_layout*    kb_layout;

    uint8_t     row_count;
    uint8_t     column_count;

    mutable struct cmmk cmmk_handle;
};
