#pragma once

#define CONFIG_HID
#define CONFIG_HID_ALL

#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <vector>

#include "RGBController.h"
#include <QDebug>
#include "platform.h"
#include "bridge_dev.h"
#include "bridge_func.h"
#include "led.h"
#include "led_effect.h"


enum RTK_BRIDGE_SPEED
{
    RTK_BRIDGE_SPEED_MIN = 1,
    RTK_BRIDGE_SPEED_NORMAL = 50,
    RTK_BRIDGE_SPEED_MAX = 100,
};

enum RTK_BRIDGE_CYCLE_MS
{
    RTK_BRIDGE_CYCLE_MIN = 200,
    RTK_BRIDGE_CYCLE_NORMAL = 2000,
    RTK_BRIDGE_CYCLE_MAX = 10000,
};

class RtkBridgeWrapper
{
public:
    RtkBridgeWrapper(dHandle fd, struct led_mem *led_mem_dev, hid_device_info *info);
    ~RtkBridgeWrapper();

    std::string get_manu_name();
    std::string get_product_name();
    std::string get_sn();
    std::string get_dev_loc();
    std::string get_fw_ver();
    int get_argb_num();
    int get_argb_brightness();

    int set_argb_direct(std::vector<RGBColor> color_buf, unsigned short brightness);
    int set_argb_effect(uint8_t mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness);

private:
    dHandle fd = NULL_DHANDLE;
    struct led_mem *led_mem_dev = NULL;
    hid_device_info *hidinfo = NULL;

    int interpolate(int vStart, int vEnd, int pos, int pos_num);
    std::string wcharToString(const wchar_t *wstr);
};
