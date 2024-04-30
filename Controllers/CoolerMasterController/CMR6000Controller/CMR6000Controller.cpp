/*-------------------------------------------------------------------*\
|  CMR6000Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster based AMD Radeon GPU (6000 series)         |
|                                                                     |
|  Eric S (edbgon)          2nd Feb 2021                              |
\*-------------------------------------------------------------------*/

#include "CMR6000Controller.h"
#include <cstring>

CMR6000Controller::CMR6000Controller(hid_device* dev_handle, char *_path, uint16_t _pid)
{
    dev             = dev_handle;
    location        = _path;
    pid             = _pid;

    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name        = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName  = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

}

CMR6000Controller::~CMR6000Controller()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string CMR6000Controller::GetDeviceName()
{
    return device_name;
}

std::string CMR6000Controller::GetSerial()
{
    return serial;
}

std::string CMR6000Controller::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMR6000Controller::GetMode()
{
    return current_mode;
}

unsigned char CMR6000Controller::GetLedSpeed()
{
    return current_speed;
}

unsigned char CMR6000Controller::GetBrightness()
{
    return current_brightness;
}

bool CMR6000Controller::GetRandomColours()
{
    return current_random;
}

uint16_t CMR6000Controller::GetPID()
{
    return pid;
}

void CMR6000Controller::SetMode(unsigned char mode, unsigned char speed, RGBColor color1, RGBColor color2, unsigned char random, unsigned char brightness)
{
    current_mode        = mode;
    current_speed       = speed;
    primary             = color1;
    secondary           = color2;
    current_random      = random;
    current_brightness  = brightness;

    SendUpdate();
}

void CMR6000Controller::SendUpdate()
{
    if(current_mode == CM_MR6000_MODE_OFF)
    {
        unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00, 0x41, 0x43 };
        int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));
        hid_write(dev, buffer, buffer_size);
    }
    else
    {
        SendEnableCommand();

        if(pid == COOLERMASTER_RADEON_6900_PID)
        {
            SendSecondColour();
        }

        unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00 };
        int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));
        memset(buffer, 0xFF, buffer_size);

        buffer[0x00] = 0x00;
        buffer[0x01] = 0x51;
        buffer[0x02] = 0x2C;
        buffer[0x03] = 0x01;
        buffer[0x04] = 0x00;
        buffer[0x05] = current_mode;
        buffer[0x06] = current_speed;
        buffer[0x07] = current_random; //random (A0)
        //buffer[0x09] = 0xFF;
        buffer[0x0A] = current_brightness;
        buffer[0x0B] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : RGBGetRValue(primary);
        buffer[0x0C] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : RGBGetGValue(primary);
        buffer[0x0D] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : RGBGetBValue(primary);
        buffer[0x0E] = 0x00;
        buffer[0x0F] = 0x00;
        buffer[0x10] = 0x00;

        /*-----------------------------------------------------------------*\
        | Index 0x08 looks to be mode specific flags / options              |
        \*-----------------------------------------------------------------*/
        switch(current_mode)
        {
            case CM_MR6000_MODE_BREATHE:
                buffer[0x08] = 0x03;
                break;
            case CM_MR6000_MODE_RAINBOW:
                buffer[0x08] = 0x05;
                break;
            case CM_MR6000_MODE_CHASE:
                buffer[0x08] = 0xC3;
                break;
            case CM_MR6000_MODE_SWIRL:
                buffer[0x08] = 0x4A;
                break;
            default:
                buffer[0x08] = 0xFF;
        }


        hid_write(dev, buffer, buffer_size);

        SendColourConfig();
        SendApplyCommand();
    }
}

void CMR6000Controller::SendEnableCommand()
{
    unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00, 0x41, 0x80 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_6K_INTERRUPT_TIMEOUT);
}

void CMR6000Controller::SendApplyCommand()
{
    unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00, 0x51, 0x28, 0x00, 0x00, 0xE0 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_6K_INTERRUPT_TIMEOUT);
}

void CMR6000Controller::SendColourConfig()
{
    unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00, 0x51, 0xA0, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x05, 0x06 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(int i = 0x0B; i < 0x1A; i++)
    {
        buffer[i] = current_mode;
    }

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_6K_INTERRUPT_TIMEOUT);
}

void CMR6000Controller::SendSecondColour()
{
    unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00, 0x51, 0x9C, 0x01, 0x00 };

    buffer[5]   = RGBGetRValue(primary);
    buffer[6]   = RGBGetGValue(primary);
    buffer[7]   = RGBGetBValue(primary);
    buffer[8]   = RGBGetRValue(secondary);
    buffer[9]   = RGBGetGValue(secondary);
    buffer[10]  = RGBGetBValue(secondary);

    hid_write(dev, buffer, CM_6K_PACKET_SIZE);
    hid_read_timeout(dev, buffer, CM_6K_PACKET_SIZE, CM_6K_INTERRUPT_TIMEOUT);
}
