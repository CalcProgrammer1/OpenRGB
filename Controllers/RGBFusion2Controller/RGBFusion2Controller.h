/*-----------------------------------------*\
|  RGBFusion2Controller.h                   |
|                                           |
|  Definitions and types for Gigabyte Aorus |
|  RGB Fusion 2.0 lighting controller       |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/15/2020   |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#pragma once


class RGBFusion2Controller
{
public:
    RGBFusion2Controller(hid_device* dev_handle);
    ~RGBFusion2Controller();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    unsigned char   GetMode();
    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);

private:
    void            dump();

    char                    device_name[32];
    unsigned int            led_count;

    void            SendApply();
    
    hid_device*             dev;
};
