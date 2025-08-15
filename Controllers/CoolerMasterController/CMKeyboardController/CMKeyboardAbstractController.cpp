/*---------------------------------------------------------*\
| CMKeyboardAbstractController.cpp                          |
|                                                           |
|   Driver for Cooler Master keyboards                      |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CMKeyboardAbstractController.h"
#include "StringUtils.h"

CMKeyboardAbstractController::CMKeyboardAbstractController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name)
{
    wchar_t tmp[HID_MAX_STR];

    m_pDev                   = dev_handle;
    m_productId              = dev_info->product_id;
    m_sLocation              = dev_info->path;
    m_deviceName             = dev_name;

    hid_get_manufacturer_string(m_pDev, tmp, HID_MAX_STR);
    m_vendorName             = StringUtils::wstring_to_string(tmp);

    hid_get_product_string(m_pDev, tmp, HID_MAX_STR);
    m_serialNumber           = StringUtils::wstring_to_string(tmp);

    bool bNotFound           = true;

    for(uint16_t i = 0; i < COOLERMASTER_KEYBOARD_DEVICE_COUNT; i++)
    {
        if(cm_kb_device_list[i]->product_id == m_productId)
        {
            bNotFound       = false;
            m_deviceIndex   = i;
            break;
        }
    }

    if(bNotFound)
    {
        LOG_ERROR("[%s] device capabilities not found. Please creata a new device request.", m_deviceName.c_str());
    }
};

CMKeyboardAbstractController::~CMKeyboardAbstractController()
{
    hid_close(m_pDev);
};

std::string CMKeyboardAbstractController::GetDeviceName()
{
    return(m_deviceName);
}

std::string CMKeyboardAbstractController::GetDeviceVendor()
{
    return(m_vendorName);
}

std::string CMKeyboardAbstractController::GetDeviceSerial()
{
    return(m_serialNumber);
}

const cm_kb_device* CMKeyboardAbstractController::GetDeviceData()
{
    return(cm_kb_device_list[m_deviceIndex]);
}

std::string CMKeyboardAbstractController::GetLocation()
{
    return(m_sLocation);
}

std::string CMKeyboardAbstractController::GetFirmwareVersion()
{
    return(m_sFirmwareVersion);
}

int CMKeyboardAbstractController::GetProductID()
{
    return(m_productId);
}

std::vector<uint8_t> CMKeyboardAbstractController::SendCommand(std::vector<uint8_t> buf, uint8_t fill)
{
    int status;
    std::vector<uint8_t> read;

    uint8_t data[CM_KEYBOARD_WRITE_SIZE];
    memset(data, fill, CM_KEYBOARD_WRITE_SIZE);

    size_t i = 1;
    for(uint8_t b : buf)
    {
        data[i++] = b;
    }

    std::lock_guard<std::mutex> guard(m_mutexSendCommand);
    status = hid_write(m_pDev, data, CM_KEYBOARD_WRITE_SIZE);

    if(status < 0)
    {
        LOG_ERROR("[%s] SendCommand() failed code %d.", m_deviceName.c_str(), status);
        return(read);
    }

    memset(data, 0, CM_KEYBOARD_WRITE_SIZE);
    status = hid_read(m_pDev, data, CM_KEYBOARD_WRITE_SIZE);

    if(status < 0)
    {
        LOG_ERROR("[%s] SendCommand() failed code %d.", m_deviceName.c_str(), status);
        return(read);
    }

    for(i = 0; i < (size_t)status; i++)
    {
        read.push_back(data[i]);
    }

    return(read);
}

/*---------------------------------------------------------*\
| Enter/leave direct control mode                           |
\*---------------------------------------------------------*/
void CMKeyboardAbstractController::SetControlMode(uint8_t modeId)
{
    SendCommand({0x41, (uint8_t)modeId});
};

/*---------------------------------------------------------*\
| Sets the currently active profile.                        |
| byte[0] = 0x51 0x00 0x00 0x00                             |
| byte[4] = profileId                                       |
| - corresponds to saved keyboard profile i.e. [1-4]        |
| - 0x05 - Used on MK and CK style keyboards?               |
\*---------------------------------------------------------*/
void CMKeyboardAbstractController::SetActiveProfile(uint8_t profileId)
{
    SendCommand({0x51, 0x00, 0x00, 0x00, profileId});
};

uint8_t CMKeyboardAbstractController::GetActiveProfile()
{
    std::vector<uint8_t> data = SendCommand({0x52, 0x00});

    if(data.size() > 4)
    {
        return((int)data[4]);
    }

    return(0xFF); // error
}

/*---------------------------------------------------------*\
| Saves changes in currently used profile.                  |
| byte[1] = 0x52                                            |
\*---------------------------------------------------------*/
void CMKeyboardAbstractController::SaveActiveProfile()
{
    SendCommand({0x50, 0x55});
}

void CMKeyboardAbstractController::SetActiveEffect(uint8_t effectId)
{
    SendCommand({0x51, 0x28, 0x00, 0x00, effectId});
};

void CMKeyboardAbstractController::SaveProfile()
{
    SendCommand({0x50, 0x55});
}


uint8_t CMKeyboardAbstractController::GetModeStatus()
{
    std::vector<uint8_t> data = SendCommand({0x52, 0x28});

    return(data[4]);
};

std::string CMKeyboardAbstractController::GetHexString(std::vector<uint8_t> buf)
{
    std::stringstream hexss;

    for(uint8_t b : buf)
    {
        hexss << std::hex << b << " ";
    }

    return(hexss.str());
}
