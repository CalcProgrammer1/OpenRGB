/*-----------------------------------------*\
|  GloriousModelOController.h               |
|                                           |
|  Definitions and types for Glorious       |
|  and other Mice                           |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    GLORIOUS_MODE_STATIC = 0x01,
};

class GloriousModelOController
{
public:
    GloriousModelOController(libusb_device_handle* dev_handle);
    ~GloriousModelOController();

    std::string     GetDeviceName();
    unsigned int    GetLEDCount();

    void            SetLEDColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetMode(unsigned char mode);
private:
    libusb_device_handle*   dev;

    char                    device_name[32];
    unsigned int            led_count;
};
