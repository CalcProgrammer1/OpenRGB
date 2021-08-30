/*-----------------------------------------*\
|  SteelSeriesRival3Controller.h            |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival 3 lighting controller              |
|                                           |
|  B Horn (bahorn) 29/8/2021                |
\*-----------------------------------------*/

#include "SteelSeriesRival3Controller.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_write(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete[] usb_pkt;
}

SteelSeriesRival3Controller::SteelSeriesRival3Controller
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;

}

SteelSeriesRival3Controller::~SteelSeriesRival3Controller()
{
    hid_close(dev);
}

std::string SteelSeriesRival3Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesRival3Controller::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesRival3Controller::GetSerialString()
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

std::string SteelSeriesRival3Controller::GetFirmwareVersion()
{
    std::string return_string;
    char usb_buf[2];
    uint16_t version;
    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0x00] = 0x10;

    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
    
    hid_read(dev, (unsigned char *)&version, 2);

    return_string = std::to_string(version);
    return return_string;
}

steelseries_type SteelSeriesRival3Controller::GetMouseType()
{
    return proto;
}

void SteelSeriesRival3Controller::Save()
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x09;
    
    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}

void SteelSeriesRival3Controller::SetLightEffectAll
    (
    unsigned char   effect,
    unsigned char   speed
    )
{
    char usb_buf[3];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x06;
    usb_buf[0x01] = 0x00;
    
    switch (effect) {
        case STEELSERIES_RIVAL_3_DIRECT:
            usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_DIRECT;
            break;

        case STEELSERIES_RIVAL_3_BREATHING:
            /*---------------------------------------------*\
            | Effect type is based on which speed is used   |
            \*---------------------------------------------*/
            switch (speed) {
               case  0:
                    usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_BREATHING_MIN;
                    break;
                case 1:
                    usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_BREATHING_MID;
                    break;
                case 2:
                    usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_BREATHING_MAX;
                    break;
                default:
                    return;
            }
            break;

        case STEELSERIES_RIVAL_3_SPECTRUM_CYCLE:
            usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_SPECTRUM_CYCLE;
            break;

        case STEELSERIES_RIVAL_3_RAINBOW_BREATHING:
            usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_RAINBOW_BREATHING;
            break;

        case STEELSERIES_RIVAL_3_DISCO:
            usb_buf[0x02] = STEELSERIES_RIVAL_3_EFFECT_DISCO;
            break;

        default:
            return;
    }

    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}

void SteelSeriesRival3Controller::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue,
    unsigned char   brightness
    )
{
    char usb_buf[7];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00] = 0x05;
    usb_buf[0x01] = 0x00;
    usb_buf[0x02] = zone_id;

    usb_buf[0x03] = red;
    usb_buf[0x04] = green;
    usb_buf[0x05] = blue;
    usb_buf[0x06] = brightness;

    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}

void SteelSeriesRival3Controller::SetColorAll
    (
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
    )
{
    char usb_buf[32];
    memset(usb_buf, 0x00, sizeof(usb_buf));
   
    usb_buf[0x00] = 0x0a;
    usb_buf[0x01] = 0x00;
    usb_buf[0x02] = 0x0f;
    
    for (int i = 1; i <= 4; i++) {
        usb_buf[0x03 * i]           = red;
        usb_buf[0x03 * i + 0x01]    = green;
        usb_buf[0x03 * i + 0x02]    = blue;
    }

    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}
