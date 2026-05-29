/*---------------------------------------------------------*\
| RealtekBridgeController.h                                 |
|                                                           |
|   Controller for Realtek USB to SSD Bridge ICs            |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <hidapi.h>

#include "RGBController.h"

#define REALTEK_BRIDGE_SYNC_METHOD_OPENRGB       6
#define REALTEK_BRIDGE_COLOR_DEPTH               3
#define REALTEK_BRIDGE_CTL_HDR_SIZE              64
#define REALTEK_BRIDGE_CTL_DATA_SIZE             3220
#define REALTEK_BRIDGE_MAX_ARGB_NUM              20
#define REALTEK_BRIDGE_MAX_APPCTL_ROW            43

#define REALTEK_BRIDGE_HID_DATALEN_CH1           4096
#define REALTEK_BRIDGE_HID_DATALEN_CH2           64
#define REALTEK_BRIDGE_HID_DATALEN_CH3           512
#define REALTEK_BRIDGE_HID_DATALEN_CH4           1200
#define REALTEK_BRIDGE_HID_DATALEN_CH5           2048

#define REALTEK_BRIDGE_HID_ID_DATA_CH1           0x57
#define REALTEK_BRIDGE_HID_ID_DATA_CH2           0x59
#define REALTEK_BRIDGE_HID_ID_DATA_CH3           0x5A
#define REALTEK_BRIDGE_HID_ID_DATA_CH4           0x5B
#define REALTEK_BRIDGE_HID_ID_DATA_CH5           0x5C

enum REALTEK_BRIDGE_LED_EFF
{
    REALTEK_BRIDGE_LED_EFF_NONE,
    REALTEK_BRIDGE_LED_EFF_ALWAYS,
    REALTEK_BRIDGE_LED_EFF_BLINK,
    REALTEK_BRIDGE_LED_EFF_BREATHE,
    REALTEK_BRIDGE_LED_EFF_SPECTRUM,
    REALTEK_BRIDGE_LED_EFF_SCROLL,
    REALTEK_BRIDGE_LED_EFF_RAINBOW_SCROLL,
    REALTEK_BRIDGE_LED_EFF_RUNNING_WATER,
    REALTEK_BRIDGE_LED_EFF_SLIDING,
    REALTEK_BRIDGE_LED_EFF_NEWTON_CRADLE,
    REALTEK_BRIDGE_LED_EFF_METEOR,
    REALTEK_BRIDGE_NUMBER_OF_LED_EFF_MODE,
};

enum REALTEK_BRIDGE_SPEED
{
    REALTEK_BRIDGE_SPEED_MIN                     = 1,
    REALTEK_BRIDGE_SPEED_NORMAL                  = 50,
    REALTEK_BRIDGE_SPEED_MAX                     = 100,
};

enum REALTEK_BRIDGE_CYCLE_MS
{
    REALTEK_BRIDGE_CYCLE_MIN                     = 200,
    REALTEK_BRIDGE_CYCLE_NORMAL                  = 2000,
    REALTEK_BRIDGE_CYCLE_MAX                     = 10000,
};

struct RealtekBridgeControllerFWVersion
{
    unsigned int    fw_major_ver;
    unsigned int    fw_minor_ver;
    unsigned int    fw_extra_ver;
    unsigned int    fw_build_ver;
    unsigned int    fw_build_date;
};

class RealtekBridgeController
{
public:
    RealtekBridgeController(hid_device* dev, hid_device_info* info);
    ~RealtekBridgeController();

    unsigned char get_support_openrgb();
    std::string get_manu_name();
    std::string get_product_name();
    std::string get_sn();
    std::string get_dev_loc();
    std::string get_fw_ver();
    int get_argb_num();
    int get_argb_brightness();
    int set_argb_brightness(unsigned short bright);

    int set_argb_direct(std::vector<RGBColor> color_buf, unsigned short brightness);
    int set_argb_effect(unsigned char mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness);

private:
    hid_device*         hdev;
    hid_device_info*    hidinfo = NULL;
    unsigned char       brctl_hdr[REALTEK_BRIDGE_CTL_HDR_SIZE] = {0};
    unsigned char*      brctl_data = NULL;

    int usb_hid_ioctl(unsigned char* usb_buf, unsigned char* data, int data_len,
                      unsigned int offset, unsigned char is_in);
    int usb_hid_get_report(int data_len, int* id);
    int set_write_unlock();
    int get_brctl_hdr();
    int set_brctl_hdr();
    int get_brctl_data();
    int set_brctl_data();
    int set_appctl(unsigned char ctl_sts);
    int set_direct(unsigned char* color, int color_num);
    int eff_set_always_on(RGBColor rgb);
    int eff_set_blink(RGBColor rgb, int cycle);
    int eff_set_breathe(RGBColor rgb, int cycle);
    int eff_set_spectrum(int cycle);
    int eff_set_scroll(RGBColor rgb, int cycle);
    int eff_set_rainbow_scroll(int cycle);
    int eff_set_running_water(RGBColor rgb, int cycle);
    int eff_set_sliding(int cycle);
    int eff_set_newton_cradle(int cycle);
    int eff_set_meteor(RGBColor rgb, int cycle);
};
