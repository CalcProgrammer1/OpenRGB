#ifndef CORSAIRK55RGBPROCONTROLLER_H
#define CORSAIRK55RGBPROCONTROLLER_H

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

class CorsairK55RGBPROController
{
public:
    CorsairK55RGBPROController(hid_device* dev_handle, const char* path);
    ~CorsairK55RGBPROController();
    std::string                     GetDeviceLocation();
    std::string                     GetFirmwareString();
    std::string                     GetName();
    std::string                     GetSerialString();

    void                            SetLEDs(std::vector<RGBColor> colors);
    void                            SetLEDsKeyboardLimited(std::vector<RGBColor> colors);
    void                            SetName(std::string device_name);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    void                    LightingControl();
    void                    SetLEDsKeyboardFull(std::vector<RGBColor> colors);
};

#endif // CORSAIRK55RGBPROCONTROLLER_H
