/*-----------------------------------------*\
|  HyperXAlloyElite2Controller.h            |
|                                           |
|  Definitions and types for HyperX Alloy   |
|  Elite2 RGB Keyboard lighting controller  |
|                                           |
|  KundaPanda (vojdo) 02/04/2021            |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXAlloyElite2Controller
{
public:
    HyperXAlloyElite2Controller(hid_device* dev_handle, const char* path);
    ~HyperXAlloyElite2Controller();

    std::string                         GetDeviceLocation();
    std::string                         GetSerialString();

    void                                SetLEDsDirect(const std::vector<RGBColor>& colors);

private:
    hid_device*                         dev;
    std::string                         location;

    void                                SendDirectInitialization();
};
