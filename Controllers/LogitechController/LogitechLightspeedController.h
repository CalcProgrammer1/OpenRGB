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
#include "LogManager.h"
#include "LogitechProtocolCommon.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once
#define LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MIN      0x01
#define LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX      0x64

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
                    uint8_t         mode,
                    uint16_t        speed,
                    uint8_t         zone,
                    uint8_t         red,
                    uint8_t         green,
                    uint8_t         blue,
                    uint8_t         brightness
                );

private:
    hid_device*             dev;
    std::string             location;
};
