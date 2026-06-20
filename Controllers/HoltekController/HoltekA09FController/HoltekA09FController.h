/*---------------------------------------------------------*\
| HoltekA09FController.h                                    |
|                                                           |
|   Driver for Holtek A09F RGB gaming mouse                 |
|   (E-Signal LUOM G10, VID 04D9 PID A09F)                  |
|                                                           |
|   Protocol reverse-engineered from USB captures           |
|   Juampe                                         2026     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <string>
#include <hidapi.h>
#include <libusb.h>
#include "RGBController.h"

/*-----------------------------------------------------------------*\
| Protocol overview                                                 |
|                                                                   |
| The device uses 6 independent transactions per configuration      |
| change, each preceded by its own EP0 selector(s):                |
|                                                                   |
|  TX0: 6 common SELs + mode SEL + common SEL8  →  B0              |
|  TX1: SEL_B1                                  →  B1 (DPI colors) |
|  TX2: SEL_B2                                  →  B2              |
|  TX3: SEL_B3                                  →  B3 + B4         |
|  TX4: SEL_B5                                  →  B5 + B4         |
|  TX5: SEL_B7                                  →  B7              |
|                                                                   |
| Selectors: HID SET_REPORT Feature via EP0                         |
|   bmRequestType=0x21  bRequest=0x09                               |
|   wValue=0x0300  wIndex=2  wLength=8                              |
|                                                                   |
| Data blocks: libusb_interrupt_transfer EP 0x03, 32 bytes each.    |
\*-----------------------------------------------------------------*/

#define HOLTEK_A09F_SELECTOR_SIZE    8
#define HOLTEK_A09F_DPI_STAGES       8

/*-----------------------------------------------------------------*\
| Mode selector bytes (8 bytes, position 7 of TX0)                  |
\*-----------------------------------------------------------------*/
#define HOLTEK_A09F_MODE_OFF            { 0x27,0x2B,0x65,0xFF,0xE8,0x35,0x7E,0x76 }
#define HOLTEK_A09F_MODE_STANDARD       { 0x27,0x2B,0x65,0xFF,0xF0,0x35,0x85,0x6E }
#define HOLTEK_A09F_MODE_BREATHING      { 0x27,0x2B,0x5D,0xFF,0xF8,0x35,0x7E,0x8E }
#define HOLTEK_A09F_MODE_NEON           { 0x27,0x2B,0x5D,0xFF,0x00,0x3A,0x46,0x8E }
#define HOLTEK_A09F_MODE_WAVE           { 0x27,0x2B,0x55,0xFF,0xC8,0x3A,0x46,0x8E }
#define HOLTEK_A09F_MODE_KEY_REACTION   { 0x27,0x2B,0x45,0xFF,0xD0,0x35,0x85,0x6E }
#define HOLTEK_A09F_MODE_TRAILING       { 0x27,0x2B,0x25,0xFF,0xD8,0x35,0x7E,0x86 }
#define HOLTEK_A09F_MODE_DRAG           { 0x27,0x2B,0x3D,0xFF,0xE0,0x35,0x7E,0x86 }
#define HOLTEK_A09F_MODE_SLIDE          { 0x27,0x2B,0x2D,0xFF,0x28,0x35,0x85,0x6E }
#define HOLTEK_A09F_MODE_YO_YO          { 0x27,0x2A,0xA5,0xFF,0x30,0x35,0x05,0x6E }
#define HOLTEK_A09F_MODE_MARBLES        { 0x27,0x2B,0x35,0xFF,0x38,0x35,0x7D,0x6E }
#define HOLTEK_A09F_MODE_FLYING_STAR    { 0x27,0x2B,0x35,0xFF,0x40,0x35,0x85,0x6E }

enum
{
    HOLTEK_A09F_MODE_VALUE_OFF          = 0x00,
    HOLTEK_A09F_MODE_VALUE_STANDARD     = 0x01,
    HOLTEK_A09F_MODE_VALUE_BREATHING    = 0x02,
    HOLTEK_A09F_MODE_VALUE_NEON         = 0x03,
    HOLTEK_A09F_MODE_VALUE_WAVE         = 0x04,
    HOLTEK_A09F_MODE_VALUE_KEY_REACTION = 0x05,
    HOLTEK_A09F_MODE_VALUE_TRAILING     = 0x06,
    HOLTEK_A09F_MODE_VALUE_DRAG         = 0x07,
    HOLTEK_A09F_MODE_VALUE_SLIDE        = 0x08,
    HOLTEK_A09F_MODE_VALUE_YO_YO        = 0x09,
    HOLTEK_A09F_MODE_VALUE_MARBLES      = 0x0A,
    HOLTEK_A09F_MODE_VALUE_FLYING_STAR  = 0x0B,
};

class HoltekA09FController
{
public:
    HoltekA09FController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~HoltekA09FController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    /*-------------------------------------------------------------*\
    | Set illumination mode.  colors holds the current DPI stage    |
    | colours; it will be embedded in B1 of the apply sequence.     |
    \*-------------------------------------------------------------*/
    void SetMode(uint8_t mode_value);

    /*-------------------------------------------------------------*\
    | Update the DPI stage colours without changing the mode.       |
    | colors must contain at least 1 entry; entries beyond          |
    | dpi_stages are filled with white.                             |
    \*-------------------------------------------------------------*/
    void SetDPIColors(const RGBColor* colors, unsigned int dpi_stages);

    /*-------------------------------------------------------------*\
    | Returns false if libusb could not claim interface 2           |
    | (e.g. duplicate detection of the same physical device).       |
    \*-------------------------------------------------------------*/
    bool IsReady() const { return ready; }

private:
    hid_device*           dev;           /* used only for serial-number query  */
    libusb_context*       usb_ctx;
    libusb_device_handle* usb_handle;    /* full-device handle, interface 2    */
    std::string           location;
    std::string           name;

    bool     ready;                        /* true if libusb claim succeeded      */
    bool     lights_off;                   /* off is emulated as static black     */
    uint8_t  b1[32];                      /* cached DPI colour block             */
    uint8_t  current_mode_sel[8];         /* cached mode selector for TX0        */

    /*-------------------------------------------------------------*\
    | Send one 8-byte selector via EP0 ctrl_transfer to iface 2.   |
    \*-------------------------------------------------------------*/
    void SendSelector(const uint8_t sel[8]);

    /*-------------------------------------------------------------*\
    | Send the full 6-transaction apply sequence.                   |
    | mode_sel: 8-byte mode selector (TX0 position 7).             |
    | b1_data:  32-byte DPI colour block (TX1).                    |
    \*-------------------------------------------------------------*/
    void SendApplySequence(const uint8_t mode_sel[8], const uint8_t b1_data[32]);

    /*-------------------------------------------------------------*\
    | Send a 32-byte interrupt OUT block to EP 0x03.               |
    \*-------------------------------------------------------------*/
    void SendBlock(const uint8_t data[32]);
};

