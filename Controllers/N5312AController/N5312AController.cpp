/*-----------------------------------------*\
|  N5312AController.cpp                     |
|                                           |
|  Driver for N5312A lighting               |
|  controller                               |
|                                           |
|  Guimard Morgan (morg) 4/02/2022          |
\*-----------------------------------------*/
#include "N5312AController.h"
#include <string.h>
#include "LogManager.h"

N5312AController::N5312AController(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    location            = info.path;
    version             = "";

    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }

    SendInit();
}

N5312AController::~N5312AController()
{
    hid_close(dev);
}

std::string N5312AController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string N5312AController::GetSerialString()
{
    return(serial_number);
}

std::string N5312AController::GetFirmwareVersion()
{
    return(version);
}

void N5312AController::SendInit()
{
    unsigned char usb_buf[N5312A_PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, N5312A_PACKET_DATA_LENGTH);

    usb_buf[0x00] = N5312A_REPORT_ID;
    usb_buf[0x01] = N5312A_INIT_BYTE;

    hid_send_feature_report(dev, usb_buf, N5312A_PACKET_DATA_LENGTH);
}

void N5312AController::SetColor(RGBColor color)
{
    unsigned char usb_buf[N5312A_PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, N5312A_PACKET_DATA_LENGTH);

    usb_buf[0x00] = N5312A_REPORT_ID;
    usb_buf[0x01] = N5312A_SET_COLOR_BYTE;
    usb_buf[0x02] = 1;
    usb_buf[0x03] = RGBGetRValue(color);
    usb_buf[0x04] = RGBGetGValue(color);
    usb_buf[0x05] = RGBGetBValue(color);

    hid_send_feature_report(dev, usb_buf, N5312A_PACKET_DATA_LENGTH);

}

void N5312AController::SetMode(RGBColor color, unsigned char mode_value, unsigned char brightness, unsigned char speed)
{
    SetColor(color);

    unsigned char usb_buf[N5312A_PACKET_DATA_LENGTH];

    memset(usb_buf, 0x00, N5312A_PACKET_DATA_LENGTH);

    usb_buf[0x00] = N5312A_REPORT_ID;
    usb_buf[0x01] = N5312A_SET_MODE_BYTE;

    usb_buf[0x02] = mode_value;

    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x01;
    usb_buf[0x05] = 0x01;

    usb_buf[0x06] = speed;
    usb_buf[0x07] = brightness;

    hid_send_feature_report(dev, usb_buf, N5312A_PACKET_DATA_LENGTH);
}
