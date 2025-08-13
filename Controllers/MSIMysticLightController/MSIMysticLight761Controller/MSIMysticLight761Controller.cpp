/*---------------------------------------------------------*\
| MSIMysticLight185Controller.cpp                           |
|                                                           |
|   Driver for MSI Mystic Light 761-byte motherboard        |
|                                                           |
|   Direct mode functionality has been implemented based on |
|   the SignalRGB project                                   |
|   (https://signalrgb.com/)                                |
|                                                           |
|   rom4ster                                    11 Jun 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <array>
#include <vector>
#include <bitset>
#include "MSIMysticLight761Controller.h"
#include "StringUtils.h"


#define NUM_CONFS sizeof(board_configs) / sizeof(mystic_light_761_config)
#define COLOR_BLACK {0, 0, 0}
#define IS_JARGB(X) (X == MSI_ZONE_JARGB_1 || X == MSI_ZONE_JARGB_2 || X == MSI_ZONE_JARGB_3)
#define GET_CHAR_PTR_REF(X) (unsigned char *) &(X)

struct mystic_light_761_config
{
    unsigned short                           pid;                   // PID of the board
    int                                      numof_onboard_leds;    // number of onboard leds
    int                                      numof_pipe1_leds;      // number of pipe 1 leds (used in per LED mode only)
    int                                      numof_pipe2_leds;      // number of pipe 2 leds (used in per LED mode only)
    int                                      numof_JRGBs;           // number of supported JRGB headers (used in per LED mode only)
    const std::vector<MSI_ZONE>*             supported_zones;       // pointer to vector of supported zones
    MSIMysticLight761Controller::DIRECT_MODE per_led_mode;          // type of direct mode support
};

const std::vector<MSI_ZONE> x870tomawhawk_zones =
{
    MSI_ZONE_JAF,
    MSI_ZONE_JARGB_1,
    MSI_ZONE_JARGB_2,
    MSI_ZONE_JARGB_3,
};

static const mystic_light_761_config board_configs[] =
{
  { 0x0076, 0,  0,  0, 1, &x870tomawhawk_zones,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI X870 TOMAHAWK WIFI
};

void init_packet(FeaturePacket_Zone_761 * packet)
{
    packet->packet.fixed1 = 0x09;
    packet->packet.fixed2 = 0x00;
    packet->packet.fixed3 = 0x00;
    packet->packet.hdr2 = 240;

    for(int i = 0; i < NUM_LEDS_761; i++)
    {
        packet->packet.colors[i] = 0x0;
    }

}

MSIMysticLight761Controller::MSIMysticLight761Controller
    (
    hid_device*     handle,
    const char*     path,
    unsigned short  pid,
    std::string     dev_name
    )
{
    dev                                             = handle;
    location                                        = path;
    name                                            = dev_name;

    supported_zones                                 = new std::vector<MSI_ZONE>;

    const mystic_light_761_config * board_config    = nullptr;

    for(std::size_t i = 0; i < NUM_CONFS; i++)
    {
        if(board_configs[i].pid == pid)
        {
            board_config = &board_configs[i];
            break;
        }
    }

    if(board_config != nullptr)
    {
        supported_zones     = (std::vector<MSI_ZONE>*) board_config->supported_zones;
        unsigned int max    = 0;

        for(std::size_t i = 0; i < board_config->supported_zones[0].size(); i++)
        {
            unsigned int curr_val = (unsigned int) (board_config->supported_zones[0][i]);

            if((unsigned int) curr_val > max)
            {
                max = curr_val;
            }
        }
    }

    for(MSI_ZONE supp_zone : *supported_zones)
    {
        ZoneConfig conf;

        conf.msi_zone   = supp_zone;
        ZoneData * dat  = new ZoneData;
        conf.zone_data  = dat;

        zone_configs.push_back(conf);
    }

    if(dev)
    {
        ReadFwVersion();
        ReadSettings();
        data                        = new FeaturePacket_761;

        data->jaf.zone              = MSI_ZONE_JAF;
        data->jargb1.zone           = MSI_ZONE_JARGB_1;
        data->jargb2.zone           = MSI_ZONE_JARGB_2;
        data->jargb3.zone           = MSI_ZONE_JARGB_3;

        data->jaf.packet.hdr0       = 0x08;
        data->jargb1.packet.hdr0    = 0x04;
        data->jargb2.packet.hdr0    = 0x04;
        data->jargb3.packet.hdr0    = 0x04;

        data->jaf.packet.hdr1       = 0x00;
        data->jargb1.packet.hdr1    = 0x00;
        data->jargb2.packet.hdr1    = 0x01;
        data->jargb3.packet.hdr1    = 0x02;

        init_packet(&data->jaf);
        init_packet(&data->jargb1);
        init_packet(&data->jargb2);
        init_packet(&data->jargb3);
    }

}

MSIMysticLight761Controller::~MSIMysticLight761Controller()
{
    hid_close(dev);
}

void MSIMysticLight761Controller::SetMode
    (
    MSI_ZONE        /*zone*/,
    MSI_MODE        /*mode*/,
    MSI_SPEED       /*speed*/,
    MSI_BRIGHTNESS  /*brightness*/,
    bool            /*rainbow_color*/
    )
{
    return; // Only supporting direct for now
}

std::string MSIMysticLight761Controller::GetDeviceName()
{
    return name;
}

std::string MSIMysticLight761Controller::GetFWVersion()
{
    return std::string("AP/LD ").append(version_APROM).append(" / ").append(version_LDROM);
}

