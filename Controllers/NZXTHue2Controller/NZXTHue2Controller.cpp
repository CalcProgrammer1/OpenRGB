/*---------------------------------------------------------*\
|  Processing Code for NZXT Hue 2                           |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/29/2016       |
\*---------------------------------------------------------*/

#include "NZXTHue2Controller.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

NZXTHue2Controller::NZXTHue2Controller(hid_device* dev_handle)
{
    dev = dev_handle;

    GetStripsOnChannel(HUE_2_CHANNEL_1);
}

NZXTHue2Controller::~NZXTHue2Controller()
{

}

unsigned int NZXTHue2Controller::GetStripsOnChannel(unsigned int /*channel*/)
{
    unsigned int ret_val = 0;

    unsigned char usb_buf[] =
    {
        0x20, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 64);

    for(int chan = 0; chan < 4; chan++)
    {
        unsigned int start = 0x0F + (6 * chan);
        unsigned int num_leds_on_channel = 0;
        for(int dev = 0; dev < 6; dev++)
        {
            switch(usb_buf[start + dev])
            {
            case 0x01: //Hue 1 strip
                num_leds_on_channel += 10;
                break;

            case 0x02: //Aer 1 fan
                num_leds_on_channel += 8;
                break;

            case 0x04: //Hue 2 strip
                num_leds_on_channel += 10;
                break;
            
            case 0x0B: //Aer 2 fan (120mm)
                num_leds_on_channel += 8;
                break;

            case 0x0C: //Aer 2 fan (140mm)
                num_leds_on_channel += 8;
                break;

            default:
                break;
            }
        }
        channel_leds[chan] = num_leds_on_channel;
    }
    return(ret_val);
}

void NZXTHue2Controller::SetChannelEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[24];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 8 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send effect packet                                    |
    \*-----------------------------------------------------*/
    SendEffect(channel, mode, speed, direction, num_colors, &color_data[0]);
}

void NZXTHue2Controller::SetChannelLEDs
    (
    unsigned char   channel,
    RGBColor *      colors,
    unsigned int    num_colors
    )
{
    unsigned char color_data[120];

    /*-----------------------------------------------------*\
    | Fill in color data (up to 40 colors)                  |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < num_colors; idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetGValue(color);
        color_data[pixel_idx + 0x01] = RGBGetRValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }

    /*-----------------------------------------------------*\
    | Send first group of color data                        |
    \*-----------------------------------------------------*/
    SendDirect(channel, 0, 20, &color_data[0]);

    /*-----------------------------------------------------*\
    | Send second group of color data if necessary          |
    \*-----------------------------------------------------*/
    if(num_colors > 20)
    {
        SendDirect(channel, 1, 20, &color_data[60]);
    }

    /*-----------------------------------------------------*\
    | Send apply packet                                     |
    \*-----------------------------------------------------*/
    SendApply(channel);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void NZXTHue2Controller::SendApply
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x22;
    usb_buf[0x01]   = 0xA0;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x04]   = 0x01;
    usb_buf[0x07]   = 0x28;
    usb_buf[0x0A]   = 0x80;
    usb_buf[0x0C]   = 0x32;
    usb_buf[0x0F]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::SendDirect
    (
    unsigned char   channel,
    unsigned char   group,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x22;
    usb_buf[0x01]   = 0x10 | group;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x03]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], color_data, color_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 64);
}

void NZXTHue2Controller::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   speed,
    bool            direction,
    unsigned char   color_count,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Effect packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x28;
    usb_buf[0x01]   = 0x03;
    usb_buf[0x02]   = (unsigned char)(1 << channel);
    usb_buf[0x03]   = 0x28;

    /*-----------------------------------------------------*\
    | Set mode in USB packet                                |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = mode;

    /*-----------------------------------------------------*\
    | Set speed in USB packet                               |
    \*-----------------------------------------------------*/
    usb_buf[0x05]   = speed;

    /*-----------------------------------------------------*\
    | Set direction in USB packet                           |
    \*-----------------------------------------------------*/
    usb_buf[0x06]   = direction ? 0x01 : 0x00;

    /*-----------------------------------------------------*\
    | Set color count in USB packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x08]   = color_count;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x0A], color_data, color_count * 3);

    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 64);
}
