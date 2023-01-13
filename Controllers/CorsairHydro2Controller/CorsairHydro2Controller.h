/*-----------------------------------------*\
|  CorsairHydro2Controller.h                |
|                                           |
|  Controller for Corsair H100i v2          |
|                                           |
|  Tim Demand (tim.dmd) 1/10/2023           |
\*-----------------------------------------*/

#include "RGBController.h"
#include <vector>
#ifdef __FreeBSD__
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

#pragma once

class CorsairHydro2Controller
{
public:
    CorsairHydro2Controller(libusb_device_handle* dev_handle);
    ~CorsairHydro2Controller();

    std::string GetLocation();

    void SetLED(std::vector<RGBColor>& colors);

private:
    libusb_device_handle* dev;
    std::string firmware_version;
    std::string location;

    void SendInit();
};
