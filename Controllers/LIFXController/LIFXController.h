/*---------------------------------------------------------*\
| LIFXController.h                                          |
|                                                           |
|   Driver for LIFX                                         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <climits>
#include "RGBController.h"
#include "net_port.h"

#define LIFX_MANUFACTURER    "LIFX"
#define LIFX_UDP_PORT        "56700"
#define LIFX_PROTOCOL        1024
#define TARGET_LENGTH        8
#define DEFAULT_KELVIN       3500
#define HSBK_LENGTH          8

/*---------------------*\
| Packet size constants |
\*---------------------*/
#define LIFX_PACKET_HEADER_LENGTH                   36
#define LIFX_SET_COLOR_PACKET_LENGTH                13
#define LIFX_SET_COLOR_ZONES_PACKET_LENGTH          15
#define LIFX_GET_COLOR_ZONES_PACKET_LENGTH          2
#define LIFX_STATE_ZONE_PACKET_LENGTH               10
#define LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_LENGTH 664

/*---------------------------------------------------------------------------*\
| https://lan.developer.lifx.com/docs/field-types#multizoneapplicationrequest |
\*---------------------------------------------------------------------------*/
enum
{
    LIFX_MULTIZONE_APPLICATION_REQUEST_NO_APPLY   = 0,
    LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY      = 1,
    LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY_ONLY = 2
};


/*----------------------------------------------------------------*\
| https://lan.developer.lifx.com/docs/representing-color-with-hsbk |
\*----------------------------------------------------------------*/
typedef struct
{
    unsigned short hue;        /* 0-360 value normalized to 0-65535                            */
    unsigned short saturation; /* 0-1 value normalized to 0-65535                              */
    unsigned short brightness; /* 0-1 value normalized to 0-65535                              */
    unsigned short kelvin;     /* 0-65535 value                                                */
                               /*   Note: Devices may only support a subset of the full range. */
} hsbk_t;

/*-----------------*\
| LIFX packet types |
\*-----------------*/
enum
{
    LIFX_PACKET_TYPE_SET_COLOR                = 102,
    LIFX_PACKET_TYPE_SET_COLOR_ZONES          = 501,
    LIFX_PACKET_TYPE_GET_COLOR_ZONES          = 502,
    LIFX_PACKET_TYPE_STATE_ZONE               = 503,
    LIFX_PACKET_TYPE_SET_EXTENDED_COLOR_ZONES = 510
};

/*-----------------------------------------------------*\
| LIFX header packet offsets                            |
| https://lan.developer.lifx.com/docs/encoding-a-packet |
\*-----------------------------------------------------*/
enum
{
    LIFX_HEADER_PACKET_OFFSET_SIZE                                   = 0,  /* 2 bytes, size of the entire message in bytes  */
    LIFX_HEADER_PACKET_OFFSET_PROTOCOL                               = 2,  /* Protocol number, must be 1024                 */
    LIFX_HEADER_PACKET_OFFSET_ADDRESSABLE_TAGGED_ORIGIN              = 3,  /* Bits 0-3 are part of Protocol                 */
                                                                           /* Bit 4, addressable flag                       */
                                                                           /* Bit 5, tagged flag                            */
                                                                           /* Bit 6/7, origin value                         */
    LIFX_HEADER_PACKET_OFFSET_SOURCE                                 = 4,  /* Source identifier, unique value set by client */
    LIFX_HEADER_PACKET_OFFSET_TARGET                                 = 8,  /* 6 byte device address (MAC) or zero           */
                                                                           /* Last two bytes should be 0                    */
    LIFX_HEADER_PACKET_OFFSET_RESPONSE_REQUIRED_ACKNOWLEDGE_REQUIRED = 22, /* Bit 0, res_required flag                      */
                                                                           /* Bit 1, ack_required flag                      */
    LIFX_HEADER_PACKET_OFFSET_SEQUENCE                               = 23, /* Wrap around message sequence number           */
    LIFX_HEADER_PACKET_OFFSET_PACKET_TYPE                            = 32  /* Message type determines the payload used      */
};

