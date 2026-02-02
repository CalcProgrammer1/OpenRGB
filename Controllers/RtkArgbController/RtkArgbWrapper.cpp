#ifndef __FreeBSD__

#include "RtkArgbWrapper.h"

static struct usb_id id[] = {
    {RTK_ARGB_VID, RTK_ARGB_PID},
    };

RtkArgbWrapper::RtkArgbWrapper(struct argb_device *dev, argbCtl *argbctl, hid_device_info *info)
{
    this->adev = dev;
    this->argb_ctl = argbctl;
    this->hidinfo = info;
    for (int i = 0; i < NUM_ARGB_GRP; i++)
    {
        appctl[i] = LED_CTL_FW;
        led_argb_set_appctl(adev, i, appctl[i]);
    }
}

RtkArgbWrapper::~RtkArgbWrapper()
{
    if (adev)
    {
        for (int i = 0; i < NUM_ARGB_GRP; i++)
        {
            appctl[i] = LED_CTL_LAMP;
            led_argb_set_appctl(adev, i, appctl[i]);
        }
        argb_dev_close(adev);
        adev = NULL;
    }

    if (argb_ctl)
    {
        led_argb_deinit(argb_ctl);
        free(argb_ctl);
        argb_ctl = NULL;
    }
}

void RtkArgbWrapper::argb_reinit()
{
    struct argb_dev_info *devs = NULL;
    std::lock_guard<std::mutex> lock(my_mutex);
    int retry_num = 3;

retry:
    if (retry_num == 0)
        goto exit;
    if (devs)
        argb_dev_free_scan(devs);

    if (argb_ctl)
    {
        if (adev)
        {
            bridge_disconnect(adev, 1);
            argb_dev_close(adev);
            adev = NULL;
        }
        QThread::msleep(1000);

        devs = argb_dev_scan(id, ARRAY_SIZE(id));
opendev:
        if (!devs) {
            retry_num--;
            goto retry;
        }

        adev = argb_path_open(devs->path, BRINTF_TYPE_HID, NULL);
        if (!adev) {
            devs = devs->next;
            goto opendev;
        }

        if (!led_argb_get_ic_support_sync_light(adev, SYNC_METHOD_OPENRGB)) {
            devs = devs->next;
            goto opendev;
        }

        if (bridge_write_unlock(adev)) {
            devs = devs->next;
            goto opendev;
        }

        if (led_argb_init(adev, argb_ctl)) {
            devs = devs->next;
            goto opendev;
        }

        argb_ctl->sync_method = SYNC_METHOD_OPENRGB;
        for (int i = 0; i < NUM_ARGB_GRP; i++)
        {
            appctl[i] = LED_CTL_FW;
            led_argb_set_appctl(adev, i, appctl[i]);
        }
    }
exit:
    if (devs)
        argb_dev_free_scan(devs);
    return;
}

std::string RtkArgbWrapper::get_manu_name()
{
    return wcharToString(hidinfo->manufacturer_string);
}

std::string RtkArgbWrapper::get_product_name()
{
    return wcharToString(hidinfo->product_string);
}

std::string RtkArgbWrapper::get_sn()
{
    return wcharToString(hidinfo->serial_number);
}

std::string RtkArgbWrapper::get_dev_loc()
{
    return hidinfo->path;
}

std::string RtkArgbWrapper::get_fw_ver()
{
    struct bridge_fw_version fw_ver;
    std::string ver = "";
    bridge_get_fw_ver(adev, &fw_ver);
    ver += std::to_string(fw_ver.fw_major_ver) + "." +
           std::to_string(fw_ver.fw_minor_ver) + "." +
           std::to_string(fw_ver.fw_extra_ver) + "." +
           std::to_string(fw_ver.fw_build_date);
    return ver;
}

int RtkArgbWrapper::get_fix_grps()
{
    int fix_grps = 0;
    PGINFO info = { 0 };

    if (!pg_read(adev, &info))
    {
        fix_grps = info.customized_led[4];
    }
    return fix_grps;
}

int RtkArgbWrapper::get_argb_num(int grp_num)
{
    int ret = -1;
    uint16_t num_rgb = 0;
    ret = led_argb_get_rgb_num(adev, argb_ctl, grp_num, &num_rgb);
    if (ret)
    {
        num_rgb = 0;
        goto exit;
    }
exit:
    return num_rgb;
}

int RtkArgbWrapper::get_argb_brightness(int grp_num)
{
    int ret = -1;
    uint16_t bright = 0;
    ret = led_argb_get_bright(adev, argb_ctl, grp_num, &bright);
    if (ret)
    {
        bright = 0;
        goto exit;
    }
exit:
    return bright;
}

