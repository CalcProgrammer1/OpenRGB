/*-------------------------------------------------------------------*\
|  BloodyB820RController.h                                            |
|                                                                     |
|  Driver for A4Tech Bloody B820R Keyboard Controller                 |
|                                                                     |
|  Mohammed Julfikar Ali Mahbub (o-julfikar)          01 Apr 2024     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>
#include "RGBController.h"

#pragma once

#define NA                                      0xFFFFFFFF
#define HID_MAX_STR                             255

#define BLOODY_B820R_PACKET_SIZE        64
#define BLOODY_B820R_KEYCOUNT           104
#define BLOODY_B820R_MODE_BYTE           3
#define BLOODY_B820R_DATA_BYTE           8

/*---------------------------------------------------------*\
| Bloody B820R product ID                                    |
\*---------------------------------------------------------*/
#define BLOODY_B820R_PID                               0xFA10

enum
{
    BLOODY_B820R_MODE_DIRECT     = 0x01,    // Direct LED control - Independently set LEDs in zone
    BLOODY_B820R_MODE_RELEASE    = 0x02,    // Release the OpenRGB control from the keyboard
    BLOODY_B820R_MODE_INIT       = 0x03,    // Regain the OpenRGB control of the keyboard
    BLOODY_B820R_MODE_BREATHING  = 0x04,    // Set breathing RGB effect
};

class BloodyB820RController
{
public:
    BloodyB820RController(hid_device* dev_handle, const char* path);
    ~BloodyB820RController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetLEDDirect(std::vector<RGBColor> colors);
    void            SetMode(int mode, int speed, unsigned char brightness, unsigned char dir, unsigned char red, unsigned char green, unsigned char blue);
private:
    std::string     location;
    hid_device*     dev;

    void            InitDevice();
    void            ReleaseDevice();
};
