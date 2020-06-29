/*-----------------------------------------*\
|  LogitechG810Controller.h                 |
|                                           |
|  Definitions and types for Logitech G810  |
|  Orion Spectrum keyboard light controller |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/11/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    LOGITECH_G810_ZONE_MODE_KEYBOARD     = 0x00,
    LOGITECH_G810_ZONE_MODE_LOGO         = 0x01,
};

enum
{
    LOGITECH_G810_ZONE_DIRECT_KEYBOARD   = 0x01,
    LOGITECH_G810_ZONE_DIRECT_MEDIA      = 0x02,
    LOGITECH_G810_ZONE_DIRECT_LOGO       = 0x10,
    LOGITECH_G810_ZONE_DIRECT_INDICATORS = 0x40,
};

enum
{
    LOGITECH_G810_MODE_OFF               = 0x00,
    LOGITECH_G810_MODE_STATIC            = 0x01,
    LOGITECH_G810_MODE_BREATHING         = 0x02,
    LOGITECH_G810_MODE_CYCLE             = 0x03,
    LOGITECH_G810_MODE_WAVE              = 0x04,
};

enum
{
    LOGITECH_G810_SPEED_SLOWEST          = 0x00, /* Slowest speed                */
    LOGITECH_G810_SPEED_SLOW             = 0x01, /* Slow speed                   */
    LOGITECH_G810_SPEED_NORMAL           = 0x02, /* Normal speed                 */
    LOGITECH_G810_SPEED_FAST             = 0x03, /* Fast speed                   */
    LOGITECH_G810_SPEED_FASTEST          = 0x04, /* Fastest speed                */
};

class LogitechG810Controller
{
public:
    LogitechG810Controller(hid_device* dev_handle_0x11, hid_device* dev_handle_0x12);
    ~LogitechG810Controller();

    void        Commit();
    
    void        SetDirect
                    (
                    unsigned char       zone,
                    unsigned char       frame_count,
                    unsigned char *     frame_data
                    );

    void        SetMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device* dev_pkt_0x11;
    hid_device* dev_pkt_0x12;

    void        SendDirectFrame
                    (
                    unsigned char       zone,
                    unsigned char       frame_count,
                    unsigned char *     frame_data
                    );

    void        SendMode
                    (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

    void        SendCommit();
};
