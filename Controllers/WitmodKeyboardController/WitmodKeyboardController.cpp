/*---------------------------------------------------------*\
| WitmodKeyboardController.cpp                               |
|                                                           |
|   Driver for Witmod keyboards                             |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "WitmodKeyboardController.h"

using namespace std::chrono_literals;

WitmodKeyboardController::WitmodKeyboardController(hid_device *dev_handle, const hid_device_info &info, std::string dev_name)
{
    dev             = dev_handle;
    location        = info.path;
    name            = dev_name;
    vendor          = "Witmod";
    keyboard_size   = KEYBOARD_SIZE_TKL;

    ReadDeviceInfo();
}

WitmodKeyboardController::~WitmodKeyboardController()
{
    hid_close(dev);
}

std::string WitmodKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string WitmodKeyboardController::GetNameString()
{
    return(name);
}

std::string WitmodKeyboardController::GetSerialString()
{
    wchar_t serial_wchar[128];
    hid_get_serial_number_string(dev, serial_wchar, 128);
    std::wstring serial_wstring(serial_wchar);

    std::string serial_string;
    std::transform(serial_wstring.begin(), serial_wstring.end(), std::back_inserter(serial_string), [] (wchar_t i)
    {
        return (char)i;
    });

    return serial_string;
}

std::string WitmodKeyboardController::GetVendorString()
{
    return(vendor);
}

std::string WitmodKeyboardController::GetVersionString()
{
    return(version);
}

KEYBOARD_SIZE WitmodKeyboardController::GetKeyboardSize()
{
    return(keyboard_size);
}

/*---------------------------------------------------------*\
| Ask the keyboard which model it is.  The boards share      |
| their vendor ID, product ID and product string, so the    |
| model in the reply to this request is the only way to      |
| tell the sizes apart.  A board that does not answer, or    |
| reports an unknown model, keeps the default TKL layout.    |
\*---------------------------------------------------------*/
void WitmodKeyboardController::ReadDeviceInfo()
{
    unsigned char usb_buf[WITMOD_PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, WITMOD_PACKET_DATA_LENGTH);

    usb_buf[0] = 0x01;
    usb_buf[1] = WITMOD_INFO_COMMAND;

    hid_write(dev, usb_buf, WITMOD_PACKET_DATA_LENGTH);

    for(int retry = 0; retry < WITMOD_INFO_RETRIES; retry++)
    {
        /*-----------------------------------------*\
        |  The extra byte keeps the reply string    |
        |  terminated if it fills the packet        |
        \*-----------------------------------------*/
        char reply[WITMOD_PACKET_DATA_LENGTH + 1];
        memset(reply, 0x00, WITMOD_PACKET_DATA_LENGTH + 1);

        int reply_size = hid_read_timeout(dev, (unsigned char *)reply, WITMOD_PACKET_DATA_LENGTH, WITMOD_INFO_TIMEOUT);

        if(reply_size <= 0)
        {
            continue;
        }

        /*-----------------------------------------*\
        |  Effects send reports of their own, so    |
        |  skip anything that is not the answer     |
        \*-----------------------------------------*/
        if((reply[0] != 0x01) || (reply[1] != WITMOD_INFO_COMMAND))
        {
            continue;
        }

        /*-----------------------------------------*\
        |  Step over the command bytes and the      |
        |  padding ahead of the string              |
        \*-----------------------------------------*/
        const char* info = &reply[2];

        for(int idx = 2; idx < WITMOD_PACKET_DATA_LENGTH; idx++, info++)
        {
            if(*info > ' ')
            {
                break;
            }
        }

        if(*info == '\0')
        {
            continue;
        }

        LOG_INFO("[%s] Device info: %s", name.c_str(), info);

        const char* version_field = strrchr(info, 'V');

        if(version_field != NULL)
        {
            version = version_field;
        }

        if(strstr(info, WITMOD_MODEL_GK8120) != NULL)
        {
            LOG_INFO("[%s] GK8120 full size board, using the 104 key layout", name.c_str());

            keyboard_size   = KEYBOARD_SIZE_FULL;
            name            = "Witmod GK8120";
        }
        else if(strstr(info, WITMOD_MODEL_GK8110) != NULL)
        {
            LOG_INFO("[%s] GK8110 TKL board, using the 87 key layout", name.c_str());

            keyboard_size   = KEYBOARD_SIZE_TKL;
            name            = "Witmod GK8110";
        }
        else
        {
            LOG_WARNING("[%s] Unrecognised model, defaulting to the 87 key layout", name.c_str());
        }

        return;
    }

    LOG_WARNING("[%s] Device info request went unanswered, defaulting to the 87 key layout", name.c_str());
}

