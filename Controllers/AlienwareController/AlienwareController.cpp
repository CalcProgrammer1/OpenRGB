/*---------------------------------------------------------*\
| AlienwareController.cpp                                   |
|                                                           |
|   Driver for Dell Alienware RGB USB controller            |
|                                                           |
|   Gabriel Marcano (gemarcano)                 21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cstdint>
#include <map>
#include <thread>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "RGBController.h"
#include "AlienwareController.h"
#include "LogManager.h"
#include "StringUtils.h"

typedef uint32_t alienware_platform_id;

/*---------------------------------------------------------*\
| Some devices appear to report the wrong number of zones.  |
| Record that here.                                         |
\*---------------------------------------------------------*/
static const std::map<alienware_platform_id, uint8_t> zone_quirks_table =
{
    { 0x0C01,   4 },    // Dell G5 SE 5505
    { 0x0A01,  16 },    // Dell G7 15 7500
    { 0x0E03,   4 },    // Dell G15   5511
    { 0x0E0A,   4 }     // Dell G15   5530

};

/*---------------------------------------------------------*\
| Add zones for devices here, mapping the platform ID to    |
| the zone names                                            |
\*---------------------------------------------------------*/
static const std::map<alienware_platform_id, std::vector<const char*>> zone_names_table =
{
    { 0x0C01,   { "Left",           "Middle",       "Right",        "Numpad"    } },
    { 0x0A01,   { "Left",           "Center Left",  "Center Right", "Right",
                  "Light Bar 1",    "Light Bar 2",  "Light Bar 3",
                  "Light Bar 4",    "Light Bar 5",  "Light Bar 6",
                  "Light Bar 7",    "Light Bar 8",  "Light Bar 9",
                  "Light Bar 10",   "Light Bar 11", "Light Bar 12"              } },
    { 0x0E03,   { "Left",           "Middle",       "Right",        "Numpad"    } },
    { 0x0E0A,   { "Left",           "Middle",       "Right",        "Numpad"    } }
};

static void SendHIDReport(hid_device *dev, const unsigned char* usb_buf, size_t usb_buf_size)
{
    using namespace std::chrono_literals;

    hid_send_feature_report(dev, usb_buf, usb_buf_size);

    /*-----------------------------------------------------*\
    | The controller really doesn't like really spammed by  |
    | too many commands at once... the delay may be command |
    | dependent also.  Delay for longer if the command is   |
    | changing animation state                              |
    \*-----------------------------------------------------*/
    unsigned char command       = usb_buf[2];
    unsigned char subcommand    = usb_buf[3];

    if( ( command == ALIENWARE_COMMAND_USER_ANIM                    )
     && ( ( subcommand == ALIENWARE_COMMAND_USER_ANIM_FINISH_PLAY )
       || ( subcommand == ALIENWARE_COMMAND_USER_ANIM_FINISH_SAVE ) ) )
    {
        std::this_thread::sleep_for(1s);
    }
    else
    {
        std::this_thread::sleep_for(60ms);
    }
}

