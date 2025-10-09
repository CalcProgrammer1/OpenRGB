/*---------------------------------------------------------*\
| QMKVialRGBController.cpp                                  |
|                                                           |
|   Driver for VialRGB QMK Keyboard Protocol                |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <stdio.h>
#include "hsv.h"
#include "QMKVialRGBController.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| Portions of this controller adapted from Raspberry Pi     |
| RPiKeyboardConfig utility:                                |
|   https://github.com/raspberrypi/rpi-keyboard-config      |
\*---------------------------------------------------------*/

QMKVialRGBController::QMKVialRGBController(hid_device *dev_handle, const char *path)
{
    /*-----------------------------------------------------*\
    | Initialize controller fields                          |
    \*-----------------------------------------------------*/
    dev         = dev_handle;
    location    = path;
    supported   = false;

    /*-----------------------------------------------------*\
    | Read product string                                   |
    \*-----------------------------------------------------*/
    wchar_t product_string[256];

    int ret = hid_get_product_string(dev, product_string, 256);

    if(ret != 0)
    {
        name = "";
    }
    else
    {
        name = StringUtils::wstring_to_string(product_string);
    }

    /*-----------------------------------------------------*\
    | Read vendor string                                    |
    \*-----------------------------------------------------*/
    wchar_t vendor_string[256];

    ret = hid_get_manufacturer_string(dev, vendor_string, 256);

    if(ret != 0)
    {
        vendor = "";
    }
    else
    {
        vendor = StringUtils::wstring_to_string(vendor_string);
    }

    /*-----------------------------------------------------*\
    | Read serial string                                    |
    \*-----------------------------------------------------*/
    wchar_t serial_string[256];

    ret = hid_get_serial_number_string(dev, serial_string, 256);

    if(ret != 0)
    {
        serial = "";
    }
    else
    {
        serial = StringUtils::wstring_to_string(serial_string);
    }

    /*-----------------------------------------------------*\
    | Get VIA, Vial, and VialRGB information                |
    \*-----------------------------------------------------*/
    CmdGetViaProtocolVersion(&via_protocol_version);

    if(via_protocol_version < 9)
    {
        return;
    }

    CmdGetVialInfo(&vial_protocol_version, &keyboard_uid, &vialrgb_flag);

    if((vial_protocol_version < 4) || ((vialrgb_flag & 1) == 0))
    {
        supported = false;
        return;
    }

    CmdGetVialRGBInfo(&vialrgb_protocol_version, &maximum_brightness);

    /*-----------------------------------------------------*\
    | Get list of supported effects                         |
    \*-----------------------------------------------------*/
    CmdGetSupportedEffects();

    /*-----------------------------------------------------*\
    | Get count of LEDs                                     |
    \*-----------------------------------------------------*/
    CmdGetNumberLEDs(&number_leds);

    /*-----------------------------------------------------*\
    | Get info and keycode for all LEDs                     |
    \*-----------------------------------------------------*/
    for(unsigned short led_index = 0; led_index < number_leds; led_index++)
    {
        led_info.push_back(CmdGetLEDInfo(led_index));
        keycodes.push_back(CmdGetKeycode(0, led_info[led_index].row, led_info[led_index].col));
    }

    supported = true;
}

QMKVialRGBController::~QMKVialRGBController()
{
    hid_close(dev);
}

std::string QMKVialRGBController::GetLocation()
{
    return("HID: " + location);
}

std::string QMKVialRGBController::GetName()
{
    return(name);
}

std::string QMKVialRGBController::GetSerial()
{
    return(serial);
}

std::string QMKVialRGBController::GetVendor()
{
    return(vendor);
}

std::string QMKVialRGBController::GetVersion()
{
    /*-----------------------------------------------------*\
    | Format UID string                                     |
    \*-----------------------------------------------------*/
    char uid_buf[17];
    snprintf(uid_buf, sizeof(uid_buf), "%016llX", keyboard_uid);

    /*-----------------------------------------------------*\
    | Format multi-line version text                        |
    \*-----------------------------------------------------*/
    return("VIA: " + std::to_string(via_protocol_version) + "\r\n"
         + "Vial: " + std::to_string(vial_protocol_version) + "\r\n"
         + "VialRGB: " + std::to_string(vialrgb_protocol_version) + "\r\n"
         + "UID: " + uid_buf);
}

bool QMKVialRGBController::GetSupported()
{
    return(supported);
}

unsigned short QMKVialRGBController::GetEffect(std::size_t effect_idx)
{
    return(supported_effects[effect_idx]);
}

