/*-----------------------------------------*\
|  DasKeyboardController.cpp                |
|                                           |
|  Driver for Das Keyboard RGB keyboard     |
|  lighting controller                      |
|                                           |
|  Frank Niessen (denk_mal) 12/16/2020      |
\*-----------------------------------------*/

#include <cstring>
#include "DasKeyboardController.h"

using namespace std::chrono_literals;

DasKeyboardController::DasKeyboardController(hid_device *dev_handle, const char *path)
{
    dev                     = dev_handle;
    location                = path;
    version                 = "";
    useTraditionalSendData  = false;

    SendInitialize();
}

DasKeyboardController::~DasKeyboardController()
{
    hid_close(dev);
}

std::string DasKeyboardController::GetDeviceLocation()
{
    return "HID: " + location;
}

std::string DasKeyboardController::GetSerialString()
{
    wchar_t serial_string[128]  = {};
    int err                     = hid_get_serial_number_string(dev, serial_string, 128);

    std::string return_string;
    if(!err)
    {
        std::wstring return_wstring = serial_string;
        return_string = std::string(return_wstring.begin(), return_wstring.end());
    }

    if(return_string.empty())
    {
        return_string = version;
    }

    return return_string;
}

std::string DasKeyboardController::GetVersionString()
{
    if(version.length() < 17)
    {
        return version;
    }

    std::string fw_version  = "V";
    fw_version             += version.substr(6, 2);
    fw_version             += ".";
    fw_version             += version.substr(15, 2);
    fw_version             += ".0";

    return fw_version;
}

std::string DasKeyboardController::GetLayoutString()
{
    /*-----------------------------------------------------*\
    | Experimental for now; should be '16' for US and '28'  |
    | for EU layout                                         |
    \*-----------------------------------------------------*/
    if(version.length() < 17)
    {
        return "NONE";
    }
    std::string layout_id = version.substr(3, 2);

    if(layout_id == "16")
    {
        return "US";
    }

    return "EU";
}

void DasKeyboardController::SendColors(unsigned char key_id, unsigned char mode,
                                       unsigned char red, unsigned char green, unsigned char blue)
{
    if (key_id < 130)
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
        cnt_receive = ReceiveData(usb_buf);

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

    do
    {
        SendData(usb_buf_send, sizeof(usb_buf_send));
        ReceiveData(usb_buf_receive);
    } while(usb_buf_receive[0] == 0);
}

void DasKeyboardController::SendData(const unsigned char *data, unsigned int length)
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

void DasKeyboardController::SendDataModern(const unsigned char *data, unsigned int length)
{
    /*-----------------------------------------------------*\
    | modern SendData (send whole bytes in one transfer)    |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];

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

    usb_buf[++length] = chk_sum;
    length++;

    hid_send_feature_report(dev, usb_buf, length);

    /*-----------------------------------------------------*\
    | Hack to work around a firmware bug in v21.27.0        |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(0.3ms);
}

void DasKeyboardController::SendDataTraditional(const unsigned char *data, unsigned int length)
{
    /*-----------------------------------------------------*\
    | traditional SendData (split into chunks of 8 byte)    |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Fill data into send buffer                            |
    \*-----------------------------------------------------*/
    unsigned int chk_sum = 0;
    usb_buf[8] = 0;

    for(unsigned int idx = 0; idx < length + 1; idx += 7)
    {
        usb_buf[0] = 1;
        for (unsigned int fld_idx = 1; fld_idx < 8; fld_idx++)
        {
            unsigned int tmp_idx = idx + fld_idx - 1;
            if (tmp_idx < length)
            {
                usb_buf[fld_idx] = data[tmp_idx];
                chk_sum ^= data[tmp_idx];
            } else if (tmp_idx == length)
            {
                usb_buf[fld_idx] = chk_sum;
            } else
            {
                usb_buf[fld_idx] = 0;
            }
        }
        hid_send_feature_report(dev, usb_buf, 8);

        /*-----------------------------------------------------*\
        | Hack to work around a firmware bug in v21.27.0        |
        \*-----------------------------------------------------*/
        std::this_thread::sleep_for(0.3ms);
    }
}

int DasKeyboardController::ReceiveData(unsigned char *data)
{
    int idx = 0;
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Fill data from receive buffer                         |
    \*-----------------------------------------------------*/
    unsigned int chk_sum = 0;

    do
    {
        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[0x00] = 0x01;

        hid_get_feature_report(dev, usb_buf, 8);

        if(usb_buf[0])
        {
            for(unsigned int ii = 0; ii < 8; ii++)
            {
                data[idx++] = usb_buf[ii];
                chk_sum ^= usb_buf[ii];
            }
        }
    } while (usb_buf[0]);

    /*-----------------------------------------------------*\
    | If checksum is not correct, clean up data buffer      |
    \*-----------------------------------------------------*/
    if(chk_sum)
    {
        for (int ii = 0; ii < idx; ii++)
        {
            data[ii] = 0;
        }
        return -1;
    }

    if(idx)
    {
        idx = data[1];

        /*-----------------------------------------------------*\
        | Remove first two bytes (signature?) and content length|
        \*-----------------------------------------------------*/
        for(int ii = 0; ii < idx - 1; ii++)
        {
            data[ii] = data[ii + 2];
        }

        /*-----------------------------------------------------*\
        | Remove checksum                                       |
        \*-----------------------------------------------------*/
        data[idx + 1] = 0;

        /*-----------------------------------------------------*\
        | Remove duplicate bytes at the end                     |
        \*-----------------------------------------------------*/
        data[idx--] = 0;
        data[idx] = 0;
    }

    return idx;
}
