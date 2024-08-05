/*---------------------------------------------------------*\
| RazerKrakenController.h                                   |
|                                                           |
|   Driver for Razer Kraken                                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                28 Feb 2021 |
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

union razer_kraken_effect_byte
{
    unsigned char value;

    struct razer_kraken_effect_byte_bits
    {
        unsigned char on_off_static :1;
        unsigned char single_colour_breathing :1;
        unsigned char spectrum_cycling :1;
        unsigned char sync :1;
        unsigned char two_colour_breathing :1;
        unsigned char three_colour_breathing :1;
    } bits;
};

/*---------------------------------------------------------*\
| Razer Kraken Report Types (taken from OpenRazer)          |
\*---------------------------------------------------------*/
PACK(struct razer_kraken_request_report
{
    unsigned char report_id;
    unsigned char destination;
    unsigned char length;
    unsigned char addr_h;
    unsigned char addr_l;
    unsigned char arguments[32];
});

PACK(struct razer_kraken_response_report
{
    unsigned char report_id;
    unsigned char arguments[36];
});

class RazerKrakenController
{
public:
    RazerKrakenController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerKrakenController();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();

    void                    SetModeBreathingOneColor(unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetModeBreathingTwoColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2);
    void                    SetModeBreathingThreeColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, unsigned char r3, unsigned char g3, unsigned char b3);
    void                    SetModeCustom(unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetModeOff();
    void                    SetModeSpectrumCycle();
    void                    SetModeStatic(unsigned char red, unsigned char grn, unsigned char blu);

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;

    /*---------------------------------------------------------*\
    | Kraken LED/Mode Addresses                                 |
    \*---------------------------------------------------------*/
    unsigned short          breathing_address[3];
    unsigned short          custom_address;
    unsigned short          led_mode_address;

    /*---------------------------------------------------------*\
    | Index of device in Razer device list                      |
    \*---------------------------------------------------------*/
    unsigned int            device_index;

    /*---------------------------------------------------------*\
    | Private functions based on OpenRazer                      |
    \*---------------------------------------------------------*/
    razer_kraken_request_report razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address);
    razer_kraken_effect_byte    razer_kraken_create_effect_byte();

    std::string                 razer_get_firmware();
    std::string                 razer_get_serial();

    void                        razer_set_mode_breathing_one_color(unsigned char red, unsigned char grn, unsigned char blu);
    void                        razer_set_mode_breathing_two_colors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2);
    void                        razer_set_mode_breathing_three_colors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, unsigned char r3, unsigned char g3, unsigned char b3);
    void                        razer_set_mode_custom(unsigned char red, unsigned char grn, unsigned char blu);
    void                        razer_set_mode_none();
    void                        razer_set_mode_spectrum_cycle();
    void                        razer_set_mode_static(unsigned char red, unsigned char grn, unsigned char blu);

    int                         razer_usb_receive(razer_kraken_response_report* report);
    int                         razer_usb_send(razer_kraken_request_report* report);


};
