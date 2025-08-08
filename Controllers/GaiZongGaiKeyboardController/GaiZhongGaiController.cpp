/*---------------------------------------------------------*\
| GaiZhongGaiController.cpp                                 |
|                                                           |
|   Driver for GaiZhongGai keyboard                         |
|                                                           |
|   An Yang                                     24 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <thread>
#include "GaiZhongGaiController.h"
#include "StringUtils.h"

/*---------------------------------------------------------------*\
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
| https://oshwhub.com/myng/42-jian-pan                            |
| https://oshwhub.com/hivisme/17jian-shuo-zi-xiao-jian-pan        |
| https://oshwhub.com/yangzen/xing-huo-2-qi-guang-ban-qu-dong-    |
| https://oshwhub.com/morempty/CH552gyin-liang-xuan-niu           |
\*---------------------------------------------------------------*/

GaiZhongGaiKeyboardController::GaiZhongGaiKeyboardController(hid_device* dev_handle, hid_device_info* info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info->path;
    name        = dev_name;
    usb_pid     = info->product_id;
    /*-----------------------------------------------------*\
    | Obtaining the Firmware Version                        |
    \*-----------------------------------------------------*/
    char str[10];
    snprintf(str, 10, "Ver%04X", info->release_number);
    version = str;
    /*-----------------------------------------------------*\
    | Gets the light board connection shape                 |
    \*-----------------------------------------------------*/
    if( usb_pid == GAIZHONGGAI_LIGHT_BOARD_PID )
    {
        uint8_t usb_write_buf[65];
        uint8_t usb_read_buf[65];
        memset(usb_write_buf, 0x00, sizeof(usb_write_buf) );
        memset(usb_read_buf , 0x00, sizeof(usb_read_buf)  );

        usb_write_buf[1] = 0x85;
        usb_write_buf[2] = 0x00;
        usb_write_buf[3] =   60;

        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);
        memcpy(data_flash, usb_read_buf + 3, 60);

        usb_write_buf[2] =   60;
        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);
        memcpy(data_flash + 60, usb_read_buf + 3, 60);

        usb_write_buf[2] =  120;
        usb_write_buf[3] =    8;
        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);
        memcpy(data_flash + 120, usb_read_buf + 3, 8);
    }

    /*-----------------------------------------------------*\
    | Gets the RGB_HUB LED LEN                              |
    \*-----------------------------------------------------*/
    if( usb_pid == GAIZHONGGAI_RGB_HUB_GREEN_PID    ||
        usb_pid == GAIZHONGGAI_RGB_HUB_BLUE_PID
        )
    {
        uint8_t usb_write_buf[65];
        uint8_t usb_read_buf[65];
        memset(usb_write_buf, 0x00, sizeof(usb_write_buf) );
        memset(usb_read_buf , 0x00, sizeof(usb_read_buf)  );
        memset(data_flash   , 0x00, sizeof(data_flash)    );

        usb_write_buf[1] = 0x87;    //Read length command
        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);

        memcpy(data_flash , usb_read_buf + 2 , 16);
    }
}

uint8_t* GaiZhongGaiKeyboardController::GetDataFlash()
{
    return data_flash;
}

uint16_t GaiZhongGaiKeyboardController::GetChannelLen(uint8_t ch)
{
    uint8_t offset;
    offset = ch * 2;
    return (data_flash[offset] << 8) | data_flash[offset + 1];
}

void GaiZhongGaiKeyboardController::SetChannelLen(uint8_t ch , uint16_t len)
{
    uint8_t offset;
    offset = ch * 2;

    if( usb_pid == GAIZHONGGAI_RGB_HUB_GREEN_PID && ch == 3 && len== 637 )
    {
        /*-----------------------------------------------------*\
        | Automatic measurement of quantity                     |
        \*-----------------------------------------------------*/
        uint8_t usb_write_buf[65];
        uint8_t usb_read_buf[65];
        memset(usb_write_buf, 0x00, sizeof(usb_write_buf) );
        memset(usb_read_buf , 0x00, sizeof(usb_read_buf)  );
        usb_write_buf[1] = 0x88;

        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);  //Wait about 10ms

        memcpy(data_flash , usb_read_buf + 2 , 16);
    }
    else if(
            usb_pid == GAIZHONGGAI_RGB_HUB_GREEN_PID    ||
            usb_pid == GAIZHONGGAI_RGB_HUB_BLUE_PID
        )
    {
        uint8_t usb_write_buf[65];
        uint8_t usb_read_buf[65];
        memset(usb_write_buf, 0x00, sizeof(usb_write_buf) );
        memset(usb_read_buf , 0x00, sizeof(usb_read_buf)  );

        usb_write_buf[1]        = 0x86;
        data_flash[offset]      = len >> 8;
        data_flash[offset + 1]  = len & 0xFF;
        memcpy(usb_write_buf + 3 , data_flash , 16);

        hid_write(dev, usb_write_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        memset(usb_write_buf, 0x00, sizeof(usb_write_buf) );
        usb_write_buf[1] = 0x87;    //Read length command

        hid_write(dev, usb_write_buf, 65);
        hid_read (dev, usb_read_buf , 65);

        memcpy(data_flash , usb_read_buf + 2 , 16);
    }
}

GaiZhongGaiKeyboardController::~GaiZhongGaiKeyboardController()
{
    /*-----------------------------------------------------*\
    | Restore built-in light effect                         |
    \*-----------------------------------------------------*/
    uint8_t usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1] = 0xFF;
    hid_write(dev, usb_buf, 65);

    hid_close(dev);
}

std::string GaiZhongGaiKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string GaiZhongGaiKeyboardController::GetNameString()
{
    return(name);
}

std::string GaiZhongGaiKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
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
    unsigned int    color_data_size/*color_data_size*/
    )
{
    uint8_t usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    switch(usb_pid)
    {
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

            memset(usb_buf, 0x00, sizeof(usb_buf)); //Redundant data set 0
            usb_buf[1] = 0x13;
            memcpy(usb_buf + 2, color_data + 63 * 3, 15);
            hid_write(dev, usb_buf, 65);
            break;

        case GAIZHONGGAI_42_PRO_PID:        //42%
            usb_buf[1] = 0x10;
            memcpy(usb_buf + 2, color_data +  0 * 3, 63);
            hid_write(dev, usb_buf, 65);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

            usb_buf[1] = 0x11;
            memcpy(usb_buf + 2, color_data + 21 * 3, 63);
            hid_write(dev, usb_buf, 65);
            break;

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

        case GAIZHONGGAI_DIAL_PID:          //Dial
            usb_buf[1] = 0x10;
            memcpy(usb_buf + 2, color_data + 85 * 3, 63);
            hid_write(dev, usb_buf, 65);
            break;

        case GAIZHONGGAI_LIGHT_BOARD_PID:   //"Cololight"
        case GAIZHONGGAI_RGB_HUB_GREEN_PID: //WS2812 controller
        case GAIZHONGGAI_RGB_HUB_BLUE_PID:  //WS2812 controller
            for(uint8_t i = 0; i < 32; i++) //Maximum up to 640 RGB LED
            {
                if(i > color_data_size / 60)
                {
                    break;
                }

                usb_buf[1] = i;
                for(uint8_t j = 0; j < 60; j++ )
                {
                    if((unsigned int)(i * 60 + j) < color_data_size)
                    {
                        usb_buf[j + 2] = color_data[i * 60 + j];
                    }
                    else
                    {
                        usb_buf[j + 2] = 0; //Redundant data set 0
                    }
                }
                hid_write(dev, usb_buf, 65);
            }
            break;
    }
}