AlienwareController::AlienwareController(hid_device* dev_handle, const hid_device_info& info, std::string name)
{
    HidapiAlienwareReport report;

    dev                 = dev_handle;
    device_name         = name;
    location            = info.path;

    /*-----------------------------------------------------*\
    | Get serial number                                     |
    \*-----------------------------------------------------*/
    serial_number       = StringUtils::wstring_to_string(info.serial_number);

    /*-----------------------------------------------------*\
    | Get zone information by checking firmware             |
    | configuration                                         |
    \*-----------------------------------------------------*/
    report              = Report(ALIENWARE_COMMAND_REPORT_CONFIG);
    alienware_platform_id platform_id = report.data[4] << 8 | report.data[5];

    /*-----------------------------------------------------*\
    | Check if the device reports the wrong number of zones |
    \*-----------------------------------------------------*/
    unsigned number_of_zones = zone_quirks_table.count(platform_id) ? zone_quirks_table.at(platform_id) : report.data[6];

    /*-----------------------------------------------------*\
    | Get firmware version                                  |
    \*-----------------------------------------------------*/
    report              = Report(ALIENWARE_COMMAND_REPORT_FIRMWARE);

    std::stringstream fw_string;

    fw_string << static_cast<unsigned>(report.data[4]) << '.' << static_cast<unsigned>(report.data[5]) << '.' << static_cast<unsigned>(report.data[6]);
    version             = fw_string.str();

    /*-----------------------------------------------------*\
    | Initialize Alienware zones                            |
    \*-----------------------------------------------------*/
    zones.resize(number_of_zones);

    if(zone_names_table.count(platform_id))
    {
        LOG_INFO("[%s] Known platform: %8X, Number of zones: %d", ALIENWARE_CONTROLLER_NAME, platform_id, number_of_zones);
        zone_names = zone_names_table.at(platform_id);
    }
    else
    {
        LOG_WARNING("[%s] Unknown platform: %8X, Number of zones: %d", ALIENWARE_CONTROLLER_NAME, platform_id, number_of_zones);

        /*-------------------------------------------------*\
        | If this is an unknown controller, set the name of |
        | all regions to "Unknown"                          |
        \*-------------------------------------------------*/
        for(size_t i = 0; i < number_of_zones; i++)
        {
            zone_names.emplace_back("Unknown");
        }
    }

    /*-----------------------------------------------------*\
    | Set defaults for all zones                            |
    | It doesn't seem possible to read the controller's     |
    | current state, hence the default value being set here.|
    \*-----------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].color[0]    = 0x000000;
        zones[zone_idx].color[1]    = 0x000000;
        zones[zone_idx].mode        = ALIENWARE_MODE_COLOR;

        /*-------------------------------------------------*\
        | Default period value from ACC                     |
        \*-------------------------------------------------*/
        zones[zone_idx].period      = 2000;
        zones[zone_idx].tempo       = ALIENWARE_TEMPO_MAX;
        zones[zone_idx].dim         = 0;
    }

    /*-----------------------------------------------------*\
    | Initialize dirty flags                                |
    \*-----------------------------------------------------*/
    dirty               = true;
    dirty_dim           = true;
}

AlienwareController::~AlienwareController()
{

}

unsigned int AlienwareController::GetZoneCount()
{
    return((unsigned int)zones.size());
}

std::vector<const char*> AlienwareController::GetZoneNames()
{
    return(zone_names);
}

std::string AlienwareController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AlienwareController::GetDeviceName()
{
    return(device_name);
}

std::string AlienwareController::GetSerialString()
{
    return(serial_number);
}

std::string AlienwareController::GetFirmwareVersion()
{
    return(version);
}

AlienwareController::HidapiAlienwareReport AlienwareController::GetResponse()
{
    /*-----------------------------------------------------*\
    | Zero init. This is not updated if there's a problem.  |
    \*-----------------------------------------------------*/
    HidapiAlienwareReport result;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(result.data, 0x00, sizeof(result.data));


    hid_get_feature_report(dev, result.data, HIDAPI_ALIENWARE_REPORT_SIZE);

    return(result);
}

AlienwareController::HidapiAlienwareReport AlienwareController::Report(uint8_t subcommand)
{
    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = 0x03;
    usb_buf[0x02] = ALIENWARE_COMMAND_REPORT;
    usb_buf[0x03] = subcommand;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    return(GetResponse());
}

AlienwareReport AlienwareController::GetStatus(uint8_t subcommand)
{
    HidapiAlienwareReport   data    = Report(subcommand);
    AlienwareReport         result  = AlienwareReport{};

    /*-----------------------------------------------------*\
    | Skip first byte, as that's the report number, which   |
    | should be 0                                           |
    \*-----------------------------------------------------*/
    memcpy(result.data, &data.data[1], sizeof(result.data));

    return(result);
}

