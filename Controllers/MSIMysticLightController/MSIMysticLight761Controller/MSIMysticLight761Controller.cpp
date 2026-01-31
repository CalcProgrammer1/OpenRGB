/*---------------------------------------------------------*\
| MSIMysticLight761Controller.cpp                           |
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
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "LogManager.h"
#include "MSIMysticLight761Controller.h"
#include "StringUtils.h"

#define NUM_CONFS sizeof(board_configs) / sizeof(mystic_light_761_config)
#define COLOR_BLACK {0, 0, 0}
#define IS_JARGB(X) (X == MSI_ZONE_JARGB_1 || X == MSI_ZONE_JARGB_2 || X == MSI_ZONE_JARGB_3)
#define GET_CHAR_PTR_REF(X) (unsigned char *) &(X)
#define ARRAY_ROW(X, Y) get_zone_setup_index(X)*16 + Y
#define ARRAY_ROW_RAW(X, Y) X*16 + Y
#define SETUP_ARRAY_SIZE 290

struct mystic_light_761_config
{
    const std::string *                      name;                  // Name of the board
    int                                      numof_onboard_leds;    // number of onboard leds
    int                                      numof_pipe1_leds;      // number of pipe 1 leds (used in per LED mode only)
    int                                      numof_pipe2_leds;      // number of pipe 2 leds (used in per LED mode only)
    int                                      numof_JRGBs;           // number of supported JRGB headers (used in per LED mode only)
    const std::vector<MSI_ZONE>*             supported_zones;       // pointer to vector of supported zones
    MSIMysticLight761Controller::DIRECT_MODE per_led_mode;          // type of direct mode support
};

static const std::vector<MSI_ZONE> zone_set1 =
{
        MSI_ZONE_JAF,
        MSI_ZONE_JARGB_1,
        MSI_ZONE_JARGB_2,
        MSI_ZONE_JARGB_3,
};

static const std::string board_names[] =
{
    "MSI MAG X870 TOMAHAWK WIFI (MS-7E51)",
    "MSI MAG B850M MORTAR WIFI (MS-7E61)",
    "MSI MPG B850I EDGE TI WIFI (MS-7E79)",
    "MSI X870 GAMING PLUS WIFI (MS-7E47)",
    "MSI B850 GAMING PLUS WIFI6E (MS-7E80)",
    "MSI B850M GAMING PLUS WIFI6E (MS-7E81)",
    "MSI MPG X870E CARBON WIFI (MS-7E49)",
    "MSI Z890 GAMING PLUS WIFI (MS-7E34)",
    "MSI X870E GAMING PLUS WIFI (MS-7E70)",
    "MSI MAG X870E TOMAHAWK WIFI (MS-7E59)",
    "MSI PRO B850-P WIFI (MS-7E56)",
    "MSI B850M GAMING PLUS WIFI (MS-7E66)",
    "MSI PRO X870E-P WIFI (MS-7E70)",
    "MSI MPG X870I EDGE TI EVO WIFI (MS-7E50)",
    "MSI B850 GAMING PLUS WIFI (MS-7E56)",
    "MSI PRO X870-P WIFI (MS-7E47)",
    "MSI MPG X870E EDGE TI WIFI (MS-7E59)",
};

static const mystic_light_761_config board_configs[] =
{
    { &(board_names[0]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI X870 TOMAHAWK WIFI
    { &(board_names[1]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI MAG B850M MORTAR WIFI
    { &(board_names[2]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI MPG B850I EDGE TI WIFI
    { &(board_names[3]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI X870 GAMING PLUS WIFI
    { &(board_names[4]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI B850 GAMING PLUS WIFI6E
    { &(board_names[5]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI B850M GAMING PLUS WIFI6E
    { &(board_names[6]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI MPG X870E CARBON WIFI
    { &(board_names[7]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI Z890 GAMING PLUS WIFI
    { &(board_names[8]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI X870E GAMING PLUS WIFI
    { &(board_names[9]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },    // MSI MAG X870E TOMAHAWK WIFI
    { &(board_names[10]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI PRO B850-P WIFI (MS-7E56)
    { &(board_names[11]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI B850M GAMING PLUS WIFI
    { &(board_names[12]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI PRO X870E-P WIFI
    { &(board_names[13]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI MPG X870I EDGE TI EVO WIFI (MS-7E50)
    { &(board_names[14]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI B850 GAMING PLUS WIFI (MS-7E56)
    { &(board_names[15]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI PRO X870-P WIFI
    { &(board_names[16]), 0,  0,  0, 1, &zone_set1,  MSIMysticLight761Controller::DIRECT_MODE_ZONE_BASED },   // MSI MPG X870E EDGE TI WIFI
};

enum MSI_ZONE setup_map [] =
    {
        MSI_ZONE_JARGB_1,
        MSI_ZONE_JARGB_2,
        MSI_ZONE_JARGB_3,
        MSI_ZONE_JAF,
        MSI_ZONE_J_PIPE_1,
        MSI_ZONE_J_PIPE_2,
        MSI_ZONE_J_PIPE_3,
        MSI_ZONE_J_PIPE_4,
        MSI_ZONE_J_PIPE_5,
        MSI_ZONE_J_RGB_1,
        MSI_ZONE_J_RGB_2,
        MSI_ZONE_ON_BOARD_LED_0,
        MSI_ZONE_ON_BOARD_LED_1,
        MSI_ZONE_ON_BOARD_LED_2,
        MSI_ZONE_ON_BOARD_LED_3,
        MSI_ZONE_ON_BOARD_LED_4,
        MSI_ZONE_ON_BOARD_LED_5,

};

int get_zone_setup_index(MSI_ZONE index)
{
    int size = sizeof(setup_map) / sizeof(setup_map[0]);
    for(int i = 0; i < size; i++)
    {
        if(setup_map[i] == index)
        {
            return i;
        }
    }
    return -1;
}

// Copying from signal plugin
// DO NOT MODIFY THIS ARRAY DIRECTLY, MAKE COPY
unsigned char initial_setup_array[] =
{
    0x50,
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x15, 0x78, //JARGB 1
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x15, 0x78, //JARGB 2
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x15, 0x78, //JARGB 3
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x15, 0x78, //JAF //Fans go here with the weird connector
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x95, 0x1E, //JPIPE1
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //JPIPE2 //95 for active zones 94 for inactive
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x95, 0x1E, //JPIPE3
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //JPIPE4
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //JPIPE5
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x95, 0x1E, //JRGB1
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //JRGB2
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard1
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard2
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard3
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard4
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard5
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x1E, //Onboard6
    0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x95, 0x1E, //Select all?
    0x00
};

unsigned char * initializer_array()
{
    unsigned char * arr = (unsigned char *) malloc(SETUP_ARRAY_SIZE);
    for(int i = 0; i < SETUP_ARRAY_SIZE; i++)
    {
        arr[i] = initial_setup_array[i];
    }
    return arr;
}

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
    std::string     dev_name
    )
{
    dev                                             = handle;
    location                                        = path;
    name                                            = dev_name;

    const mystic_light_761_config * board_config    = nullptr;
    for(std::size_t i = 0; i < NUM_CONFS; i++)
    {
        if(*(board_configs[i].name) == name)
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

            if(curr_val > max)
            {
                max = curr_val;
            }
        }

        // Need to send configuration to board
        unsigned char * conf_arr = initializer_array();

        // First set everything off
        for(int i = 4; i < 17; i++)
        {
            conf_arr[ARRAY_ROW_RAW(i, 1)]  = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 2)]  = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 6)]  = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 10)] = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 11)] = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 12)] = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 13)] = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 14)] = 0x00;
            conf_arr[ARRAY_ROW_RAW(i, 15)] = 0x00;
        }

        for(std::size_t i = 0; i < supported_zones->size(); i++)
        {
            MSI_ZONE supp_zone = (*supported_zones)[i];
            ZoneConfig conf;
            conf.msi_zone = supp_zone;
            // Turn on relevant zones (0-3 are always active)
            if(get_zone_setup_index(supp_zone) > 3)
            {
                conf_arr[ARRAY_ROW(supp_zone,1 )] = 0x09;
                conf_arr[ARRAY_ROW(supp_zone,2 )] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,6 )] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,10)] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,11)] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,12)] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,13)] = 0xFF;
                conf_arr[ARRAY_ROW(supp_zone,14)] = 0x03;
                conf_arr[ARRAY_ROW(supp_zone,15)] = 0x95;
            }
            ZoneData * dat = new ZoneData;
            conf.zone_data = dat;
            zone_configs.push_back(conf);
        }

        if(dev)
        {
            location = path;

            ReadName();
            ReadFwVersion();
            ReadSettings();

            // Push config so setting colors works
            int res = hid_send_feature_report(dev, conf_arr, SETUP_ARRAY_SIZE);
            LOG_INFO("Sending configuration resulted in %i\n", res);

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

        free(conf_arr);
    }
    else
    {
        throw std::runtime_error(BOARD_UNSUPPORTED_ERROR);
    }

}

MSIMysticLight761Controller::~MSIMysticLight761Controller()
{
    hid_close(dev);

    if(data)
    {
        delete data;
        data = nullptr;
    }

    for(ZoneConfig& zone : zone_configs)
    {
        if(zone.zone_data)
        {
            delete zone.zone_data;
            zone.zone_data = nullptr;
        }
    }
    zone_configs.clear();
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
    return std::string("AP/LD ").append(version_aprom).append(" / ").append(version_ldrom);
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
    if(ret < 0)
    {
        flag = false;
    }
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb1.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0)
    {
        flag = false;
    }
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb2.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0)
    {
        flag = false;
    }
    ret = hid_send_feature_report(dev, GET_CHAR_PTR_REF(data->jargb3.packet) , sizeof(FeaturePacket_PerLED_761));
    if(ret < 0)
    {
        flag = false;
    }

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

void set_data_color(FeaturePacket_Zone_761 * packet, std::size_t index, unsigned char color_val  )
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
    std::size_t     index,
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

    std::size_t candidate_index = (index * 3);

    if((candidate_index + 2) <= GetMaxDirectLeds(zone))
    {
        set_data_color(ptr, candidate_index,     red);
        set_data_color(ptr, candidate_index + 1, grn);
        set_data_color(ptr, candidate_index + 2, blu);
    }
}

ZoneData *MSIMysticLight761Controller::GetZoneData
    (
    FeaturePacket_761&  /*data_packet*/,
    MSI_ZONE            zone
    )
{
    for(std::size_t i = 0; i < zone_configs.size(); i++)
    {
        if(zone_configs[i].msi_zone == zone)
        {
            return zone_configs[i].zone_data;
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
void MSIMysticLight761Controller::ReadName()
{
    wchar_t tname[256];

    /*-----------------------------------------------------*\
    | Get the manufacturer string from HID                  |
    \*-----------------------------------------------------*/
    hid_get_manufacturer_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Convert to std::string                                |
    \*-----------------------------------------------------*/
    name = StringUtils::wstring_to_string(tname);

    /*-----------------------------------------------------*\
    | Get the product string from HID                       |
    \*-----------------------------------------------------*/
    hid_get_product_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Append the product string to the manufacturer string  |
    \*-----------------------------------------------------*/
    name.append(" ").append(StringUtils::wstring_to_string(tname));
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

bool MSIMysticLight761Controller::IsDirectModeActive()
{
    return true;
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
