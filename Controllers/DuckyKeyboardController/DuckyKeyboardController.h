/*-----------------------------------------*\
|  DuckyKeyboardController.h                |
|                                           |
|  Definitions and types for Ducky RGB      |
|  keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*-----------------------------------------------------*\
| Ducky vendor ID                                       |
\*-----------------------------------------------------*/
#define DUCKY_VID                       0x04D9

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define DUCKY_SHINE_7_ONE_2_RGB_PID     0x0348
#define DUCKY_ONE_2_RGB_TKL_PID         0x0356

class DuckyKeyboardController
{
public:
    DuckyKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid);
    ~DuckyKeyboardController();

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

    void SendInitialize();
    void SendInitializeColorPacket();
    unsigned int SendColorDataPacket
        (
        unsigned char   packet_id,
        unsigned char*  color_data,
        unsigned int    color_size
        );
    void SendTerminateColorPacket();
};
