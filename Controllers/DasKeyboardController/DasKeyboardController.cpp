/*---------------------------------------------------------*\
| DasKeyboardController.cpp                                 |
|                                                           |
|   Driver for Das Keyboard keyboard                        |
|                                                           |
|   Frank Niessen (denk_mal)                    16 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "DasKeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

DasKeyboardController::DasKeyboardController(hid_device *dev_handle, const char *path, std::string dev_name)
{
    dev                    = dev_handle;
    location               = path;
    name                   = dev_name;
    version                = "";
    useTraditionalSendData = false;

    SendInitialize();
}

DasKeyboardController::~DasKeyboardController()
{
    hid_close(dev);
}

std::string DasKeyboardController::GetLayoutString()
{
    /*-----------------------------------------------------------*\
    | Experimental for now; should be '16 or 63' for US and '28'  |
    | for EU layout                                               |
    \*-----------------------------------------------------------*/
    if(version.length() < 17)
    {
        return("NONE");
    }
    std::string layout_id = version.substr(3, 2);

    if(layout_id == "16" || layout_id == "63")
    {
        return("US");
    }

    return("EU");
}

std::string DasKeyboardController::GetLocationString()
{
    return("HID: " + location);
}

std::string DasKeyboardController::GetNameString()
{
    return(name);
}

std::string DasKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string DasKeyboardController::GetVersionString()
{
    if(version.length() < 17)
    {
        return(version);
    }

    std::string fw_version  = "V";
    fw_version             += version.substr(6, 2);
    fw_version             += ".";
    fw_version             += version.substr(15, 2);
    fw_version             += ".0";

    return(fw_version);
}

void DasKeyboardController::SendColors(unsigned char key_id, unsigned char mode,
                                       unsigned char red, unsigned char green, unsigned char blue)
{
    if(key_id < 130)
    {
        unsigned char usb_buf[] = {0xEA,
                                   0x08,
                                   0x78,
                                   0x08,
                                   static_cast<unsigned char>(key_id),
                                   mode,
                                   red,
                                   green,
                                   blue};

        SendData(usb_buf, sizeof(usb_buf));
    }
    else
    {
        /*-----------------------------------------------------*\
        | Special handling for the Q-Button; only color, no mode|
        \*-----------------------------------------------------*/
        unsigned char usb_buf[] = {0xEA,
                                   0x06,
                                   0x78,
                                   0x06,
                                   red,
                                   green,
                                   blue};

        SendData(usb_buf, sizeof(usb_buf));
    }
}


void DasKeyboardController::SendInitialize()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    int cnt_receive = 0;

    while(!cnt_receive)
    {
        /*-----------------------------------------------------*\
        | Set up Initialize connection                          |
        \*-----------------------------------------------------*/
        unsigned char usb_init[] = {0xEA, 0x02, 0xB0};
        SendData(usb_init, sizeof(usb_init));

        /*-----------------------------------------------------*\
        | Get Version String                                    |
        \*-----------------------------------------------------*/
        cnt_receive = ReceiveData(usb_buf, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | check if the faster modern transfer method is working |
        \*-----------------------------------------------------*/
        if(!cnt_receive)
        {
            if(useTraditionalSendData)
            {
                break;
            }
            useTraditionalSendData = true;
        }
    }

    std::string fw_version(reinterpret_cast<char *>(&usb_buf[2]));
    version = fw_version;
}

