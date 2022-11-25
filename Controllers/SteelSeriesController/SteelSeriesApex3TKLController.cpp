/*-----------------------------------------*\
|  SteelSeriesApex3TKLController.cpp        |
|                                           |
|  Paul K. Gerke - 27.10.2022               |
|  Dr_no (Chris M)                          |
\*-----------------------------------------*/

#include "SteelSeriesApex3TKLController.h"
#include <cstring>
#include <LogManager.h>

SteelSeriesApex3TKLController::SteelSeriesApex3TKLController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path,
    const char*         device_name
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
    strcpy(this->device_name, device_name);
}

SteelSeriesApex3TKLController::~SteelSeriesApex3TKLController()
{
    hid_close(dev);
}

std::string SteelSeriesApex3TKLController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesApex3TKLController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesApex3TKLController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if (ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

steelseries_type SteelSeriesApex3TKLController::GetKeyboardType()
{
    return proto;
}

void SteelSeriesApex3TKLController::SetBrightness(unsigned char brightness)
{
    unsigned char buf[STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE];
    memset(buf, 0x00, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x23;
    buf[0x02]           = brightness;
    hid_write(dev, buf, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);
}

unsigned char SteelSeriesApex3TKLController::GetBrightness()
{
    unsigned char buf[STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE];
    memset(buf, 0x00, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);

    buf[0x01]           = 0xA3;
    hid_write(dev, buf, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);

    int read_back = hid_read_timeout(dev, buf, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE, 100);
    if (read_back >= 2 && buf[0x00] == 0xA3)
    {
        return(buf[0x01]);
    }
    LOG_ERROR("[%s] Failed load brightness; defaulting to max", device_name);
    return(STEELSERIES_APEX3TKL_BRIGHTNESS_MAX);
}

void SteelSeriesApex3TKLController::SetRainbowMode()
{
    unsigned char buf[STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);
    buf[0] = 0x00;
    buf[1] = 0x22;
    buf[2] = 0xFF;
    buf[3] = 0x00;
    buf[4] = 0x00;
    hid_write(dev, buf, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);
}

void SteelSeriesApex3TKLController::SetColor(std::vector<RGBColor> colors)
{
    unsigned char buf[STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE];
    memset(buf, 0x00, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);

    buf[0x01]           = 0x21;
    buf[0x02]           = 0xFF;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        uint8_t index   = i * 3;

        buf[index + 3]  = RGBGetRValue(colors[i]);;
        buf[index + 4]  = RGBGetGValue(colors[i]);;
        buf[index + 5]  = RGBGetBValue(colors[i]);;
    }

    hid_write(dev, buf, STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE);
}
