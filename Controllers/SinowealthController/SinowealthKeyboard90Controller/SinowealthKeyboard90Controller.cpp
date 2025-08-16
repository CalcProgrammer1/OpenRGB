/*------------------------------------------*\
|  SinowealthKeyboard90Controller.cpp        |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0090,                   |
|  made spefically for Genesis Thor 300      |
|                                            |
|  Jan Baier 30/06/2022                      |
\*-----------------------------------------=*/

#include <cstring>
#include "LogManager.h"
#include "SinowealthKeyboard90Controller.h"
#include "StringUtils.h"

using namespace thor300;

SinowealthKeyboard90Controller::SinowealthKeyboard90Controller(hid_device* dev_handle, const char* path, const unsigned short pid, std::string dev_name)
{
    dev             = dev_handle;
    location        = path;
    name            = dev_name;
    usb_pid         = pid;
}

SinowealthKeyboard90Controller::~SinowealthKeyboard90Controller()
{
    hid_close(dev);
}

std::string SinowealthKeyboard90Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SinowealthKeyboard90Controller::GetNameString()
{
    return(name);
}

std::string SinowealthKeyboard90Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short SinowealthKeyboard90Controller::GetUSBPID()
{
    return(usb_pid);
}

void SinowealthKeyboard90Controller::SendFeatureReport
    (
    unsigned char cmd,
    unsigned char arg1,
    unsigned char arg2,
    unsigned char arg3,
    unsigned char arg4,
    unsigned char arg5
    )
{
    unsigned char usb_buf[8];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up control packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x0A;
    usb_buf[0x01] = cmd;
    usb_buf[0x02] = arg1;
    usb_buf[0x03] = arg2;
    usb_buf[0x04] = arg3;
    usb_buf[0x05] = arg4;
    usb_buf[0x06] = arg5;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)usb_buf, sizeof(usb_buf));
}

void SinowealthKeyboard90Controller::SendMode
    (
    unsigned char mode,
    unsigned char brightness,
    unsigned char speed,
    unsigned char color
    )
{
    SendFeatureReport(0x03, 0x01);
    SendFeatureReport(0x0A, mode, brightness, speed, color);
}

void SinowealthKeyboard90Controller::SendSingleLED
    (
    unsigned char key,
    unsigned char red,
    unsigned char green,
    unsigned char blue
    )
{
    SendFeatureReport(0x0C, 0x01, key, red, green, blue);
}

void SinowealthKeyboard90Controller::SendCommit()
{
    SendSingleLED(0x89);
}
