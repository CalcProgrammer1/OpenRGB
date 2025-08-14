/*---------------------------------------------------------*\
| MSIOptixController.cpp                                    |
|                                                           |
|   Driver for MSI Optix                                    |
|                                                           |
|   Morgan Guimard (morg)                       10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "MSIOptixController.h"
#include "StringUtils.h"

MSIOptixController::MSIOptixController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

MSIOptixController::~MSIOptixController()
{
    hid_close(dev);
}

std::string MSIOptixController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIOptixController::GetNameString()
{
    return(name);
}

std::string MSIOptixController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned char MSIOptixController::GetMysteriousFlag(unsigned char mode_value)
{
    switch(mode_value)
    {
        case RAINBOW_MODE_VALUE:
        case STACK_MODE_VALUE:
        case BREATHING_MODE_VALUE:
        case FLASHING_MODE_VALUE:
        case DOUBLE_FLASHING_MODE_VALUE:
        case STATIC_MODE_VALUE:
        case METEOR_MODE_VALUE:
        case LIGHNING_MODE_VALUE:
        case PLANETARY_MODE_VALUE:
        case DOUBLE_METEOR_MODE_VALUE:
        case ENERGY_MODE_VALUE:
        case MARQUEE_MODE_VALUE:
            return MSI_OPTIX_MYSTERIOUS_FLAG;

        default: return 0x00;
    }
}

void MSIOptixController::SetDirect(std::vector<RGBColor> colors, unsigned char brightness)
{
    unsigned char usb_buf[MSI_OPTIX_REPORT_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = MSI_OPTIC_REPORT_ID;

    unsigned char offset = 0x00;

    usb_buf[offset + 0x01] = DIRECT_MODE_VALUE;         // mode
    usb_buf[offset + 0x02] = 0xff;                      // color r value
    usb_buf[offset + 0x03] = 0xff;                      // color g value
    usb_buf[offset + 0x04] = 0xff;                      // color b value
    usb_buf[offset + 0x05] = 0x00;                      // speed
    usb_buf[offset + 0x06] = brightness;                // Brightness
    usb_buf[offset + 0x07] = 0x00;                      // always 00
    usb_buf[offset + 0x08] = 0xff;                      // always ff
    usb_buf[offset + 0x09] = 0x00;                      // always 00
    usb_buf[offset + 0x0A] = MSI_OPTIX_MYSTERIOUS_FLAG; // enigma
    usb_buf[offset + 0x0B] = 0x00;                      // always 00

    /*-----------------------------------------*\
    | Duplicate the block - enigma              |
    \*-----------------------------------------*/
    offset = 0x0B;

    usb_buf[offset + 0x01] = DIRECT_MODE_VALUE;         // mode
    usb_buf[offset + 0x02] = 0xff;                      // color r value
    usb_buf[offset + 0x03] = 0xff;                      // color g value
    usb_buf[offset + 0x04] = 0xff;                      // color b value
    usb_buf[offset + 0x05] = 0x00;                      // speed
    usb_buf[offset + 0x06] = brightness;                // Brightness
    usb_buf[offset + 0x07] = 0x00;                      // always 00
    usb_buf[offset + 0x08] = 0xff;                      // always ff
    usb_buf[offset + 0x09] = 0x00;                      // always 00
    usb_buf[offset + 0x0A] = MSI_OPTIX_MYSTERIOUS_FLAG; // enigma
    usb_buf[offset + 0x0B] = 0x00;                      // always 00


    /*-----------------------------------------*\
    | Colors block position                     |
    \*-----------------------------------------*/
    offset += 0x0B;

    /*-----------------------------------------*\
    | Start at index 25 in the colors block     |
    | multiplied by the 3 channels (rgb)        |
    \*-----------------------------------------*/
    offset += MSI_OPTIX_DIRECT_COLOR_OFFSET;

    for(unsigned int i = 0; i < MSI_OPTIX_NUMBER_OF_LEDS; i++)
    {
        usb_buf[++offset] =  RGBGetRValue(colors[i]);
        usb_buf[++offset] =  RGBGetGValue(colors[i]);
        usb_buf[++offset] =  RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void MSIOptixController::SetMode(std::vector<RGBColor> colors, unsigned char brightness, unsigned char speed, unsigned char mode_value, unsigned int mode_flags)
{
    unsigned char red = 0xff;
    unsigned char grn = 0xff;
    unsigned char blu = 0xff;

    if(mode_flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        red =  RGBGetRValue(colors[0]);
        grn =  RGBGetGValue(colors[0]);
        blu =  RGBGetBValue(colors[0]);
    }

    unsigned char usb_buf[MSI_OPTIX_REPORT_SIZE];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = MSI_OPTIC_REPORT_ID;

    unsigned char offset = 0x00;

    usb_buf[offset + 0x01] = mode_value;                    // mode
    usb_buf[offset + 0x02] = red;                           // color r value
    usb_buf[offset + 0x03] = grn;                           // color g value
    usb_buf[offset + 0x04] = blu;                           // color b value
    usb_buf[offset + 0x05] = speed;                         // speed
    usb_buf[offset + 0x06] = brightness;                    // Brightness
    usb_buf[offset + 0x07] = 0x00;                          // always 00
    usb_buf[offset + 0x08] = 0xff;                          // always ff
    usb_buf[offset + 0x09] = 0x00;                          // always 00
    usb_buf[offset + 0x0A] = GetMysteriousFlag(mode_value); // enigma
    usb_buf[offset + 0x0B] = 0x00;                          // always 00

    /*-----------------------------------------*\
    | Duplicate the block - enigma              |
    \*-----------------------------------------*/
    offset = 0x0B;

    usb_buf[offset + 0x01] = mode_value;                    // mode
    usb_buf[offset + 0x02] = red;                           // color r value
    usb_buf[offset + 0x03] = grn;                           // color g value
    usb_buf[offset + 0x04] = blu;                           // color b value
    usb_buf[offset + 0x05] = speed;                         // speed
    usb_buf[offset + 0x06] = brightness;                    // Brightness
    usb_buf[offset + 0x07] = 0x00;                          // always 00
    usb_buf[offset + 0x08] = 0xff;                          // always ff
    usb_buf[offset + 0x09] = 0x00;                          // always 00
    usb_buf[offset + 0x0A] = GetMysteriousFlag(mode_value); // enigma
    usb_buf[offset + 0x0B] = 0x00;                          // always 00

    /*-----------------------------------------*\
    | Colors block position                     |
    \*-----------------------------------------*/
    offset += 0x0B;

    if(mode_flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        /*-----------------------------------------*\
        | Start at index 25 in the colors block     |
        | multiplied by the 3 channels (rgb)        |
        \*-----------------------------------------*/
        offset += MSI_OPTIX_DIRECT_COLOR_OFFSET;

        for(unsigned int i = 0; i < MSI_OPTIX_NUMBER_OF_LEDS; i++)
        {
            usb_buf[++offset] =  RGBGetRValue(colors[i]);
            usb_buf[++offset] =  RGBGetGValue(colors[i]);
            usb_buf[++offset] =  RGBGetBValue(colors[i]);
        }
    }

    else if(mode_flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        for(unsigned int i = 0; i < MSI_OPTIX_COLOR_PACKET_SIZE; i++)
        {
            usb_buf[++offset] =  red;
            usb_buf[++offset] =  grn;
            usb_buf[++offset] =  blu;
        }
    }

    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}
