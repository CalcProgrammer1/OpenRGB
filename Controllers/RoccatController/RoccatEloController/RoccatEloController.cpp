#include "RoccatEloController.h"
#include <string.h>
#include <cmath>

RoccatEloController::RoccatEloController(hid_device* dev_handle, const hid_device_info& info)
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

RoccatEloController::~RoccatEloController()
{
    hid_close(dev);
}

std::string RoccatEloController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RoccatEloController::GetSerialString()
{
    return(serial_number);
}

void RoccatEloController::SendInit()
{
    unsigned char usb_buf[ROCCAT_ELO_REPORT_SIZE];

    memset(usb_buf, 0x00, ROCCAT_ELO_REPORT_SIZE);

    usb_buf[0x00] = ROCCAT_ELO_REPORT_ID;
    usb_buf[0x01] = 0x01;

    hid_write(dev, usb_buf, ROCCAT_ELO_REPORT_SIZE);

    usb_buf[0x01] = 0x02;

    hid_write(dev, usb_buf, ROCCAT_ELO_REPORT_SIZE);

    usb_buf[0x01] = 0x03;
    usb_buf[0x03] = 0x01;

    hid_write(dev, usb_buf, ROCCAT_ELO_REPORT_SIZE);

    SendDirect(0);

    memset(usb_buf, 0x00, ROCCAT_ELO_REPORT_SIZE);

    usb_buf[0x00] = ROCCAT_ELO_REPORT_ID;
    usb_buf[0x01] = 0x01;

    hid_write(dev, usb_buf, ROCCAT_ELO_REPORT_SIZE);
}

void RoccatEloController::SendDirect(RGBColor color)
{
    unsigned char usb_buf[ROCCAT_ELO_REPORT_SIZE];

    memset(usb_buf, 0x00, ROCCAT_ELO_REPORT_SIZE);

    usb_buf[0x00] = ROCCAT_ELO_REPORT_ID;
    usb_buf[0x01] = 0x04;
    usb_buf[0x04] = RGBGetRValue(color);
    usb_buf[0x05] = RGBGetGValue(color);
    usb_buf[0x06] = RGBGetBValue(color);

    hid_write(dev, usb_buf, ROCCAT_ELO_REPORT_SIZE);
}

