#include "LogitechG203LController.h"

#include <cstring>

LogitechG203LController::LogitechG203LController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

LogitechG203LController::~LogitechG203LController()
{
    hid_close(dev);
}

std::string LogitechG203LController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LogitechG203LController::GetSerialString()
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

void LogitechG203LController::SendApply()
{
    unsigned char usb_buf[20];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x7A;

    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);
}

void LogitechG203LController::SetSingleLED(int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[20];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x19;

    usb_buf[0x04] = (unsigned char)led;
    usb_buf[0x05] = red;
    usb_buf[0x06] = green;
    usb_buf[0x07] = blue;

    usb_buf[0x08] = 0xFF;

    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);

    SendApply();
}

void LogitechG203LController::SetMode(
    int             mode,
    int             speed,
    unsigned char   bright,
    unsigned char   dir,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue)
{
    unsigned char usb_buf[20];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    //Header
    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x0E;
    usb_buf[0x03] = 0x1A;
    //Common Data
    usb_buf[0x04] = 0x00;
    usb_buf[0x05] = (unsigned char)mode;
    usb_buf[0x06] = red;
    usb_buf[0x07] = green;
    usb_buf[0x08] = blue;
    //mode specific Data and position
    if(mode == LOGITECH_G203L_MODE_STATIC) usb_buf[0x09] = 0x02;
    if(mode == LOGITECH_G203L_MODE_CYCLE)
    {
        usb_buf[0x0B] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = bright;
    }
    if(mode == LOGITECH_G203L_MODE_BREATHING)
    {
        usb_buf[0x09] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0A] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0C] = bright;
    }
    if(mode == LOGITECH_G203L_MODE_WAVE)
    {
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = dir ? 0x01 : 0x06; //0x01: Left->Right   0x06: Right->Left
        usb_buf[0x0E] = bright;
        usb_buf[0x0F] = (unsigned char)((speed>>8) & 0x000000FF);
    }
    if(mode == LOGITECH_G203L_MODE_COLORMIXING)
    {
        usb_buf[0x0C] = (unsigned char)(speed & 0x000000FF);
        usb_buf[0x0D] = (unsigned char)((speed>>8) & 0x000000FF);
        usb_buf[0x0E] = bright;
    }
    
    //END BYTE
    usb_buf[0x10] = 0x01;

    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);
}

void LogitechG203LController::SetDevice(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[20];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = 0xFF;
    usb_buf[0x02] = 0x12;
    usb_buf[0x03] = 0x1A;

    usb_buf[0x04] = 0x01;
    usb_buf[0x05] = RGBGetRValue(colors[0]);
    usb_buf[0x06] = RGBGetGValue(colors[0]);
    usb_buf[0x07] = RGBGetBValue(colors[0]);

    usb_buf[0x08] = 0x02;
    usb_buf[0x09] = RGBGetRValue(colors[1]);
    usb_buf[0x0A] = RGBGetGValue(colors[1]);
    usb_buf[0x0B] = RGBGetBValue(colors[1]);

    usb_buf[0x0C] = 0x03;
    usb_buf[0x0D] = RGBGetRValue(colors[2]);
    usb_buf[0x0E] = RGBGetGValue(colors[2]);
    usb_buf[0x0F] = RGBGetBValue(colors[2]);
    
    usb_buf[0x10] = 0xFF;

    hid_write(dev, usb_buf, 20);
    hid_read(dev, usb_buf, 20);

    SendApply();
}
