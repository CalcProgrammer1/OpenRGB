/*---------------------------------------------------------*\
|  GigabyteCastor3Controller.cpp                            |
|                                                           |
|  Driver for Gigabyte Aorus Waterforce X II 360 AIO        |
|  (Castor3 USB HID controller)                             |
|                                                           |
|  RGB ring control only — LCD not implemented              |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "GigabyteCastor3Controller.h"
#include "StringUtils.h"

GigabyteCastor3Controller::GigabyteCastor3Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    /*---------------------------------------------------------*\
    |  Query firmware version via de 00                         |
    |  Response: de [version_byte]                              |
    |  Observed: de 02 -> version 2                             |
    \*---------------------------------------------------------*/
    unsigned char de_payload[] = { 0xDE, 0x00 };
    SendPacket(de_payload, 2);

    unsigned char response[CASTOR3_IN_SIZE];
    int bytes_read = hid_read_timeout(dev, response, CASTOR3_IN_SIZE, CASTOR3_IN_TIMEOUT_MS);

    if(bytes_read > 1)
    {
        /*-----------------------------------------------------*\
        |  Strip report ID 0x99 if present on IN packets         |
        \*-----------------------------------------------------*/
        int offset = 0;
        if(response[0] == CASTOR3_REPORT_ID)
        {
            offset = 1;
        }

        if(response[offset] == 0xDE && bytes_read > offset + 1)
        {
            firmware_version = std::to_string(response[offset + 1]);
        }
    }
}

GigabyteCastor3Controller::~GigabyteCastor3Controller()
{
    hid_close(dev);
}

std::string GigabyteCastor3Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GigabyteCastor3Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string GigabyteCastor3Controller::GetFirmwareVersion()
{
    return firmware_version;
}

void GigabyteCastor3Controller::SetEffect
    (
    unsigned char               style,
    unsigned char               speed,
    unsigned char               brightness,
    unsigned char               b4,
    unsigned char               b5,
    castor3_color_type          color_type,
    std::vector<RGBColor>       colors
    )
{
    /*---------------------------------------------------------*\
    |  Wire protocol for LED effect apply                       |
    |  (from castor3.py led_set_effect, confirmed from          |
    |   led_profile.pcapng + .cled.dat cross-reference)         |
    |                                                           |
    |  Step 1: c9 [Style] [Speed*20] [Bright] [b4] [b5]        |
    |  Step 2: cd [R] [G] [B]          (single-color only)      |
    |    -OR-  b0..b3 palette registers (palette effects)       |
    |  Step 3: b6                       (commit)                |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    |  Step 1: c9 — effect select + parameters                  |
    \*---------------------------------------------------------*/
    unsigned char c9_payload[] =
    {
        0xC9,
        style,
        (unsigned char)(speed * 20),
        brightness,
        b4,
        b5
    };
    SendPacket(c9_payload, sizeof(c9_payload));

    /*---------------------------------------------------------*\
    |  Step 2a: cd — primary color for single-color effects     |
    |  Effects: static, pulse, flash, dflash, gradient, off     |
    \*---------------------------------------------------------*/
    if(color_type == CASTOR3_COLORS_SINGLE)
    {
        unsigned char r = 0xFF, g = 0x66, b = 0x00;

        if(colors.size() > 0)
        {
            r = RGBGetRValue(colors[0]);
            g = RGBGetGValue(colors[0]);
            b = RGBGetBValue(colors[0]);
        }

        unsigned char cd_payload[] = { 0xCD, r, g, b };
        SendPacket(cd_payload, sizeof(cd_payload));
    }
    /*---------------------------------------------------------*\
    |  Step 2b: b0..b3 — palette registers for multi-color      |
    |  Each register carries 2 colors (6 bytes RGB + 1 pad):    |
    |    bN [Style] [R1 G1 B1] [R2 G2 B2] 0x00                |
    |  b0=colors 1+2, b1=colors 3+4, b2=colors 5+6, b3=7+8   |
    |                                                           |
    |  Effects: colorshift(8), tricolor(3), spin(3), switch(2)  |
    \*---------------------------------------------------------*/
    else if(color_type == CASTOR3_COLORS_PALETTE)
    {
        /*-----------------------------------------------------*\
        |  Build palette: pad to 8 colors by repeating last     |
        \*-----------------------------------------------------*/
        RGBColor palette[8];

        for(int i = 0; i < 8; i++)
        {
            if(i < (int)colors.size())
            {
                palette[i] = colors[i];
            }
            else if(colors.size() > 0)
            {
                palette[i] = colors[colors.size() - 1];
            }
            else
            {
                palette[i] = ToRGBColor(0xFF, 0x00, 0x00);
            }
        }

        unsigned char regs[] = { 0xB0, 0xB1, 0xB2, 0xB3 };

        for(int i = 0; i < 4; i++)
        {
            RGBColor c1 = palette[i * 2];
            RGBColor c2 = palette[i * 2 + 1];

            unsigned char bN_payload[] =
            {
                regs[i],
                style,
                (unsigned char)RGBGetRValue(c1), (unsigned char)RGBGetGValue(c1), (unsigned char)RGBGetBValue(c1),
                (unsigned char)RGBGetRValue(c2), (unsigned char)RGBGetGValue(c2), (unsigned char)RGBGetBValue(c2),
                0x00
            };
            SendPacket(bN_payload, sizeof(bN_payload));
        }
    }

    /*---------------------------------------------------------*\
    |  Step 3: b6 — commit/apply                                |
    \*---------------------------------------------------------*/
    unsigned char b6_payload[] = { 0xB6 };
    SendPacket(b6_payload, sizeof(b6_payload));
}

void GigabyteCastor3Controller::SetOff()
{
    /*---------------------------------------------------------*\
    |  Off = Style 0x01 (Static) with Speed=0                   |
    |  From castor3.py: off uses style 0x01, speed_wire=0       |
    \*---------------------------------------------------------*/
    std::vector<RGBColor> black;
    black.push_back(ToRGBColor(0x00, 0x00, 0x00));

    unsigned char c9_payload[] =
    {
        0xC9,
        CASTOR3_STYLE_STATIC,
        0x00,                       /* speed_wire = 0 for off */
        CASTOR3_BRIGHTNESS_DEFAULT,
        0x02,                       /* b4 */
        0x01                        /* b5 */
    };
    SendPacket(c9_payload, sizeof(c9_payload));

    unsigned char cd_payload[] = { 0xCD, 0x00, 0x00, 0x00 };
    SendPacket(cd_payload, sizeof(cd_payload));

    unsigned char b6_payload[] = { 0xB6 };
    SendPacket(b6_payload, sizeof(b6_payload));
}

void GigabyteCastor3Controller::SendPacket(const unsigned char* payload, unsigned int payload_len)
{
    /*---------------------------------------------------------*\
    |  Castor3 HID OUT transport:                               |
    |  - Byte 0: Report ID (0x99)                               |
    |  - Bytes 1..6143: payload, zero-padded                    |
    |  Total write size: 6144 bytes                             |
    \*---------------------------------------------------------*/
    unsigned char buf[CASTOR3_OUT_TOTAL];
    memset(buf, 0x00, CASTOR3_OUT_TOTAL);

    buf[0] = CASTOR3_REPORT_ID;

    if(payload_len > CASTOR3_OUT_PAYLOAD)
    {
        payload_len = CASTOR3_OUT_PAYLOAD;
    }

    memcpy(&buf[1], payload, payload_len);

    hid_write(dev, buf, CASTOR3_OUT_TOTAL);
}