bool AlienwareController::Dim(std::vector<uint8_t> zones, double percent)
{
    /*-----------------------------------------------------*\
    | Bail out if there are no zones to update              |
    \*-----------------------------------------------------*/
    if(!zones.size())
    {
        return(true);
    }

    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    uint16_t num_zones  = (uint16_t)zones.size();

    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = ALIENWARE_COMMAND_DIM;
    usb_buf[0x03]       = static_cast<uint8_t>(percent);
    usb_buf[0x04]       = num_zones >> 8;
    usb_buf[0x05]       = num_zones & 0xFF;

    for(size_t i = 0; i < num_zones; i++)
    {
        usb_buf[0x06+i] = zones[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | For this command, error is if the output equals the   |
    | input                                                 |
    \*-----------------------------------------------------*/
    return((response.data[1] == 0x03) && memcmp(usb_buf, response.data, HIDAPI_ALIENWARE_REPORT_SIZE));
}

bool AlienwareController::UserAnimation(uint16_t subcommand, uint16_t animation, uint16_t duration)
{
    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00 per hidapi      |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = ALIENWARE_COMMAND_USER_ANIM;
    usb_buf[0x03]       = subcommand >> 8;
    usb_buf[0x04]       = subcommand & 0xFF;
    usb_buf[0x05]       = animation >> 8;
    usb_buf[0x06]       = animation & 0xFF;
    usb_buf[0x07]       = duration >> 8;
    usb_buf[0x08]       = duration & 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Every subcommand appears to report its result on a    |
    | different byte                                        |
    \*-----------------------------------------------------*/
    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | The only time the 0x03 byte is zero is if the         |
    | controller has crashed                                |
    \*-----------------------------------------------------*/
    if(response.data[1] == 0)
    {
        return(false);
    }

    switch(subcommand)
    {
        case ALIENWARE_COMMAND_USER_ANIM_FINISH_SAVE:
            return(!response.data[7]);
        case ALIENWARE_COMMAND_USER_ANIM_FINISH_PLAY:
            return(!response.data[5]);
        case ALIENWARE_COMMAND_USER_ANIM_PLAY:
            return(!response.data[7]);
        default:
            return(true);
    }
}

bool AlienwareController::SelectZones(const std::vector<uint8_t>& zones)
{
    /*-----------------------------------------------------*\
    | Bail if zones is empty, and return false to indicate  |
    | nothing has changed                                   |
    \*-----------------------------------------------------*/
    if(!zones.size())
    {
        return(false);
    }

    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    uint16_t num_zones  = (uint16_t)zones.size();

    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = ALIENWARE_COMMAND_SELECT_ZONES;
    usb_buf[0x03]       = 1; // loop?
    usb_buf[0x04]       = num_zones >> 8;
    usb_buf[0x05]       = num_zones & 0xFF;

    for(size_t i = 0; i < num_zones; i++)
    {
        usb_buf[0x06+i] = zones[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | For this command, error is if the output equals the   |
    | input                                                 |
    \*-----------------------------------------------------*/
    return((response.data[1] == 0x03) && memcmp(usb_buf, response.data, HIDAPI_ALIENWARE_REPORT_SIZE));
}

bool AlienwareController::ModeAction(uint8_t mode, uint16_t duration, uint16_t tempo, RGBColor color)
{
    return(ModeAction(&mode, &duration, &tempo, &color, 1));
}

bool AlienwareController::ModeAction
    (
    const uint8_t*  mode,
    const uint16_t* duration,
    const uint16_t* tempo,
    const RGBColor* color,
    unsigned        amount
    )
{
    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Amount must be 3 or less, as that's how many          |
    | subcommands can fit into one report                   |
    \*-----------------------------------------------------*/
    if(amount > 3)
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]               = 0x00;
    usb_buf[0x01]               = 0x03;
    usb_buf[0x02]               = ALIENWARE_COMMAND_ADD_ACTION;

    for(unsigned int i = 0; i < amount; i++)
    {
        usb_buf[0x03 + (8 * i)] = mode[i];
        usb_buf[0x04 + (8 * i)] = duration[i] >> 8;
        usb_buf[0x05 + (8 * i)] = duration[i] & 0xFF;
        usb_buf[0x06 + (8 * i)] = tempo[i] >> 8;
        usb_buf[0x07 + (8 * i)] = tempo[i] & 0xFF;
        usb_buf[0x08 + (8 * i)] = RGBGetRValue(color[i]);
        usb_buf[0x09 + (8 * i)] = RGBGetGValue(color[i]);
        usb_buf[0x0A + (8 * i)] = RGBGetBValue(color[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | For this command, error is if the output equals the   |
    | input                                                 |
    \*-----------------------------------------------------*/
    return((response.data[1] == 0x03) && memcmp(usb_buf, response.data, HIDAPI_ALIENWARE_REPORT_SIZE));
}

bool AlienwareController::MultiModeAction
    (
    const uint8_t*  mode,
    const uint16_t* duration,
    const uint16_t* tempo,
    const RGBColor* color,
    unsigned        amount
    )
{
    bool         result = true;
    unsigned int left   = amount;

    while(left && result)
    {
        unsigned int tmp_amount;

        tmp_amount          = std::min(left, 3u);
        result             &= ModeAction(mode, duration, tempo, color, tmp_amount);
        mode               += tmp_amount;
        duration           += tmp_amount;
        tempo              += tmp_amount;
        color              += tmp_amount;
        left               -= tmp_amount;
    }

    return(result);
}

bool AlienwareController::SetColorDirect(RGBColor color, std::vector<uint8_t> zones)
{
    /*-----------------------------------------------------*\
    | Bail if zones is empty                                |
    \*-----------------------------------------------------*/
    if(zones.empty())
    {
        return(true);
    }

    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    uint16_t num_zones      = (uint16_t)zones.size();

    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x03;
    usb_buf[0x02]           = ALIENWARE_COMMAND_SET_COLOR;
    usb_buf[0x03]           = RGBGetRValue(color);
    usb_buf[0x04]           = RGBGetGValue(color);
    usb_buf[0x05]           = RGBGetBValue(color);
    usb_buf[0x06]           = num_zones >> 8;
    usb_buf[0x07]           = num_zones & 0xFF;

    for(size_t i = 0; i < num_zones; i++)
    {
        usb_buf[0x08 + i]   = zones[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | For this command, error is if the output equals the   |
    | input                                                 |
    \*-----------------------------------------------------*/
    return((response.data[1] == 0x03) && memcmp(usb_buf, response.data, HIDAPI_ALIENWARE_REPORT_SIZE));
}

bool AlienwareController::Reset()
{
    /*-----------------------------------------------------*\
    | Bail if zones is empty                                |
    \*-----------------------------------------------------*/
    if(zones.empty())
    {
        return(true);
    }

    unsigned char usb_buf[HIDAPI_ALIENWARE_REPORT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00, per hidapi     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = ALIENWARE_COMMAND_RESET;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    SendHIDReport(dev, usb_buf, sizeof(usb_buf));

    HidapiAlienwareReport response = GetResponse();

    /*-----------------------------------------------------*\
    | For this command, error is if the output equals the   |
    | input                                                 |
    \*-----------------------------------------------------*/
    return(response.data[1] == 0x03);
}

void AlienwareController::SetMode(uint8_t zone, uint8_t mode)
{
    if(mode != zones[zone].mode)
    {
        zones[zone].mode        = mode;
        dirty                   = true;
    }
}

void AlienwareController::SetColor(uint8_t zone, RGBColor color)
{
    SetColor(zone, color, zones[zone].color[1]);
}

void AlienwareController::SetColor(uint8_t zone, RGBColor color1, RGBColor color2)
{
    if ((color1 == zones[zone].color[0]) && (color2 == zones[zone].color[1]))
    {
        return;
    }

    zones[zone].color[0] = color1;
    zones[zone].color[1] = color2;
    dirty = true;
}

void AlienwareController::SetPeriod(uint8_t zone, uint16_t period)
{
    if(period != zones[zone].period)
    {
        zones[zone].period      = period;
        dirty                   = true;
    }
}

void AlienwareController::SetTempo(uint8_t zone, uint16_t tempo)
{
    if(tempo != zones[zone].tempo)
    {
        zones[zone].tempo       = tempo;
        dirty                   = true;
    }
}

void AlienwareController::SetDim(uint8_t zone, uint8_t dim)
{
    if(dim != zones[zone].dim)
    {
        zones[zone].dim         = dim;
        dirty_dim               = true;
    }
}

void AlienwareController::UpdateDim()
{
    if(!dirty_dim)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Collect all zones that share dim settings, and update |
    | them together                                         |
    \*-----------------------------------------------------*/
    std::map<uint8_t, std::vector<uint8_t>> dim_zone_map;

    for(size_t i = 0; i < zones.size(); i++)
    {
        dim_zone_map[zones[i].dim].emplace_back((uint8_t)i);
    }

    for(std::pair<const uint8_t, std::vector<uint8_t>> &pair : dim_zone_map)
    {
        /*-------------------------------------------------*\
        | Bail on an error...                               |
        \*-------------------------------------------------*/
        if(!Dim(pair.second, pair.first))
        {
            return;
        }
    }

    dirty_dim = false;
}

bool AlienwareController::UpdateDirect()
{
    /*-----------------------------------------------------*\
    | Collect all zones that share dim settings, and update |
    | them together                                         |
    \*-----------------------------------------------------*/
    std::map<RGBColor, std::vector<uint8_t>> color_zone_map;

    for(size_t i = 0; i < zones.size(); i++)
    {
        color_zone_map[zones[i].color[0]].emplace_back((uint8_t)i);
    }

    for(std::pair<const RGBColor, std::vector<uint8_t>> &pair : color_zone_map)
    {
        /*-------------------------------------------------*\
        | Bail on an error...                               |
        \*-------------------------------------------------*/
        if(!SetColorDirect(pair.first, pair.second))
        {
            return false;
        }
    }
    return true;
}

static const RGBColor rainbow_colors[4][7] =
{
    { 0xFF0000, 0xFFA500, 0xFFFF00, 0x008000, 0x00BFFF, 0x0000FF, 0x800080 },
    { 0x800080, 0xFF0000, 0xFFA500, 0xFFFF00, 0x008000, 0x00BFFF, 0x0000FF },
    { 0x0000FF, 0x800080, 0xFF0000, 0xFFA500, 0xFFFF00, 0x008000, 0x00BFFF },
    { 0x00BFFF, 0x0000FF, 0x800080, 0xFF0000, 0xFFA500, 0xFFFF00, 0x008000 }
};

void AlienwareController::UpdateMode()
{
    /*-----------------------------------------------------*\
    | If there are no updates, don't bother running this    |
    \*-----------------------------------------------------*/
    if(!dirty)
    {
        return;
    }

    bool result = UserAnimation(ALIENWARE_COMMAND_USER_ANIM_NEW, ALIENWARE_COMMAND_USER_ANIM_KEYBOARD, 0);

    if(!result)
    {
        return;
    }

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        alienware_zone zone = zones[zone_idx];

        result = SelectZones({static_cast<uint8_t>(zone_idx)});

        if(!result)
        {
            return;
        }

        /*-------------------------------------------------*\
        | Some modes use 0x07D0 for their duration as sent  |
        | by AWCC traces, maybe 2000ms?                     |
        \*-------------------------------------------------*/
        switch (zone.mode)
        {
            case ALIENWARE_MODE_COLOR:
                result = ModeAction(zone.mode, 2000, ALIENWARE_TEMPO_MAX, zone.color[0]);
                break;

            case ALIENWARE_MODE_PULSE:
                result = ModeAction(zone.mode, zone.period, zone.tempo, zone.color[0]);
                break;

            case ALIENWARE_MODE_MORPH:
                {
                    uint8_t  zones[2]   = { zone.mode,              zone.mode     };
                    uint16_t periods[2] = { zone.period,            zone.period   };
                    uint16_t tempos[2]  = { zone.tempo,             zone.tempo    };
                    RGBColor colors[2]  = { zone.color[0],          zone.color[1] };

                    result = MultiModeAction(zones, periods, tempos, colors, 2);
                }
                break;

            case ALIENWARE_MODE_SPECTRUM:
                {
                    uint8_t  zones[7]   = { ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH };
                    uint16_t periods[7] = { zone.period,            zone.period,
                                            zone.period,            zone.period,
                                            zone.period,            zone.period,
                                            zone.period };
                    uint16_t tempos[7] = {  zone.tempo,             zone.tempo,
                                            zone.tempo,             zone.tempo,
                                            zone.tempo,             zone.tempo,
                                            zone.tempo };

                    result = MultiModeAction(zones, periods, tempos, rainbow_colors[0], 7);
                }
                break;

            case ALIENWARE_MODE_RAINBOW:
                {
                    uint8_t  zones[7]   = { ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH,
                                            ALIENWARE_MODE_MORPH };
                    uint16_t periods[7] = { zone.period,            zone.period,
                                            zone.period,            zone.period,
                                            zone.period,            zone.period,
                                            zone.period };
                    uint16_t tempos[7]  = { zone.tempo,             zone.tempo,
                                            zone.tempo,             zone.tempo,
                                            zone.tempo,             zone.tempo,
                                            zone.tempo };

                    result = MultiModeAction(zones, periods, tempos, rainbow_colors[zone_idx], 7);
                }
                break;

            case ALIENWARE_MODE_BREATHING:
                {
                    uint8_t  zones[2]   = { ALIENWARE_MODE_MORPH,   ALIENWARE_MODE_MORPH };
                    uint16_t periods[2] = { zone.period,            zone.period          };
                    uint16_t tempos[2]  = { zone.tempo,             zone.tempo           };
                    RGBColor colors[2]  = { zone.color[0],          0x0                  };

                    result = MultiModeAction(zones, periods, tempos, colors, 2);
                }
                break;

            default:
                result = false;
        }

        if(!result)
        {
            return;
        }
    }

    result = UserAnimation(ALIENWARE_COMMAND_USER_ANIM_FINISH_PLAY, ALIENWARE_COMMAND_USER_ANIM_KEYBOARD, 0);

    /*-------------------------------------------------*\
    | Don't update dirty flag if there's an error       |
    \*-------------------------------------------------*/
    if(!result)
    {
        return;
    }

    dirty = false;
}

void AlienwareController::UpdateController()
{
    UpdateMode();
    UpdateDim();
}
