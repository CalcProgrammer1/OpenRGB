/*---------------------------------------------------------*\
| RealtekARGBController.cpp                                 |
|                                                           |
|   Controller for Realtek USB ARGB ICs                     |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RealtekARGBController.h"
#include "MathUtils.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

static const unsigned char hid_set_packet[] =
{
    0x56, 0x53, 0x42, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xE3,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char hid_get_packet[] =
{
    0x56, 0x53, 0x42, 0x43, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x10, 0xE2,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char hid_end_packet[] =
{
    0x58, 0x53, 0x42, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

RealtekARGBController::RealtekARGBController(hid_device* dev, hid_device_info* info)
{
    hdev = dev;
    hidinfo = info;
    for(int i = 0; i < REALTEK_ARGB_NUM_ARGB_GRP; i++)
    {
        argbctl_data[i] = (unsigned char*)calloc(REALTEK_ARGB_CTL_DATA_SIZE, 1);
        memset(argbctl_data[i], 0, REALTEK_ARGB_CTL_DATA_SIZE);
        prev_bright[i] = 0xFFFF;
    }
    device_init();

    keepalive_thread_run = false;
    keepalive_thread     = NULL;
}

RealtekARGBController::~RealtekARGBController()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    if(keepalive_thread != NULL)
    {
        keepalive_thread_run = false;
        keepalive_thread->join();
        delete keepalive_thread;
    }

    if(hdev)
    {
        for(int i = 0; i < REALTEK_ARGB_NUM_ARGB_GRP; i++)
        {
            int ret = set_appctl(i, REALTEK_ARGB_LED_CTL_FW);
            if(!ret)
            {
                appctl[i] = REALTEK_ARGB_LED_CTL_FW;
            }
        }
        hid_close(hdev);
        hdev = NULL;
    }
    memset(argbctl_hdr, 0, REALTEK_ARGB_CTL_HDR_SIZE);
    for(int i = 0; i < REALTEK_ARGB_NUM_ARGB_GRP; i++)
        free(argbctl_data[i]);
}

void RealtekARGBController::KeepaliveThreadFunction()
{
    /*-----------------------------------------------------------------*\
    | One shot thread to rescan device                                  |
    \*-----------------------------------------------------------------*/
    while(keepalive_thread_run.load())
    {
        std::this_thread::sleep_for(2s);
        device_rescan();
        keepalive_thread_run = false;
    }
}

int RealtekARGBController::usb_hid_ioctl(unsigned char* usb_buf, unsigned char* data, int data_len,
                                         unsigned int offset, unsigned char is_in)
{
    int id;
    int ret = 0;
    int buf_len = usb_hid_get_report(data_len, &id);

    if(hdev == NULL)
    {
        free(usb_buf);
        return -1;
    }

    if(!is_in && data_len)
    {
        usb_buf[0x04] = data[0];
    }
    memcpy(&usb_buf[0x08], &data_len, sizeof(data_len));
    memcpy(&usb_buf[0x1B], &data_len, sizeof(data_len));
    ret = hid_send_feature_report(hdev, usb_buf, sizeof(hid_get_packet));

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, data, data_len);
    usb_buf[0x00] = id;
    if(is_in)
    {
        ret = hid_get_feature_report(hdev, usb_buf, buf_len);
        memcpy(data, usb_buf + offset, data_len);
    }
    else
    {
        ret = hid_send_feature_report(hdev, usb_buf, buf_len);
    }

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_end_packet, sizeof(hid_end_packet));
    ret = hid_get_feature_report(hdev, usb_buf, sizeof(hid_end_packet));
    free(usb_buf);
    return (ret > 0) ? 0 : ret;
}