void DasKeyboardController::SendApply()
{
    /*-----------------------------------------------------*\
    | Set up Terminate Color packet                         |
    \*-----------------------------------------------------*/
    unsigned char usb_buf_send[]    = {0xEA, 0x03, 0x78, 0x0a};
    unsigned char usb_buf_receive[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    SendData(usb_buf_send, sizeof(usb_buf_send));
    ReceiveData(usb_buf_receive, sizeof(usb_buf_receive));
}

void DasKeyboardController::SendData(const unsigned char *data, const unsigned int length)
{
    if(useTraditionalSendData)
    {
        SendDataTraditional(data, length);
    }
    else
    {
        SendDataModern(data, length);
    }
}

void DasKeyboardController::SendDataModern(const unsigned char *data, const unsigned int length)
{
    /*-----------------------------------------------------*\
    | modern SendData (send whole bytes in one transfer)    |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];

    unsigned int err_cnt = 3;
    int          res     = -1;
    while(res == -1)
    {
        /*-----------------------------------------------------*\
        | Fill data into send buffer                            |
        \*-----------------------------------------------------*/
        unsigned int chk_sum = 0;
        usb_buf[0] = 1;

        for(unsigned int idx = 0; idx < length; idx++)
        {
            usb_buf[idx + 1] = data[idx];
            chk_sum ^= data[idx];
        }
        usb_buf[length + 1] = chk_sum;

        res = hid_send_feature_report(dev, usb_buf, length + 2);
        if(res == -1)
        {
            if(!err_cnt--)
            {
                return;
            }
        }
        /*-----------------------------------------------------*\
        | Hack to work around a firmware bug in v21.27.0        |
        \*-----------------------------------------------------*/
        std::this_thread::sleep_for(0.3ms);
    }
}

void DasKeyboardController::SendDataTraditional(const unsigned char *data, const unsigned int length)
{
    /*-----------------------------------------------------*\
    | traditional SendData (split into chunks of 8 byte)    |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Fill data into send buffer                            |
    \*-----------------------------------------------------*/
    unsigned int err_cnt = 3;
    unsigned int chk_sum = 0;
    usb_buf[8] = 0;

    for(unsigned int idx = 0; idx < length + 1; idx += 7)
    {
        usb_buf[0] = 1;
        for(unsigned int fld_idx = 1; fld_idx < 8; fld_idx++)
        {
            unsigned int tmp_idx = idx + fld_idx - 1;
            if(tmp_idx < length)
            {
                usb_buf[fld_idx] = data[tmp_idx];
                chk_sum ^= data[tmp_idx];
            }
            else if(tmp_idx == length)
            {
                usb_buf[fld_idx] = chk_sum;
            }
            else
            {
                usb_buf[fld_idx] = 0;
            }
        }
        int res = hid_send_feature_report(dev, usb_buf, 8);
        if(res == -1)
        {
            idx = 0;
            if(!err_cnt--)
            {
                return;
            }
        }

        /*-----------------------------------------------------*\
        | Hack to work around a firmware bug in v21.27.0        |
        \*-----------------------------------------------------*/
        std::this_thread::sleep_for(0.3ms);
    }
}

int DasKeyboardController::ReceiveData(unsigned char *data, const unsigned int max_length)
{
    unsigned char              usb_buf[9];
    std::vector<unsigned char> receive_buf;

    /*-----------------------------------------------------*\
    | Fill data from receive buffer                         |
    \*-----------------------------------------------------*/
    unsigned int chk_sum = 0;

    do
    {
        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[0x00] = 0x01;

        int res = hid_get_feature_report(dev, usb_buf, 8);
        if(res == -1)
        {
            break;
        }

        if(usb_buf[0])
        {
            for(unsigned int ii = 0; ii < 8; ii++)
            {
                receive_buf.push_back(usb_buf[ii]);
                chk_sum ^= usb_buf[ii];
            }
        }
    } while(usb_buf[0]);

    /*-----------------------------------------------------*\
    | clean up data buffer                                  |
    \*-----------------------------------------------------*/
    for(unsigned int ii = 0; ii < max_length; ii++)
    {
        data[ii] = 0;
    }

    /*-----------------------------------------------------*\
    | If checksum is not correct, return with empty buffer  |
    \*-----------------------------------------------------*/
    if(chk_sum)
    {
        return(-1);
    }

    unsigned int response_size = 0;
    if(receive_buf.size() > 1)
    {
        response_size = receive_buf.at(1);

        if(response_size + 2 > receive_buf.size())
        {
            return(-1);
        }
        if(response_size > max_length)
        {
            response_size = max_length;
        }

        /*-----------------------------------------------------*\
        | Remove first two bytes (signature?) and content length|
        \*-----------------------------------------------------*/
        for(unsigned int ii = 0; ii < response_size - 1; ii++)
        {
            data[ii] = receive_buf.at(ii + 2);
        }
    }

    return(response_size);
}
