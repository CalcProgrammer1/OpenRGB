/*---------------------------------------------------------------*\
|  GaiZhongGaiKeyboardController.cpp                              |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
|  An Yang     2022/6/12                                          |
\*---------------------------------------------------------------*/




#include <cstring>
#include "GaiZhongGaiKeyboardController.h"

GaiZhongGaiKeyboardController::GaiZhongGaiKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid)
{
    dev         = dev_handle;
    location    = path;
    usb_pid     = pid;

}

GaiZhongGaiKeyboardController::~GaiZhongGaiKeyboardController()
{
    /*-----------------------------------------------------*\
    | Restore built-in light effect                         |
    \*-----------------------------------------------------*/
    char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1]=0xFF;
    hid_write(dev, (unsigned char *)usb_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    hid_close(dev);
}

std::string GaiZhongGaiKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GaiZhongGaiKeyboardController::GetSerialString()
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

unsigned short GaiZhongGaiKeyboardController::GetUSBPID()
{
    return(usb_pid);
}

void GaiZhongGaiKeyboardController::SendColors
    (
    unsigned char*  color_data,
    unsigned int    color_data_size
    )
{
    char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    if(usb_pid==GaiZhongGai_17_TOUCH_PRO_PID)//PAD
    {
        usb_buf[1]=0x10;
        memcpy(usb_buf+2,color_data+68*3,51);
        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
    else if(usb_pid==GaiZhongGai_17_PRO_PID)//PAD+Touch
    {
        usb_buf[1]=0x10;
        memcpy(usb_buf+2,color_data+68*3,60);
        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
    else if(usb_pid==GaiZhongGai_68_PRO_PID)//68%
    {
        usb_buf[1]=0x10;
        memcpy(usb_buf+2,color_data+ 0*3,63);
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        usb_buf[1]=0x11;
        memcpy(usb_buf+2,color_data+21*3,63);
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        usb_buf[1]=0x12;
        memcpy(usb_buf+2,color_data+42*3,63);
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[1]=0x13;
        memcpy(usb_buf+2,color_data+63*3,15);
        hid_write(dev, (unsigned char *)usb_buf, 65);

    }
}