int RealtekARGBController::usb_hid_get_report(int data_len, int* id)
{
    int retlen = 0;

    if(data_len <= REALTEK_ARGB_HID_DATALEN_CH2)
    {
        *id = REALTEK_ARGB_HID_ID_DATA_CH2;
        retlen = REALTEK_ARGB_HID_DATALEN_CH2;
    }
    else if(data_len <= REALTEK_ARGB_HID_DATALEN_CH3)
    {
        *id = REALTEK_ARGB_HID_ID_DATA_CH3;
        retlen = REALTEK_ARGB_HID_DATALEN_CH3;
    }
    else if(data_len <= REALTEK_ARGB_HID_DATALEN_CH4)
    {
        *id = REALTEK_ARGB_HID_ID_DATA_CH4;
        retlen = REALTEK_ARGB_HID_DATALEN_CH4;
    }
    else if(data_len <= REALTEK_ARGB_HID_DATALEN_CH5)
    {
        *id = REALTEK_ARGB_HID_ID_DATA_CH5;
        retlen = REALTEK_ARGB_HID_DATALEN_CH5;
    }
    else
    {
        *id = REALTEK_ARGB_HID_ID_DATA_CH1;
        retlen = REALTEK_ARGB_HID_DATALEN_CH1;
    }
    return retlen;
}

void RealtekARGBController::device_init()
{
    set_write_unlock();
    get_argbctl_hdr();
    get_argbctl_data();
    for(int i = 0; i < REALTEK_ARGB_NUM_ARGB_GRP; i++)
    {
        int ret = set_appctl(i, REALTEK_ARGB_LED_CTL_FW);
        if(!ret)
        {
            appctl[i] = REALTEK_ARGB_LED_CTL_FW;
        }
    }
}

int RealtekARGBController::set_write_unlock()
{
    int ret = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    int data_len = 96;
    unsigned int addr = 0xAC004000;
    unsigned char* data = (unsigned char*)calloc(data_len, 1);
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0x92;
    memcpy(&usb_buf[0x17], &addr, sizeof(addr));
    ret = usb_hid_ioctl(usb_buf, data, data_len, 0, true);
    free(data);
    return ret;
}

unsigned char RealtekARGBController::get_support_openrgb()
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char is_support = 0;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xCC;
    usb_buf[0x14]   = 0x04;
    usb_buf[0x15]   = REALTEK_ARGB_SYNC_METHOD_OPENRGB;
    if(usb_hid_ioctl(usb_buf, &is_support, sizeof(is_support), 0, true))
    {
        is_support = 0;
    }
    return is_support;
}

std::string RealtekARGBController::get_manu_name()
{
    return StringUtils::wchar_to_char(hidinfo->manufacturer_string);
}

std::string RealtekARGBController::get_product_name()
{
    return StringUtils::wchar_to_char(hidinfo->product_string);
}

std::string RealtekARGBController::get_sn()
{
    return StringUtils::wchar_to_char(hidinfo->serial_number);
}

std::string RealtekARGBController::get_dev_loc()
{
    return hidinfo->path;
}

std::string RealtekARGBController::get_fw_ver()
{
    std::string ver = "";
    struct RealtekARGBControllerFWVersion fw_ver;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xA5;
    if(!usb_hid_ioctl(usb_buf, (unsigned char*)&fw_ver, sizeof(fw_ver), 0, true))
    {
        ver += std::to_string(fw_ver.fw_major_ver) + "." +
               std::to_string(fw_ver.fw_minor_ver) + "." +
               std::to_string(fw_ver.fw_extra_ver) + "." +
               std::to_string(fw_ver.fw_build_date);
    }
    return ver;
}

std::string RealtekARGBController::get_ic_uuid()
{
    unsigned int uuid = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xC3;
    usb_hid_ioctl(usb_buf, (unsigned char*)&uuid, sizeof(uuid), 0, true);

    return StringUtils::u32int_to_hexString(uuid);
}

std::string RealtekARGBController::get_dev_name()
{
    bool got_custled = false;
    std::string devname = get_product_name();

    if(custled[0])
    {
        got_custled = true;
    }
    else
    {
        if(!get_custled(custled, sizeof(custled)))
        {
            got_custled = true;
        }
    }

    if(got_custled)
    {
        if(custled[6] == REALTEK_ARGB_CUST_DEVNAME_MANU_UUID)
        {
            devname = get_manu_name() + get_ic_uuid();
        }
    }
    return devname;
}

int RealtekARGBController::get_fix_grps()
{
    bool got_custled = false;
    static int fix_grps = 0;

    if(custled[0])
    {
        got_custled = true;
    }
    else
    {
        if(!get_custled(custled, sizeof(custled)))
        {
            got_custled = true;
        }
    }

    if(got_custled)
    {
        fix_grps = custled[4];
    }
    return fix_grps;
}

