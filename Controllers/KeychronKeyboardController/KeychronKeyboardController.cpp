/*---------------------------------------------------------*\
| KeychronKeyboardController.cpp                            |
|                                                           |
|   Driver for Keychron keyboard                            |
|                                                           |
|   Morgan Guimard (morg)                       20 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "KeychronKeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

KeychronKeyboardController::KeychronKeyboardController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

KeychronKeyboardController::~KeychronKeyboardController()
{
    hid_close(dev);
}

std::string KeychronKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string KeychronKeyboardController::GetNameString()
{
    return(name);
}

std::string KeychronKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void KeychronKeyboardController:: SetLedSequencePositions(std::vector<unsigned int> positions)
{
    led_sequence_positions = positions;
}

void KeychronKeyboardController::SetMode(std::vector<mode> modes, int active_mode, std::vector<RGBColor> colors)
{
    /*-----------------------------------------*\
    |  Turn customization on/off                |
    |  Custom mode needs to turn it on          |
    \*-----------------------------------------*/
    SetCustomization(modes[active_mode].value == CUSTOM_MODE_VALUE);

    /*-----------------------------------------*\
    |  Tells the device we're about to send the |
    |  pages (18 pages)                         |
    \*-----------------------------------------*/
    StartEffectPage();

    unsigned char usb_buf[PACKET_DATA_LENGTH];

    /*-----------------------------------------*\
    |  Configure the modes                      |
    |  LED Effect Page structure:               |
    |                                           |
    |  OK.. this was from the original PDF      |
    |  which appears to not be exact/up to date |
    |-------------------------------------------|
    | [0] Specialeffects mode1-32               |
    | [1] colorFull color: 0x00 Monochrome:0x01 |
    | [2] R Color ratio 0x00-0xFF               |
    | [3] G Color Ratio0x00-0xFF                |
    | [4] B Colour ratio0x00-0xFF               |
    |       full color is 0,invalid             |
    | [5] dynamicdirection                      |
    |       left to right: 0x00                 |
    |       right to left: 0x01                 |
    |       down to up:    0x02                 |
    |       up to down:    0x03                 |
    | [6] brightnesscontrol 0x00-0x0F           |
    |       0x0F brightest                      |
    | [7] Periodiccontrol0x00-0x0F              |
    |       0x0F longest cycle                  |
    | [8:13] Reserved                           |
    | [14] Checkcode_L0xAA                      |
    | [15] Checkcode_H0x55                      |
    |-------------------------------------------|
    | Fixes:                                    |
    |  color mode is    8th byte                |
    |  brightness is    9th byte                |
    |  speed is         10th byte               |
    |  direction is     11th byte               |
    \*-----------------------------------------*/
    unsigned char selected_mode[EFFECT_PAGE_LENGTH];

    for(unsigned int i = 0; i < 5; i++)                                 // 5 packets
    {
        memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

        for(unsigned int j = 0; j < 4; j++)                             // of 4 effects
        {
            const mode& m = modes[1 + j + i * 4];                       // skip 1 first mode (Custom)

            int offset = j * EFFECT_PAGE_LENGTH;

            usb_buf[offset + 0] = m.value;   // mode value

            if(m.flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
            {
                usb_buf[offset + 1] = RGBGetRValue(m.colors[0]);
                usb_buf[offset + 2] = RGBGetGValue(m.colors[0]);
                usb_buf[offset + 3] = RGBGetBValue(m.colors[0]);
            }

            usb_buf[offset + 8]  = m.color_mode == MODE_COLORS_RANDOM;  // random switch
            usb_buf[offset + 9]  = m.brightness;
            usb_buf[offset + 10] = m.speed;
            usb_buf[offset + 11] = m.direction;

            usb_buf[offset + 14] = EFFECT_PAGE_CHECK_CODE_L;
            usb_buf[offset + 15] = EFFECT_PAGE_CHECK_CODE_H;

            /*-----------------------------------------*\
            |  Backup active mode values for later use  |
            |  Custom and off share the same mode value |
            \*-----------------------------------------*/
            if(m.value == modes[active_mode].value || (m.value == LIGHTS_OFF_MODE_VALUE && modes[active_mode].value == CUSTOM_MODE_VALUE))
            {
                usb_buf[offset + 9] = modes[active_mode].brightness;

                for(unsigned int x = 0; x < EFFECT_PAGE_LENGTH; x++)
                {
                    selected_mode[x] = usb_buf[offset+x];
                }
            }
        }

        Send(usb_buf);                                                  // Sends the packet
    }

    // packets count sent: 5

    /*-----------------------------------------*\
    |  3 times an empty packet - guess why...   |
    \*-----------------------------------------*/
    for(unsigned int i = 0; i < 3; i++)
    {
        memset(usb_buf, 0x00, PACKET_DATA_LENGTH);
        Send(usb_buf);
    }

    // packets count sent: 8

    /*-----------------------------------------*\
    |  Customization stuff                      |
    |  9 times * 16 blocks 80 RR GG BB          |
    \*-----------------------------------------*/
    unsigned char color_buf[COLOR_BUF_SIZE];
    memset(color_buf, 0x00, COLOR_BUF_SIZE);

    for(unsigned int i = 0; i < COLOR_BUF_SIZE; i += 4)
    {
        color_buf[i] = 0x80;
    }

    for(unsigned int c = 0; c < colors.size(); c++)
    {
        int offset = led_sequence_positions[c] * 4;

        color_buf[offset + 1]   = RGBGetRValue(colors[c]);
        color_buf[offset + 2]   = RGBGetGValue(colors[c]);
        color_buf[offset + 3]   = RGBGetBValue(colors[c]);
    }

    for(unsigned int p = 0; p < 9; p++)
    {
        memcpy(usb_buf, &color_buf[p * PACKET_DATA_LENGTH], PACKET_DATA_LENGTH);
        Send(usb_buf);
    }

    // packets count sent:  17

    /*-----------------------------------------*\
    |  Tells the device what the active mode is |
    |  This is the last packet                  |
    \*-----------------------------------------*/
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);
    memcpy(usb_buf, &selected_mode[0], EFFECT_PAGE_LENGTH);
    Send(usb_buf);

    // packets count sent: 18 - let's hope the keyboard ACK in next frame

    /*-----------------------------------------*\
    |  Tells the device that the pages are sent |
    \*-----------------------------------------*/
    EndCommunication();

    /*-----------------------------------------*\
    | Tells the device to apply what we've sent |
    \*-----------------------------------------*/
    StartEffectCommand();
}

