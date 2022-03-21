#pragma once

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#define PACKET_LENGTH 1025
#define K65_WRITE_COMMAND 0x08

class CorsairK65MiniController
{
public:
    CorsairK65MiniController(hid_device* dev_handle, const char* path);
    ~CorsairK65MiniController();

    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();
    void                    SetLEDs(std::vector<RGBColor> colors, std::vector<unsigned int> positions);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;

    void                    LightingControl();
};
