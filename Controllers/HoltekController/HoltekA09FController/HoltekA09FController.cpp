/*---------------------------------------------------------*\
| HoltekA09FController.cpp                                  |
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

#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <libusb.h>
#include "HoltekA09FController.h"
#include "StringUtils.h"

/*-----------------------------------------------------------------*\
| TX0 common selectors (positions C1-C6 and C8, 8 bytes each).     |
| The 7th position (index 6) is the variable mode selector.        |
\*-----------------------------------------------------------------*/
static const uint8_t TX0_SEL_COMMON[7][8] =
{
    { 0x27,0x27,0xD5,0xFF,0xEC,0xE5,0x7E,0x76 },   /* C1 */
    { 0x25,0x2D,0xAD,0xFF,0xE8,0xEA,0xEE,0xEE },   /* C2 */
    { 0x27,0x2B,0xD5,0xFF,0xF0,0xED,0x7E,0x76 },   /* C3 */
    { 0x27,0x2B,0xDD,0xFF,0xF0,0xD5,0x7E,0x76 },   /* C4 */
    { 0x27,0x2B,0x75,0xFF,0xC8,0x25,0x7E,0x76 },   /* C5 */
    { 0x27,0x27,0xD5,0x00,0xD8,0x40,0xB1,0xAE },   /* C6 */
    { 0x27,0x2A,0x8D,0xFF,0xE8,0x5D,0x7E,0x36 },   /* C8 */
};

/*-----------------------------------------------------------------*\
| Per-transaction block selectors (TX1-TX5).                        |
\*-----------------------------------------------------------------*/
static const uint8_t SEL_B1[8] = { 0x27,0x2A,0x85,0xFF,0xF0,0x5D,0x7E,0x36 };
static const uint8_t SEL_B2[8] = { 0x27,0x2B,0xF5,0xFF,0xD8,0x55,0x7E,0xB6 };
static const uint8_t SEL_B3[8] = { 0x27,0x2D,0x55,0xFF,0xF0,0x6D,0x80,0x76 };
static const uint8_t SEL_B5[8] = { 0x27,0x2D,0x2D,0xFF,0xF8,0x6D,0x80,0x76 };
static const uint8_t SEL_B7[8] = { 0x27,0x2B,0xF5,0xFF,0x00,0x5D,0x7E,0xD6 };