int WitmodKeyboardController::GetDirection(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:
            return 0x00;
        case MODE_DIRECTION_RIGHT:
            return 0x01;
        case MODE_DIRECTION_UP:
            return 0x02;
        case MODE_DIRECTION_DOWN:
            return 0x03;
        case MODE_DIRECTION_HORIZONTAL: // actually direction out
            return 0x04;
        case MODE_DIRECTION_VERTICAL:   // actually direction in
            return 0x05;
        default:
            return 0x00;
    }
}

int WitmodKeyboardController::GetDirectionRound(int direction)
{
    switch(direction)
    {
        case MODE_DIRECTION_LEFT:   // actually anticlockwise
                return 0x07;
        case MODE_DIRECTION_RIGHT:  // actually clockwise
                return 0x06;
        default:
                return 0x00;
    }
}

void WitmodKeyboardController::SetMode(mode m)
{
    /*---------------------------------------------*\
    |  Mode set command                             |
    \*---------------------------------------------*/

    unsigned char usb_buf[WITMOD_PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, WITMOD_PACKET_DATA_LENGTH);

    usb_buf[0] = 0x01;
    usb_buf[1] = 0x07;
    usb_buf[6] = m.value;
    usb_buf[7] = m.brightness; // brightness
    usb_buf[8] = m.speed; // speed

    if(m.colors_max == 1 || m.colors_max == 2)
    {
        usb_buf[9]  = RGBGetRValue(m.colors[0]); // front R
        usb_buf[10] = RGBGetGValue(m.colors[0]); // front G
        usb_buf[11] = RGBGetBValue(m.colors[0]); // front B
    }

    if(m.colors_max == 2 && m.color_mode != MODE_COLORS_RANDOM)
    {
        usb_buf[12] = RGBGetRValue(m.colors[1]); // back R
        usb_buf[13] = RGBGetGValue(m.colors[1]); // back G
        usb_buf[14] = RGBGetBValue(m.colors[1]); // back B
    }

    if(m.value == WITMOD_RADAR_MODE_VALUE || m.value == WITMOD_RUNNING_LINE_MODE_VALUE)
    {
        usb_buf[15] = GetDirectionRound(m.direction);
    }
    else
    {
        usb_buf[15] = GetDirection(m.direction);
    }

    usb_buf[16] = m.color_mode == MODE_COLORS_RANDOM;

    hid_write(dev, usb_buf, WITMOD_PACKET_DATA_LENGTH);
    // sleep is necessary
    std::this_thread::sleep_for(10ms);
}

void WitmodKeyboardController::SetLEDsData(std::vector<RGBColor> colors, std::vector<led> leds)
{
    /*---------------------------------------------*\
    |  LEDs data set command                        |
    \*---------------------------------------------*/

    unsigned char usb_buf[WITMOD_PACKET_DATA_LENGTH * 8];
    memset(usb_buf, 0x00, WITMOD_PACKET_DATA_LENGTH * 8);

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        int offset          = leds[i].value;
        usb_buf[offset - 1] = RGBGetRValue(colors[i]);
        usb_buf[offset]     = RGBGetGValue(colors[i]);
        usb_buf[offset + 1] = RGBGetBValue(colors[i]);
    }

    for(unsigned int i = 0; i < 8; i++)
    {
        unsigned char packet[WITMOD_PACKET_DATA_LENGTH];
        memset(packet, 0x00, WITMOD_PACKET_DATA_LENGTH);

        packet[0] = 0x01;
        packet[1] = 0x0F;
        packet[4] = i;  // package number
        packet[5] = i == 7 ? 0x12 : 0x36; // package size

        for (int x = 6; x < WITMOD_PACKET_DATA_LENGTH; x++)
        {
            packet[x] = usb_buf[WITMOD_PACKET_DATA_LENGTH * i + x];
        }
        hid_write(dev, packet, WITMOD_PACKET_DATA_LENGTH);
    }

    // sleep is necessary
    std::this_thread::sleep_for(10ms);
}
