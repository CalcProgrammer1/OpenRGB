#include "CorsairK55RGBPROController.h"
/*-----------------------------------------*\
|  CorsairK55RGBPROController.cpp           |
|                                           |
|  Driver for Corsair K55 RGB PRO Keyboard  |
\*-----------------------------------------*/

#include "LogManager.h"

CorsairK55RGBPROController::CorsairK55RGBPROController(hid_device* dev_handle, const char* path)
{
    dev             = dev_handle;
    location        = path;

    LightingControl();
}

CorsairK55RGBPROController::~CorsairK55RGBPROController()
{
    hid_close(dev);
}

std::string CorsairK55RGBPROController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairK55RGBPROController::GetFirmwareString()
{
    return "";
}

std::string CorsairK55RGBPROController::GetName()
{
    return name;
}

void CorsairK55RGBPROController::SetName(std::string device_name)
{
    name = device_name;
}

std::string CorsairK55RGBPROController::GetSerialString()
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

void CorsairK55RGBPROController::LightingControl()
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    //This is requered
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x03;
    usb_buf[0x05] = 0x02;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x02;
    usb_buf[0x03] = 0x5F;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x04] = 0x01;

    hid_write(dev, (unsigned char *)usb_buf, 65);
}

void CorsairK55RGBPROController::SetLEDs(std::vector<RGBColor>colors)
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x06;
    usb_buf[0x04] = 0x12;

    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        RGBColor color = colors[color_idx];
        usb_buf[9  + color_idx] = RGBGetRValue(color);
        usb_buf[15 + color_idx] = RGBGetGValue(color);
        usb_buf[21 + color_idx] = RGBGetBValue(color);
    }

    hid_write(dev, (unsigned char *)usb_buf, 65);
}
