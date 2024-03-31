#pragma once

#include <string>
#include <hidapi/hidapi.h>

#include "RGBController.h"

class HIDLampArrayController
{
public:
    HIDLampArrayController(hid_device *dev_handle, const char *path);
    ~HIDLampArrayController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

private:
    hid_device *dev;
    std::string location;
};
