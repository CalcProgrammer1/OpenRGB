/*---------------------------------------------------------*\
| MSIMysticLight112Controller.cpp                           |
|                                                           |
|   Driver for MSI Mystic Light 112-byte motherboard        |
|                                                           |
|   thombo                                      17 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <array>
#include <bitset>
#include "MSIMysticLight112Controller.h"
#include "StringUtils.h"

#define BITSET(val, bit, pos)       ((unsigned char)std::bitset<8>(val).set((pos), (bit)).to_ulong())

struct Config
{
    unsigned short                  pid;                    // PID of the board
    size_t                          numof_onboard_leds;     // number of onboard leds
    const std::vector<MSI_ZONE>*    supported_zones;        // pointer to vector of supported zones
};

const std::vector<MSI_ZONE> zones_set =
{
    MSI_ZONE_J_RGB_1,
    MSI_ZONE_J_RAINBOW_1,
    MSI_ZONE_J_CORSAIR,
    MSI_ZONE_ON_BOARD_LED_0
};

MSIMysticLight112Controller::MSIMysticLight112Controller
    (
    hid_device*     handle,
    const char*     path,
    std::string     dev_name
    )
{
    dev         = handle;
    location    = path;
    name        = dev_name;

    if(dev)
    {
        ReadFwVersion();
        ReadSettings();
    }

    /*-----------------------------------------*\
    | Initialize save flag                      |
    \*-----------------------------------------*/
    data.save_data = 0;
    data.on_board_led.colorFlags = 0x81;    // force MS bit of color flags to 1 to have expected zone control

    /*-----------------------------------------*\
    | Initialize zone based per LED data        |
    \*-----------------------------------------*/
    numof_onboard_leds = 7;
    supported_zones    = &zones_set;

    zone_based_per_led_data.j_rgb_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rgb_1.colorFlags = BITSET(zone_based_per_led_data.j_rgb_1.colorFlags, true, 7u);
    zone_based_per_led_data.j_rainbow_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.j_rainbow_1.colorFlags = BITSET(zone_based_per_led_data.j_rainbow_1.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led.colorFlags = BITSET(zone_based_per_led_data.on_board_led.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_1.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_1.colorFlags = BITSET(zone_based_per_led_data.on_board_led_1.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_2.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_2.colorFlags = BITSET(zone_based_per_led_data.on_board_led_2.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_3.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_3.colorFlags = BITSET(zone_based_per_led_data.on_board_led_3.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_4.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_4.colorFlags = BITSET(zone_based_per_led_data.on_board_led_4.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_5.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2;
    zone_based_per_led_data.on_board_led_5.colorFlags = BITSET(zone_based_per_led_data.on_board_led_5.colorFlags, true, 7u);
    zone_based_per_led_data.on_board_led_6.speedAndBrightnessFlags = MSI_BRIGHTNESS_LEVEL_100 << 2 << 2;
    zone_based_per_led_data.on_board_led_6.colorFlags = BITSET(zone_based_per_led_data.on_board_led_6.colorFlags, true, 7u);
    zone_based_per_led_data.save_data = 0;

    direct_mode = false;
}

MSIMysticLight112Controller::~MSIMysticLight112Controller()
{
    hid_close(dev);
}

void MSIMysticLight112Controller::SetMode
    (
    MSI_ZONE        zone,
    MSI_MODE        mode,
    MSI_SPEED       speed,
    MSI_BRIGHTNESS  brightness,
    bool            rainbow_color
    )
{
    ZoneData* zone_data = GetZoneData(data, zone);

    if(zone_data == nullptr)
    {
        return;
    }

    if (zone <= MSI_ZONE_ON_BOARD_LED_0)
    {
        zone_data->effect                    = mode;
        zone_data->speedAndBrightnessFlags   = (brightness << 2) | (speed & 0x03);
        zone_data->colorFlags                = BITSET(zone_data->colorFlags, !rainbow_color, 7u);
        zone_data->padding                   = 0x00;

        if(mode > MSI_MODE_DOUBLE_FLASHING)
        {
            zone_data->speedAndBrightnessFlags |= SYNC_SETTING_JRGB;
            zone_data->colorFlags              |= SYNC_SETTING_ONBOARD;
        }
        else
        {
            zone_data->speedAndBrightnessFlags &= ~SYNC_SETTING_JRGB;
            zone_data->colorFlags              &= ~SYNC_SETTING_ONBOARD;
        }
    }

    if((zone >= MSI_ZONE_ON_BOARD_LED_0) && (mode <= MSI_MODE_DOUBLE_FLASHING))
    {
        zone_data = GetZoneData(data, (MSI_ZONE)((int)zone + 1));

        if(zone_data != nullptr)
        {
            zone_data->effect                    = mode;
            zone_data->speedAndBrightnessFlags   = (brightness << 2) | (speed & 0x03);
            zone_data->colorFlags                = BITSET(zone_data->colorFlags, !rainbow_color, 7u);
            zone_data->padding                   = 0x00;
        }
    }
}

std::string MSIMysticLight112Controller::GetDeviceName()
{
    return name;
}

std::string MSIMysticLight112Controller::GetFWVersion()
{
    std::string firmware_version;
    firmware_version = "APROM: " + version_APROM + ", LDROM: " + version_LDROM;
    return firmware_version;
}

std::string MSIMysticLight112Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMysticLight112Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

bool MSIMysticLight112Controller::ReadSettings()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    return(hid_get_feature_report(dev, (unsigned char*)&data, sizeof(data)) == sizeof data);
}

bool MSIMysticLight112Controller::Update
    (
    bool save
    )
{
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    if(direct_mode)
    {
        return (hid_send_feature_report(dev, (unsigned char*)&zone_based_per_led_data, sizeof(zone_based_per_led_data)) == sizeof(zone_based_per_led_data));
    }
    else
    {
        data.save_data = save;
        return (hid_send_feature_report(dev, (unsigned char*)&data, sizeof(data)) == sizeof(data));
    }
}

void MSIMysticLight112Controller::SetZoneColor
    (
    MSI_ZONE        zone,
    unsigned char   red1,
    unsigned char   grn1,
    unsigned char   blu1,
    unsigned char   red2,
    unsigned char   grn2,
    unsigned char   blu2
    )
{
    ZoneData* zone_data = GetZoneData(data, zone);

    if(zone_data == nullptr)
    {
        return;
    }

    if (zone <= MSI_ZONE_ON_BOARD_LED_0)
    {
        zone_data->color.R  = red1;
        zone_data->color.G  = grn1;
        zone_data->color.B  = blu1;
        zone_data->color2.R = red2;
        zone_data->color2.G = grn2;
        zone_data->color2.B = blu2;
    }

    if(zone >= MSI_ZONE_ON_BOARD_LED_0)
    {
        zone_data = GetZoneData(data, (MSI_ZONE)((int)zone + 1));

        if(zone_data != nullptr)
        {
            zone_data->color.R  = red1;
            zone_data->color.G  = grn1;
            zone_data->color.B  = blu1;
            zone_data->color2.R = red2;
            zone_data->color2.G = grn2;
            zone_data->color2.B = blu2;
        }
    }
}

void MSIMysticLight112Controller::SetLedColor
    (
    MSI_ZONE        zone,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    if(zone >= MSI_ZONE_ON_BOARD_LED_0)
    {
        zone = (MSI_ZONE)((int)zone + 1);
    }

    ZoneData *zone_data = GetZoneData(zone_based_per_led_data, zone);

    if(zone_data == nullptr)
    {
        return;
    }

    zone_data->color.R  = red;
    zone_data->color.G  = grn;
    zone_data->color.B  = blu;
    zone_data->color2.R = red;
    zone_data->color2.G = grn;
    zone_data->color2.B = blu;
}

ZoneData *MSIMysticLight112Controller::GetZoneData
    (
    FeaturePacket_112&  data_packet,
    MSI_ZONE            zone
    )
{
    switch(zone)
    {
        case MSI_ZONE_J_RGB_1:
            return &data_packet.j_rgb_1;
        case MSI_ZONE_J_RAINBOW_1:
            return &data_packet.j_rainbow_1;
        case MSI_ZONE_ON_BOARD_LED_0:
            return &data_packet.on_board_led;
        case MSI_ZONE_ON_BOARD_LED_1:
            return &data_packet.on_board_led_1;
        case MSI_ZONE_ON_BOARD_LED_2:
            return &data_packet.on_board_led_2;
        case MSI_ZONE_ON_BOARD_LED_3:
            return &data_packet.on_board_led_3;
        case MSI_ZONE_ON_BOARD_LED_4:
            return &data_packet.on_board_led_4;
        case MSI_ZONE_ON_BOARD_LED_5:
            return &data_packet.on_board_led_5;
        case MSI_ZONE_ON_BOARD_LED_6:
            return &data_packet.on_board_led_6;
        case MSI_ZONE_J_CORSAIR:
            return &data_packet.j_corsair_1;
        default:
            break;
    }

    return nullptr;
}

bool MSIMysticLight112Controller::ReadFwVersion()
{
    unsigned char request[64];
    unsigned char response[64];
    int ret_val = 64;

    /*-----------------------------------------------------*\
    | First read the APROM                                  |
    | Checksum also available at report ID 180, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Zero out buffers                                      |
    \*-----------------------------------------------------*/
    memset(request, 0x00, sizeof(request));
    memset(response, 0x00, sizeof(response));

    /*-----------------------------------------------------*\
    | Set up APROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB0;

    /*-----------------------------------------------------*\
    | Fill request from 0x02 to 0x61 with 0xCC              |
    \*-----------------------------------------------------*/
    memset(&request[0x02], 0xCC, sizeof(request) - 2);

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    unsigned char highValue = response[2] >> 4;
    unsigned char lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_APROM = std::to_string((int)highValue).append(".").append(std::to_string((int)lowValue));

    /*-----------------------------------------------------*\
    | First read the LDROM                                  |
    | Checksum also available at report ID 184, with MSB    |
    | stored at index 0x08 and LSB at 0x09                  |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Set up LDROM Firmware Version Request packet          |
    \*-----------------------------------------------------*/
    request[0x00]   = 0x01;
    request[0x01]   = 0xB6;

    /*-----------------------------------------------------*\
    | Send request and receive response packets             |
    \*-----------------------------------------------------*/
    ret_val &= hid_write(dev, request, 64);
    ret_val &= hid_read(dev, response, 64);

    /*-----------------------------------------------------*\
    | Extract high and low values from response             |
    \*-----------------------------------------------------*/
    highValue = response[2] >> 4;
    lowValue  = response[2] & 0x0F;

    /*-----------------------------------------------------*\
    | Build firmware string <high>.<low>                    |
    \*-----------------------------------------------------*/
    version_LDROM = std::to_string((int)highValue).append(".").append(std::to_string((int)lowValue));

    /*-----------------------------------------------------*\
    | If return value is zero it means an HID transfer      |
    | failed                                                |
    \*-----------------------------------------------------*/
    return(ret_val > 0);
}

MSI_MODE MSIMysticLight112Controller::GetMode()
{
    return (MSI_MODE)data.on_board_led.effect;
}

void MSIMysticLight112Controller::GetMode
    (
    MSI_ZONE        zone,
    MSI_MODE        &mode,
    MSI_SPEED       &speed,
    MSI_BRIGHTNESS  &brightness,
    bool            &rainbow_color,
    unsigned int    &color
    )
{
    /*-----------------------------------------------------*\
    | Get data for given zone                               |
    \*-----------------------------------------------------*/
    ZoneData *zone_data = GetZoneData(data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(zone_data == nullptr)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update pointers with data                             |
    \*-----------------------------------------------------*/
    mode            = (MSI_MODE)zone_data->effect;
    speed           = (MSI_SPEED)(zone_data->speedAndBrightnessFlags & 0x03);
    brightness      = (MSI_BRIGHTNESS)((zone_data->speedAndBrightnessFlags >> 2) & 0x1F);
    rainbow_color   = (zone_data->colorFlags & 0x80) == 0 ? true : false;
    color           = ToRGBColor(zone_data->color.R, zone_data->color.G, zone_data->color.B);
}

void MSIMysticLight112Controller::SetDirectMode
    (
    bool mode
    )
{
    direct_mode = mode;
}

size_t MSIMysticLight112Controller::GetMaxOnboardLeds()
{
    return numof_onboard_leds;
}
