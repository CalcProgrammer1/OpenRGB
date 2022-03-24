/*-----------------------------------------*\
|  TrustGXT114Controller.h                  |
|                                           |
|  Driver for Trust GXT 114                 |
|  controller - header file                 |
|                                           |
|  Guimard Morgan (morg) 1/24/2022          |
\*-----------------------------------------*/

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define TRUST_GXT_114_REPORT_SIZE       154
#define TRUST_GXT_114_NUMBER_OF_LEDS    1
#define TRUST_GXT_114_REPORT_ID         0x04

enum
{
    STATIC_MODE_VALUE               = 0x28,
    BREATHING_MODE_VALUE            = 0x22,
    BLINK_MODE_VALUE                = 0x42
};

enum
{
    TRUST_GXT_114_BRIGHTNESS_MIN    = 0x12,
    TRUST_GXT_114_BRIGHTNESS_MAX    = 0xA2
};

enum
{
    TRUST_GXT_114_SPEED_MIN         = 0x12,
    TRUST_GXT_114_SPEED_MAX         = 0x62
};

class TrustGXT114Controller
{
public:
    TrustGXT114Controller(hid_device* dev_handle, const hid_device_info& info);
    ~TrustGXT114Controller();

    std::string                 GetSerialString();
    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersion();
    bool                        Test();
    void                        SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 serial_number;
    std::string                 version;
};
