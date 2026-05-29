/*---------------------------------------------------------*\
| RealtekBridgeController.cpp                               |
|                                                           |
|   Controller for Realtek USB to SSD Bridge ICs            |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RealtekBridgeController.h"
#include "hsv.h"
#include <StringUtils.h>
#include <MathUtils.h>

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

RealtekBridgeController::RealtekBridgeController(hid_device* dev, hid_device_info* info)
{
    hdev = dev;
    hidinfo = info;
    brctl_data = (unsigned char*)calloc(REALTEK_BRIDGE_CTL_DATA_SIZE, 1);
    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);

    set_write_unlock();
    get_brctl_hdr();
    get_brctl_data();
    set_appctl(true);
}

RealtekBridgeController::~RealtekBridgeController()
{
    if(hdev)
    {
        set_appctl(false);
        hid_close(hdev);
        hdev = NULL;
    }
    memset(brctl_hdr, 0, REALTEK_BRIDGE_CTL_HDR_SIZE);
    free(brctl_data);
}

int RealtekBridgeController::usb_hid_ioctl(unsigned char* usb_buf, unsigned char* data, int data_len,
                                           unsigned int offset, unsigned char is_in)
{
    int id;
    int ret     = 0;
    int buf_len = usb_hid_get_report(data_len, &id);

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

int RealtekBridgeController::usb_hid_get_report(int data_len, int* id)
{
    int retlen  = 0;

    if(data_len <= REALTEK_BRIDGE_HID_DATALEN_CH2)
    {
        *id = REALTEK_BRIDGE_HID_ID_DATA_CH2;
        retlen = REALTEK_BRIDGE_HID_DATALEN_CH2;
    }
    else if(data_len <= REALTEK_BRIDGE_HID_DATALEN_CH3)
    {
        *id = REALTEK_BRIDGE_HID_ID_DATA_CH3;
        retlen = REALTEK_BRIDGE_HID_DATALEN_CH3;
    }
    else if(data_len <= REALTEK_BRIDGE_HID_DATALEN_CH4)
    {
        *id = REALTEK_BRIDGE_HID_ID_DATA_CH4;
        retlen = REALTEK_BRIDGE_HID_DATALEN_CH4;
    }
    else if(data_len <= REALTEK_BRIDGE_HID_DATALEN_CH5)
    {
        *id = REALTEK_BRIDGE_HID_ID_DATA_CH5;
        retlen = REALTEK_BRIDGE_HID_DATALEN_CH5;
    }
    else
    {
        *id = REALTEK_BRIDGE_HID_ID_DATA_CH1;
        retlen = REALTEK_BRIDGE_HID_DATALEN_CH1;
    }
    return retlen;
}

int RealtekBridgeController::set_write_unlock()
{
    int            ret      = 0;
    int            buf_len  = REALTEK_BRIDGE_HID_DATALEN_CH1;
    int            data_len = 96;
    unsigned int   addr     = 0xAC004000;
    unsigned char* data     = (unsigned char*)calloc(data_len, 1);
    unsigned char* usb_buf  = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0x92;
    memcpy(&usb_buf[0x17], &addr, sizeof(addr));
    ret = usb_hid_ioctl(usb_buf, data, data_len, 0, true);
    free(data);
    return ret;
}

int RealtekBridgeController::get_brctl_hdr()
{
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xCC;
    usb_buf[0x14]   = 0x02;
    return usb_hid_ioctl(usb_buf, brctl_hdr, REALTEK_BRIDGE_CTL_HDR_SIZE, 0, true);
}

int RealtekBridgeController::set_brctl_hdr()
{
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x02;
    return usb_hid_ioctl(usb_buf, brctl_hdr, REALTEK_BRIDGE_CTL_HDR_SIZE, 0, false);
}

int RealtekBridgeController::get_brctl_data()
{
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
    int            offset  = REALTEK_BRIDGE_CTL_HDR_SIZE;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xCC;
    usb_buf[0x14]   = 0x02;
    memcpy(&usb_buf[0x17], &offset, sizeof(offset));
    return usb_hid_ioctl(usb_buf, brctl_data, REALTEK_BRIDGE_CTL_DATA_SIZE, 0, true);
}

int RealtekBridgeController::set_brctl_data()
{
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
    int            offset  = REALTEK_BRIDGE_CTL_HDR_SIZE;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x02;
    memcpy(&usb_buf[0x17], &offset, sizeof(offset));
    return usb_hid_ioctl(usb_buf, brctl_data, REALTEK_BRIDGE_CTL_DATA_SIZE, 0, false);
}

unsigned char RealtekBridgeController::get_support_openrgb()
{
    int            buf_len    = REALTEK_BRIDGE_HID_DATALEN_CH1;
    unsigned char  is_support = 0;
    unsigned char* usb_buf    = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_get_packet, sizeof(hid_get_packet));
    usb_buf[0x13]   = 0xCC;
    usb_buf[0x14]   = 0x04;
    usb_buf[0x15]   = REALTEK_BRIDGE_SYNC_METHOD_OPENRGB;
    if(usb_hid_ioctl(usb_buf, &is_support, sizeof(is_support), 0, true))
    {
        is_support = 0;
    }
    return is_support;
}

std::string RealtekBridgeController::get_manu_name()
{
    return StringUtils::wchar_to_char(hidinfo->manufacturer_string);
}

std::string RealtekBridgeController::get_product_name()
{
    return StringUtils::wchar_to_char(hidinfo->product_string);
}

std::string RealtekBridgeController::get_sn()
{
    return StringUtils::wchar_to_char(hidinfo->serial_number);
}

std::string RealtekBridgeController::get_dev_loc()
{
    return hidinfo->path;
}

std::string RealtekBridgeController::get_fw_ver()
{
    struct RealtekBridgeControllerFWVersion fw_ver;
    std::string    ver     = "";
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
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

int RealtekBridgeController::get_argb_num()
{
    int num_rgb = 0;
    memcpy(&num_rgb, &brctl_hdr[24], sizeof(num_rgb));

    return num_rgb;
}

int RealtekBridgeController::get_argb_brightness()
{
    int bright = 0;
    memcpy(&bright, &brctl_hdr[28], 2);

    return bright;
}

int RealtekBridgeController::set_argb_brightness(unsigned short bright)
{
    memcpy(&brctl_hdr[28], &bright, sizeof(bright));
    set_brctl_hdr();
    return 0;
}

int RealtekBridgeController::set_appctl(unsigned char ctl_sts)
{
    int            buf_len = REALTEK_BRIDGE_HID_DATALEN_CH1;
    unsigned char* usb_buf = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x01;
    usb_buf[0x15]   = ctl_sts;
    return usb_hid_ioctl(usb_buf, &ctl_sts, sizeof(ctl_sts), 0, false);
}

int RealtekBridgeController::set_direct(unsigned char* color, int color_num)
{
    int            buf_len  = REALTEK_BRIDGE_HID_DATALEN_CH1;
    int            data_len = color_num * REALTEK_BRIDGE_COLOR_DEPTH;
    unsigned char* usb_buf  = (unsigned char*)calloc(buf_len, 1);// will release in usb_hid_ioctl function

    memset(usb_buf, 0x00, buf_len);
    memcpy(usb_buf, hid_set_packet, sizeof(hid_set_packet));
    usb_buf[0x13]   = 0x4C;
    usb_buf[0x14]   = 0x03;
    usb_buf[0x15]   = REALTEK_BRIDGE_SYNC_METHOD_OPENRGB;
    memcpy(&usb_buf[0x17], &color_num, sizeof(color_num));
    return usb_hid_ioctl(usb_buf, color, data_len, 0, false);
}

int RealtekBridgeController::eff_set_always_on(RGBColor rgb)
{
    int i;
    int j;
    int row         = 1;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 1;
        brctl_data[i * single_size + 1] = row;

        for(j = 0; j < row; j++)
        {
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_blink(RGBColor rgb, int cycle)
{
    int i;
    int j;
    int row         = 2;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 2;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 1;

        for(j = 0; j < row; j++)
        {
            if((j & 0x1) == 0)
            {
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
            }
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_breathe(RGBColor rgb, int cycle)
{
    int i;
    int j;
    int row         = 2;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 4;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 2;

        for(j = 0; j < row; j++)
        {
            if((j & 0x1) == 0)
            {
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
                brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
            }
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_spectrum(int cycle)
{
    int i;
    int j;
    int row         = 6;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;
    static const RGBColor rainbow_table[6] = {0x0000FF, 0x0050FF, 0x0080FF, 0x00FF00, 0xFF0000, 0xFF0080};

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 4;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 3;

        for(j = 0; j < row; j++)
        {
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rainbow_table[j]);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rainbow_table[j]);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rainbow_table[j]);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_scroll(RGBColor rgb, int cycle)
{
    int i;
    int j;
    int start_idx   = -1;
    int row         = get_argb_num() * 2 - 2;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 2;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 4;

        if(start_idx == -1)
        {
            start_idx = i;
        }
        j = i - start_idx;
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        if(i != start_idx)
        {
            j = row - (i - start_idx);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_rainbow_scroll(int cycle)
{
    int      i;
    int      j;
    int      start_idx   = -1;
    int      row         = get_argb_num() * 2 - 2;
    int      maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int      single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;
    RGBColor slide_table[REALTEK_BRIDGE_MAX_ARGB_NUM] = {0};
    hsv_t    hsv_color;

    hsv_color.saturation = 255;
    hsv_color.value = 255;
    for(i = 0; i < get_argb_num(); i++)
    {
        hsv_color.hue = (i % get_argb_num()) * (360 / get_argb_num());
        slide_table[i] = hsv2rgb(&hsv_color);
    }

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 2;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 0x85;

        if(start_idx == -1)
        {
            start_idx = i;
        }
        j = i - start_idx;
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(slide_table[i]);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(slide_table[i]);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(slide_table[i]);
        if(i != start_idx)
        {
            j = row - (i - start_idx);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(slide_table[i]);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(slide_table[i]);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(slide_table[i]);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_running_water(RGBColor rgb, int cycle)
{
    int i;
    int j;
    int start_idx   = -1;
    int row         = get_argb_num() * 2;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 2;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 5;

        if(start_idx == -1)
        {
            start_idx = i;
        }

        for(j = i - start_idx; j < row / 2; j++)
        {
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_sliding(int cycle)
{
    int      i;
    int      j;
    int      start_idx   = -1;
    int      row         = get_argb_num();
    int      maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int      single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;
    RGBColor slide_table[REALTEK_BRIDGE_MAX_ARGB_NUM] = {0};
    RGBColor rgb         = 0;
    hsv_t    hsv_color;

    hsv_color.saturation = 255;
    hsv_color.value = 255;
    for(i = 0; i < get_argb_num(); i++)
    {
        hsv_color.hue = (i % get_argb_num()) * (360 / get_argb_num());
        slide_table[i] = hsv2rgb(&hsv_color);
    }

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 4;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 0x83;

        if(start_idx == -1)
        {
            start_idx = i;
        }

        for(j = 0; j < row; j++)
        {
            rgb = slide_table[(i - start_idx + j) % row];
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_newton_cradle(int cycle)
{
    int      i;
    int      j;
    int      start_idx   = -1;
    int      row;
    int      maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int      single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;
    RGBColor rgb         = 0;
    static const int const_ball_num = 3;
    static const RGBColor ball_color[] = {0x0080FF, 0x0000FF, 0x00FF00, 0xFF0000};
    static const int mid = get_argb_num() >> 1;

    if(get_argb_num() <= const_ball_num)
    {
        return 0;
    }

    row = (get_argb_num() - const_ball_num) * 2;

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 2;
        brctl_data[i * single_size + 1] = row;
        memcpy(&brctl_data[i * single_size + 2], &cycle, 2);
        brctl_data[i * single_size + 8] = 0x84;

        if(start_idx == -1)
        {
            start_idx = i;
        }

        for(j = 0; j < row; j++)
        {
            rgb = 0;
            if(i - start_idx > mid - 2 &&
               i - start_idx < mid + 2)
            {
                if(j < mid - 1 || j > row - mid)
                {
                    if(i - start_idx == mid - 1)
                    {
                        rgb = ball_color[1];
                    }
                    else if(i - start_idx == mid)
                    {
                        rgb = ball_color[2];
                    }
                    else
                    {
                        rgb = ball_color[3];
                    }
                }
                else
                {
                    if(i - start_idx == mid - 1)
                    {
                        rgb = ball_color[0];
                    }
                    else if(i - start_idx == mid)
                    {
                        rgb = ball_color[1];
                    }
                    else
                    {
                        rgb = ball_color[2];
                    }
                }
            }
            else if(i - start_idx < mid)
            {
                if(i - start_idx == j || i - start_idx + j == row - 1)
                {
                    rgb = ball_color[0];
                }
            }
            else if(i - start_idx > mid + 1)
            {
                if(i - start_idx - j == const_ball_num ||
                   i - start_idx + j == row - 1 + const_ball_num)
                {
                    rgb = ball_color[3];
                }
            }

            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
            brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
        }
    }
    return set_brctl_data();
}

int RealtekBridgeController::eff_set_meteor(RGBColor rgb, int cycle)
{
    int i;
    int j;
    int row         = 2;
    int maxrow      = REALTEK_BRIDGE_MAX_APPCTL_ROW;
    int single_size = maxrow * REALTEK_BRIDGE_COLOR_DEPTH + 32;
    int stable_ms   = cycle / row;
    int latency     = stable_ms / get_argb_num();

    memset(brctl_data, 0, REALTEK_BRIDGE_CTL_DATA_SIZE);
    for(i = 0; i < get_argb_num(); i++)
    {
        brctl_data[i * single_size + 0] = 4;
        brctl_data[i * single_size + 1] = row;
        brctl_data[i * single_size + 6] = stable_ms / 100;
        brctl_data[i * single_size + 7] = stable_ms / 100;
        brctl_data[i * single_size + 8] = 6;
        memcpy(&brctl_data[i * single_size + 9], &latency, 2);

        j = 0;
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 32] = RGBGetRValue(rgb);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 33] = RGBGetGValue(rgb);
        brctl_data[i * single_size + j * REALTEK_BRIDGE_COLOR_DEPTH + 34] = RGBGetBValue(rgb);
    }
    return set_brctl_data();
}

int RealtekBridgeController::set_argb_direct(std::vector<RGBColor> color_buf, unsigned short brightness)
{
    int                   ret         = -1;
    size_t                color_num   = color_buf.size();
    size_t                buf_len     = color_num * REALTEK_BRIDGE_COLOR_DEPTH;
    static unsigned short prev_bright = 0xFFFF;
    unsigned char*        buf;

    if(color_num <= 0)
    {
        goto exit;
    }

    ret = set_appctl(true);
    if(ret)
    {
        goto exit;
    }

    if(prev_bright != brightness)
    {
        prev_bright = brightness;
        ret = set_argb_brightness(brightness << 8);
        if(ret)
        {
            goto exit;
        }
    }

    buf = (unsigned char*)malloc(buf_len);
    memset(buf, 0, buf_len);
    for(int i = 0; i < (int)color_num; i++)
    {
        buf[i * REALTEK_BRIDGE_COLOR_DEPTH + 0] = RGBGetRValue(color_buf[i]);
        buf[i * REALTEK_BRIDGE_COLOR_DEPTH + 1] = RGBGetGValue(color_buf[i]);
        buf[i * REALTEK_BRIDGE_COLOR_DEPTH + 2] = RGBGetBValue(color_buf[i]);
    }
    ret = set_direct(buf, (int)color_num);
    free(buf);
    if(ret)
    {
        goto exit;
    }
exit:
    return ret;
}

int RealtekBridgeController::set_argb_effect(unsigned char mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness)
{
    int                   ret         = -1;
    int                   cycle       = MathUtils::IntInterpolate(REALTEK_BRIDGE_CYCLE_MAX, REALTEK_BRIDGE_CYCLE_MIN, 0, REALTEK_BRIDGE_SPEED_MAX, speed);
    RGBColor              rgb         = 0;
    static unsigned short prev_bright = 0xFFFF;

    if(color_buf.size() >= 1)
    {
        rgb = color_buf[0];
    }

    if(prev_bright != brightness)
    {
        prev_bright = brightness;
        ret = set_argb_brightness(brightness << 8);
        if(ret)
        {
            goto exit;
        }
    }

    switch(mode)
    {
    case REALTEK_BRIDGE_LED_EFF_ALWAYS:
        ret = eff_set_always_on(rgb);
        break;
    case REALTEK_BRIDGE_LED_EFF_BLINK:
        ret = eff_set_blink(rgb, cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_BREATHE:
        ret = eff_set_breathe(rgb, cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_SPECTRUM:
        ret = eff_set_spectrum(cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_SCROLL:
        ret = eff_set_scroll(rgb, cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_RAINBOW_SCROLL:
        ret = eff_set_rainbow_scroll(cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_RUNNING_WATER:
        ret = eff_set_running_water(rgb, cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_SLIDING:
        ret = eff_set_sliding(cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_NEWTON_CRADLE:
        ret = eff_set_newton_cradle(cycle);
        break;
    case REALTEK_BRIDGE_LED_EFF_METEOR:
        ret = eff_set_meteor(rgb, cycle);
        break;
    default:
        break;
    }
    if(!ret)
    {
        ret = set_appctl(false);
    }
exit:
    return ret;
}