int RtkArgbWrapper::set_argb_num(int grp_num, int new_num)
{
    int group, ret = 0;
    uint16_t i, argb_num = new_num;
    LampPosition *poses = NULL;
    struct LampArrayAttributesReport laar;
    struct LampAttributesResponseReport larr;
    std::lock_guard<std::mutex> lock(my_mutex);

    for (group = 0; group < NUM_ARGB_GRP; group++)
    {
        if (group == grp_num)
            argb_num = new_num;
        else
            led_argb_get_rgb_num(adev, argb_ctl, group, &argb_num);

        if (led_argb_set_rgb_num(adev, argb_ctl, group, argb_num, false))
            continue;

        if (poses)
        {
            free(poses);
            poses = NULL;
        }
        poses = (LampPosition*)malloc(sizeof(LampPosition) * argb_num);
        memset(poses, 0, sizeof(LampPosition) * argb_num);
        memcpy(&laar, argb_ctl->mdl[group].laar, sizeof(laar));
        memcpy(&larr, argb_ctl->mdl[group].larr, sizeof(larr));
        laar.LampCount = argb_num;
        for (i = 0; i < argb_num; i++)
        {
            poses[i].XInMicrometers = i + argb_ctl->mdl[group].poses[0].XInMicrometers;
            poses[i].YInMicrometers = argb_ctl->mdl[group].poses[0].YInMicrometers;
            poses[i].ZInMicrometers = argb_ctl->mdl[group].poses[0].ZInMicrometers;
        }

        if (led_argb_set_id1(adev, argb_ctl, group, &laar, false))
            continue;

        if (led_argb_set_id3(adev, argb_ctl, group, &larr, false))
            continue;

        if (led_argb_set_pos(adev, argb_ctl, group, poses, true))
            continue;
    }
exit:
    if (poses)
    {
        free(poses);
        poses = NULL;
    }

    return ret;
}

int RtkArgbWrapper::set_argb_direct(int grp_num, std::vector<RGBColor> color_buf, unsigned short brightness)
{
    int ret = -1;
    int color_num = color_buf.size();
    int buf_len = color_num * ARGB_COLOR_DEPTH;
    static unsigned short prev_bright = 0xFFFF;
    uint8_t *buf;
    std::lock_guard<std::mutex> lock(my_mutex);

    if (color_num <= 0)
        goto exit;

    if (appctl[grp_num] != LED_CTL_APP)
    {
        appctl[grp_num] = LED_CTL_APP;
        ret = led_argb_set_appctl(adev, grp_num, appctl[grp_num]);
        if (ret)
            goto exit;
    }

    if (prev_bright != brightness)
    {
        prev_bright = brightness;
        ret = led_argb_set_bright(adev, argb_ctl, grp_num, brightness << 8, true);
        if (ret)
            goto exit;
    }

    buf = (uint8_t*)malloc(buf_len);
    memset(buf, 0, buf_len);
    for (int i = 0; i < color_num; i++)
    {
        buf[i * ARGB_COLOR_DEPTH + 0] = RGBGetRValue(color_buf[i]);
        buf[i * ARGB_COLOR_DEPTH + 1] = RGBGetGValue(color_buf[i]);
        buf[i * ARGB_COLOR_DEPTH + 2] = RGBGetBValue(color_buf[i]);
    }
    ret = led_argb_write_rgb_direct(adev, argb_ctl, buf, color_num, grp_num);
    free(buf);
    if (ret)
        goto exit;
exit:
    return ret;
}

int RtkArgbWrapper::set_argb_effect(int grp_num, uint8_t mode, std::vector<RGBColor> color_buf, int speed, unsigned short brightness)
{
    int ret = -1;
    int cycle = interpolate(RTK_ARGB_CYCLE_MAX, RTK_ARGB_CYCLE_MIN, speed, RTK_ARGB_SPEED_MAX + 1);
    std::lock_guard<std::mutex> lock(my_mutex);

    if (mode == EFF_ALWAYS_ON)
        cycle = 0;

    ret = led_argb_set_eff_id(adev, argb_ctl, grp_num, mode, false);
    if (ret)
        goto exit;

    if (color_buf.size() >= 1)
    {
        ret = led_argb_set_p_color(adev, argb_ctl, grp_num, color_buf[0], false);
        if (ret)
            goto exit;
    }
    if (color_buf.size() >= 2)
    {
        ret = led_argb_set_s_color(adev, argb_ctl, grp_num, color_buf[1], false);
        if (ret)
            goto exit;
    }

    ret = led_argb_set_cycle(adev, argb_ctl, grp_num, cycle, false);
    if (ret)
        goto exit;

    ret = led_argb_set_ramp(adev, argb_ctl, grp_num, 0, false);
    if (ret)
        goto exit;

    ret = led_argb_set_stable(adev, argb_ctl, grp_num, 0, false);
    if (ret)
        goto exit;

    ret = led_argb_set_bright(adev, argb_ctl, grp_num, brightness << 8, true);
    if (ret)
        goto exit;

    appctl[grp_num] = LED_CTL_FW;
    ret = led_argb_set_appctl(adev, grp_num, appctl[grp_num]);
    if (ret)
        goto exit;
exit:
    return ret;
}

int RtkArgbWrapper::interpolate(int vStart, int vEnd, int pos, int pos_num)
{
    if (vStart != vEnd)
        return (int)round((vStart * (pos_num - 1 - pos) + vEnd * pos) / (double)(pos_num - 1));
    else
        return vStart;
}

std::string RtkArgbWrapper::wcharToString(const wchar_t *wstr)
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

#endif
