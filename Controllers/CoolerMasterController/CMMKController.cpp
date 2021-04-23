/*-------------------------------------------------------------------*\
|  CMMKController.cpp                                                 |
|                                                                     |
|  Driver for Coolermaster MasterKeys Keyboards                       |
|                                                                     |
|  Lukas N (chmod222)       28th Jun 2020                             |
|  Tam D (too.manyhobbies)  25th Apr 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMMKController.h"

CMMKController::CMMKController(hid_device* dev, hid_device_info* dev_info)
{
    location        =  dev_info->path;

    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name        = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    cmmk_attach_path(&cmmk_handle, dev_info->path, dev_info->product_id, -1);

    char buf[32] = {0};
    cmmk_get_firmware_version(&cmmk_handle, buf, 32);

    firmware_version = buf;
}

CMMKController::~CMMKController()
{
    cmmk_detach(&cmmk_handle);
}

std::string CMMKController::GetDeviceName()
{
    return device_name;
}

std::string CMMKController::GetLocation()
{
    return location;
}

std::string CMMKController::GetFirmwareVersion()
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

void CMMKController::SetMode(cmmk_effect_snake eff)
{
    ActivateEffect(CMMK_EFFECT_SNAKE);
    cmmk_set_effect_snake(&cmmk_handle, &eff); 
}

bool CMMKController::PositionValid(int y, int x)
{
    return(cmmk_lookup_key_id(&cmmk_handle, y, x) >= 0);
}

void CMMKController::ActivateMode(int mode)
{
    if (current_mode != mode)
    {
        cmmk_set_control_mode(&cmmk_handle, mode);

        current_mode = mode;
        current_effect = -1;
    }
}

void CMMKController::ActivateEffect(int effect)
{
    ActivateMode(CMMK_EFFECT);

    if (current_effect != effect)
    {
        cmmk_set_active_effect(&cmmk_handle, (enum cmmk_effect_id)effect);

        current_effect = effect;
    }
}
