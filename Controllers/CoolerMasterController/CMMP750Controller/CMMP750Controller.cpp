/*---------------------------------------------------------*\
| CMMP750Controller.cpp                                     |
|                                                           |
|   Driver for Cooler Master MP750 mousemat                 |
|                                                           |
|   Chris M (Dr_No)                             16 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CMMP750Controller.h"
#include "StringUtils.h"

static unsigned char colour_mode_data[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },                 /* Off                          */
    { 0x01, 0x04, 0xFF, 0x00, 0xFF, 0x00 },                 /* Static                       */
    { 0x02, 0x04, 0xFF, 0x00, 0xFF, 0x80 },                 /* Blinking                     */
    { 0x03, 0x04, 0xFF, 0x00, 0xFF, 0x80 },                 /* Breathing                    */
    { 0x04, 0x04, 0x80, 0x00, 0x00, 0x00 },                 /* Colour Cycle                 */
    { 0x05, 0x04, 0x80, 0x00, 0x00, 0x00 }                  /* Colour Breath                */
};

static unsigned char speed_mode_data[9] =
{
    0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00    /* Speed Definition             */
};

CMMP750Controller::CMMP750Controller(hid_device* dev_handle, char *_path)
{
    dev             = dev_handle;
    location        = _path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    GetStatus();        //When setting up device get current status
}

CMMP750Controller::~CMMP750Controller()
{
    hid_close(dev);
}

void CMMP750Controller::GetStatus()
{
    unsigned char buffer[0x41]  = { 0x00 };
    int buffer_size             = (sizeof(buffer) / sizeof(buffer[0]));
    buffer[1]                   = 0x07;

    hid_write(dev, buffer, buffer_size);
    hid_read(dev, buffer, buffer_size);

    if((buffer[0] == 0x80) && (buffer[1] == 0x05))
    {
        current_mode  = buffer[2];
        current_red   = buffer[3];
        current_green = buffer[4];
        current_blue  = buffer[5];

        for(int i = 0; (speed_mode_data[i] >= buffer[6] && i <= MP750_SPEED_FASTEST); i++)
        {
            current_speed = i;
        }
    }
    else
    {
        //Code should never reach here however just in case there is a failure set something
        current_mode  = CM_MP750_MODE_COLOR_CYCLE;      //Unicorn Spew
        current_red   = 0xFF;
        current_green = 0xFF;
        current_blue  = 0xFF;
        current_speed = MP750_SPEED_NORMAL;
    }
}

std::string CMMP750Controller::GetDeviceName()
{
    return(device_name);
}

std::string CMMP750Controller::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CMMP750Controller::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMMP750Controller::GetMode()
{
    return(current_mode);
}

unsigned char CMMP750Controller::GetLedRed()
{
    return(current_red);
}

unsigned char CMMP750Controller::GetLedGreen()
{
    return(current_green);
}

unsigned char CMMP750Controller::GetLedBlue()
{
    return(current_blue);
}

unsigned char CMMP750Controller::GetLedSpeed()
{
    return(current_speed);
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
    unsigned char buffer[0x41] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(std::size_t i = 0; i < CM_COLOUR_MODE_DATA_SIZE; i++)
    {
        buffer[i+1] = colour_mode_data[current_mode][i];
    }

   if(current_mode > CM_MP750_MODE_BREATHING)
    {
        //If the mode is random colours set SPEED at BYTE2
        buffer[CM_RED_BYTE]     = speed_mode_data[current_speed];
    }
    else
    {
        //Otherwise SPEED is BYTE5
        buffer[CM_RED_BYTE]     = current_red;
        buffer[CM_GREEN_BYTE]   = current_green;
        buffer[CM_BLUE_BYTE]    = current_blue;
        buffer[CM_SPEED_BYTE]   = speed_mode_data[current_speed];
    }

    hid_write(dev, buffer, buffer_size);
}
