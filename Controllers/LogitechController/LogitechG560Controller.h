/*-----------------------------------------*\
|  LogitechG560Controller.h                 |
|                                           |
|  Definitions and types for Logitech G560  |
|  RGB Speaker                              |
|                                           |
|  Cheerpipe     10/28/2020                 |
|  Based on                                 |
|  TheRogueZeta   8/31/2020                 |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G560_MODE_OFF                      = 0x00,
    LOGITECH_G560_MODE_DIRECT                   = 0x01,
    LOGITECH_G560_MODE_CYCLE                    = 0x02,
    LOGITECH_G560_MODE_BREATHING                = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are mutipled by 100 later to give lots of GUI steps.                                   |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G560_SPEED_SLOWEST                 = 0xC8,     /* Slowest speed                   */
    LOGITECH_G560_SPEED_NORMAL                  = 0x32,     /* Normal speed                    */
    LOGITECH_G560_SPEED_FASTEST                 = 0x0A,     /* Fastest speed                   */
};

class LogitechG560Controller
{
public:
    LogitechG560Controller(hid_device* dev_handle, const char* path);
    ~LogitechG560Controller();
    std::string GetDeviceLocation();
    void SetDirectMode(uint8_t zone);
    void SetOffMode(uint8_t zone);
    void        SendSpeakerMode
                   (
                    unsigned char       zone,           //0x04
                    unsigned char       mode,           //0x05
                    unsigned char       speed,          //0x0B
                    unsigned char       red,            //0x06
                    unsigned char       green,          //0x07
                    unsigned char       blue            //0x08
                   );

private:
    hid_device*             dev;
    std::string             location;
    void fail_retry_write(hid_device *device, const unsigned char *data, size_t length);
};


