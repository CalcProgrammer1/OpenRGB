/*---------------------------------------------------------*\
| CMKeyboardAbstractController.h                            |
|                                                           |
|   Driver for Cooler Master keyboards                      |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstring>
#include <stdint.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <hidapi.h>
#include "CMKeyboardDevices.h"
#include "KeyboardLayoutManager.h"
#include "RGBController.h"
#include "LogManager.h"

#define HID_MAX_STR                         255
#define CM_KEYBOARD_WRITE_SIZE              65
#define CM_MAX_LEDS                         255
#define CM_KEYBOARD_TIMEOUT                 50
#define CM_KEYBOARD_TIMEOUT_SHORT           3

struct cm_keyboard_effect
{
    uint8_t  effectId;
    uint8_t  p1;
    uint8_t  p2;
    uint8_t  p3;
    RGBColor color1;
    RGBColor color2;
};

/*---------------------------------------------------------*\
| byte[0] = 0x41                                            |
| byte[1] = modeId                                          |
\*---------------------------------------------------------*/
enum cm_keyboard_control_mode
{
    MODE_FIRMWARE           = 0x00,
    MODE_EFFECT             = 0x01,
    MODE_MANUAL_V2          = 0x02,
    MODE_CUSTOM_PROFILE     = 0x03,
    MODE_CUSTOM_PROFILE_V2  = 0x05,
    MODE_DIRECT             = 0x80
};

class CMKeyboardAbstractController
{
public:
    CMKeyboardAbstractController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name);
    virtual ~CMKeyboardAbstractController();

    /*---------------------------------------------------------*\
    | Common USB controller fuctions                            |
    \*---------------------------------------------------------*/
    int                 GetProductID();
    std::string         GetDeviceName();
    std::string         GetDeviceVendor();
    std::string         GetDeviceSerial();
    std::string         GetLocation();
    std::string         GetFirmwareVersion();
    const cm_kb_device* GetDeviceData();

    /*---------------------------------------------------------*\
    | Keyboard Layout Manager support funtions                  |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    | Common keyboard driver functions                          |
    \*---------------------------------------------------------*/
    virtual void        SetControlMode(uint8_t modeId);
    virtual void        SetActiveProfile(uint8_t profileId);
    virtual uint8_t     GetActiveProfile();
    virtual void        SaveActiveProfile();
    virtual void        SaveProfile();
    virtual void        SetActiveEffect(uint8_t effectId);
    virtual uint8_t     GetModeStatus();
    virtual void        InitializeModes(std::vector<mode> &modes) = 0;
    virtual KEYBOARD_LAYOUT GetKeyboardLayout() = 0;

    /*---------------------------------------------------------*\
    | Protocol specific funtions to be implmented               |
    \*---------------------------------------------------------*/
    virtual void        SetLeds(std::vector<led> leds, std::vector<RGBColor> colors) = 0;
    virtual void        SetSingleLED(uint8_t in_led, RGBColor in_color) = 0;
    virtual void        Initialize() = 0;
    virtual void        Shutdown() = 0;
    virtual void        SetLEDControl(bool bManual) = 0; // FW or SW control
    virtual void        SetCustomMode() = 0;
    virtual void        SetMode(mode selectedMode) = 0;

protected:
    /*---------------------------------------------------------*\
    | Utility functions.                                        |
    \*---------------------------------------------------------*/
    std::vector<std::uint8_t> SendCommand(std::vector<uint8_t> buf, uint8_t fill=0x00);
    std::string         GetHexString(std::vector<uint8_t> buf);
    std::string         m_sFirmwareVersion;
    std::string         m_deviceName;
    hid_device*         m_pDev;
    uint16_t            m_productId;
    uint16_t            m_deviceIndex;
    std::string         m_vendorName;
    std::string         m_sLocation;
    std::string         m_serialNumber;
    KEYBOARD_LAYOUT     m_keyboardLayout;
    std::map<int, int>  mapModeValueEffect;
    std::mutex          m_mutex;
    std::mutex          m_mutexSendCommand;
};

