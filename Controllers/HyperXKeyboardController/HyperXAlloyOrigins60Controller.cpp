/*-----------------------------------------*\
|  HyperXAlloyOrigins60Controller.cpp       |
|                                           |
|  Driver for HyperX Alloy Origins 60 RGB   |
|  Keyboard lighting controller             |
|                                           |
|  Derek Huber 10/24/2022                   |
\*-----------------------------------------*/

#include "HyperXAlloyOrigins60Controller.h"

#include <cstring>

HyperXAlloyOrigins60Controller::HyperXAlloyOrigins60Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

HyperXAlloyOrigins60Controller::~HyperXAlloyOrigins60Controller()
{
    hid_close(dev);
}

std::string HyperXAlloyOrigins60Controller::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXAlloyOrigins60Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void HyperXAlloyOrigins60Controller::SetLEDsDirect(std::vector<RGBColor> colors)
{
    /*-----------------------------------------------------*\
    | Set up variables to track progress of color transmit  |
    | Do this after inserting blanks                        |
    \*-----------------------------------------------------*/
    int colors_to_send = colors.size();
    int colors_sent    = 0;

    SendDirectInitialization();

    for(int pkt_idx = 0; pkt_idx < 5; pkt_idx++)
    {
        if(colors_to_send > 16)
        {
            SendDirectColorPacket(&colors[colors_sent], 16);
            colors_sent    += 16;
            colors_to_send -= 16;
        }
        else if(colors_to_send > 0)
        {
            SendDirectColorPacket(&colors[colors_sent], colors_to_send);
            colors_sent    += colors_to_send;
            colors_to_send -= colors_to_send;
        }
        else
        {
            RGBColor temp = 0x00000000;
            SendDirectColorPacket(&temp, 1);
        }
    }
}

void HyperXAlloyOrigins60Controller::SendDirectInitialization()
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Initialization packet                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = 0x04;
    buf[0x02]   = 0xF2;
    buf[0x09]   = 0x05;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}

void HyperXAlloyOrigins60Controller::SendDirectColorPacket
    (
    RGBColor*       color_data,
    unsigned int    color_count
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Initialization packet                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;

    /*-----------------------------------------------------*\
    | The maximum number of colors per packet is 16         |
    \*-----------------------------------------------------*/
    if(color_count > 16)
    {
        color_count = 16;
    }

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < color_count; color_idx++)
    {
        buf[(color_idx * 4) + 1] = 0x81;
        buf[(color_idx * 4) + 2] = RGBGetRValue(color_data[color_idx]);
        buf[(color_idx * 4) + 3] = RGBGetGValue(color_data[color_idx]);
        buf[(color_idx * 4) + 4] = RGBGetBValue(color_data[color_idx]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}