bool RealtekARGBController::get_zone_enable(int grp_num)
{
    bool got_custled = false;
    bool en = false;

    if(custled[0])
    {
        got_custled = true;
    }
    else
    {
        if(!get_custled(custled, sizeof(custled)))
        {
            got_custled = true;
        }
    }

    if(got_custled)
    {
        en = (custled[5] & (0x1 << grp_num)) ? true : false;
    }
    return en;
}

int RealtekARGBController::get_argb_num(int grp_num)
{
    int num_rgb = 0;
    memcpy(&num_rgb, &argbctl_hdr[32 + grp_num * 2], 2);

    return num_rgb;
}

int RealtekARGBController::get_argb_brightness(int grp_num)
{
    int bright = 0;
    memcpy(&bright, &argbctl_hdr[42 + grp_num * 2], 2);

    return bright;
}

int RealtekARGBController::set_argb_brightness(int grp_num, unsigned short bright)
{
    memcpy(&argbctl_hdr[42 + grp_num * sizeof(bright)], &bright, sizeof(bright));
    set_argbctl_hdr();
    return 0;
}

int RealtekARGBController::set_appctl(unsigned char grp_num, unsigned char ctl_sts)
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x01;
    usb_buf[0x15]   = ctl_sts;
    usb_buf[0x16]   = grp_num;
    return usb_hid_ioctl(usb_buf, &ctl_sts, sizeof(ctl_sts), 0, false);
}

int RealtekARGBController::get_custled(unsigned char* cust, unsigned int cust_len)
{
    int ret = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned int addr = 0xAC004000;
    unsigned int offset = 0x800;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len + offset, 1);// will release in usb_hid_ioctl function
    unsigned char* data = (unsigned char*)calloc(buf_len, 1);

    memset(data, 0x00, buf_len);
    memset(usb_buf, 0x00, buf_len + offset);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0x92;
    memcpy(&usb_buf[0x17], &addr, sizeof(addr));
    ret = usb_hid_ioctl(usb_buf, data, buf_len, offset, true);
    if(!ret)
    {
        memcpy(cust, &data[70], cust_len);
    }
    free(data);
    return ret;
}

int RealtekARGBController::get_argbctl_hdr()
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xCC;
    usb_buf[0x14]   = 0x02;
    return usb_hid_ioctl(usb_buf, argbctl_hdr, REALTEK_ARGB_CTL_HDR_SIZE, 0, true);
}

int RealtekARGBController::set_argbctl_hdr()
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x02;
    return usb_hid_ioctl(usb_buf, argbctl_hdr, REALTEK_ARGB_CTL_HDR_SIZE, 0, false);
}

int RealtekARGBController::get_argbctl_data()
{
    int ret = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    int offset = REALTEK_ARGB_CTL_HDR_SIZE;
    unsigned char* usb_buf;

    for(int i = 0; i < REALTEK_ARGB_NUM_ARGB_GRP; i++)
    {
        usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function
        memset(usb_buf, 0x00, buf_len);
        memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
        usb_buf[0x13]   = 0xCC;
        usb_buf[0x14]   = 0x02;
        memcpy(&usb_buf[0x17], &offset, sizeof(offset));
        ret = usb_hid_ioctl(usb_buf, argbctl_data[i], REALTEK_ARGB_CTL_DATA_SIZE, 0, true);
        offset += REALTEK_ARGB_CTL_DATA_SIZE;
    }
    return ret;
}

int RealtekARGBController::set_argbctl_data(unsigned char grp_num)
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    int offset = REALTEK_ARGB_CTL_HDR_SIZE + REALTEK_ARGB_CTL_DATA_SIZE * grp_num;
    unsigned char* usb_buf;

    usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function
    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x02;
    memcpy(&usb_buf[0x17], &offset, sizeof(offset));
    return usb_hid_ioctl(usb_buf, argbctl_data[grp_num], REALTEK_ARGB_CTL_DATA_SIZE, 0, false);
}

