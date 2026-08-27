/*---------------------------------------------------------*\
| TurtleBeachKP7Controller.h                                |
|                                                           |
|   Driver for Turtle Beach Command Series KP7 keypad       |
|                                                           |
|   Joe Peterson (joevpeterson@gmail.com)       05 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <hidapi.h>

/*---------------------------------------------------------*\
| Turtle Beach vendor ID (Voyetra Turtle Beach, Inc.)       |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_VID                            0x10F5

/*---------------------------------------------------------*\
| KP7 product ID and RGB/config interface number.  The KP7  |
| also enumerates PID 0x503E for the internal USB hub used  |
| when docked to the KB7 keyboard - that is not this device |
| and is not handled here.                                  |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_PID                        0x503A
#define TURTLE_BEACH_KP7_INTERFACE                  4

/*---------------------------------------------------------*\
| RGB report layout (interface 4, EP 0x06 OUT, interrupt,   |
| 64 byte reports):                                         |
|                                                           |
|       byte 0      report ID (0xEF)                        |
|       byte 1      page index (0x00 or 0x01)               |
|       byte 2+     raw LED data, [R, G, B] per LED         |
|                                                           |
| One LED frame does not fit in a single 64 byte report so  |
| it is split across two pages.                             |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_LED_COUNT                  33
#define TURTLE_BEACH_KP7_REPORT_SIZE                64
#define TURTLE_BEACH_KP7_REPORT_ID                  0xEF
#define TURTLE_BEACH_KP7_PAGE0_LED_BYTES            62
#define TURTLE_BEACH_KP7_PAGE1_LED_BYTES            37

/*---------------------------------------------------------*\
| Small feature report channel on the same interface, sent  |
| over the control endpoint rather than EP 0x06.  Without   |
| this write the device stays locked to its onboard preset  |
| effects and ignores the raw pixel stream entirely.        |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_ENTER_DIRECT_REPORT_ID     14

/*---------------------------------------------------------*\
| The device reverts to its onboard hue shift effect if it  |
| stops hearing from the host, so the last frame is re-sent |
| on this interval to hold it in direct mode.               |
\*---------------------------------------------------------*/
#define TURTLE_BEACH_KP7_KEEPALIVE_DELAY_MS         50

class TurtleBeachKP7Controller
{
public:
    TurtleBeachKP7Controller(hid_device* dev_handle, const char* path);
    ~TurtleBeachKP7Controller();

    std::string         GetLocation();
    std::string         GetSerialString();

    void                EnterDirectMode();
    void                SendDirect(unsigned char* led_data, unsigned int led_data_size);

private:
    void                KeepAliveThreadFunction();
    void                SendFrame(unsigned char* led_data, unsigned int led_data_size);

    hid_device*         dev;
    std::string         location;

    std::thread         keepalive_thread;
    std::atomic<bool>   keepalive_running;
    std::mutex          led_data_mutex;
    unsigned char       last_led_data[TURTLE_BEACH_KP7_LED_COUNT * 3];
    unsigned int        last_led_data_size;
};