/*---------------------------------------------------------------------------*\
| LIFX set color packet offsets                                               |
| https://lan.developer.lifx.com/docs/changing-a-device#setcolor---packet-102 |
\*---------------------------------------------------------------------------*/
enum
{
                                                                             /* 1 byte, reserved                                     */
    LIFX_SET_COLOR_PACKET_OFFSET_HUE        = LIFX_PACKET_HEADER_LENGTH + 1, /* 2 bytes, hue as a 0-65535 value                      */
    LIFX_SET_COLOR_PACKET_OFFSET_SATURATION = LIFX_PACKET_HEADER_LENGTH + 3, /* 2 bytes, saturation as a 0-65535 value               */
    LIFX_SET_COLOR_PACKET_OFFSET_BRIGHTNESS = LIFX_PACKET_HEADER_LENGTH + 5, /* 2 bytes, brightness as a 0-65535 value               */
    LIFX_SET_COLOR_PACKET_OFFSET_KELVIN     = LIFX_PACKET_HEADER_LENGTH + 7, /* 2 bytes, kelvin as a 0-65535 value.                  */
                                                                             /*     Note: The actual max for this is device specific */
    LIFX_SET_COLOR_PACKET_OFFSET_DURATION   = LIFX_PACKET_HEADER_LENGTH + 9, /* 4 bytes, transition time in ms                       */
};

/*--------------------------------------------------------------------------------*\
| LIFX set color zones packet offsets                                              |
| https://lan.developer.lifx.com/docs/changing-a-device#setcolorzones---packet-501 |
\*--------------------------------------------------------------------------------*/
enum
{
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_START_INDEX = LIFX_PACKET_HEADER_LENGTH + 0,  /* 1 byte, the first zone in the segment we are changing */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_END_INDEX   = LIFX_PACKET_HEADER_LENGTH + 1,  /* 1 byte, the last zone in the segment we are changing  */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_HUE         = LIFX_PACKET_HEADER_LENGTH + 2,  /* 2 bytes, hue as a 0-65535 value                       */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_SATURATION  = LIFX_PACKET_HEADER_LENGTH + 4,  /* 2 bytes, saturation as a 0-65535 value                */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_BRIGHTNESS  = LIFX_PACKET_HEADER_LENGTH + 6,  /* 2 bytes, brightness as a 0-65535 value                */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_KELVIN      = LIFX_PACKET_HEADER_LENGTH + 8,  /* 2 bytes, kelvin as a 0-65535 value.                   */
                                                                                     /*     Note: The actual max for this is device specific  */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_DURATION    = LIFX_PACKET_HEADER_LENGTH + 10, /* 4 bytes, transition time in ms                        */
    LIFX_SET_COLOR_ZONES_PACKET_OFFSET_APPLY       = LIFX_PACKET_HEADER_LENGTH + 14  /* 1 byte, multizone application request                 */
};

/*-------------------------------------------------------------------------------------------*\
| LIFX get color zones packet offsets                                                         |
| https://lan.developer.lifx.com/docs/querying-the-device-for-data#getcolorzones---packet-502 |
\*-------------------------------------------------------------------------------------------*/
enum
{
    LIFX_GET_COLOR_ZONES_PACKET_OFFSET_START_INDEX = LIFX_PACKET_HEADER_LENGTH + 0, /* 1 byte, The first zone you want to get information from  */
    LIFX_GET_COLOR_ZONES_PACKET_OFFSET_END_INDEX   = LIFX_PACKET_HEADER_LENGTH + 1, /* 1 byte, The second zone you want to get information from */
};

/*-------------------------------------------------------------------------------*\
| LIFX state zone packet offsets                                                  |
| https://lan.developer.lifx.com/docs/information-messages#statezone---packet-503 |
\*-------------------------------------------------------------------------------*/
enum
{
    LIFX_STATE_ZONE_PACKET_OFFSET_ZONES_COUNT = LIFX_PACKET_HEADER_LENGTH + 0, /* 1 byte, the total number of zones on the strip.      */
    LIFX_STATE_ZONE_PACKET_OFFSET_ZONE_INDEX  = LIFX_PACKET_HEADER_LENGTH + 1, /* 1 byte, the zone this packet refers to.              */
    LIFX_STATE_ZONE_PACKET_OFFSET_HUE         = LIFX_PACKET_HEADER_LENGTH + 2, /* 2 bytes, hue as a 0-65535 value                      */
    LIFX_STATE_ZONE_PACKET_OFFSET_SATURATION  = LIFX_PACKET_HEADER_LENGTH + 4, /* 2 bytes, saturation as a 0-65535 value               */
    LIFX_STATE_ZONE_PACKET_OFFSET_BRIGHTNESS  = LIFX_PACKET_HEADER_LENGTH + 6, /* 2 bytes, brightness as a 0-65535 value               */
    LIFX_STATE_ZONE_PACKET_OFFSET_KELVIN      = LIFX_PACKET_HEADER_LENGTH + 8, /* 2 bytes, kelvin as a 0-65535 value.                  */
                                                                               /*     Note: The actual max for this is device specific */
};

