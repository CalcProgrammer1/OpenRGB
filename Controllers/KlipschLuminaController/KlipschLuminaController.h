/*---------------------------------------------------------*\
| KlipschLuminaController.h                                 |
|                                                           |
|   Driver for Klipsch ProMedia Lumina speakers             |
|                                                           |
|   wlof                                        26 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <string>
#include "hidapi_wrapper.h"
#include "RGBController.h"

#define KLIPSCH_LUMINA_LAMPARRAY_CONTROL_ID 6

#define KLIPSCH_LUMINA_REPORT_SIZE          256
#define KLIPSCH_LUMINA_BUFFER_SIZE          (KLIPSCH_LUMINA_REPORT_SIZE + 1)

#define KLIPSCH_LUMINA_NUM_SATELLITES       2
#define KLIPSCH_LUMINA_LEDS_PER_SATELLITE   6
#define KLIPSCH_LUMINA_LED_COUNT            (KLIPSCH_LUMINA_NUM_SATELLITES * KLIPSCH_LUMINA_LEDS_PER_SATELLITE)

/*---------------------------------------------------------*\
| Command frame layout                                      |
|   A5 5A 70 <cmd> <frame length> 00 <data...> <checksum>   |
| The checksum makes the sum of all frame bytes zero.       |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_FRAME_HEADER_0       0xA5
#define KLIPSCH_LUMINA_FRAME_HEADER_1       0x5A
#define KLIPSCH_LUMINA_FRAME_HEADER_2       0x70
#define KLIPSCH_LUMINA_FRAME_OVERHEAD       7

enum
{
    KLIPSCH_LUMINA_CMD_LIGHT_MODE           = 0x0B,     /* 1 byte: light mode               */
    KLIPSCH_LUMINA_CMD_LED_LEVEL            = 0x0F,     /* 2 bytes: selector, brightness    */
    KLIPSCH_LUMINA_CMD_CUSTOM_LIGHTING      = 0x11,     /* 36 bytes: 12 x RGB               */
    KLIPSCH_LUMINA_CMD_CUSTOM_RGB           = 0x1A,     /* 6 bytes: primary and secondary   */
};

enum
{
    KLIPSCH_LUMINA_MODE_RAINBOW             = 0x01,
    KLIPSCH_LUMINA_MODE_BREATHE             = 0x02,
    KLIPSCH_LUMINA_MODE_STATIC              = 0x03,
    KLIPSCH_LUMINA_MODE_AURORA              = 0x04,
    KLIPSCH_LUMINA_MODE_MUSIC_REACT         = 0x05,
    KLIPSCH_LUMINA_MODE_OFF                 = 0x06,
    KLIPSCH_LUMINA_MODE_DIRECT              = 0x07,     /* "Screen React" in Klipsch's app  */
    KLIPSCH_LUMINA_MODE_UNKNOWN             = 0xFF,     /* State not read                   */
};

/*---------------------------------------------------------*\
| Offsets into the 257 byte state buffer read from the      |
| device (byte 0 is the report ID)                          |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_STATE_LIGHT_MODE     13
#define KLIPSCH_LUMINA_STATE_SERIAL         14
#define KLIPSCH_LUMINA_STATE_SERIAL_LEN     15
#define KLIPSCH_LUMINA_STATE_FIRMWARE       35
#define KLIPSCH_LUMINA_STATE_FIRMWARE_LEN   5
#define KLIPSCH_LUMINA_STATE_LED_LEVEL      50
#define KLIPSCH_LUMINA_STATE_CUSTOM_RGB     54

/*---------------------------------------------------------*\
| LED brightness, sent after a selector of 1                |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_LED_LEVEL_SELECTOR   1
#define KLIPSCH_LUMINA_BRIGHTNESS_MIN       0
#define KLIPSCH_LUMINA_BRIGHTNESS_MAX       100

/*---------------------------------------------------------*\
| Delays.  The state poll covers the 12 s the device takes  |
| to answer after power up or a USB reset.                  |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_STATE_TIMEOUT_MS     15000
#define KLIPSCH_LUMINA_STATE_DELAY_MS       100
#define KLIPSCH_LUMINA_MODE_DELAY_MS        300
#define KLIPSCH_LUMINA_LAMPARRAY_DELAY_MS   200

/*---------------------------------------------------------*\
| Frames closer than about 5 ms are dropped and the LEDs    |
| fall back to white, space them by the MinUpdateInterval   |
| advertised by the LampArray interface                     |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_FRAME_INTERVAL_MS    50

class KlipschLuminaController
{
public:
    KlipschLuminaController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, hid_device* lamparray_handle, const char* path);
    ~KlipschLuminaController();

    std::string     GetDeviceLocation();
    std::string     GetFirmwareVersion();
    std::string     GetSerialString();
    unsigned char   GetLightMode();
    unsigned char   GetBrightness();
    RGBColor        GetPrimaryColor();
    RGBColor        GetSecondaryColor();

    void            SetLightMode(unsigned char mode);
    void            SetCustomColors(RGBColor primary, RGBColor secondary);
    void            SetLEDColors(RGBColor* colors);
    void            SetBrightness(unsigned char level);

private:
    hidapi_wrapper          wrapper;
    hid_device*             dev;
    hid_device*             lamparray_dev;
    std::string             location;
    std::string             firmware;
    std::string             serial;
    unsigned char           light_mode;
    unsigned char           brightness;
    RGBColor                primary_color;
    RGBColor                secondary_color;
    bool                    direct_unlocked;

    std::chrono::steady_clock::time_point   last_frame_time;

    void            ReadState();
    void            SendCommand(unsigned char cmd, const unsigned char* data, unsigned char data_len);
};
