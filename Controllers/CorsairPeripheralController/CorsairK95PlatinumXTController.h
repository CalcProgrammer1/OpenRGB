/*-----------------------------------------*\
|  CorsairK95PlatinumXTController.h         |
|                                           |
|  Driver for Corsair K95 Platinum XT       |
|  Keyboard                                 |
|                                           |
|  Guimard Morgan (morg) 6/07/2022          |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include <string.h>
#include <hidapi/hidapi.h>

#define K95_PLATINUM_XT_REPORT_LENGTH           65
#define K95_PLATINUM_XT_COLOR_DATA_LENGTH       488
#define K95_PLATINUM_XT_REPORT_ID               0x08
#define K95_PLATINUM_XT_NUMBER_OF_PAGES         0x08
#define K95_PLATINUM_XT_READ_COMMAND            0x02
#define K95_PLATINUM_XT_DIRECT_START_STREAM     0x07
#define K95_PLATINUM_XT_DIRECT_BLOCK_STREAM     0x06

class CorsairK95PlatinumXTController
{
public:
    CorsairK95PlatinumXTController(hid_device* dev_handle, const char* path);
    ~CorsairK95PlatinumXTController();

    std::string     GetDeviceLocation();
    std::string     GetFirmwareString();
    std::string     GetName();
    std::string     GetSerialString();

    void            SendDirect(const std::vector<RGBColor>& colors, const std::vector<std::tuple<std::string,unsigned int>>& leds_positions);

private:
    hid_device*     dev;

    std::string     firmware_version;
    std::string     location;
    std::string     name;

    void            Init();
};
