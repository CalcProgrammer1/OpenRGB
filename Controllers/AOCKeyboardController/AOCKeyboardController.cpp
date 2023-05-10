/*-----------------------------------------*\
|  AOCKeyboardController.cpp                |
|                                           |
|  Driver for AOC Keyboard lighting         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/10/2023   |
\*-----------------------------------------*/

#include "AOCKeyboardController.h"

#include <cstring>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

AOCKeyboardController::AOCKeyboardController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AOCKeyboardController::~AOCKeyboardController()
{
    hid_close(dev);
}

std::string AOCKeyboardController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string AOCKeyboardController::GetSerialString()
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

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void AOCKeyboardController::SendDirect
    (
    RGBColor*       color_data
    )
{
    SendPacket(AOC_KEYBOARD_MODE_STATIC,
               AOC_KEYBOARD_SINGLE_COLOR,
               AOC_KEYBOARD_BRIGHTNESS_HIGH,
               AOC_KEYBOARD_SPEED_MEDIUM,
               AOC_KEYBOARD_DIRECTION_CLOCKWISE,
               color_data);
}

void AOCKeyboardController::SendPacket
    (
    unsigned char   mode,
    unsigned char   random,
    unsigned char   brightness,
    unsigned char   speed,
    unsigned char   direction,
    RGBColor*       color_data
    )
{
    unsigned char buf[117];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up RGB configuration packet                       |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x14;
    buf[0x01]   = 0x01;

    buf[0x06]   = mode;

    buf[0x07 + (9 * mode) + 0] = RGBGetRValue(color_data[0]);
    buf[0x07 + (9 * mode) + 1] = RGBGetGValue(color_data[0]);
    buf[0x07 + (9 * mode) + 2] = RGBGetBValue(color_data[0]);
    buf[0x07 + (9 * mode) + 3] = random;
    buf[0x07 + (9 * mode) + 4] = direction;
    buf[0x07 + (9 * mode) + 5] = speed;
    buf[0x07 + (9 * mode) + 6] = brightness;

    unsigned short checksum = 0x4A9E;

    for(unsigned int buf_idx = 0; buf_idx < 115; buf_idx++)
    {
        checksum += buf[buf_idx];
    }

    buf[115]    = checksum & 0xFF;
    buf[116]    = checksum >> 8;


    unsigned char buf3[] =
{
0x09, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
};
    hid_write(dev, buf3, 64);


    std::this_thread::sleep_for(10ms);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 117);

    std::this_thread::sleep_for(10ms);

    unsigned char buf2[] =
{
0x09, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
};
    hid_write(dev, buf2, 64);
}
