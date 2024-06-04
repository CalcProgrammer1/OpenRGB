/*---------------------------------------------------------*\
| QMKOpenRGBRev9Controller.cpp                              |
|                                                           |
|   Driver for OpenRGB QMK Keyboard Protocol Revision 9     |
|   Revision 9 was initially supported by OpenRGB 0.6       |
|                                                           |
|   Kasper                                      10 Oct 2020 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "QMKOpenRGBRev9Controller.h"

using namespace std::chrono_literals;

QMKOpenRGBRev9Controller::QMKOpenRGBRev9Controller(hid_device *dev_handle, const char *path) :
        QMKOpenRGBBaseController(dev_handle, path, 20)
{
}

QMKOpenRGBRev9Controller::~QMKOpenRGBRev9Controller()
{
}

void QMKOpenRGBRev9Controller::GetLEDInfo(unsigned int led)
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
    usb_buf[0x01] = QMK_OPENRGB_GET_LED_INFO;
    usb_buf[0x02] = led;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    if(usb_buf[62] != QMK_OPENRGB_FAILURE)
    {
        led_points.push_back(point_t{usb_buf[QMK_OPENRGB_POINT_X_BYTE], usb_buf[QMK_OPENRGB_POINT_Y_BYTE]});
        led_flags.push_back(usb_buf[QMK_OPENRGB_FLAG_BYTE]);
        led_colors.push_back(ToRGBColor(usb_buf[QMK_OPENRGB_R_COLOR_BYTE], usb_buf[QMK_OPENRGB_G_COLOR_BYTE], usb_buf[QMK_OPENRGB_B_COLOR_BYTE]));
    }

    if(usb_buf[QMK_OPENRGB_KEYCODE_BYTE] != 0)
    {
        if (qmk_keycode_keyname_map.count(usb_buf[QMK_OPENRGB_KEYCODE_BYTE]) > 0)
        {
            led_names.push_back(qmk_keycode_keyname_map[usb_buf[QMK_OPENRGB_KEYCODE_BYTE]]);
        }
        else
        {
            LOG_DEBUG("[%s] Key code: %d (%02X) @ offset %d was not found in the QMK keyname map",
                      device_name.c_str(), usb_buf[QMK_OPENRGB_KEYCODE_BYTE],
                      usb_buf[QMK_OPENRGB_KEYCODE_BYTE], led);
            led_names.push_back(KEY_EN_UNUSED);
        }
    }
}

bool QMKOpenRGBRev9Controller::GetIsModeEnabled(unsigned int mode)
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
    usb_buf[0x01] = QMK_OPENRGB_GET_IS_MODE_ENABLED;
    usb_buf[0x02] = mode;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    return usb_buf[1] == QMK_OPENRGB_SUCCESS ? true : false;
}

void QMKOpenRGBRev9Controller::DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
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
    usb_buf[0x02] = led;
    usb_buf[0x03] = red;
    usb_buf[0x04] = green;
    usb_buf[0x05] = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
}

void QMKOpenRGBRev9Controller::DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)
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
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = tmp_leds_per_update;

        for (unsigned int led_idx = 0; led_idx < tmp_leds_per_update; led_idx++)
        {
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        if(delay > 0ms)
        {
            std::this_thread::sleep_for(delay);
        }

        leds_sent += tmp_leds_per_update;
    }
}
