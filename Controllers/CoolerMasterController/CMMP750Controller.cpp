/*-------------------------------------------------------------------*\
|  CMMP750Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster MP750 mousepad                             |
|                                                                     |
|  Chris M (Dr_No)          16th Apr 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMMP750Controller.h"

CMMP750Controller::CMMP750Controller(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path)
{
    int tmp_size = wcslen(_vendor);

    dev = dev_handle;

    for (int i=0; ( i<tmp_size && i<CM_DEVICE_NAME_SIZE); i++)
    {
        device_name[i] = (char)_vendor[i];
    }
    for (int j=0; ( j<wcslen(_vendor) && tmp_size+j<CM_DEVICE_NAME_SIZE); j++)
        device_name[tmp_size+j] = (char)_device_name[j];

    location = _path;

    current_mode = MP750_MODE_STATIC;
    current_speed = MP750_SPEED_NORMAL;
}

CMMP750Controller::~CMMP750Controller()
{
    hid_close(dev);
}

char* CMMP750Controller::GetDeviceName()
{
    return device_name;
}

char* CMMP750Controller::GetSerial()
{
    return serial;
}

std::string CMMP750Controller::GetLocation()
{
    return location;
}

void CMMP750Controller::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode  = mode;
    current_speed = speed;

    SendUpdate();
}

void CMMP750Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void CMMP750Controller::SendUpdate()
{
    unsigned char buffer[0x40] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(int i = 0; i < CM_COLOUR_MODE_DATA_SIZE; i++)
    {
        buffer[i] = colour_mode_data[current_mode][i];
    }

   if(current_mode > MP750_MODE_BREATHING)
    { 
        //If the mode is random colours set SPEED at BYTE2
        buffer[CM_RED_BYTE] = speed_mode_data[current_mode][current_speed];
    }
    else
    { 
        //Otherwise SPEED is BYTE5
        buffer[CM_RED_BYTE] = current_red;
        buffer[CM_GREEN_BYTE] = current_green;
        buffer[CM_BLUE_BYTE] = current_blue;
        buffer[CM_SPEED_BYTE] = speed_mode_data[current_mode][current_speed];
    }

    hid_write(dev, buffer, buffer_size);
}