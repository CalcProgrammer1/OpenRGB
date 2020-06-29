/*-------------------------------------------------------------------*\
|  CMMKController.cpp                                                 |
|                                                                     |
|  Driver for Coolermaster MasterKeys Keyboards                       |
|                                                                     |
|  Lukas N (chmod222)       28th Jun 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMMKController.h"

#include <locale>
#include <codecvt>

CMMKController::CMMKController(hid_device_info* _dev_info, wchar_t *_vendor, wchar_t *_device_name, char *_path)
{
    std::wstring tmp_vendor_str = _vendor;
    std::wstring tmp_device_str = _device_name;

    std::wstring full_name = _vendor + std::wstring{L" "} + _device_name;

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    device_name = converter.to_bytes(full_name);

    cmmk_attach_path(&cmmk_handle, _dev_info->path, _dev_info->product_id, -1);

    char buf[32] = {0};

    cmmk_get_firmware_version(&cmmk_handle, buf, 32);

    firmware_version = buf;

    location = _path;
}

CMMKController::~CMMKController()
{
    cmmk_detach(&cmmk_handle);
}

std::string CMMKController::GetDeviceName() const
{
    return device_name;
}

std::string CMMKController::GetLocation() const
{
    return location;
}

std::string CMMKController::GetFirmwareVersion() const
{
    return firmware_version;
}

void CMMKController::SetFirmwareControl()
{
    ActivateMode(CMMK_FIRMWARE);
}

void CMMKController::SetManualControl()
{
    ActivateMode(CMMK_MANUAL);
}

void CMMKController::SetSingle(int row, int col, struct rgb color)
{
    cmmk_set_single_key(&cmmk_handle, row, col, &color);
}

void CMMKController::SetAll(struct cmmk_color_matrix const& colors)
{
    cmmk_set_leds(&cmmk_handle, &colors);
}

void CMMKController::SetAllSingle(struct rgb color)
{
    cmmk_set_all_single(&cmmk_handle, &color);
}

void CMMKController::SetMode(cmmk_effect_fully_lit eff)
{
    ActivateEffect(CMMK_EFFECT_FULLY_LIT);
    cmmk_set_effect_fully_lit(&cmmk_handle, &eff);
}

void CMMKController::SetMode(cmmk_effect_breathe eff)
{
    ActivateEffect(CMMK_EFFECT_BREATHE);
    cmmk_set_effect_breathe(&cmmk_handle, &eff);
}

void CMMKController::SetMode(cmmk_effect_cycle eff)
{
    ActivateEffect(CMMK_EFFECT_CYCLE);
    cmmk_set_effect_cycle(&cmmk_handle, &eff);
}

void CMMKController::SetMode(cmmk_effect_single eff)
{
    ActivateEffect(CMMK_EFFECT_SINGLE);
    cmmk_set_effect_single(&cmmk_handle, &eff);
}

void CMMKController::SetMode(cmmk_effect_wave eff)
{
    ActivateEffect(CMMK_EFFECT_WAVE);
    cmmk_set_effect_wave(&cmmk_handle, &eff);
}

void CMMKController::SetMode(cmmk_effect_ripple eff)
{
    ActivateEffect(CMMK_EFFECT_RIPPLE);
    cmmk_set_effect_ripple(&cmmk_handle, &eff); 
}

void CMMKController::SetMode(cmmk_effect_cross eff)
{
    ActivateEffect(CMMK_EFFECT_CROSS);
    cmmk_set_effect_cross(&cmmk_handle, &eff); 
}

void CMMKController::SetMode(cmmk_effect_raindrops eff)
{
    ActivateEffect(CMMK_EFFECT_RAINDROPS);
    cmmk_set_effect_raindrops(&cmmk_handle, &eff); 
}

void CMMKController::SetMode(cmmk_effect_stars eff)
{
    ActivateEffect(CMMK_EFFECT_STARS);
    cmmk_set_effect_stars(&cmmk_handle, &eff); 
}

bool CMMKController::PositionValid(int y, int x) const
{
    return cmmk_lookup_key_id(&cmmk_handle, y, x) >= 0;
}

void CMMKController::ActivateMode(int mode)
{
    if (current_mode != mode) {
        cmmk_set_control_mode(&cmmk_handle, mode);

        current_mode = mode;
        current_effect = -1;
    }
}

void CMMKController::ActivateEffect(int effect)
{
    ActivateMode(CMMK_EFFECT);

    if (current_effect != effect) {
        cmmk_set_active_effect(&cmmk_handle, (enum cmmk_effect_id)effect);

        current_effect = effect;
    }
}