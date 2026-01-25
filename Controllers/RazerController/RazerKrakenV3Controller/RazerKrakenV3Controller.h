/*---------------------------------------------------------*\
| RazerKrakenV3Controller.h                                 |
|                                                           |
|   Driver for Razer devices with 13-byte reports           |
|                                                           |
|   Greg Sandstrom (superstrom)                  1 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"
#include "DeviceGuardManager.h"
#include "RazerKrakenController.h"

#define RAZER_KRAKEN_V3_REPORT_ID 0x40

enum
{
    RAZER_KRAKEN_V3_CMD_LIGHTING_SET_MODE       = 0x01,
    RAZER_KRAKEN_V3_CMD_LIGHTING_SET_BRIGHTNESS = 0x02,
    RAZER_KRAKEN_V3_CMD_LIGHTING_SET_COLOR      = 0x03,
};

enum
{
    RAZER_KRAKEN_V3_MODE_ID_DIRECT      = 0x08,
    RAZER_KRAKEN_V3_MODE_ID_WAVE        = 0x03,
};

PACK(struct razer_kraken_v3_request_report
{
    unsigned char report_id;     // usb_buf[0]
    unsigned char command_id;    // usb_buf[1]
    unsigned char arguments[13]; // usb_buf[2...]
});

class RazerKrakenV3Controller
{
public:
    RazerKrakenV3Controller(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerKrakenV3Controller();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();
    unsigned char           GetMaxBrightness();

    void                    SetDirect(RGBColor* colors);
    void                    SetBrightness(unsigned char brightness);

    void                    SetModeDirect();
    void                    SetModeWave();
    void                    SetModeBreathing(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device-specific protocol settings                         |
    \*---------------------------------------------------------*/
    unsigned char           dev_transaction_id;
    unsigned char           dev_led_id;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;

    /*---------------------------------------------------------*\
    | Index of device in Razer device list                      |
    \*---------------------------------------------------------*/
    unsigned int            device_index;

    /*---------------------------------------------------------*\
    | HID report index for request and response                 |
    \*---------------------------------------------------------*/
    unsigned char           report_index;
    unsigned char           response_index;

    /*---------------------------------------------------------*\
    | Matrix type                                               |
    \*---------------------------------------------------------*/
    unsigned char           matrix_type;

    /*---------------------------------------------------------*\
    | Private functions based on OpenRazer                      |
    \*---------------------------------------------------------*/
    std::string             razer_get_firmware();
    std::string             razer_get_serial();

    razer_kraken_v3_request_report razer_kraken_create_v3_report();

    razer_kraken_request_report razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address);
    int razer_usb_receive(razer_kraken_response_report* report);
    int razer_usb_send(razer_kraken_request_report* report);
};
