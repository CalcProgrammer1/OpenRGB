#pragma once

#define CONFIG_HID
#define CONFIG_HID_ALL
#define CONFIG_ARGB_DEV

#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <thread>

#include "RGBController.h"
#include <QDebug>
#include <QThread>
#include "platform.h"
#include "bridge_func.h"
#include "argb_dev.h"
#include "argb_ctl.h"
#include "fwupdate.h"

#define RTK_ARGB_VID                   0x0BDA
#define RTK_ARGB_PID                   0x9209

enum RTK_ARGB_SPEED
{
    RTK_ARGB_SPEED_MIN = 1,
    RTK_ARGB_SPEED_NORMAL = 50,
    RTK_ARGB_SPEED_MAX = 100,
};

enum RTK_ARGB_CYCLE_MS
{
    RTK_ARGB_CYCLE_MIN = 200,
    RTK_ARGB_CYCLE_NORMAL = 2000,
    RTK_ARGB_CYCLE_MAX = 10000,
};

enum RTK_ARGB_CUST_DEVNAME
{
    CUST_DEVNAME_NULL = 0x0,
    CUST_DEVNAME_MANU_UUID = 0x1,
};

class RtkArgbWrapper
{
public:
    RtkArgbWrapper(struct argb_device *dev, argbCtl *argbctl, hid_device_info *info);
    ~RtkArgbWrapper();

    void argb_reinit();
    std::string get_manu_name();
    std::string get_product_name();
    std::string get_sn();
    std::string get_dev_loc();
    std::string get_fw_ver();
    std::string get_ic_uuid();
    std::string get_dev_name();
    int get_fix_grps();
    int get_argb_num(int grp_num);
    int get_argb_brightness(int grp_num);

    int set_argb_num(int grp_num, int new_num);
    int set_argb_direct(int grp_num, std::vector<RGBColor> color_buf, unsigned short brightness);
    int set_argb_effect(int grp_num, uint8_t mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness);

    argbCtl *argb_ctl = NULL;
private:
    struct argb_device *adev;
    std::mutex my_mutex;
    hid_device_info *hidinfo = NULL;
    int appctl[NUM_ARGB_GRP] = {LED_CTL_FW};

    int interpolate(int vStart, int vEnd, int pos, int pos_num);
    std::string wcharToString(const wchar_t *wstr);
};
