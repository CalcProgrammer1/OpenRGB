#pragma once

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#define ROCCAT_ELO_REPORT_SIZE  16
#define ROCCAT_ELO_LEDS_COUNT   1
#define ROCCAT_ELO_REPORT_ID    0xFF

class RoccatEloController
{
public:
    RoccatEloController(hid_device* dev_handle, const hid_device_info& info);
    ~RoccatEloController();

    std::string     GetSerialString();
    std::string     GetDeviceLocation();

    void            SendDirect(RGBColor color);

private:
    hid_device*     dev;
    std::string     location;
    std::string     serial_number;
    std::string     version;

    void            SendInit();
};
