/*-----------------------------------------*\
|  CorsairWirelessController.h              |
|                                           |
|  Definitions and types for Corsair RGB    |
|  wireless keyboard lighting controller    |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/8/2021    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class CorsairWirelessController
{
public:
    CorsairWirelessController(hid_device* dev_handle, const char* path);
    ~CorsairWirelessController();

    device_type     GetDeviceType();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareString();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetLEDs(std::vector<RGBColor> colors);
    void            SetLEDsKeyboardFull(std::vector<RGBColor> colors);
    void            SetName(std::string device_name);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    void            EnterDirectMode();
    void            ExitDirectMode();
    void            StartDirectMode();
    void            SendDirectFrame(bool first_frame, unsigned char* data);
};