int RealtekARGBController::set_eff_id(unsigned char grp_num, unsigned short effid)
{
    memcpy(argbctl_data[grp_num], &effid, sizeof(effid));
    return 0;
}

int RealtekARGBController::set_p_color(unsigned char grp_num, RGBColor color)
{
    memcpy(argbctl_data[grp_num] + 8, &color, sizeof(color));
    return 0;
}

int RealtekARGBController::set_s_color(unsigned char grp_num, RGBColor color)
{
    memcpy(argbctl_data[grp_num] + 12, &color, sizeof(color));
    return 0;
}

int RealtekARGBController::set_cycle(unsigned char grp_num, unsigned short cycle)
{
    memcpy(argbctl_data[grp_num] + 2, &cycle, sizeof(cycle));
    return 0;
}

int RealtekARGBController::set_ramp(unsigned char grp_num, unsigned short ramp)
{
    memcpy(argbctl_data[grp_num] + 4, &ramp, sizeof(ramp));
    return 0;
}

int RealtekARGBController::set_stable(unsigned char grp_num, unsigned short stable)
{
    memcpy(argbctl_data[grp_num] + 6, &stable, sizeof(stable));
    return 0;
}

int RealtekARGBController::set_subcmd(unsigned char grp_num, int subcmd)
{
    memcpy(argbctl_data[grp_num] + 16, &subcmd, sizeof(subcmd));
    return 0;
}

int RealtekARGBController::set_direct(unsigned char* color, int color_num, unsigned char grp_num)
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    int data_len = color_num * REALTEK_ARGB_COLOR_DEPTH;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x03;
    usb_buf[0x15]   = REALTEK_ARGB_SYNC_METHOD_OPENRGB;
    usb_buf[0x16]   = grp_num;
    memcpy(&usb_buf[0x17], &color_num, sizeof(color_num));
    return usb_hid_ioctl(usb_buf, color, data_len, 0, false);
}

int RealtekARGBController::get_flash_argbctl_hdr(unsigned char* data)
{
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned int addr = 0xAC000000;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0x92;
    memcpy(&usb_buf[0x17], &addr, sizeof(addr));
    return usb_hid_ioctl(usb_buf, data, buf_len, 0, true);
}

int RealtekARGBController::set_flash_argbctl_hdr(unsigned int offset, unsigned int data_len)
{
    int ret = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    unsigned int addr = 0xAC000000;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function
    unsigned char* data = (unsigned char*)calloc(buf_len, 1);
    memset(data, 0, buf_len);

    ret = get_flash_argbctl_hdr(data);
    if(!ret)
    {
        memset(usb_buf, 0x00, buf_len);
        memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
        usb_buf[0x13]   = 0x10;
        memcpy(&usb_buf[0x17], &addr, sizeof(addr));
        ret = usb_hid_ioctl(usb_buf, (unsigned char*)&addr, sizeof(addr), 0, false);

        usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function
        memset(usb_buf, 0x00, buf_len);
        memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
        usb_buf[0x13]   = 0x11;
        memcpy(data + offset, argbctl_hdr + offset, data_len);
        ret = usb_hid_ioctl(usb_buf, data, buf_len, 0, false);
    }
    free(data);
    return ret;
}

int RealtekARGBController::set_argb_direct(int grp_num, std::vector<RGBColor> color_buf, unsigned short brightness)
{
    int ret = -1;
    size_t color_num = color_buf.size();
    size_t buf_len = color_num * REALTEK_ARGB_COLOR_DEPTH;
    unsigned char* buf;
    std::lock_guard<std::mutex> lock(my_mutex);

    if(color_num == 0)
    {
        goto exit;
    }

    if(appctl[grp_num] != REALTEK_ARGB_LED_CTL_APP)
    {
        ret = set_appctl(grp_num, REALTEK_ARGB_LED_CTL_APP);
        if(ret)
        {
            goto exit;
        }
        else
        {
            appctl[grp_num] = REALTEK_ARGB_LED_CTL_APP;
        }
    }

    if(prev_bright[grp_num] != brightness)
    {
        prev_bright[grp_num] = brightness;
        ret = set_argb_brightness(grp_num, brightness << 8);
        if(ret)
        {
            goto exit;
        }
    }

    buf = (uint8_t*)malloc(buf_len);
    memset(buf, 0, buf_len);
    for(size_t i = 0; i < color_num; i++)
    {
        buf[i * REALTEK_ARGB_COLOR_DEPTH + 0] = RGBGetRValue(color_buf[i]);
        buf[i * REALTEK_ARGB_COLOR_DEPTH + 1] = RGBGetGValue(color_buf[i]);
        buf[i * REALTEK_ARGB_COLOR_DEPTH + 2] = RGBGetBValue(color_buf[i]);
    }
    ret = set_direct(buf, (int)color_num, grp_num);
    free(buf);
exit:
    return ret;
}

