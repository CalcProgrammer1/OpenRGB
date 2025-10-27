/*---------------------------------------------------------*\
| RazerKrakenV4Controller.h                                 |
|                                                           |
|   Driver for Razer devices with 64-byte report            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"
#include "DeviceGuardManager.h"

/*---------------------------------------------------------*\
| Struct packing macro for GCC and MSVC                     |
\*---------------------------------------------------------*/
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

PACK(struct razer_kraken_v4_report
{
    unsigned char               report_id;
    unsigned char               status;
    unsigned char               transaction_id;
    unsigned short              remaining_packets;
    unsigned char               protocol_type;
    unsigned char               data_size;
    unsigned char               command_class;
    unsigned char               command_id;
    unsigned char               wireless_flag;
    unsigned char               arguments[52];
    unsigned char               crc;
    unsigned char               reserved;
});

enum
{
    RAZER_KRAKEN_V4_CMD_GET_SERIAL              = 0x00,
    RAZER_KRAKEN_V4_CMD_GET_FIRMWARE_INFO       = 0x02,
    RAZER_KRAKEN_V4_CMD_LIGHTING_SET_MODE       = 0xC0,
    RAZER_KRAKEN_V4_CMD_LIGHTING_SET_BRIGHTNESS = 0xC1,
};

class RazerKrakenV4Controller
{
public:
    RazerKrakenV4Controller(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerKrakenV4Controller();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();
    unsigned char           GetMaxBrightness();

    void                    SetDirect(RGBColor* colors);
    void                    SetBrightness(unsigned char brightness);

    void                    SetModeWave();

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device-specific protocol settings                         |
    \*---------------------------------------------------------*/
    unsigned char           dev_transaction_id;
    unsigned char           dev_led_id;
    unsigned char           dev_wireless_flag;

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
    unsigned char           razer_kraken_v4_calculate_crc(razer_kraken_v4_report* report);
    razer_kraken_v4_report  razer_kraken_v4_create_report(unsigned char command_class, unsigned char command_id, unsigned char data_size);
    razer_kraken_v4_report  razer_kraken_v4_create_response();

    std::string             razer_get_firmware();
    std::string             razer_get_serial();

    int                     razer_usb_receive(razer_kraken_v4_report* report);
    int                     razer_usb_send(razer_kraken_v4_report* report);
};
