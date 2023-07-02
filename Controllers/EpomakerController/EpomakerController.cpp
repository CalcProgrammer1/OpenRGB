/*-------------------------------------------------------------------*\
|  EpomakerController.cpp                                             |
|                                                                     |
|  Driver for Epomaker Keyboard                                       |
|                                                                     |
|  Alvaro Munoz (alvaromunoz)          2023-06-05                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "EpomakerController.h"
#include <math.h>

EpomakerController::EpomakerController(hid_device* dev_handle, char *_path)
{
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    dev = dev_handle;

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    location = _path;

    current_mode  = Epomaker_MODE_ALWAYS_ON;
    current_speed = Epomaker_SPEED_DEFAULT;
    current_brightness = Epomaker_BRIGHTNESS_DEFAULT;
    current_dazzle = Epomaker_OPTION_DAZZLE_OFF;
    current_option = Epomaker_OPTION_DEFAULT;

}

EpomakerController::~EpomakerController()
{
    hid_close(dev);
}

std::string EpomakerController::GetDeviceName()
{
    return (device_name);
}

std::string EpomakerController::GetSerial()
{
    return (serial);
}

std::string EpomakerController::GetLocation()
{
    return ("HID: " + location);
}

void EpomakerController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    current_mode  = mode;
    current_speed = speed;
    current_brightness = brightness;

    SendUpdate();
}

void EpomakerController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void EpomakerController::SetDazzle(bool is_dazzle)
{
    if(is_dazzle)
    {
        current_dazzle = Epomaker_OPTION_DAZZLE_ON;
    }
    else
    {
        current_dazzle = Epomaker_OPTION_DAZZLE_OFF;
    }
}

void EpomakerController::SetOption(unsigned char option)
{
    current_option = option;
}

void EpomakerController::SendUpdate()
{
    unsigned char command[Epomaker_PACKET_LENGTH] = { 0x00 };
    command[0] = 0xf6;
    command[1] = 0x0a;

    unsigned char buffer[Epomaker_PACKET_LENGTH] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[Epomaker_BYTE_COMMAND]    = Epomaker_COMMAND_RGB;
    buffer[Epomaker_BYTE_MODE]       = current_mode;
    buffer[Epomaker_BYTE_SPEED]      = current_speed;
    buffer[Epomaker_BYTE_BRIGHTNESS] = current_brightness;
    buffer[Epomaker_BYTE_FLAGS]      = current_option | current_dazzle;
    buffer[Epomaker_BYTE_RED]        = current_red;
    buffer[Epomaker_BYTE_GREEN]      = current_green;
    buffer[Epomaker_BYTE_BLUE]       = current_blue;

    int sum_bits = 0;
    for (int i=Epomaker_BYTE_COMMAND; i<=Epomaker_BYTE_BLUE; i++)
    {
        sum_bits += buffer[i];
    }

    int next_pow2 = pow(2, ceil(log2(sum_bits)));
    int filler = next_pow2 - sum_bits - 1;

    buffer[Epomaker_BYTE_FILLER]     = filler;

    hid_send_feature_report(dev, command, buffer_size);
    hid_send_feature_report(dev, buffer, buffer_size);
}
