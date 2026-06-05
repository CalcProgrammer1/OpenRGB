/*---------------------------------------------------------*\
| RealtekARGBController.h                                   |
|                                                           |
|   Controller for Realtek USB ARGB ICs                     |
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
#include <thread>
#include <hidapi.h>

#include "RGBController.h"

#define REALTEK_ARGB_VID                       0x0BDA
#define REALTEK_ARGB_PID                       0x9209
#define REALTEK_ARGB_HID2SCSI_PG               0xFF00
#define REALTEK_ARGB_HID2SCSI_USAGE            0x0001

#define REALTEK_ARGB_NUM_ARGB_GRP              5
#define REALTEK_ARGB_BRINTF_TYPE_HID           1
#define REALTEK_ARGB_SYNC_METHOD_OPENRGB       6
#define REALTEK_ARGB_COLOR_DEPTH               3
#define REALTEK_ARGB_MAX                       400

#define REALTEK_ARGB_CTL_HDR_SIZE              80
#define REALTEK_ARGB_CTL_DATA_SIZE             96

#define REALTEK_ARGB_HID_DATALEN_CH1           4096
#define REALTEK_ARGB_HID_DATALEN_CH2           64
#define REALTEK_ARGB_HID_DATALEN_CH3           512
#define REALTEK_ARGB_HID_DATALEN_CH4           1200
#define REALTEK_ARGB_HID_DATALEN_CH5           2048

#define REALTEK_ARGB_HID_ID_DATA_CH1           0x57
#define REALTEK_ARGB_HID_ID_DATA_CH2           0x59
#define REALTEK_ARGB_HID_ID_DATA_CH3           0x5A
#define REALTEK_ARGB_HID_ID_DATA_CH4           0x5B
#define REALTEK_ARGB_HID_ID_DATA_CH5           0x5C

enum REALTEK_ARGB_LED_CTL
{
    REALTEK_ARGB_LED_CTL_FW,
    REALTEK_ARGB_LED_CTL_APP,
    REALTEK_ARGB_LED_CTL_LAMP,
};

enum REALTEK_ARGB_EFFECT_ID
{
    REALTEK_ARGB_EFF_NULL,
    REALTEK_ARGB_EFF_ALWAYS_ON,
    REALTEK_ARGB_EFF_BLINK,
    REALTEK_ARGB_EFF_BREATH,
    REALTEK_ARGB_EFF_SPECTRUM,
    REALTEK_ARGB_EFF_SCROLL,
    REALTEK_ARGB_EFF_RAINBOW_SCROLL,
    REALTEK_ARGB_EFF_RUNNING_WATER,
    REALTEK_ARGB_EFF_SLIDING,
    REALTEK_ARGB_EFF_NEWTON_CRADLE,
    REALTEK_ARGB_EFF_METEOR,
    REALTEK_ARGB_EFF_RAINBOW_SLIDING,
    REALTEK_ARGB_EFF_RAINBOW_FADE_SLIDING,
    REALTEK_ARGB_EFF_WIDE_SLIDING,
    REALTEK_ARGB_EFF_DOT_MATRIX,                    // reserved: Not supported by OpenRGB
    REALTEK_ARGB_EFF_DOT_MATRIX_BREATH,             // reserved: Not supported by OpenRGB
    REALTEK_ARGB_EFF_ZIGZAG,
    REALTEK_ARGB_EFF_STARRY_NIGHT,
    REALTEK_ARGB_EFF_STACK,
};

enum REALTEK_ARGB_SPEED
{
    REALTEK_ARGB_SPEED_MIN                     = 1,
    REALTEK_ARGB_SPEED_NORMAL                  = 50,
    REALTEK_ARGB_SPEED_MAX                     = 100,
};

enum REALTEK_ARGB_CYCLE_MS
{
    REALTEK_ARGB_CYCLE_MIN                     = 200,
    REALTEK_ARGB_CYCLE_NORMAL                  = 2000,
    REALTEK_ARGB_CYCLE_MAX                     = 10000,
};

enum REALTEK_ARGB_CUST_DEVNAME
{
    REALTEK_ARGB_CUST_DEVNAME_NULL             = 0x0,
    REALTEK_ARGB_CUST_DEVNAME_MANU_UUID        = 0x1,
};

struct RealtekARGBControllerSetEffParam
{
    unsigned int speed;
    unsigned short brightness;
    unsigned char dir;
    unsigned char random_color;
};

struct RealtekARGBControllerFWVersion
{
    unsigned int fw_major_ver;
    unsigned int fw_minor_ver;
    unsigned int fw_extra_ver;
    unsigned int fw_build_ver;
    unsigned int fw_build_date;
};

class RealtekARGBController
{
public:
    RealtekARGBController(hid_device* dev, hid_device_info* info);
    ~RealtekARGBController();

    unsigned char get_support_openrgb();
    std::string get_manu_name();
    std::string get_product_name();
    std::string get_sn();
    std::string get_dev_loc();
    std::string get_fw_ver();
    std::string get_ic_uuid();
    std::string get_dev_name();
    int get_fix_grps();
    bool get_zone_enable(int grp_num);
    int get_argb_num(int grp_num);
    int get_argb_brightness(int grp_num);
    int set_argb_brightness(int grp_num, unsigned short bright);

    int set_argb_direct(int grp_num, std::vector<RGBColor> color_buf, unsigned short brightness);
    int set_argb_effect(int grp_num, uint8_t mode, std::vector<RGBColor> color_buf, struct RealtekARGBControllerSetEffParam* param);
    int set_argb_num(int grp_num, unsigned short new_num);
    int device_reboot();
    void device_rescan_trigger();
private:
    hid_device* hdev;
    std::mutex my_mutex;
    unsigned char argbctl_hdr[REALTEK_ARGB_CTL_HDR_SIZE] = {0};
    unsigned char* argbctl_data[REALTEK_ARGB_NUM_ARGB_GRP] = {0};
    hid_device_info* hidinfo = NULL;
    int appctl[REALTEK_ARGB_NUM_ARGB_GRP] = {REALTEK_ARGB_LED_CTL_FW};
    unsigned short prev_bright[REALTEK_ARGB_NUM_ARGB_GRP] = {0};
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    unsigned char custled[16] = {0};

    void KeepaliveThreadFunction();

    int usb_hid_ioctl(unsigned char* usb_buf, unsigned char* data, int data_len,
                      unsigned int offset, unsigned char is_in);
    int usb_hid_get_report(int data_len, int* id);

    void device_init();
    int set_write_unlock();
    int set_appctl(unsigned char grp_num, unsigned char ctl_sts);
    int set_argbctl_data(unsigned char* data, int data_len, int offset);
    int get_custled(unsigned char* cust, unsigned int cust_len);
    int get_argbctl_hdr();
    int set_argbctl_hdr();
    int get_argbctl_data();
    int set_argbctl_data(unsigned char grp_num);
    int set_eff_id(unsigned char grp_num, unsigned short effid);
    int set_p_color(unsigned char grp_num, RGBColor color);
    int set_s_color(unsigned char grp_num, RGBColor color);
    int set_cycle(unsigned char grp_num, unsigned short cycle);
    int set_ramp(unsigned char grp_num, unsigned short ramp);
    int set_stable(unsigned char grp_num, unsigned short stable);
    int set_subcmd(unsigned char grp_num, int subcmd);
    int set_direct(unsigned char* color, int color_num, unsigned char grp_num);

    int get_flash_argbctl_hdr(unsigned char* data);
    int set_flash_argbctl_hdr(unsigned int offset, unsigned int data_len);

    void device_rescan();
};
