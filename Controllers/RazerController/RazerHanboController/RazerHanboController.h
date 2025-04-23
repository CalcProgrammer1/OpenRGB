/*---------------------------------------------------------*\
| RazerHanboController.h                                    |
|                                                           |
|   Driver for Razer Hanbo devices                          |
|                                                           |
|   Joseph East (dripsnek)                      12 Apr 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*---------------------------------------------------------*\
| Struct packing macro for GCC and MSVC                     |
\*---------------------------------------------------------*/
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

/*---------------------------------------------------------*\
| The Hanbo does not advertise HID report IDs.              |
| In this case under Windows, payloads sent to hidapi must  |
| be prefixed with a byte of value 0 even if this exceeds   |
| the device report length. This dummy byte never ends up   |
| on the wire. Hidapi via libusb under Linux is compatible  |
| with this behavior. This is generally not compatible with |
| hidraw when using it directly.                            |
|                                                           |
| Request payloads start at header[1] to make this byte.    |
| Responses are unaffected and start at header[0].          |
\*---------------------------------------------------------*/
PACK(struct razer_hanbo_report
{
    unsigned char header[2];
    unsigned char arguments[63];
});

enum
{
    MIN_BRIGHTNESS          = 0x00,
    MAX_BRIGHTNESS          = 0x64,
};

enum
{
    PUMP,
    FAN1,
    FAN2,
    FAN3
};

class RazerHanboController
{
public:
    RazerHanboController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerHanboController();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetSerialString();
    std::string             GetName();
    void                    SetDirectMode();
    void                    SetModeOff();
    void                    SetModeSpectrumCycle();
    void                    SetZoneLeds(int zone_idc, const zone& input_zone);
    void                    SetBrightness(int zone, unsigned int brightness);

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             serial_string;
    std::string             location;
    std::string             name;

    /*---------------------------------------------------------*\
    | Index of device in Razer device list                      |
    \*---------------------------------------------------------*/
    unsigned int            device_index;

    /*---------------------------------------------------------*\
    | Private functions                                         |
    \*---------------------------------------------------------*/
    razer_hanbo_report      razer_hanbo_create_report(unsigned char header);
    void                    GetFirmware();
    int                     UsbReceive(razer_hanbo_report* report);
    int                     UsbSend(razer_hanbo_report* report);
};
