/*-------------------------------------------------------------------*\
|  CMR6000Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster based AMD Radeon GPU (6000 series)         |
|                                                                     |
|  Eric S (edbgon)          2nd Feb 2021                              |
\*-------------------------------------------------------------------*/

#include "CMR6000Controller.h"
#include <cstring>

CMR6000Controller::CMR6000Controller(hid_device* dev_handle, char *_path)
{
    dev             = dev_handle;
    location        = _path;

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

    GetStatus();        //When setting up device get current status
}

CMR6000Controller::~CMR6000Controller()
{
    if(dev)
    {
        hid_close(dev);
    }
}

void CMR6000Controller::GetStatus()
{
    unsigned char buffer[CM_6K_PACKET_SIZE]     = { 0x00, 0x52, 0xA0, 0x01, 0x00, 0x00, 0x03 };
    int buffer_size                             = (sizeof(buffer) / sizeof(buffer[0]));

    // Request mode
    hid_write(dev, buffer, buffer_size);
    hid_read(dev, buffer, buffer_size);

    unsigned char cmdbuffer[CM_6K_PACKET_SIZE]  = { 0x00, 0x52, 0x2C, 0x01, 0x00 };
    int cmdbuffer_size                          = (sizeof(cmdbuffer) / sizeof(cmdbuffer[0]));

    current_mode    = buffer[0x0A];
    cmdbuffer[0x05] = current_mode;
    hid_write(dev, cmdbuffer, cmdbuffer_size);
    hid_read(dev, cmdbuffer, cmdbuffer_size);

    current_speed       = cmdbuffer[0x05];
    current_brightness  = cmdbuffer[0x09];
    current_red         = cmdbuffer[0x0A];
    current_green       = cmdbuffer[0x0B];
    current_blue        = cmdbuffer[0x0C];
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

unsigned char CMR6000Controller::GetLedRed()
{
    return current_red;
}

unsigned char CMR6000Controller::GetLedGreen()
{
    return current_green;
}

unsigned char CMR6000Controller::GetLedBlue()
{
    return current_blue;
}

unsigned char CMR6000Controller::GetLedSpeed()
{
    return current_speed;
}

bool CMR6000Controller::GetRandomColours()
{
    return current_random;
}

void CMR6000Controller::SetMode(unsigned char mode, unsigned char speed, unsigned char red, unsigned char green, unsigned char blue, unsigned char random)
{
    current_mode        = mode;
    current_speed       = speed;
    current_red         = red;
    current_green       = green;
    current_blue        = blue;
    current_random      = random;
    current_brightness  = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0x7F : 0xFF;    //Color_Cycle brightness needs to be clamped to 0x7F to avoid wash out

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

        unsigned char buffer[CM_6K_PACKET_SIZE] = { 0x00 };
        int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));
        memset(buffer, 0xFF, buffer_size);

        buffer[0x00] = 0x00;
        buffer[0x01] = 0x51;
        buffer[0x02] = 0x2C;
        buffer[0x03] = 0x01;
        buffer[0x04] = 0x00;
        buffer[0x05] = current_mode;
        buffer[0x06] = (current_mode == CM_MR6000_MODE_STATIC) ? 0xFF: current_speed;
        buffer[0x07] = (current_mode == CM_MR6000_MODE_BREATHE)? current_random : 0x00; //random (A0)
        buffer[0x08] = (current_mode == CM_MR6000_MODE_BREATHE)? 0x03 : 0xFF;
        //buffer[0x09] = 0xFF;
        buffer[0x0A] = current_brightness;
        buffer[0x0B] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : current_red;
        buffer[0x0C] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : current_green;
        buffer[0x0D] = (current_mode == CM_MR6000_MODE_COLOR_CYCLE) ? 0xFF : current_blue;
        buffer[0x0E] = 0x00;
        buffer[0x0F] = 0x00;
        buffer[0x10] = 0x00;

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
