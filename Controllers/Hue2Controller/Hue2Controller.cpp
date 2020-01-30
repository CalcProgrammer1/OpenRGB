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
    current_mode        = HUE_2_MODE_FIXED;
    current_speed       = HUE_2_SPEED_NORMAL;
    current_direction   = false;
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

void Hue2Controller::SetMode(unsigned char mode, unsigned char speed, bool direction)
{
    current_mode        = mode;
    current_speed       = speed;
    current_direction   = direction;
}

void Hue2Controller::SetChannelLEDs(unsigned char channel, std::vector<RGBColor> colors)
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
    usb_buf[0x04]   = current_mode;

    /*-----------------------------------------------------*\
    | Set speed in USB packet                               |
    \*-----------------------------------------------------*/
    usb_buf[0x05]   = current_speed;

    /*-----------------------------------------------------*\
    | Set direction in USB packet                           |
    \*-----------------------------------------------------*/
    usb_buf[0x06]   = current_direction ? 0x01 : 0x00;
    
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