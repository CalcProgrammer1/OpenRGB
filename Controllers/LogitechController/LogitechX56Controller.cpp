/*-----------------------------------------*\
|  LogitechX56Controller.cpp                |
|                                           |
|  Generic RGB Interface Logitech X56       |
|  Class                                    |
|                                           |
|  Edbgon 11/06/2021                        |
\*-----------------------------------------*/

#include "LogitechX56Controller.h"
#include <cstring>

LogitechX56Controller::LogitechX56Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechX56Controller::~LogitechX56Controller()
{
    hid_close(dev);
}

std::string LogitechX56Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

char* LogitechX56Controller::GetDeviceName()
{
    return device_name;
}

std::string LogitechX56Controller::GetSerialString()
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

void LogitechX56Controller::SetColor(RGBColor color, uint8_t brightness)
{
    unsigned char buf[X56_CONTROLLER_PACKET_SIZE];
    unsigned char cbuf[X56_CONTROLLER_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, X56_CONTROLLER_PACKET_SIZE);
    memset(cbuf, 0x00, X56_CONTROLLER_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up init packet                                    |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x09;
    buf[0x02]   = 0x02;
    buf[0x03]   = brightness;

    /*-----------------------------------------------------*\
    | Set up color packet                                   |
    \*-----------------------------------------------------*/
    cbuf[0x00]  = 0x09;
    cbuf[0x02]  = 0x03;
    cbuf[0x03]  = RGBGetRValue(color);
    cbuf[0x04]  = RGBGetGValue(color);
    cbuf[0x05]  = RGBGetBValue(color);

    /*-----------------------------------------------------*\
    | Send packets                                          |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf,  X56_CONTROLLER_PACKET_SIZE);
    hid_send_feature_report(dev, cbuf, X56_CONTROLLER_PACKET_SIZE);

}

void LogitechX56Controller::Save()
{
    uint8_t buffer[X56_CONTROLLER_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buffer, 0x00, X56_CONTROLLER_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up init packet                                    |
    \*-----------------------------------------------------*/
    buffer[0x00]    = 0x01;
    buffer[0x01]    = 0x01;

    hid_send_feature_report(dev, buffer, X56_CONTROLLER_PACKET_SIZE);
}
