/*-----------------------------------------*\
|  LegoDimensionsToypadBaseController.h     |
|                                           |
|  Driver for Lego Dimensions ToypadBase    |
|  controller - header file                 |
|                                           |
|  Guimard Morgan (morg) 06/02/2023         |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#define LEGO_DIMENSIONS_TOYPAD_BASE_REPORT_ID       0x55
#define LEGO_DIMENSIONS_TOYPAD_BASE_PACKET_LENGTH   32

enum
{
    LEGO_DIMENSIONS_TOYPAD_BASE_ACTIVATE_VALUE      = 0xB0,
    LEGO_DIMENSIONS_TOYPAD_BASE_DIRECT_MODE_VALUE   = 0xC0,
    LEGO_DIMENSIONS_TOYPAD_BASE_FLASH_MODE_VALUE    = 0xC3,
    LEGO_DIMENSIONS_TOYPAD_BASE_FADE_MODE_VALUE     = 0xC2
};

enum
{
    LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MIN      = 0x00,
    LEGO_DIMENSIONS_TOYPAD_BASE_SPEED_MAX      = 0xFF
};

class LegoDimensionsToypadBaseController
{
public:
    LegoDimensionsToypadBaseController(hid_device* dev_handle, const hid_device_info& info);
    ~LegoDimensionsToypadBaseController();

    std::string GetSerialString();
    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();

    void        SetDirect(unsigned char zone, RGBColor color);
    void        SetMode(unsigned char zone, unsigned char mode_value, uint8_t speed, RGBColor color);

protected:
    hid_device* dev;

private:
    std::string location;
    std::string serial_number;
    std::string version;

    void Activate();
};