/*----------------------------------------------------------------------------------------*\
| LIFX set extended color zones packet offsets                                             |
| https://lan.developer.lifx.com/docs/changing-a-device#setextendedcolorzones---packet-510 |
\*----------------------------------------------------------------------------------------*/
enum
{
    LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_DURATION     = LIFX_PACKET_HEADER_LENGTH + 0, /* 4 bytes, transition time in ms                    */
    LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_APPLY        = LIFX_PACKET_HEADER_LENGTH + 4, /* 1 byte, multizone application request             */
    LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_ZONE_INDEX   = LIFX_PACKET_HEADER_LENGTH + 5, /* 2 bytes, The first zone to apply colors from.     */
                                                                                              /*     If the light has more than 82 zones, then     */
                                                                                              /*     send multiple messages with different indices */
                                                                                              /*     to update the whole device.                   */
    LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_COLORS_COUNT = LIFX_PACKET_HEADER_LENGTH + 7, /* 1 byte, The number of colors in the colors field  */
    LIFX_SET_EXTENDED_COLOR_ZONES_PACKET_OFFSET_COLORS       = LIFX_PACKET_HEADER_LENGTH + 8, /* 656 bytes (82 * 4 * 2), 82 HSBK values to change  */
                                                                                              /*     the device with                               */
};

class LIFXController
{
public:
    LIFXController(std::string ip, std::string name, bool multizone, bool extended_multizone);
    ~LIFXController();

    std::string  GetLocation();
    std::string  GetName();
    std::string  GetVersion();
    std::string  GetManufacturer();
    std::string  GetUniqueID();
    unsigned int GetZoneCount();

    void         FetchZoneCount();
    void         SetColors(std::vector<RGBColor> colors);

private:
    RGBColor       cached_colors[UCHAR_MAX];
    unsigned int   zone_count;
    size_t         data_buf_size;
    unsigned char* data;
    unsigned char  sequence;
    std::string    name;
    std::string    firmware_version;
    std::string    module_name;
    std::string    module_mac;
    std::string    location;
    net_port       port;
    bool           multizone;
    bool           extended_multizone;

    void           SetColor(RGBColor color);
    void           SetZoneColor(RGBColor color, unsigned int zone);
    void           SetZoneColors(std::vector<RGBColor> colors);
    void           RGBColorToHSBK(RGBColor color, hsbk_t* hsbk);

    /*---------------------*\
    | Packet helper methods |
    \*---------------------*/
    void           HeaderPacketSetDefaults(unsigned short packet_type);
    unsigned short HeaderPacketGetSize();
    void           HeaderPacketSetSize(unsigned short size);
    unsigned short HeaderPacketGetProtocol();
    void           HeaderPacketSetProtocol(unsigned short protocol=LIFX_PROTOCOL);
    void           HeaderPacketSetAddressable(bool addressable=true);
    void           HeaderPacketSetTagged(bool tagged=false);
    void           HeaderPacketSetOrigin(unsigned char origin=0);
    void           HeaderPacketSetSource(unsigned int source=2);
    void           HeaderPacketSetTarget(unsigned char* target);
    void           HeaderPacketSetResponseRequired(bool response_required=false);
    void           HeaderPacketSetAcknowledgeRequired(bool acknowledge_required=false);
    void           HeaderPacketSetSequence(unsigned char sequence);
    unsigned short HeaderPacketGetPacketType();
    void           HeaderPacketSetPacketType(unsigned short packet_type);

    void           SetColorPacketSetDuration(unsigned int duration=0);
    void           SetColorPacketSetHSBK(hsbk_t* hsbk);

    void           SetColorZonesPacketSetStartIndex(unsigned char start_index);
    void           SetColorZonesPacketSetEndIndex(unsigned char end_index);
    void           SetColorZonesPacketSetHSBK(hsbk_t* hsbk);
    void           SetColorZonesPacketSetDuration(unsigned int duration=0);
    void           SetColorZonesPacketSetApply(unsigned char apply=LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY);

    void           GetColorZonesPacketSetStartIndex(unsigned char start_index=0);
    void           GetColorZonesPacketSetEndIndex(unsigned char end_index=0);

    unsigned char  StateZonePacketGetZonesCount();

    void           SetExtendedColorZonesPacketSetDuration(unsigned int duration=0);
    void           SetExtendedColorZonesPacketSetApply(unsigned char apply=LIFX_MULTIZONE_APPLICATION_REQUEST_APPLY);
    void           SetExtendedColorZonesPacketSetZoneIndex(unsigned short zone_index=0);
    void           SetExtendedColorZonesPacketSetColors(std::vector<RGBColor> colors);
};
