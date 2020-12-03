#include "RedragonK556Controller.h"

#include <cstring>

RedragonK556Controller::RedragonK556Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

std::string RedragonK556Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RedragonK556Controller::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void RedragonK556Controller::SetKeyboardColors
    (
    unsigned char *     color_data,
    unsigned int        size
    )
{
    unsigned int    packet_size = 0;
    unsigned int    packet_offset = 0;

    while(size > 0)
    {
        if(size >= REDRAGON_K556_MAX_PACKET_SIZE)
        {
            packet_size     = REDRAGON_K556_MAX_PACKET_SIZE;
        }
        else
        {
            packet_size     = size;
        }
        
        SendKeyboardData
            (
            &color_data[packet_offset],
            packet_size,
            packet_offset
            );

        size           -= packet_size;
        packet_offset  += packet_size;
    }
}

void RedragonK556Controller::SendKeyboardMode
    (
    unsigned char       mode
    )
{
    SendKeyboardParameter(REDRAGON_K556_PARAMETER_MODE, 1, &mode);
}

void RedragonK556Controller::SendKeyboardModeEx
    (
    unsigned char       mode,
    unsigned char       brightness,
    unsigned char       speed,
    unsigned char       direction,
    unsigned char       random_flag,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char parameter_data[8];

    parameter_data[0]   = mode;
    parameter_data[1]   = brightness;
    parameter_data[2]   = speed;
    parameter_data[3]   = direction;
    parameter_data[4]   = random_flag;
    parameter_data[5]   = red;
    parameter_data[6]   = green;
    parameter_data[7]   = blue;

    SendKeyboardParameter(0, 8, parameter_data);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void RedragonK556Controller::ComputeChecksum
    (
    char                usb_buf[64]
    )
{
    unsigned short checksum = 0;

    for(unsigned int byte_idx = 0x03; byte_idx < 64; byte_idx++)
    {
        checksum += usb_buf[byte_idx];
    }

    usb_buf[0x01] = checksum & 0xFF;
    usb_buf[0x02] = checksum >> 8;
}

void RedragonK556Controller::SendKeyboardBegin()
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Begin (0x01) packet                   |
    |   Note: Not computing checksum as packet contents are |
    |         fixed                                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = REDRAGON_K556_COMMAND_BEGIN;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = REDRAGON_K556_COMMAND_BEGIN;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonK556Controller::SendKeyboardEnd()
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard End (0x02) packet                     |
    |   Note: Not computing checksum as packet contents are |
    |         fixed                                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = REDRAGON_K556_COMMAND_END;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = REDRAGON_K556_COMMAND_END;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonK556Controller::SendKeyboardData
    (
    unsigned char *     data,
    unsigned char       data_size,
    unsigned short      data_offset
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Color Data (0x11) packet              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = 0x11;

    usb_buf[0x04]           = data_size;
    usb_buf[0x05]           = data_offset & 0x00FF;
    usb_buf[0x06]           = data_offset >> 8;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], data, data_size);
    
    /*-----------------------------------------------------*\
    | Compute Checksum                                      |
    \*-----------------------------------------------------*/
    ComputeChecksum(usb_buf);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}

void RedragonK556Controller::SendKeyboardParameter
    (
    unsigned char       parameter,
    unsigned char       parameter_size,
    unsigned char*      parameter_data
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Parameter (0x06) packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = REDRAGON_K556_COMMAND_SET_PARAMETER;
    usb_buf[0x04]           = parameter_size;
    usb_buf[0x05]           = parameter;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], parameter_data, parameter_size);

    /*-----------------------------------------------------*\
    | Compute Checksum                                      |
    \*-----------------------------------------------------*/
    ComputeChecksum(usb_buf);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read(dev, (unsigned char *)usb_buf, 64);
}
