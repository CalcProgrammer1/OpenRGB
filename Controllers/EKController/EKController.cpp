/*-------------------------------------------------------------------*\
|  EKController.cpp                                                   |
|                                                                     |
|  Driver for EK Loop Connect                                         |
|                                                                     |
|  Chris M (Dr_No)          16th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "EKController.h"

EKController::EKController(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path)
{
    int tmp_size = wcslen(_vendor);

    dev = dev_handle;

    for(int i=0; (i < tmp_size) && (i < EK_DEVICE_NAME_SIZE); i++)
    {
        device_name[i] = (char)_vendor[i];
    }

    for(int j=0; (j < wcslen(_vendor)) && ((tmp_size + j) < EK_DEVICE_NAME_SIZE); j++)
    {
        device_name[tmp_size+j] = (char)_device_name[j];
    }

    location = _path;

    current_mode = EK_MODE_STATIC;
    current_speed = EK_SPEED_NORMAL;
}

EKController::~EKController()
{
    hid_close(dev);
}

char* EKController::GetDeviceName()
{
    return device_name;
}

char* EKController::GetSerial()
{
    return serial;
}

std::string EKController::GetLocation()
{
    return location;
}

void EKController::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode  = mode;
    current_speed = speed;

    SendUpdate();
}

void EKController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void EKController::SendUpdate()
{
    unsigned char buffer[EK_PACKET_LENGTH] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(int i = 0; i < EK_COLOUR_MODE_DATA_SIZE; i++)
    {
        buffer[i] = colour_mode_data[current_mode][i];
    }

    //Set the relevant colour info
    buffer[EK_RED_BYTE] = current_red;
    buffer[EK_GREEN_BYTE] = current_green;
    buffer[EK_BLUE_BYTE] = current_blue;
    buffer[EK_SPEED_BYTE] = speed_mode_data[current_mode][current_speed];

    buffer[10] = 0x10;
    buffer[47] = 0xFF;
    buffer[48] = 0x00;

    hid_write(dev, buffer, buffer_size);
}