std::string MSIMysticLight761Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIMysticLight761Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

bool MSIMysticLight761Controller::ReadSettings()
{
    /*-----------------------------------------------------*\
    | Read packet from hardware, return true if successful  |
    \*-----------------------------------------------------*/
    unsigned char buffer [500];
    buffer[0] = 0x51;
    return (hid_get_feature_report(dev, buffer, 500)) > 0 ;
}

bool MSIMysticLight761Controller::Update
    (
    bool /*save*/
    )
{

    int ret = 0;
    bool flag = true;
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jaf.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0) { flag = false;}
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb1.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0) { flag = false;}
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb2.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0) { flag = false;}
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb3.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0) { flag = false;}

    return flag;
}

void MSIMysticLight761Controller::SetZoneColor
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
    for(std::size_t i = 0; i < zone_configs.size(); i++)
    {
        if(zone_configs[i].msi_zone == zone)
        {
            zone_configs[i].zone_data->color.R = red1;
            zone_configs[i].zone_data->color.G = grn1;
            zone_configs[i].zone_data->color.B = blu1;
            zone_configs[i].zone_data->color2.R = red2;
            zone_configs[i].zone_data->color2.G = grn2;
            zone_configs[i].zone_data->color2.B = blu2;
        }
    }
}

void set_data_color(FeaturePacket_Zone_761 * packet, int index, unsigned char color_val  )
{
    if(packet == nullptr)
    {
        return;
    }
    packet->packet.colors[index] = color_val;
}

void MSIMysticLight761Controller::SetLedColor
    (
    MSI_ZONE        zone,
    int             index,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{

    FeaturePacket_Zone_761 * ptr = nullptr;
    switch(zone)
    {
        case MSI_ZONE_JAF:
            ptr = &data->jaf;
            break;
        case MSI_ZONE_JARGB_1:
            ptr = &data->jargb1;
            break;
        case MSI_ZONE_JARGB_2:
            ptr = &data->jargb2;
            break;
        case MSI_ZONE_JARGB_3:
            ptr = &data->jargb3;
            break;
        default:
            break;
    }

    if(index >= 0)
    {
        std::size_t candidate_index = (index * 3);

        if((candidate_index + 2) <= GetMaxDirectLeds(zone))
        {
            set_data_color(ptr, (int)candidate_index,     red);
            set_data_color(ptr, (int)candidate_index + 1, grn);
            set_data_color(ptr, (int)candidate_index + 2, blu);
        }
    }

}

ZoneData *MSIMysticLight761Controller::GetZoneData
    (
    FeaturePacket_761&  /*data_packet*/,
    MSI_ZONE            zone
    )
{
    for(ZoneConfig zd : zone_configs)
    {
        if(zd.msi_zone == zone)
        {
            return zd.zone_data;
        }
    }

    return nullptr;
}

Color *MSIMysticLight761Controller::GetPerLedZoneData
    (
    MSI_ZONE  zone
    )
{
    return &(GetZoneData(*data, zone)->color);
}

RainbowZoneData *MSIMysticLight761Controller::GetRainbowZoneData
    (
    MSI_ZONE /*zone*/
    )
{
    return nullptr;
}

bool MSIMysticLight761Controller::ReadFwVersion()
{
    return true;
}

MSI_MODE MSIMysticLight761Controller::GetMode()
{
    return MSI_MODE_DIRECT_DUMMY;
}

void MSIMysticLight761Controller::GetMode
    (
    MSI_ZONE            zone,
    MSI_MODE &          mode,
    MSI_SPEED &         speed,
    MSI_BRIGHTNESS &    brightness,
    bool &              rainbow_color,
    unsigned int &      color
    )
{
    /*-----------------------------------------------------*\
    | Get data for given zone                               |
    \*-----------------------------------------------------*/
    ZoneData *zone_data = GetZoneData(*data, zone);

    /*-----------------------------------------------------*\
    | Return if zone is invalid                             |
    \*-----------------------------------------------------*/
    if(!zone_data)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Update pointers with data                             |
    \*-----------------------------------------------------*/

    // Actual support of non direct modes needs to be further investigated
    mode            = (MSI_MODE)zone_data->effect;
    speed           = (MSI_SPEED)(zone_data->speedAndBrightnessFlags & 0x03);
    brightness      = (MSI_BRIGHTNESS)((zone_data->speedAndBrightnessFlags >> 2) & 0x1F);
    rainbow_color   = (zone_data->colorFlags & 0x80) == 0 ? true : false;
    color           = ToRGBColor(zone_data->color.R, zone_data->color.G, zone_data->color.B);
}

void MSIMysticLight761Controller::SetCycleCount
    (
    MSI_ZONE        /*zone*/,
    unsigned char   /*cycle_num*/
    )
{
    return;
}

void MSIMysticLight761Controller::SetDirectMode
    (
    bool /*mode*/
    )
{
    SelectPerLedProtocol();
}


size_t MSIMysticLight761Controller::GetMaxDirectLeds
    (
    MSI_ZONE zone
    )
{
    switch(zone)
    {
    case MSI_ZONE_JAF:
    case MSI_ZONE_JARGB_1:
    case MSI_ZONE_JARGB_2:
    case MSI_ZONE_JARGB_3:
        return 240;
        break;
    default:
        return 1;
    }
}


void MSIMysticLight761Controller::SelectPerLedProtocol()
{
    return;
}
