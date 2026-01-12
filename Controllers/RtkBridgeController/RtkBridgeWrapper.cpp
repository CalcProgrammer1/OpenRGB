#include "RtkBridgeWrapper.h"


RtkBridgeWrapper::RtkBridgeWrapper(dHandle fd, struct led_mem *led_mem_dev, hid_device_info *info)
{
    this->fd = fd;
    this->led_mem_dev = led_mem_dev;
    this->hidinfo = info;

    led_set_appctl(fd, true);
}

RtkBridgeWrapper::~RtkBridgeWrapper()
{
    led_set_appctl(fd, false);

    if (!INVALID_DHANDLE(fd))
        bridge_close_handle(fd);

    if (led_mem_dev) {
        led_mem_deinit(led_mem_dev);
        free(led_mem_dev);
        led_mem_dev = NULL;
    }
}

std::string RtkBridgeWrapper::get_manu_name()
{
    return wcharToString(hidinfo->manufacturer_string);
}

std::string RtkBridgeWrapper::get_product_name()
{
    return wcharToString(hidinfo->product_string);
}

std::string RtkBridgeWrapper::get_sn()
{
    return wcharToString(hidinfo->serial_number);
}

std::string RtkBridgeWrapper::get_dev_loc()
{
    return hidinfo->path;
}

std::string RtkBridgeWrapper::get_fw_ver()
{
    struct bridge_fw_version fw_ver;
    std::string ver = "";

    bridge_get_fw_ver(fd, &fw_ver);
    ver += std::to_string(fw_ver.fw_major_ver) + "." +
           std::to_string(fw_ver.fw_minor_ver) + "." +
           std::to_string(fw_ver.fw_extra_ver) + "." +
           std::to_string(fw_ver.fw_build_date);
    return ver;
}

int RtkBridgeWrapper::get_argb_num()
{
    int ret = -1;
    int num_rgb = 0;

    ret = led_mem_get_rgb_num(led_mem_dev, &num_rgb);
    if (ret)
    {
        num_rgb = 0;
        goto exit;
    }
exit:
    return num_rgb;
}

int RtkBridgeWrapper::get_argb_brightness()
{
    int ret = -1;
    uint16_t bright = 0;

    ret = led_mem_get_bright(fd, led_mem_dev, &bright);
    if (ret)
    {
        bright = 0;
        goto exit;
    }
exit:
    return bright;
}

int RtkBridgeWrapper::set_argb_direct(std::vector<RGBColor> color_buf, unsigned short brightness)
{
    int ret = -1;
    int color_num = color_buf.size();
    int buf_len = color_num * 3;
    static unsigned short prev_bright = 0xFFFF;
    uint8_t *buf;

    if (color_num <= 0)
        goto exit;

    ret = led_set_appctl(fd, true);
    if (ret)
        goto exit;

    if (prev_bright != brightness)
    {
        prev_bright = brightness;
        ret = led_mem_set_bright(fd, led_mem_dev, brightness << 8, true);
        if (ret)
            goto exit;
    }

    buf = (uint8_t*)malloc(buf_len);
    memset(buf, 0, buf_len);
    for (int i = 0; i < color_num; i++)
    {
        buf[i * 3 + 0] = RGBGetRValue(color_buf[i]);
        buf[i * 3 + 1] = RGBGetGValue(color_buf[i]);
        buf[i * 3 + 2] = RGBGetBValue(color_buf[i]);
    }
    ret = led_mem_write_rgb_direct(fd, led_mem_dev, buf, color_num);
    free(buf);
    if (ret)
        goto exit;
exit:
    return ret;
}

int RtkBridgeWrapper::set_argb_effect(uint8_t mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness)
{
    int ret = -1;
    int cycle = interpolate(RTK_BRIDGE_CYCLE_MAX, RTK_BRIDGE_CYCLE_MIN, speed, RTK_BRIDGE_SPEED_MAX + 1);
    RGBQUAD rgb = { 0, 0, 0, 0 };
    static unsigned short prev_bright = 0xFFFF;

    if (color_buf.size() >= 1)
    {
        rgb.rgbRed = color_buf[0] & 0xFF;
        rgb.rgbGreen = (color_buf[0] & 0xFF00) >> 8;
        rgb.rgbBlue = (color_buf[0] & 0xFF0000) >> 16;
    }

    if (prev_bright != brightness)
    {
        prev_bright = brightness;
        ret = led_mem_set_bright(fd, led_mem_dev, brightness << 8, false);
        if (ret)
            goto exit;
    }

    switch (mode)
    {
    case LED_EFF_ALWAYS:
        ret = led_eff_set_always_on(fd, led_mem_dev, rgb);
        break;
    case LED_EFF_BLINK:
        ret = led_eff_set_blink(fd, led_mem_dev, rgb, cycle);
        break;
    case LED_EFF_BREATHE:
        ret = led_eff_set_breathe(fd, led_mem_dev, rgb, cycle);
        break;
    case LED_EFF_SPECTRUM:
        ret = led_eff_set_spectrum(fd, led_mem_dev, cycle);
        break;
    case LED_EFF_SCROLL:
        ret = led_eff_set_scroll(fd, led_mem_dev, rgb, cycle);
        break;
    case LED_EFF_RAINBOW_SCROLL:
        ret = led_eff_set_rainbow_scroll(fd, led_mem_dev, cycle);
        break;
    case LED_EFF_RUNNING_WATER:
        ret = led_eff_set_running_water(fd, led_mem_dev, rgb, cycle);
        break;
    case LED_EFF_SLIDING:
        ret = led_eff_set_sliding(fd, led_mem_dev, cycle);
        break;
    case LED_EFF_NEWTON_CRADLE:
        ret = led_eff_set_newton_cradle(fd, led_mem_dev, cycle);
        break;
    case LED_EFF_METEOR:
        ret = led_eff_set_meteor(fd, led_mem_dev, rgb, cycle);
        break;
    default:
        break;
    }
exit:
    return ret;
}

int RtkBridgeWrapper::interpolate(int vStart, int vEnd, int pos, int pos_num)
{
    if (vStart != vEnd)
        return (int)round((vStart * (pos_num - 1 - pos) + vEnd * pos) / (double)(pos_num - 1));
    else
        return vStart;
}

std::string RtkBridgeWrapper::wcharToString(const wchar_t *wstr)
{
#ifdef _WIN32
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
#else
    int size_needed = wcstombs(nullptr, wstr, 0);
#endif
    std::string str(size_needed, 0);

#ifdef _WIN32
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size_needed, nullptr, nullptr);
#else
    wcstombs(&str[0], wstr, size_needed);
#endif
    return str;
}
