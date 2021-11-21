/*-----------------------------------------*\
|  AsusAuraStrixEvolveController.cpp        |
|                                           |
|  Driver for ASUS Aura RGB USB             |
|  lighting controller                      |
|                                           |
|  Mola19 11/30/2021                        |
\*-----------------------------------------*/

#include "AsusAuraStrixEvolveController.h"

#include <cstring>
#include <chrono>
#include <thread>

AuraStrixEvolveController::AuraStrixEvolveController(hid_device* dev_handle, const char* path, uint16_t pid)
{
    dev         = dev_handle;
    location    = path;
    device_pid  = pid;
}

AuraStrixEvolveController::~AuraStrixEvolveController()
{
    hid_close(dev);
}

std::string AuraStrixEvolveController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraStrixEvolveController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string AuraStrixEvolveController::GetVersion()
{
    unsigned char usb_buf[9] = { 0x0c, 0xc4, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    hid_send_feature_report(dev, usb_buf, 9);

    unsigned char usb_buf_out[9] = { 0x0c };
    hid_get_feature_report(dev, usb_buf_out, 9);

    return std::string("1." +  std::to_string(usb_buf_out[3]));
}

int AuraStrixEvolveController::GetActiveProfile()
{
    int profile;

    do
    {
        unsigned char usb_buf[9] = { 0x0c, 0xdf, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        hid_send_feature_report(dev, usb_buf, 9);

        unsigned char usb_buf_out[9] = { 0x0c };
        hid_get_feature_report(dev, usb_buf_out, 9);

        profile = usb_buf_out[4] % 16;
    } while (profile > 2);

    return profile + 1;
}

void AuraStrixEvolveController::SendUpdate
    (
    unsigned char   key,
    unsigned char   value
    )
{
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, 9);

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x0c;
    usb_buf[0x01]   = 0xc4;
    usb_buf[0x02]   = 0x0f;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = key;
    usb_buf[0x05]   = value;
    hid_send_feature_report(dev, usb_buf, 9);
}


void AuraStrixEvolveController::UpdateProfile
    (
    unsigned char   key,
    unsigned char   profile,
    unsigned char   value
    )
{
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, 9);

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x0c;
    usb_buf[0x01]   = 0xde;
    usb_buf[0x02]   = key;
    usb_buf[0x03]   = profile;
    usb_buf[0x04]   = value;
    hid_send_feature_report(dev, usb_buf, 9);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void AuraStrixEvolveController::SendSavePacket()
{
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, 9);

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x0c;
    usb_buf[0x01]   = 0xc4;

    hid_send_feature_report(dev, usb_buf, 9);
}
