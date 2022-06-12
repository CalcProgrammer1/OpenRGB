/*-----------------------------------------*\
|  GaiZhongGaiKeyboardController.h                |
|                                           |
|  Definitions and types for GaiZhongGai RGB      |
|  keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*-----------------------------------------------------*\
| GaiZhongGai vendor ID                                 |
\*-----------------------------------------------------*/
#define GaiZhongGai_VID                       0x3061

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define GaiZhongGai_68_PRO_PID          0x4700
#define GaiZhongGai_17_TOUCH_PRO_PID    0x4770
#define GaiZhongGai_17_PRO_PID          0x4771

class GaiZhongGaiKeyboardController
{
public:
    GaiZhongGaiKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid);
    ~GaiZhongGaiKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    unsigned short  GetUSBPID();
    
    void SendColors
            (
            unsigned char*  color_data,
            unsigned int    color_data_size
            );

private:
    hid_device*             dev;
    std::string             location;
    unsigned short          usb_pid;
};