void KeychronKeyboardController::StartEffectCommand()
{
    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x00] = PACKET_HEADER;
    usb_buf[0x01] = LED_EFFECT_START_COMMAND;

    Send(usb_buf);
}

void KeychronKeyboardController::StartEffectPage()
{
    /*-----------------------------------------*\
    |  LED_SPECIAL_EFFECT_PACKETS:              |
    |   Packet amount that will be sent in this |
    |   transaction                             |
    \*-----------------------------------------*/
    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x00] = PACKET_HEADER;
    usb_buf[0x01] = WRITE_LED_SPECIAL_EFFECT_AREA_COMMAND;
    usb_buf[0x08] = LED_SPECIAL_EFFECT_PACKETS;

    Send(usb_buf);

    Read();
}

void KeychronKeyboardController::SetCustomization(bool state)
{
    unsigned char usb_buf[PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);
    usb_buf[0x00] = PACKET_HEADER;
    usb_buf[0x01] = state ? TURN_ON_CUSTOMIZATION_COMMAND : TURN_OFF_CUSTOMIZATION_COMMAND;
    Send(usb_buf);

    Read();
}

void KeychronKeyboardController::EndCommunication()
{
    unsigned char usb_buf[PACKET_DATA_LENGTH];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH);

    usb_buf[0x00] = PACKET_HEADER;
    usb_buf[0x01] = COMMUNICATION_END_COMMAND;

    Send(usb_buf);

    Read();
}

void KeychronKeyboardController::Read()
{
    unsigned char usb_buf[PACKET_DATA_LENGTH+1];
    memset(usb_buf, 0x00, PACKET_DATA_LENGTH+1);

    usb_buf[0x00] = REPORT_ID;

    hid_get_feature_report(dev, usb_buf, PACKET_DATA_LENGTH+1);

    std::this_thread::sleep_for(10ms);
}

void KeychronKeyboardController::Send(unsigned char data[PACKET_DATA_LENGTH])
{
    unsigned char usb_buf[PACKET_DATA_LENGTH+1];

    usb_buf[0] = REPORT_ID;

    for(unsigned int x = 0; x < PACKET_DATA_LENGTH; x++)
    {
        usb_buf[x+1] = data[x];
    }

    hid_send_feature_report(dev, usb_buf, PACKET_DATA_LENGTH+1);

    std::this_thread::sleep_for(10ms);
}