/*-----------------------------------------------------------------*\
| Fixed data blocks (from captures, unchanged across all modes).    |
\*-----------------------------------------------------------------*/
static const uint8_t BLK_B0[32] =
{
    0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0x00,0xFF,
    0x00,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0xFF,
    0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00
};
static const uint8_t BLK_B2[32] =
{
    0x08,0x10,0x18,0x30,0x60,0x7E,0x7E,0x78,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const uint8_t BLK_B3[32] =
{
    0x01,0x00,0xF0,0x00,0x01,0x00,0xF1,0x00,
    0x01,0x00,0xF2,0x00,0x01,0x00,0xF4,0x00,
    0x01,0x00,0xF3,0x00,0x07,0x00,0x01,0x00,
    0x07,0x00,0x02,0x00,0x0B,0x00,0x00,0x00
};
static const uint8_t BLK_B4[32] =
{
    0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x04,0x00,0x01,0x00,0x04,0x00,0x02,0x00
};
static const uint8_t BLK_B5[32] =
{
    0x0C,0x00,0x00,0x00,0x01,0x00,0xF1,0x00,
    0x01,0x00,0xF2,0x00,0x01,0x00,0xF4,0x00,
    0x01,0x00,0xF3,0x00,0x07,0x00,0x01,0x00,
    0x07,0x00,0x02,0x00,0x0B,0x00,0x00,0x00
};
static const uint8_t BLK_B7[32] =
{
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*-----------------------------------------------------------------*\
| Default B1 (Standard mode, default DPI stage colours).           |
\*-----------------------------------------------------------------*/
static const uint8_t DEFAULT_B1[32] =
{
    0xFF,0x00,0x00,0xFF,0x80,0x00,0xFF,0xFF,
    0x00,0x66,0x67,0xAA,0x00,0xFF,0x00,0x00,
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x55,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*-----------------------------------------------------------------*\
| Mode selector table (indexed by HOLTEK_A09F_MODE_VALUE_*)         |
\*-----------------------------------------------------------------*/
static const uint8_t MODE_SELECTORS[12][8] =
{
    HOLTEK_A09F_MODE_OFF,
    HOLTEK_A09F_MODE_STANDARD,
    HOLTEK_A09F_MODE_BREATHING,
    HOLTEK_A09F_MODE_NEON,
    HOLTEK_A09F_MODE_WAVE,
    HOLTEK_A09F_MODE_KEY_REACTION,
    HOLTEK_A09F_MODE_TRAILING,
    HOLTEK_A09F_MODE_DRAG,
    HOLTEK_A09F_MODE_SLIDE,
    HOLTEK_A09F_MODE_YO_YO,
    HOLTEK_A09F_MODE_MARBLES,
    HOLTEK_A09F_MODE_FLYING_STAR,
};

/*-----------------------------------------------------------------*\
| Sysfs helper: resolve USB bus/device numbers from a hidraw path. |
\*-----------------------------------------------------------------*/
static bool GetUSBBusDevice(const std::string& hidraw_path, uint8_t& bus_out, uint8_t& dev_out)
{
    std::string devname = hidraw_path.substr(hidraw_path.rfind('/') + 1);
    std::string sysfs   = "/sys/class/hidraw/" + devname + "/device";

    char real_path[PATH_MAX];
    if(!realpath(sysfs.c_str(), real_path))
        return false;

    /* The USB device dir is two levels above the HID device dir */
    std::string usb_base = std::string(real_path) + "/../../";

    auto read_int = [](const std::string& p, int& v) -> bool
    {
        FILE* f = fopen(p.c_str(), "r");
        if(!f) return false;
        bool ok = (fscanf(f, "%d", &v) == 1);
        fclose(f);
        return ok;
    };

    int bus = 0, devnum = 0;
    if(!read_int(usb_base + "busnum",  bus))    return false;
    if(!read_int(usb_base + "devnum",  devnum)) return false;

    bus_out = static_cast<uint8_t>(bus);
    dev_out = static_cast<uint8_t>(devnum);
    return true;
}

/*=================================================================*\
| Constructor / destructor                                          |
\*=================================================================*/

HoltekA09FController::HoltekA09FController(hid_device* dev_handle,
                                           const char* path,
                                           std::string dev_name)
{
    dev      = dev_handle;
    location = path;
    name     = dev_name;
    usb_ctx    = nullptr;
    usb_handle = nullptr;
    ready      = false;
    lights_off = false;

    memcpy(b1,                DEFAULT_B1,                         32);
    memcpy(current_mode_sel,  MODE_SELECTORS[HOLTEK_A09F_MODE_VALUE_STANDARD], 8);

    /*-------------------------------------------------------------*\
    | Open the device via libusb, detach any kernel driver from    |
    | interface 2, and claim it for exclusive use.                  |
    \*-------------------------------------------------------------*/
    libusb_init(&usb_ctx);

    uint8_t bus_num = 0, dev_num = 0;
    if(GetUSBBusDevice(path, bus_num, dev_num))
    {
        libusb_device** list = nullptr;
        ssize_t count = libusb_get_device_list(usb_ctx, &list);
        for(ssize_t i = 0; i < count; i++)
        {
            if(libusb_get_bus_number(list[i])     == bus_num &&
               libusb_get_device_address(list[i]) == dev_num)
            {
                libusb_open(list[i], &usb_handle);
                break;
            }
        }
        libusb_free_device_list(list, 1);
    }

    if(usb_handle)
    {
        if(libusb_kernel_driver_active(usb_handle, 2) == 1)
            libusb_detach_kernel_driver(usb_handle, 2);

        int rc = libusb_claim_interface(usb_handle, 2);
        if(rc == LIBUSB_SUCCESS)
        {
            ready = true;
        }
        else
        {
            /* Interface already claimed by a previous instance (duplicate HID
             * collection entry for the same physical device). Close and bail. */
            libusb_close(usb_handle);
            usb_handle = nullptr;
        }
    }
}

HoltekA09FController::~HoltekA09FController()
{
    if(usb_handle)
    {
        libusb_release_interface(usb_handle, 2);
        libusb_attach_kernel_driver(usb_handle, 2);
        libusb_close(usb_handle);
    }
    if(usb_ctx)
        libusb_exit(usb_ctx);

    hid_close(dev);
}

/*=================================================================*\
| Accessors                                                         |
\*=================================================================*/

std::string HoltekA09FController::GetDeviceLocation()
{
    return "HID: " + location;
}

std::string HoltekA09FController::GetNameString()
{
    return name;
}

std::string HoltekA09FController::GetSerialString()
{
    wchar_t serial_string[128];
    if(hid_get_serial_number_string(dev, serial_string, 128) != 0)
        return "";
    return StringUtils::wstring_to_string(serial_string);
}

/*=================================================================*\
| Private helpers                                                   |
\*=================================================================*/

void HoltekA09FController::SendSelector(const uint8_t sel[8])
{
    if(!usb_handle) return;

    uint8_t buf[8];
    memcpy(buf, sel, 8);

    /*  bmRequestType = 0x21 (Host->Device, Class, Interface)      *\
    |   bRequest      = 0x09 (SET_REPORT)                          |
    |   wValue        = 0x0300 (Feature report, ID=0)               |
    |   wIndex        = 2     (interface 2)                         |
    |   wLength       = 8                                           |
    \*-------------------------------------------------------------*/
    libusb_control_transfer(usb_handle,
                            0x21, 0x09, 0x0300, 2,
                            buf, 8,
                            2000);
}

void HoltekA09FController::SendBlock(const uint8_t data[32])
{
    if(!usb_handle) return;

    int transferred = 0;
    libusb_interrupt_transfer(usb_handle, 0x03,
                              const_cast<uint8_t*>(data), 32,
                              &transferred, 2000);
}

void HoltekA09FController::SendApplySequence(const uint8_t mode_sel[8],
                                              const uint8_t b1_data[32])
{
    /* TX0: common C1-C6 + mode selector + common C8  →  B0 */
    for(int i = 0; i < 6; i++)
        SendSelector(TX0_SEL_COMMON[i]);
    SendSelector(mode_sel);
    SendSelector(TX0_SEL_COMMON[6]);
    SendBlock(BLK_B0);

    /* TX1: SEL_B1  →  B1 (DPI colour table) */
    SendSelector(SEL_B1);
    SendBlock(b1_data);

    /* TX2: SEL_B2  →  B2 */
    SendSelector(SEL_B2);
    SendBlock(BLK_B2);

    /* TX3: SEL_B3  →  B3 + B4 */
    SendSelector(SEL_B3);
    SendBlock(BLK_B3);
    SendBlock(BLK_B4);

    /* TX4: SEL_B5  →  B5 + B4 */
    SendSelector(SEL_B5);
    SendBlock(BLK_B5);
    SendBlock(BLK_B4);

    /* TX5: SEL_B7  →  B7 */
    SendSelector(SEL_B7);
    SendBlock(BLK_B7);
}

/*=================================================================*\
| Public API                                                        |
\*=================================================================*/

void HoltekA09FController::SetMode(uint8_t mode_value)
{
    if(mode_value >= 12)
        return;

    if(mode_value == HOLTEK_A09F_MODE_VALUE_OFF)
    {
        uint8_t off_b1[32] = { 0x00 };

        lights_off = true;
        SendApplySequence(MODE_SELECTORS[HOLTEK_A09F_MODE_VALUE_STANDARD], off_b1);
        return;
    }

    lights_off = false;
    memcpy(current_mode_sel, MODE_SELECTORS[mode_value], 8);
    SendApplySequence(current_mode_sel, b1);
}

void HoltekA09FController::SetDPIColors(const RGBColor* colors, unsigned int dpi_stages)
{
    /*-------------------------------------------------------------*\
    | B1 layout (32 bytes):                                        |
    |   bytes  0- 2  = RGB stage 1                                  |
    |   bytes  3- 5  = RGB stage 2                                  |
    |   bytes  6- 8  = RGB stage 3                                  |
    |   bytes  9-11  = RGB stage 4                                  |
    |   bytes 12-14  = RGB stage 5                                  |
    |   bytes 15-17  = RGB stage 6                                  |
    |   bytes 18-20  = RGB stage 7                                  |
    |   bytes 21-23  = RGB stage 8                                  |
    |   bytes 24-31  = 0x00 (stable)                                |
    \*-------------------------------------------------------------*/
    if(dpi_stages > HOLTEK_A09F_DPI_STAGES)
        dpi_stages = HOLTEK_A09F_DPI_STAGES;

    for(unsigned int i = 0; i < HOLTEK_A09F_DPI_STAGES; i++)
    {
        unsigned int base = i * 3;
        if(i < dpi_stages)
        {
            b1[base + 0] = RGBGetRValue(colors[i]);
            b1[base + 1] = RGBGetGValue(colors[i]);
            b1[base + 2] = RGBGetBValue(colors[i]);
        }
        else
        {
            b1[base + 0] = 0xFF;
            b1[base + 1] = 0xFF;
            b1[base + 2] = 0xFF;
        }
    }
    memset(b1 + 24, 0x00, 8);

    if(lights_off)
    {
        uint8_t off_b1[32] = { 0x00 };
        SendApplySequence(MODE_SELECTORS[HOLTEK_A09F_MODE_VALUE_STANDARD], off_b1);
        return;
    }

    SendApplySequence(current_mode_sel, b1);
}

