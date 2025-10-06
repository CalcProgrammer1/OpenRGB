/*---------------------------------------------------------*\
| QMKOpenRGBBaseController.cpp                              |
|                                                           |
|   Common Driver for OpenRGB QMK Keyboard Protocol         |
|                                                           |
|   ChrisM                                      20 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "RGBControllerKeyNames.h"
#include "SettingsManager.h"
#include "QMKOpenRGBBaseController.h"

using namespace std::chrono_literals;

QMKOpenRGBBaseController::QMKOpenRGBBaseController(hid_device *dev_handle, const char *path, unsigned char max_led_count)
{
    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    json qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");
    if(qmk_settings.contains("leds_per_update"))
    {
        if(qmk_settings["leds_per_update"] > max_led_count)
        {
            qmk_settings["leds_per_update"] = max_led_count;
        }
        else if(qmk_settings["leds_per_update"] < 1)
        {
            qmk_settings["leds_per_update"] = 1;
        }
        SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
        settings_manager->SetSettings("QMKOpenRGBDevices", qmk_settings);
        settings_manager->SaveSettings();
        leds_per_update = qmk_settings["leds_per_update"];
    }
    else
    {
        leds_per_update = max_led_count;
    }

    if(qmk_settings.contains("delay"))
    {
        delay = (unsigned int)qmk_settings["delay"] * 1ms;
    }
    else
    {
        delay = 0ms;
    }

    dev         = dev_handle;
    location    = path;

    GetDeviceInfo();
    GetModeInfo();
}

QMKOpenRGBBaseController::~QMKOpenRGBBaseController()
{
    hid_close(dev);
}

std::string QMKOpenRGBBaseController::GetLocation()
{
    return("HID: " + location);
}

std::string QMKOpenRGBBaseController::GetDeviceName()
{
    return device_name;
}

std::string QMKOpenRGBBaseController::GetDeviceVendor()
{
    return device_vendor;
}

unsigned int QMKOpenRGBBaseController::GetTotalNumberOfLEDs()
{
    return total_number_of_leds;
}

unsigned int QMKOpenRGBBaseController::GetTotalNumberOfLEDsWithEmptySpace()
{
    return total_number_of_leds_with_empty_space;
}

unsigned int QMKOpenRGBBaseController::GetMode()
{
    return mode;
}

unsigned int QMKOpenRGBBaseController::GetModeSpeed()
{
    return mode_speed;
}

unsigned int QMKOpenRGBBaseController::GetModeColor()
{
    return mode_color;
}

std::vector<point_t> QMKOpenRGBBaseController::GetLEDPoints()
{
    return led_points;
}

std::vector<unsigned int> QMKOpenRGBBaseController::GetLEDFlags()
{
    return led_flags;
}

std::vector<std::string> QMKOpenRGBBaseController::GetLEDNames()
{
    return led_names;
}

std::vector<RGBColor>  QMKOpenRGBBaseController::GetLEDColors()
{
    return led_colors;
}

unsigned int QMKOpenRGBBaseController::GetProtocolVersion()
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
    usb_buf[0x01] = QMK_OPENRGB_GET_PROTOCOL_VERSION;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    return usb_buf[1];
}

std::string QMKOpenRGBBaseController::GetQMKVersion()
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
    usb_buf[0x01] = QMK_OPENRGB_GET_QMK_VERSION;

    hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);

    std::string qmk_version;
    int i = 1;
    while (usb_buf[i] != 0)
    {
        qmk_version.push_back(usb_buf[i]);
        i++;
    }

    return qmk_version;
}

void QMKOpenRGBBaseController::GetDeviceInfo()
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
    usb_buf[0x01] = QMK_OPENRGB_GET_DEVICE_INFO;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    total_number_of_leds = usb_buf[QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_BYTE];
    total_number_of_leds_with_empty_space = usb_buf[QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_WITH_EMPTY_SPACE_BYTE];

    int i = QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_WITH_EMPTY_SPACE_BYTE + 1;
    while (usb_buf[i] != 0)
    {
        device_name.push_back(usb_buf[i]);
        i++;
    }

    i++;
    while (usb_buf[i] != 0)
    {
        device_vendor.push_back(usb_buf[i]);
        i++;
    }
}

void QMKOpenRGBBaseController::GetModeInfo()
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
    usb_buf[0x01] = QMK_OPENRGB_GET_MODE_INFO;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, 65);
        bytes_read = hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    mode = usb_buf[QMK_OPENRGB_MODE_BYTE];
    mode_speed = usb_buf[QMK_OPENRGB_SPEED_BYTE];

    /*-----------------------------------------------------*\
    | QMK hue range is between 0-255 so hue needs to be     |
    | converted                                             |
    \*-----------------------------------------------------*/
    unsigned int oldRange       = 255;
    unsigned int newRange       = 359;
    unsigned int convertedHue   = (usb_buf[QMK_OPENRGB_HUE_BYTE] * newRange / oldRange);

    hsv_t hsv;
    hsv.hue                     = convertedHue;
    hsv.saturation              = usb_buf[QMK_OPENRGB_SATURATION_BYTE];
    hsv.value                   = usb_buf[QMK_OPENRGB_VALUE_BYTE];

    mode_color                  = hsv2rgb(&hsv);
}

void QMKOpenRGBBaseController::SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed)
{
    SetMode(hsv_color, mode, speed, false);
}

void QMKOpenRGBBaseController::SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed, bool save)
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
    usb_buf[0x01] = QMK_OPENRGB_SET_MODE;
    usb_buf[0x02] = hsv_color.hue * 255 / 359;
    usb_buf[0x03] = hsv_color.saturation;
    usb_buf[0x04] = hsv_color.value;
    usb_buf[0x05] = mode;
    usb_buf[0x06] = speed;
    usb_buf[0x07] = save;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
}