int RealtekARGBController::set_argb_effect(int grp_num, uint8_t mode, std::vector<RGBColor> color_buf, struct RealtekARGBControllerSetEffParam* param)
{
    int ret = -1;
    int cycle = MathUtils::IntInterpolate(REALTEK_ARGB_CYCLE_MAX, REALTEK_ARGB_CYCLE_MIN, 0, REALTEK_ARGB_SPEED_MAX, param->speed);
    std::lock_guard<std::mutex> lock(my_mutex);

    if(mode == REALTEK_ARGB_EFF_ALWAYS_ON || mode == REALTEK_ARGB_EFF_STACK)
    {
        cycle = 0;
    }

    if(color_buf.size() >= 1)
    {
        set_p_color(grp_num, color_buf[0]);
    }
    if(color_buf.size() >= 2)
    {
        set_s_color(grp_num, color_buf[1]);
    }

    set_eff_id(grp_num, mode);
    set_cycle(grp_num, cycle);
    set_ramp(grp_num, 0);
    set_stable(grp_num, 0);
    set_subcmd(grp_num, (param->dir || param->random_color) ? 0x1 : 0x0);
    set_argb_brightness(grp_num, param->brightness << 8);
    set_argbctl_data(grp_num);

    ret = set_appctl(grp_num, REALTEK_ARGB_LED_CTL_FW);
    if(!ret)
    {
        appctl[grp_num] = REALTEK_ARGB_LED_CTL_FW;
    }

    return ret;
}

int RealtekARGBController::set_argb_num(int grp_num, unsigned short new_num)
{
    memcpy(&argbctl_hdr[32 + grp_num * sizeof(new_num)], &new_num, sizeof(new_num));
    set_flash_argbctl_hdr(32 + grp_num * sizeof(new_num), sizeof(new_num));
    return 0;
}

int RealtekARGBController::device_reboot()
{
    std::lock_guard<std::mutex> lock(my_mutex);
    int ret = 0;
    int buf_len = REALTEK_ARGB_HID_DATALEN_CH1;
    int reboot_type = 1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_send/usb_hid_get function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x30;
    usb_buf[0x14]   = reboot_type;
    ret = usb_hid_ioctl(usb_buf, (unsigned char*)&reboot_type, sizeof(reboot_type), 0, false);

    hid_close(hdev);
    hdev = NULL;
    return ret;
}

void RealtekARGBController::device_rescan_trigger()
{
    keepalive_thread_run = true;
    keepalive_thread     = new std::thread(&RealtekARGBController::KeepaliveThreadFunction, this);
}

void RealtekARGBController::device_rescan()
{
    hid_device_info* info_full = hid_enumerate(REALTEK_ARGB_VID, REALTEK_ARGB_PID);
    hid_device_info* info_temp = info_full;

    while(info_temp)
    {
        if(info_temp->vendor_id  == REALTEK_ARGB_VID &&
           info_temp->product_id == REALTEK_ARGB_PID &&
           info_temp->usage      == REALTEK_ARGB_HID2SCSI_USAGE &&
           info_temp->usage_page == REALTEK_ARGB_HID2SCSI_PG)
        {
            hid_device* dev_temp = hid_open_path(info_temp->path);
            if(dev_temp)
            {
                hdev = dev_temp;
                if(get_support_openrgb())
                {
                    device_init();
                    break;
                }
                else
                {
                    hid_close(hdev);
                    hdev = NULL;
                }
            }
        }
        info_temp = info_temp->next;
    }
    hid_free_enumeration(info_full);
}
