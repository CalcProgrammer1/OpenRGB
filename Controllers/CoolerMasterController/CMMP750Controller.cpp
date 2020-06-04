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
    int tmp_size    = wcslen(_vendor);
    dev             = dev_handle;
    location        = _path;

    for (int i=0; ( i<tmp_size && i<CM_DEVICE_NAME_SIZE); i++)
    {
        device_name[i] = (char)_vendor[i];
    }
    for (int j=0; ( j<wcslen(_vendor) && tmp_size+j<CM_DEVICE_NAME_SIZE); j++)
        device_name[tmp_size+j] = (char)_device_name[j];

    GetStatus();        //When setting up device get current status
}

CMMP750Controller::~CMMP750Controller()
{
    hid_close(dev);
}

void CMMP750Controller::GetStatus()
{
    unsigned char buffer[0x40]  = { 0x00 };
    int buffer_size             = (sizeof(buffer) / sizeof(buffer[0]));
    buffer[0]                   = 0x07;

    hid_write(dev, buffer, buffer_size);
    hid_read(dev, buffer, buffer_size);

    if( buffer[0] == 0x80 && buffer[1] == 0x05 )
    {
        current_mode  = buffer[2] - 1;
        current_red   = buffer[3];
        current_green = buffer[4];
        current_blue  = buffer[5];

        for( int i = 0; speed_mode_data[current_mode][i] >= buffer[6]; i++)
            current_speed = i;
    }
    else
    {
        //Code should never reach here however just in case there is a failure set something
        current_mode = MP750_MODE_COLOR_CYCLE;      //Unicorn Spew
        current_red   = 0xFF;
        current_green = 0xFF;
        current_blue  = 0xFF;
        current_speed = MP750_SPEED_NORMAL;
    }
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

unsigned char CMMP750Controller::GetMode()
{
    return current_mode;
}

unsigned char CMMP750Controller::GetLedRed()
{
    return current_red;
}

unsigned char CMMP750Controller::GetLedGreen()
{
    return current_green;
}

unsigned char CMMP750Controller::GetLedBlue()
{
    return current_blue;
}

unsigned char CMMP750Controller::GetLedSpeed()
{
    return current_speed;
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