std::size_t QMKVialRGBController::GetEffectCount()
{
    return(supported_effects.size());
}

unsigned short QMKVialRGBController::GetKeycode(unsigned short led_index)
{
    return(keycodes[led_index]);
}

unsigned short QMKVialRGBController::GetLEDCount()
{
    return(number_leds);
}

vialrgb_led_info QMKVialRGBController::GetLEDInfo(unsigned short led_index)
{
    return(led_info[led_index]);
}

void QMKVialRGBController::GetMode
    (
    unsigned short*     mode,
    unsigned char*      speed,
    unsigned char*      hue,
    unsigned char*      sat,
    unsigned char*      val
    )
{
    CmdGetMode(mode, speed, hue, sat, val);
}

void QMKVialRGBController::SendLEDs
    (
    unsigned short      number_leds,
    RGBColor*           color_data
    )
{
    unsigned short      led_start_index     = 0;
    unsigned char       number_packet_leds  = 9;

    while(led_start_index < number_leds)
    {
        if((number_leds - led_start_index) < 9)
        {
            number_packet_leds = (number_leds - led_start_index);
        }

        CmdSendLEDs(led_start_index, number_packet_leds, &color_data[led_start_index]);

        led_start_index += number_packet_leds;
    }
}

void QMKVialRGBController::SetMode
    (
    unsigned short      mode,
    unsigned char       speed,
    unsigned char       hue,
    unsigned char       sat,
    unsigned char       val
    )
{
    CmdSetMode(mode, speed, hue, sat, val);
}

unsigned short QMKVialRGBController::CmdGetKeycode
    (
    unsigned char       layer,
    unsigned char       row,
    unsigned char       col
    )
{
    unsigned char       data[5];
    unsigned short      keycode;

    data[0] = row;
    data[1] = col;

    SendCheckCommand(CMD_VIA_DYNAMIC_KEYMAP_GET_KEYCODE, layer, data, 2, data, 5);

    memcpy(&keycode, &data[3], sizeof(unsigned short));

    return(keycode);
}

vialrgb_led_info QMKVialRGBController::CmdGetLEDInfo
    (
    unsigned short      led_index
    )
{
    vialrgb_led_info    data;

    SendCheckCommand(CMD_LIGHTING_GET_VALUE, VIALRGB_GET_LED_INFO, (unsigned char*)&led_index, sizeof(led_index), (unsigned char*)&data, sizeof(data));

    return(data);
}

void QMKVialRGBController::CmdGetMode
    (
    unsigned short*     mode,
    unsigned char*      speed,
    unsigned char*      hue,
    unsigned char*      sat,
    unsigned char*      val
    )
{
    unsigned char data[6];

    SendCheckCommand(CMD_LIGHTING_GET_VALUE, VIALRGB_GET_MODE, NULL, 0, data, 6);

    memcpy(mode, &data[0], sizeof(unsigned short));
    *speed  = data[2];
    *hue    = data[3];
    *sat    = data[4];
    *val    = data[5];
}

void QMKVialRGBController::CmdGetNumberLEDs
    (
    unsigned short*     number_leds
    )
{
    SendCheckCommand(CMD_LIGHTING_GET_VALUE, VIALRGB_GET_NUMBER_LEDS, NULL, 0, (unsigned char*)number_leds, sizeof(unsigned short));
}

void QMKVialRGBController::CmdGetSupportedEffects()
{
    unsigned short      packet_effects[15];
    unsigned short      max_effect = 0;

    supported_effects.clear();

    supported_effects.push_back(0);

    while(max_effect < VIALRGB_EFFECT_SKIP)
    {
        SendCheckCommand(CMD_LIGHTING_GET_VALUE, VIALRGB_GET_SUPPORTED, (unsigned char*)&max_effect, sizeof(max_effect), (unsigned char*)packet_effects, sizeof(packet_effects));

        for(unsigned int effect_idx = 0; effect_idx < 15; effect_idx++)
        {
            if(packet_effects[effect_idx] == VIALRGB_EFFECT_SKIP)
            {
                return;
            }
            supported_effects.push_back(packet_effects[effect_idx]);
            max_effect = packet_effects[effect_idx];
        }
    }
}

void QMKVialRGBController::CmdGetVialInfo
    (
    unsigned int*       vial_protocol,
    unsigned long long* keyboard_uid,
    unsigned char*      vialrgb_flag
    )
{
    unsigned char data[sizeof(int) + sizeof(unsigned long long) + sizeof(unsigned char)];

    SendCommand(CMD_VIAL_COMMAND, VIAL_GET_KEYBOARD_ID, NULL, 0, data, sizeof(data));

    memcpy(vial_protocol, &data[0], sizeof(int));
    memcpy(keyboard_uid, &data[sizeof(int)], sizeof(unsigned long long));
    memcpy(vialrgb_flag, &data[sizeof(int) + sizeof(unsigned long long)], sizeof(unsigned char));
}

