/*-----------------------------------------*\
|  LogitechLightspeedController.h           | 
|                                           |
|  Definitions and types for Logitech G     |
|  Lightsync Wireless Gaming Mice lighting  |
|  controller                               |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "RGBController.h"
#include "LogitechProtocolCommon.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G_PRO_WIRELESS_MODE_OFF               = 0x00,
    LOGITECH_G_PRO_WIRELESS_MODE_STATIC            = 0x01,
    LOGITECH_G_PRO_WIRELESS_MODE_CYCLE             = 0x02,
    LOGITECH_G_PRO_WIRELESS_MODE_BREATHING         = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{   
    LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST          = 0xC8,  /* Slowest speed                   */
    LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL           = 0x32,  /* Normal speed                    */
    LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST          = 0x0A,  /* Fastest speed                   */
};

class LogitechLightspeedController
{
public:
    LogitechLightspeedController(hid_device* dev_handle, const char* path);
    ~LogitechLightspeedController();

    logitech_device* lightspeed;
    
    std::string GetDeviceLocation();
    std::string GetSerialString();

    void        SendMouseMode
                   (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       zone,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
//                    unsigned char       brightness
                   );

private:
    hid_device*             dev;
    std::string             location;
};
