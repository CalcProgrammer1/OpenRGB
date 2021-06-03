#include "RedragonM711Controller.h"

#include <cstring>

RedragonM711Controller::RedragonM711Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    unsigned char active_profile = 0x00;

    SendWritePacket(0x002C, 1, &active_profile);
    SendMouseApply();
}

RedragonM711Controller::~RedragonM711Controller()
{
    hid_close(dev);
}

std::string RedragonM711Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RedragonM711Controller::GetSerialString()
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

void RedragonM711Controller::SendMouseColor
    (
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char color_buf[3];

    color_buf[0] = red;
    color_buf[1] = green;
    color_buf[2] = blue;

    SendWritePacket(0x0449, 3, color_buf);
}

void RedragonM711Controller::SendMouseMode
    (
    unsigned char       mode,
    unsigned char       speed
    )
{
    unsigned char mode_buf[3];

    mode_buf[0] = 0x01; //On
    mode_buf[1] = speed;
    mode_buf[2] = mode;

    SendWritePacket(0x044C, 3, mode_buf);
}

void RedragonM711Controller::SendMouseMode
    (
    unsigned char       mode,
    unsigned char       speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char color_mode_buf[6];

    color_mode_buf[0] = red;
    color_mode_buf[1] = green;
    color_mode_buf[2] = blue;
    color_mode_buf[3] = 0x01; //On
    color_mode_buf[4] = speed;
    color_mode_buf[5] = mode;

    SendWritePacket(0x0449, 6, color_mode_buf);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void RedragonM711Controller::SendMouseApply()
{
    char usb_buf[16];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Apply packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x02;
    usb_buf[0x01]           = 0xF1;
    usb_buf[0x02]           = 0x02;
    usb_buf[0x03]           = 0x04;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, 16);
}

void RedragonM711Controller::SendWritePacket
    (
    unsigned short      address,
    unsigned char       data_size,
    unsigned char *     data
    )
{
    char usb_buf[16];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x02;
    usb_buf[0x01]           = 0xF3;
    usb_buf[0x02]           = address & 0xFF;
    usb_buf[0x03]           = address >> 8;
    usb_buf[0x04]           = data_size;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], data, data_size);
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, 16);
}
