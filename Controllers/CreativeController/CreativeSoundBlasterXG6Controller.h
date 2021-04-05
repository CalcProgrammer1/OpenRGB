#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>

#pragma once

class CreativeSoundBlasterXG6Controller
{
public:
    CreativeSoundBlasterXG6Controller(hid_device* dev_handle, const char* path);
    ~CreativeSoundBlasterXG6Controller();
    void            SetLedColor (unsigned char red, unsigned char green, unsigned char blue);

    std::string GetDeviceLocation();
    std::string GetSerialString();

private:
    hid_device*             dev;
    std::string             location;
};
