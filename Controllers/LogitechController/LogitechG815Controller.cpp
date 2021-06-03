/*-----------------------------------------*\
|  LogitechG815Controller.cpp               |
|                                           |
|  Generic RGB Interface for Logitech G815  |
|  RGB Mechanical Gaming Keyboard           |
|                                           |
|  Cheerpipe      3/20/2021                 |
\*-----------------------------------------*/

#include "LogitechG815Controller.h"
#include <cstring>

LogitechG815Controller::LogitechG815Controller(hid_device* dev_handle_0x11, hid_device* dev_handle_0x12)
{
    dev_pkt_0x11 = dev_handle_0x11;
    dev_pkt_0x12 = dev_handle_0x12;
}

LogitechG815Controller::~LogitechG815Controller()
{

}

std::string LogitechG815Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_pkt_0x11, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void LogitechG815Controller::Commit()
{
    SendCommit();
}

void LogitechG815Controller::SetDirect
    (
    unsigned char       frame_type,
    unsigned char *     frame_data
    )
{
    SendDirectFrame(frame_type, frame_data);
}

void LogitechG815Controller::SetMode
    (
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    SendMode(LOGITECH_G815_ZONE_MODE_KEYBOARD, mode, speed, red, green, blue);
    SendMode(LOGITECH_G815_ZONE_MODE_LOGO,     mode, speed, red, green, blue);
    SendCommit();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void LogitechG815Controller::SendCommit()
{
    char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x10;
    usb_buf[0x03]           = LOGITECH_G815_COMMIT_BYTE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);
}

void LogitechG815Controller::InitializeDirect()
{
    char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x08;
    usb_buf[0x03]           = 0x3E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x08;
    usb_buf[0x03]           = 0x1E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0F;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0F;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);
}

void LogitechG815Controller::SendSingleLed
    (
    unsigned char       keyCode,
    unsigned char       r,
    unsigned char       g,
    unsigned char       b
    )
{
    char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up a 6F packet with a single color                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x10;
    usb_buf[0x03]           = LOGITECH_G815_ZONE_FRAME_TYPE_LITTLE;

    usb_buf[0x04]           = keyCode;

    usb_buf[0x05]           = r;
    usb_buf[0x06]           = g;
    usb_buf[0x07]           = b;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
}

void LogitechG815Controller::SendDirectFrame
    (
    unsigned char       frame_type,
    unsigned char *     frame_data
    )
{
    char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x10;
    usb_buf[0x03]           = frame_type;

    /*-----------------------------------------------------*\
    | Copy in frame data                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], frame_data, 16);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11,  (unsigned char *)usb_buf, 20);
}

void LogitechG815Controller::SendMode
    (
    unsigned char       zone,
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0D;
    usb_buf[0x03]           = 0x3D; //TODO: Check if it is the correct value for G815
    usb_buf[0x04]           = zone;

    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 100 * speed;

    if(mode == LOGITECH_G815_MODE_CYCLE)
    {
        usb_buf[0x0B]       = speed >> 8;
        usb_buf[0x0C]       = speed & 0xFF;
        usb_buf[0x0D]       = 0x64;
    }
    else if(mode == LOGITECH_G815_MODE_BREATHING)
    {
        usb_buf[0x09]       = speed >> 8;
        usb_buf[0x0A]       = speed & 0xFF;
        usb_buf[0x0C]       = 0x64;
    }
    else
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
    hid_read(dev_pkt_0x11, (unsigned char *)usb_buf, 20);
}
