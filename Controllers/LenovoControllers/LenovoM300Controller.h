/*-----------------------------------*\
|  LenovoM300Controller.h             |
|                                     |
|  Header file for Lenovo M300 Mouse  |
|  controller                         |
|                                     |
|  Wayne Riordan          9 Jan 2024  |
\*-----------------------------------*/
#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#define M300_DATA_SIZE  0x41
#define M300_MAX_BRIGTH 0x64
#define M300_MIN_BRIGHT 0x01

enum
{
    M300_MODE_OFF       = 0x00,
    M300_MODE_RAINBOW   = 0x01,
    M300_MODE_BREATHING = 0x02,
    M300_MODE_STATIC    = 0X03
};

class LenovoM300Controller
{
public:
    LenovoM300Controller(hid_device* dev_handle, const hid_device_info& info);
    ~LenovoM300Controller();

    std::string     GetDeviceLocation();

    void SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned int brightness);

protected:
    hid_device* device;

private:
    std::string     location;

    unsigned char CalculateFinalByte(unsigned char* ptr, int count);
};
