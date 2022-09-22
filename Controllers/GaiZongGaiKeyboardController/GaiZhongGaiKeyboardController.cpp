/*---------------------------------------------------------------*\
|  GaiZhongGaiKeyboardController.cpp                              |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
|  An Yang     2022/6/12                                          |
\*---------------------------------------------------------------*/

#include <cstring>
#include "GaiZhongGaiKeyboardController.h"

GaiZhongGaiKeyboardController::GaiZhongGaiKeyboardController(hid_device* dev_handle, hid_device_info* info)
{
    dev         = dev_handle;
    location    = info->path;
    usb_pid     = info->product_id;
    /*-----------------------------------------------------*\
    | Obtaining the Firmware Version                        |
    \*-----------------------------------------------------*/
    char str[10];
    sprintf(str, "Ver%04X", info->release_number);
    version = str;
}

GaiZhongGaiKeyboardController::~GaiZhongGaiKeyboardController()
{
    /*-----------------------------------------------------*\
    | Restore built-in light effect                         |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1] = 0xFF;
    hid_write(dev, usb_buf, 65);
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

std::string GaiZhongGaiKeyboardController::GetVersion()
{
    return(version);
}

unsigned short GaiZhongGaiKeyboardController::GetUSBPID()
{
    return(usb_pid);
}

void GaiZhongGaiKeyboardController::SendColors
    (
    unsigned char*  color_data,
    unsigned int    /*color_data_size*/
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    switch(usb_pid)
    {
        case GAIZHONGGAI_17_TOUCH_PRO_PID:  //17PAD+Touch
        case GAIZHONGGAI_20_PRO_PID:        //20PAD
            usb_buf[1] = 0x10;
            memcpy(usb_buf + 2, color_data + 68 * 3, 60);
            hid_write(dev, usb_buf, 65);
            break;

        case GAIZHONGGAI_17_PRO_PID:        //17PAD
            usb_buf[1] = 0x10;
            memcpy(usb_buf + 2, color_data + 68 * 3, 51);
            hid_write(dev, usb_buf, 65);
            break;

        case GAIZHONGGAI_68_PRO_PID:        //68%
            usb_buf[1] = 0x10;
            memcpy(usb_buf + 2, color_data +  0 * 3, 63);
            hid_write(dev, usb_buf, 65);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

            usb_buf[1] = 0x11;
            memcpy(usb_buf + 2, color_data + 21 * 3, 63);
            hid_write(dev, usb_buf, 65);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

            usb_buf[1] = 0x12;
            memcpy(usb_buf + 2, color_data + 42 * 3, 63);
            hid_write(dev, usb_buf, 65);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

            memset(usb_buf, 0x00, sizeof(usb_buf));
            usb_buf[1] = 0x13;
            memcpy(usb_buf + 2, color_data + 63 * 3, 15);
            hid_write(dev, usb_buf, 65);
            break;
    }
}
