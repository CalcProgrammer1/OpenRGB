/*-----------------------------------------*\
|  CorsairK100Controller.cpp                |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#include "CorsairK100Controller.h"
#include "LogManager.h"

static unsigned int keys_k100[] = {0x25, 0x31, 0x27, 0x35, 0x66, 0x65, 0x41, 0x2A, 0x6E, 0x5B, 0x36, 0x1A, 0x10, 0x00, 0x68, 0x42,
                                   0x62, 0x5C, 0x37, 0x1B, 0x16, 0x12, 0x19, 0x67, 0x43, 0x26, 0x77, 0x5D, 0x38, 0x1C, 0x04, 0x03,
                                   0x17, 0x44, 0x48, 0x7A, 0x39, 0x1D, 0x11, 0x05, 0x02, 0x28, 0x45, 0x49, 0x78, 0x58, 0x3A, 0x1E,
                                   0x13, 0x06, 0x15, 0x46, 0x4A, 0x79, 0x59, 0x3B, 0x1F, 0x18, 0x07, 0x01, 0x47, 0x6A, 0x4F, 0x5A,
                                   0x3C, 0x20, 0x14, 0x09, 0x0D, 0x6B, 0x2C, 0x69, 0x50, 0x55, 0x3D, 0x21, 0x08, 0x0A, 0x0C, 0x76,
                                   0x2D, 0x4E, 0x51, 0x56, 0x3E, 0x22, 0x0E, 0x0B, 0x32, 0x61, 0x4C, 0x52, 0x57, 0x3F, 0x23, 0x0F,
                                   0x2F, 0x33, 0x24, 0x4D, 0x53, 0x5E, 0x40, 0x29, 0x2B, 0x30, 0x34, /*Brightness,*/
                                   0x4B, 0x54, 0x5F, 0x60, 0x2E, 0x7C,
                                   0x7F, 0x80, 0x81, 0x82, 0x83, 0x84,//Macro
                                   0xA6, 0xA5, 0xA4, 0xA3, 0xA2, 0xA1, 0xA0, 0x9F, 0x9E, 0x9D, 0x9C, 0x86, 0x87, 0x88, 0x89, 0x8A,
                                   0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
                                   0x9B, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,0xB0, 0xB1, //Light Bar
                                   0x85, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9,//Nameplate
                                   0xBA, 0xBB, 0xBC/*Logo*/};

CorsairK100Controller::CorsairK100Controller(hid_device* dev_handle, const char* path)
{
    dev             = dev_handle;
    location        = path;
    keyboard_type   = CORSAIR_TYPE_K100;

    LightingControl();
}
CorsairK100Controller::~CorsairK100Controller()
{
    hid_close(dev);
}

std::string CorsairK100Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairK100Controller::GetFirmwareString()
{
    return "";
}

std::string CorsairK100Controller::GetName()
{
    return name;
}

void CorsairK100Controller::SetName(std::string device_name)
{
    name = device_name;
}

CorsairKeyboardType CorsairK100Controller::GetKeyboardType()
{
    return keyboard_type;
}

std::string CorsairK100Controller::GetSerialString()
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

void CorsairK100Controller::LightingControl()
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
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x4A;
    usb_buf[0x05] = 0x01;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x45;
    usb_buf[0x05] = 0x00;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    //This is requered
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x22;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    /*memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x05;
    usb_buf[0x03] = 0x01;

    hid_write(dev, (unsigned char *)usb_buf, 65);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    int res = hid_read_timeout(dev, usb_buf, 50, 1000);

    if (res == 0 || usb_buf[1] != 0x01)
    {
        LOG_INFO("[Corsair-K100] This device did not allow to take control over it. recieved response: %02X", usb_buf[1]);
        keyboard_type = CORSAIR_TYPE_UNKNOWN;
    }*/

}

void CorsairK100Controller::SetLEDs(std::vector<RGBColor>colors)
{
    switch(keyboard_type)
    {
        case CORSAIR_TYPE_K100:
            SetLEDsKeyboardFull(colors);
            break;

        case CORSAIR_TYPE_UNKNOWN:
            break;
    }
}

void CorsairK100Controller::SetLEDsKeyboardFull(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[600];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x06;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x45;
    usb_buf[0x05] = 0x02;
    usb_buf[0x08] = 0x12;

    for(std::size_t color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        RGBColor color = colors[color_idx];
        usb_buf[0x16 + keys_k100[color_idx]*3] = RGBGetRValue(color);
        usb_buf[0x17 + keys_k100[color_idx]*3] = RGBGetGValue(color);
        usb_buf[0x18 + keys_k100[color_idx]*3] = RGBGetBValue(color);
    }

    hid_write(dev, (unsigned char *)usb_buf, 600);
}
