/*-----------------------------------------*\
|  LogitechG915Controller.cpp               |
|                                           |
|  Generic RGB Interface for Logitech G915  |
|  RGB Mechanical Gaming Keyboard           |
|                                           |
|  Cheerpipe      3/20/2021                 |
\*-----------------------------------------*/

#include "LogitechG915Controller.h"
#include <cstring>

LogitechG915Controller::LogitechG915Controller(hid_device* dev_handle, bool wired)
{
    this->dev_handle = dev_handle;

    if(wired)
    {
        feature_4522_idx = 0x0E;
        feature_8040_idx = 0x13;
        feature_8071_idx = 0x09;
        feature_8081_idx = 0x0A;
    }
    else
    {
        feature_4522_idx = 0x0F;
        feature_8040_idx = 0x14;
        feature_8071_idx = 0x0A;
        feature_8081_idx = 0x0B;
    }
}

LogitechG915Controller::~LogitechG915Controller()
{

}

std::string LogitechG915Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_handle, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void LogitechG915Controller::Commit()
{
    SendCommit();
}

void LogitechG915Controller::SetDirect
    (
    unsigned char       frame_type,
    unsigned char *     frame_data
    )
{
    SendDirectFrame(frame_type, frame_data);
}

void LogitechG915Controller::SetMode
    (
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    SendMode(LOGITECH_G915_ZONE_MODE_KEYBOARD, mode, speed, red, green, blue);
    SendMode(LOGITECH_G915_ZONE_MODE_LOGO,     mode, speed, red, green, blue);
    SendCommit();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void LogitechG915Controller::SendCommit()
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8081_idx;
    usb_buf[0x03]           = LOGITECH_G915_COMMIT_BYTE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read_timeout(dev_handle,  (unsigned char *)usb_buf, 20, LOGITECH_READ_TIMEOUT);
}

void LogitechG915Controller::InitializeDirect()
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x3E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x1E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle,  (unsigned char *)usb_buf, 20);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle,  (unsigned char *)usb_buf, 20);
}

void LogitechG915Controller::SendSingleLed
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8081_idx;
    usb_buf[0x03]           = LOGITECH_G915_ZONE_FRAME_TYPE_LITTLE;

    usb_buf[0x04]           = keyCode;

    usb_buf[0x05]           = r;
    usb_buf[0x06]           = g;
    usb_buf[0x07]           = b;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle, (unsigned char *)usb_buf, 20);
}

void LogitechG915Controller::SendDirectFrame
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8081_idx;
    usb_buf[0x03]           = frame_type;

    /*-----------------------------------------------------*\
    | Copy in frame data                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], frame_data, 16);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read_timeout(dev_handle,  (unsigned char *)usb_buf, 20, LOGITECH_READ_TIMEOUT);
}

void LogitechG915Controller::SendMode
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
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = feature_8040_idx;
    usb_buf[0x03]           = 0x3D; //TODO: Check if it is the correct value for G915
    usb_buf[0x04]           = zone;

    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 100 * speed;

    if(mode == LOGITECH_G915_MODE_CYCLE)
    {
        usb_buf[0x0B]       = speed >> 8;
        usb_buf[0x0C]       = speed & 0xFF;
        usb_buf[0x0D]       = 0x64;
    }
    else if(mode == LOGITECH_G915_MODE_BREATHING)
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
    hid_write(dev_handle, (unsigned char *)usb_buf, 20);
    hid_read(dev_handle, (unsigned char *)usb_buf, 20);
}
