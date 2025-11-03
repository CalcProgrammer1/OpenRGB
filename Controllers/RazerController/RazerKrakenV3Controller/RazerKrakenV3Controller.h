/*---------------------------------------------------------*\
| RazerKrakenV3Controller.h                                 |
|                                                           |
|   Driver for Razer devices with 9-byte report             |
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

    razer_kraken_request_report razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address);
    int razer_usb_receive(razer_kraken_response_report* report);
    int razer_usb_send(razer_kraken_request_report* report);
};
