/*-----------------------------------------*\
|  LogitechGProWirelessController.h         |
|                                           |
|  Definitions and types for Logitech G Pro |
|  Wireless Gaming Mouse lighting controller|
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "RGBController.h"

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

enum
{
    LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST          = 0x00,   /* Slowest speed                */
    LOGITECH_G_PRO_WIRELESS_SPEED_SLOW             = 0x01,   /* Slow speed                   */
    LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL           = 0x02,   /* Normal speed                 */
    LOGITECH_G_PRO_WIRELESS_SPEED_FAST             = 0x03,   /* Fast speed                   */
    LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST          = 0x04,   /* Fastest speed                */
};

class LogitechGProWirelessController
{
public:
    LogitechGProWirelessController(hid_device* dev_handle);
    ~LogitechGProWirelessController();

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
};
