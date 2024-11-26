/*---------------------------------------------------------*\
| QMKOpenRGBRevDController.cpp                              |
|                                                           |
|   Driver for OpenRGB QMK Keyboard Protocol Revision D     |
|                                                           |
|   Neneya                                      26 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "QMKOpenRGBRevDController.h"

using namespace std::chrono_literals;

QMKOpenRGBRevDController::QMKOpenRGBRevDController(hid_device *dev_handle, const char *path) :
    QMKOpenRGBBaseController(dev_handle, path, 15)
{
}

QMKOpenRGBRevDController::~QMKOpenRGBRevDController()
{
}

std::vector<unsigned int>  QMKOpenRGBRevDController::GetLEDValues()
{
    return led_values;
}

void QMKOpenRGBRevDController::GetLEDInfo(unsigned int leds_count)
{
    unsigned int leds_sent           = 0;
    unsigned int leds_per_update_info     = 8;

    std::vector<point_t>        underglow_points;
    std::vector<unsigned int>   underglow_flags;
    std::vector<std::string>    underglow_names;
    std::vector<RGBColor>       underglow_colors;
    std::vector<unsigned int>   underglow_values;

    while (leds_sent < leds_count)
    {
        if ((leds_count - leds_sent) < leds_per_update_info)
        {
            leds_per_update_info = leds_count - leds_sent;
        }

        unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_OPENRGB_GET_LED_INFO;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = leds_per_update_info;

        int bytes_read = 0;
        do
        {
            hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
            bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
        } while(bytes_read <= 0);

        for (unsigned int led_idx = 0; led_idx < leds_per_update_info; led_idx++)
        {
            unsigned int offset = led_idx * 7;

            if(usb_buf[offset + QMK_OPENRGB_FLAG_BYTE] != QMK_OPENRGB_FAILURE)
            {
                if(usb_buf[offset + QMK_OPENRGB_FLAG_BYTE] & 2)
                {
                    underglow_points.push_back(point_t{usb_buf[offset + QMK_OPENRGB_POINT_X_BYTE], usb_buf[offset + QMK_OPENRGB_POINT_Y_BYTE]});
                    underglow_flags.push_back(usb_buf[offset + QMK_OPENRGB_FLAG_BYTE]);
                    underglow_colors.push_back(ToRGBColor(usb_buf[offset + QMK_OPENRGB_R_COLOR_BYTE], usb_buf[offset + QMK_OPENRGB_G_COLOR_BYTE], usb_buf[offset + QMK_OPENRGB_B_COLOR_BYTE]));
                    underglow_values.push_back((unsigned int)(underglow_values.size() + led_values.size()));
                }
                else
                {
                    led_points.push_back(point_t{usb_buf[offset + QMK_OPENRGB_POINT_X_BYTE], usb_buf[offset + QMK_OPENRGB_POINT_Y_BYTE]});
                    led_flags.push_back(usb_buf[offset + QMK_OPENRGB_FLAG_BYTE]);
                    led_colors.push_back(ToRGBColor(usb_buf[offset + QMK_OPENRGB_R_COLOR_BYTE], usb_buf[offset + QMK_OPENRGB_G_COLOR_BYTE], usb_buf[offset + QMK_OPENRGB_B_COLOR_BYTE]));
                    led_values.push_back((unsigned int)(underglow_values.size() + led_values.size()));
                }
            }

            if(usb_buf[offset + QMK_OPENRGB_KEYCODE_BYTE] != 0)
            {
                if (qmk_keycode_keyname_map.count(usb_buf[offset + QMK_OPENRGB_KEYCODE_BYTE]) > 0)
                {
                    led_names.push_back(qmk_keycode_keyname_map[usb_buf[offset + QMK_OPENRGB_KEYCODE_BYTE]]);
                }
                else
                {
                    LOG_DEBUG("[%s] Key code: %d (%02X) @ offset %d was not found in the QMK keyname map",
                              device_name.c_str(), usb_buf[offset + QMK_OPENRGB_KEYCODE_BYTE],
                              usb_buf[offset + QMK_OPENRGB_KEYCODE_BYTE],  leds_sent + led_idx);
                    led_names.push_back(KEY_EN_UNUSED);
                }
            }
            else if(usb_buf[offset + QMK_OPENRGB_FLAG_BYTE] & 2)
            {
                underglow_names.push_back("Underglow: " + std::to_string(underglow_names.size() + led_names.size()));
            }
        }

        leds_sent += leds_per_update_info;
    }

    led_points.insert(led_points.end(), underglow_points.begin(), underglow_points.end());
    led_flags.insert(led_flags.end(), underglow_flags.begin(), underglow_flags.end());
    led_colors.insert(led_colors.end(), underglow_colors.begin(), underglow_colors.end());
    led_names.insert(led_names.end(), underglow_names.begin(), underglow_names.end());
    led_values.insert(led_values.end(), underglow_values.begin(), underglow_values.end());
}

std::vector<unsigned int> QMKOpenRGBRevDController::GetEnabledModes()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_ENABLED_MODES;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    std::vector<unsigned int> enabled_modes;
    int i = 1;
    while (usb_buf[i] != 0)
    {
        enabled_modes.push_back(usb_buf[i]);
        i++;
    }
    return enabled_modes;
}

void QMKOpenRGBRevDController::DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_DIRECT_MODE_SET_SINGLE_LED;
    usb_buf[0x02] = led_values[led];
    usb_buf[0x03] = red;
    usb_buf[0x04] = green;
    usb_buf[0x05] = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
}

void QMKOpenRGBRevDController::DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)
{
    unsigned int leds_sent           = 0;
    unsigned int tmp_leds_per_update = leds_per_update;

    while (leds_sent < leds_count)
    {
        if ((leds_count - leds_sent) < tmp_leds_per_update)
        {
            tmp_leds_per_update = leds_count - leds_sent;
        }

        unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_OPENRGB_DIRECT_MODE_SET_LEDS;
        usb_buf[0x02] = tmp_leds_per_update;

        for (unsigned int led_idx = 0; led_idx < tmp_leds_per_update; led_idx++)
        {
            usb_buf[(led_idx * 4) + 3] = led_values[led_idx + leds_sent];
            usb_buf[(led_idx * 4) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 4) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 4) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        if(delay > 0ms)
        {
            std::this_thread::sleep_for(delay);
        }

        leds_sent += tmp_leds_per_update;
    }
}
