/*---------------------------------------------------------*\
|  GigabyteCastor3Controller.h                              |
|                                                           |
|  Driver for Gigabyte Aorus Waterforce X II 360 AIO        |
|  (Castor3 USB HID controller)                             |
|                                                           |
|  RGB ring control only — LCD not implemented              |
|                                                           |
|  Protocol reference: castor3.py reverse engineering        |
|  VID=0x0414  PID=0x7A5E  Report ID=0x99                  |
|  OUT size: 6144 (report ID + 6143 payload)                |
|  IN size:  255                                            |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

/*---------------------------------------------------------*\
|  Castor3 USB IDs                                          |
\*---------------------------------------------------------*/

#define CASTOR3_VID                     0x0414
#define CASTOR3_PID                     0x7A5E

/*---------------------------------------------------------*\
|  Castor3 HID transport                                    |
\*---------------------------------------------------------*/

#define CASTOR3_REPORT_ID               0x99
#define CASTOR3_OUT_PAYLOAD             6143    /* payload after report ID */
#define CASTOR3_OUT_TOTAL               6144    /* report ID + payload     */
#define CASTOR3_IN_SIZE                 255
#define CASTOR3_IN_TIMEOUT_MS           2000

/*---------------------------------------------------------*\
|  LED effect style IDs (c9 byte[1])                        |
|  From castor3.py LED_EFFECTS, cross-referenced with       |
|  .cled.dat profile files and led_profile.pcapng           |
\*---------------------------------------------------------*/

#define CASTOR3_STYLE_STATIC            0x01
#define CASTOR3_STYLE_PULSE             0x02
#define CASTOR3_STYLE_CYCLE             0x03
#define CASTOR3_STYLE_FLASH             0x04
#define CASTOR3_STYLE_DFLASH            0x05
#define CASTOR3_STYLE_GRADIENT          0x06
#define CASTOR3_STYLE_COLORSHIFT        0x07
#define CASTOR3_STYLE_WAVE              0x08
#define CASTOR3_STYLE_RAINBOW           0x0A
#define CASTOR3_STYLE_TRICOLOR          0x0B
#define CASTOR3_STYLE_SPIN              0x0C
#define CASTOR3_STYLE_SWITCH            0x0D

/*---------------------------------------------------------*\
|  LED speed range: 1-5 (wire value = speed * 20)           |
|  LED brightness range: 1-10 (direct on wire)              |
\*---------------------------------------------------------*/

#define CASTOR3_SPEED_MIN               1
#define CASTOR3_SPEED_MAX               5
#define CASTOR3_SPEED_DEFAULT           5
#define CASTOR3_BRIGHTNESS_MIN          1
#define CASTOR3_BRIGHTNESS_MAX          10
#define CASTOR3_BRIGHTNESS_DEFAULT      10

/*---------------------------------------------------------*\
|  Color type for effects                                   |
\*---------------------------------------------------------*/

enum castor3_color_type
{
    CASTOR3_COLORS_NONE     = 0,    /* cycle, wave, rainbow    */
    CASTOR3_COLORS_SINGLE   = 1,    /* static, pulse, flash... */
    CASTOR3_COLORS_PALETTE  = 2,    /* colorshift, tricolor... */
};

class GigabyteCastor3Controller
{
public:
    GigabyteCastor3Controller(hid_device* dev_handle, const char* path);
    ~GigabyteCastor3Controller();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();

    /*---------------------------------------------------------*\
    |  LED effect application                                   |
    |                                                           |
    |  Wire sequence per effect apply:                          |
    |    c9 [Style] [Speed*20] [Bright] [b4] [b5]              |
    |    cd [R] [G] [B]                  — single-color only    |
    |    b0 [Style] [R1G1B1] [R2G2B2] 00 — palette colors 1+2 |
    |    b1 [Style] [R3G3B3] [R4G4B4] 00 — colors 3+4         |
    |    b2 [Style] [R5G5B5] [R6G6B6] 00 — colors 5+6         |
    |    b3 [Style] [R7G7B7] [R8G8B8] 00 — colors 7+8         |
    |    b6                               — commit/apply        |
    \*---------------------------------------------------------*/

    void            SetEffect(unsigned char style,
                              unsigned char speed,
                              unsigned char brightness,
                              unsigned char b4,
                              unsigned char b5,
                              castor3_color_type color_type,
                              std::vector<RGBColor> colors);

    void            SetOff();

private:
    hid_device*     dev;
    std::string     location;
    std::string     firmware_version;

    void            SendPacket(const unsigned char* payload, unsigned int payload_len);
};
