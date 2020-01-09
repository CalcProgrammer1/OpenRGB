/*---------------------------------------------------------*\
|  Processing Code for NZXT Hue 2                           |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/29/2016       |
\*---------------------------------------------------------*/

#include "Hue2Controller.h"

#include <fstream>
#include <iostream>
#include <string>

Hue2Controller::Hue2Controller(libusb_device_handle* dev_handle)
{
    dev = dev_handle;

    GetStripsOnChannel(HUE_2_CHANNEL_1);
}

Hue2Controller::~Hue2Controller()
{
}

unsigned int Hue2Controller::GetStripsOnChannel(unsigned int /*channel*/)
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
    int             actual = 0;

    libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x81, usb_buf, 64, &actual, 0);

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
            
            case 0x0B: //Aer 2 fan
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

void Hue2Controller::SetChannelEffect(unsigned int channel, unsigned int mode, std::vector<RGBColor> colors)
{
    unsigned char usb_buf[] =
    {
        0x28, 0x03, 0x00, 0x28,
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
    int             actual = 0;

    /*-----------------------------------------------------*\
    | Set channel in USB packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0x02]   = 1 << channel;

    /*-----------------------------------------------------*\
    | Set mode in USB packet                                |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = mode;

    /*-----------------------------------------------------*\
    | Set color count in USB packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x08]   = colors.size();

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < colors.size(); idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        usb_buf[pixel_idx + 0x0A] = RGBGetGValue(color);
        usb_buf[pixel_idx + 0x0B] = RGBGetRValue(color);
        usb_buf[pixel_idx + 0x0C] = RGBGetBValue(color);
    }

    libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x81, usb_buf, 64, &actual, 0);
}

void Hue2Controller::SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors)
{
    unsigned char usb_buf[] =
    {
        0x22, 0x10, 0x01, 0x00,
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

    unsigned char usb_apply[] =
    {
        0x22, 0xA0, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x28,
        0x00, 0x00, 0x80, 0x00,
        0x32, 0x00, 0x00, 0x01,
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
    int             actual;

    std::size_t pkt_max = 20;

    /*-----------------------------------------------------*\
    | Set channel in USB packets                            |
    \*-----------------------------------------------------*/
    usb_buf[0x02]   = 1 << channel;
    usb_apply[0x02] = 1 << channel;

    /*-----------------------------------------------------*\
    | Send first packet for first 20 LEDs                   |
    \*-----------------------------------------------------*/
    if(pkt_max > colors.size())
    {
        pkt_max = colors.size();
    }

    for (std::size_t idx = 0; idx < pkt_max; idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        usb_buf[pixel_idx + 4] = RGBGetGValue(color);
        usb_buf[pixel_idx + 5] = RGBGetRValue(color);
        usb_buf[pixel_idx + 6] = RGBGetBValue(color);
    }

    libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);

    /*-----------------------------------------------------*\
    | Send second packet for second 20 LEDs if necessary    |
    \*-----------------------------------------------------*/
    for(int idx = 4; idx < 64; idx++)
    {
        usb_buf[idx] = 0;
    }
    
    usb_buf[0x01] = 0x11;
    pkt_max       = 0;

    if (colors.size() > 20)
    {
        pkt_max = colors.size() - 20;
    }

    if(pkt_max > 0)
    {
        for (std::size_t idx = 0; idx < pkt_max; idx++)
        {
            int pixel_idx = idx * 3;
            RGBColor color = colors[idx + 20];
            usb_buf[pixel_idx + 4] = RGBGetGValue(color);
            usb_buf[pixel_idx + 5] = RGBGetRValue(color);
            usb_buf[pixel_idx + 6] = RGBGetBValue(color);
        }

        libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);
    }

    /*-----------------------------------------------------*\
    | Send apply packet                                     |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_apply, 64, &actual, 0);
}