void QMKVialRGBController::CmdGetVialRGBInfo
    (
    unsigned short*     vialrgb_protocol_version,
    unsigned char*      maximum_brightness
    )
{
    unsigned char data[sizeof(unsigned short) + sizeof(unsigned char)];

    SendCommand(CMD_VIAL_COMMAND, VIAL_GET_KEYBOARD_ID, NULL, 0, data, sizeof(data));

    memcpy(vialrgb_protocol_version, &data[0], sizeof(unsigned short));
    memcpy(maximum_brightness, &data[sizeof(unsigned char)], sizeof(unsigned short));
}

void QMKVialRGBController::CmdGetViaProtocolVersion
    (
    unsigned short*     via_protocol_version
    )
{
    SendCheckCommand(CMD_GET_PROTOCOL_VERSION, 0, NULL, 0, (unsigned char*)via_protocol_version, sizeof(unsigned int));
}

void QMKVialRGBController::CmdSendLEDs
    (
    unsigned short      start_index,
    unsigned char       number_leds,
    RGBColor*           color_data
    )
{
    unsigned char data[30];

    memcpy(&data[0], &start_index, sizeof(start_index));
    memcpy(&data[2], &number_leds, sizeof(number_leds));

    if(number_leds > 9)
    {
        number_leds = 9;
    }

    for(unsigned char led_index = 0; led_index < number_leds; led_index++)
    {
        /*-------------------------------------------------*\
        | VialRGB sends direct packets in HSV for some      |
        | inexplicable reason, so do the RGB to HSV         |
        | conversion before sending                         |
        \*-------------------------------------------------*/
        hsv_t hsv_color;
        rgb2hsv(color_data[led_index], &hsv_color);

        data[3 + (led_index * 3)]   = (unsigned char)((float)hsv_color.hue * (256.0f / 360.0f));
        data[4 + (led_index * 3)]   = hsv_color.saturation;
        data[5 + (led_index * 3)]   = hsv_color.value;
    }

    SendCheckCommand(CMD_LIGHTING_SET_VALUE, VIALRGB_DIRECT_FASTSET, data, sizeof(data), NULL, 0);
}

void QMKVialRGBController::CmdSetMode
    (
    unsigned short      mode,
    unsigned char       speed,
    unsigned char       hue,
    unsigned char       sat,
    unsigned char       val
    )
{
    unsigned char data[6];

    memcpy(&data[0], &mode, sizeof(unsigned short));

    data[2] = speed;
    data[3] = hue;
    data[4] = sat;
    data[5] = val;

    SendCommand(CMD_LIGHTING_SET_VALUE, VIALRGB_SET_MODE, data, sizeof(data), NULL, 0);
}

int QMKVialRGBController::SendCommand
    (
    unsigned char       cmd,
    unsigned char       subcmd,
    unsigned char*      data_in,
    unsigned char       data_in_size,
    unsigned char*      data_out,
    unsigned char       data_out_size
    )
{
    unsigned char usb_buf[MSG_LEN + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0]  = 0x00;
    usb_buf[1]  = cmd;
    usb_buf[2]  = subcmd;

    memcpy(&usb_buf[3], data_in, data_in_size);

    hid_write(dev, usb_buf, sizeof(usb_buf));
    int bytes_received = hid_read_timeout(dev, usb_buf, sizeof(usb_buf)-1, 1000);

    memcpy(data_out, &usb_buf[0], data_out_size);

    return(bytes_received);
}

int QMKVialRGBController::SendCheckCommand
    (
    unsigned char       cmd,
    unsigned char       subcmd,
    unsigned char*      data_in,
    unsigned char       data_in_size,
    unsigned char*      data_out,
    unsigned char       data_out_size
    )
{
    unsigned char usb_buf[MSG_LEN + 1];

    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0]  = 0x00;
    usb_buf[1]  = cmd;
    usb_buf[2]  = subcmd;

    memcpy(&usb_buf[3], data_in, data_in_size);

    hid_write(dev, usb_buf, sizeof(usb_buf));
    int bytes_received = hid_read_timeout(dev, usb_buf, sizeof(usb_buf)-1, 1000);

    memcpy(data_out, &usb_buf[2], data_out_size);

    return(bytes_received);
}
