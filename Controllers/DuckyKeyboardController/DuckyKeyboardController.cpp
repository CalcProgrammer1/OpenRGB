/*-----------------------------------------*\
|  DuckyKeyboardController.cpp              |
|                                           |
|  Driver for Ducky RGB keyboardlighting    |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

#include <cstring>
#include "DuckyKeyboardController.h"

DuckyKeyboardController::DuckyKeyboardController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    SendInitialize();
}

DuckyKeyboardController::~DuckyKeyboardController()
{
    
}

std::string DuckyKeyboardController::GetDeviceLocation()
{
    return(location);
}

void DuckyKeyboardController::SendColors
    (
    unsigned char*  color_data,
    unsigned int    color_data_size
    )
{
    unsigned int    bytes_sent;
    unsigned char*  color_data_ptr = color_data;

    SendInitializeColorPacket();

    for(int i = 0; i < 8; i++)
    {
        bytes_sent = SendColorDataPacket(i, color_data_ptr, color_data_size);

        color_data_ptr += bytes_sent;
        color_data_size -= bytes_sent;
    }

    SendTerminateColorPacket();
}

void DuckyKeyboardController::SendInitialize()
{
    char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Initialize Direct Mode packet                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x41;
    usb_buf[0x02]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 65);
}

void DuckyKeyboardController::SendInitializeColorPacket()
{
    char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Initialize Color packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x56;
    usb_buf[0x02]   = 0x81;
    usb_buf[0x05]   = 0x01;
    usb_buf[0x09]   = 0x08;
    usb_buf[0x0D]   = 0xAA;
    usb_buf[0x0E]   = 0xAA;
    usb_buf[0x0F]   = 0xAA;
    usb_buf[0x10]   = 0xAA;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 65);
}

unsigned int DuckyKeyboardController::SendColorDataPacket
    (
    unsigned char   packet_id,
    unsigned char*  color_data,
    unsigned int    color_size
    )
{
    unsigned int bytes_sent;
    char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Color Data packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x56;
    usb_buf[0x02]   = 0x83;
    usb_buf[0x03]   = packet_id;

    if(packet_id == 0x00)
    {
        usb_buf[0x05]   = 0x01;
        usb_buf[0x09]   = 0x80;
        usb_buf[0x0A]   = 0x01;
        usb_buf[0x0C]   = 0xC1;
        usb_buf[0x11]   = 0xFF;
        usb_buf[0x12]   = 0xFF;
        usb_buf[0x13]   = 0xFF;
        usb_buf[0x14]   = 0xFF;
    }

    /*-----------------------------------------------------*\
    | Copy in color data                                    |
    \*-----------------------------------------------------*/
    if(packet_id == 0x00)
    {
        bytes_sent = 65 - 0x19;

        if(color_size < bytes_sent)
        {
            bytes_sent = color_size;
        }

        memcpy(&usb_buf[0x19], color_data, bytes_sent);
    }
    else
    {
        bytes_sent = 65 - 0x05;

        if(color_size < bytes_sent)
        {
            bytes_sent = color_size;
        }

        memcpy(&usb_buf[0x05], color_data, bytes_sent);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 65);

    return(bytes_sent);
}

void DuckyKeyboardController::SendTerminateColorPacket()
{
    char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Terminate Color packet                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x05]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 65);
}
