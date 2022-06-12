/*-----------------------------------------*\
|  GaiZhongGaiKeyboardController.cpp              |
|                                           |
|  Driver for GaiZhongGai RGB keyboardlighting    |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/4/2020    |
\*-----------------------------------------*/

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
    usb_buf[1]=0XFF;
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
        usb_buf[1]=0X10;
        for(char i=0;i<17;i++)
        {
            usb_buf[i*3+2]=color_data[(68+i)*3+1];//G
            usb_buf[i*3+3]=color_data[(68+i)*3+0];//R
            usb_buf[i*3+4]=color_data[(68+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
    else if(usb_pid==GaiZhongGai_17_PRO_PID)//PAD+Touch
    {
        usb_buf[1]=0X10;
        for(char i=0;i<20;i++)
        {
            usb_buf[i*3+2]=color_data[(68+i)*3+1];//G
            usb_buf[i*3+3]=color_data[(68+i)*3+0];//R
            usb_buf[i*3+4]=color_data[(68+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
    else if(usb_pid==GaiZhongGai_68_PRO_PID)//68%
    {
        usb_buf[1]=0X10;
        for(char i=0;i<21;i++)
        {
            usb_buf[i*3+2]=color_data[( 0+i)*3+1];//G
            usb_buf[i*3+3]=color_data[( 0+i)*3+0];//R
            usb_buf[i*3+4]=color_data[( 0+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        usb_buf[1]=0X11;
        for(char i=0;i<21;i++)
        {
            usb_buf[i*3+2]=color_data[(21+i)*3+1];//G
            usb_buf[i*3+3]=color_data[(21+i)*3+0];//R
            usb_buf[i*3+4]=color_data[(21+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        usb_buf[1]=0X12;
        for(char i=0;i<21;i++)
        {
            usb_buf[i*3+2]=color_data[(42+i)*3+1];//G
            usb_buf[i*3+3]=color_data[(42+i)*3+0];//R
            usb_buf[i*3+4]=color_data[(42+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[1]=0X13;
        for(char i=0;i<5;i++)
        {
            usb_buf[i*3+2]=color_data[(63+i)*3+1];//G
            usb_buf[i*3+3]=color_data[(63+i)*3+0];//R
            usb_buf[i*3+4]=color_data[(63+i)*3+2];//B
        }
        hid_write(dev, (unsigned char *)usb_buf, 65);

    }
}






