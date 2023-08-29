/*-------------------------------------------*\
|  RGBController_GenesisXenon200.h            |
|                                             |
|  Definitions and types for Genesis Xenon    |
|  200 Mouse                                  |
|                                             |
|  chrabonszcz 07/2023                        |
\*-------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "hidapi/hidapi.h"

class GenesisXenon200Controller
{
public:
    GenesisXenon200Controller(hid_device* dev_handle, hid_device* cmd_dev_handle,  const char* path);
    ~GenesisXenon200Controller();

    void SaveMode(unsigned char mode, unsigned char value, RGBColor color);
    std::string GetLocationString();
private:
    hid_device* dev;
    hid_device* cmd_dev;
    std::string location;
};
