/*---------------------------------------------------------------*\
|  GaiZhongGaiKeyboardController.h                                |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
| An Yang     2022/6/12                                           |
\*---------------------------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*-----------------------------------------------------*\
| GaiZhongGai vendor ID                                 |
\*-----------------------------------------------------*/
#define GAIZHONGGAI_VID                       0x3061

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define GAIZHONGGAI_68_PRO_PID          0x4700
#define GAIZHONGGAI_17_TOUCH_PRO_PID    0x4770
#define GAIZHONGGAI_17_PRO_PID          0x4771
#define GAIZHONGGAI_20_PRO_PID          0x4772

class GaiZhongGaiKeyboardController
{
public:
    GaiZhongGaiKeyboardController(hid_device* dev_handle, hid_device_info* info);
    ~GaiZhongGaiKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    std::string     GetVersion();
    unsigned short  GetUSBPID();
    
    void SendColors
    (
    unsigned char*  color_data,
    unsigned int    color_data_size
    );

private:
    hid_device*             dev;
    std::string             location;
    std::string             version;
    unsigned short          usb_pid;
};
