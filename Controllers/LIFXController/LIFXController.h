/*---------------------------------------------------------*\
|  Definitions for LIFX                                     |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 2/5/2022         |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

#pragma once

enum
{
    LIFX_FRAME_HEADER_OFFSET_SIZE       = 0,    /* 2 bytes, size of the entire message in bytes */
    LIFX_FRAME_HEADER_OFFSET_PROTOCOL   = 2,    /* Protocol number, must be 1024                */
    LIFX_FRAME_HEADER_OFFSET_FLAGS      = 3,    /* Bits 0-3 are part of Protocol                */
                                                /* Bit 4, addressable flag                      */
                                                /* Bit 5, tagged flag                           */
                                                /* Bit 6/7, origin value                        */
    LIFX_FRAME_HEADER_OFFSET_SOURCE     = 4,    /* Source identifier, unique value set by client*/
    LIFX_FRAME_ADDRESS_OFFSET_TARGET    = 8,    /* 6 byte device address (MAC) or zero          */
                                                /* Last two bytes should be 0                   */
    LIFX_FRAME_ADDRESS_OFFSET_FLAGS     = 22,   /* Bit 0, res_required flag                     */
                                                /* Bit 1, ack_required flag                     */
    LIFX_FRAME_ADDRESS_OFFSET_SEQUENCE  = 23,   /* Wrap around message sequence number          */
    LIFX_PROTOCOL_HEADER_OFFSET_TYPE    = 32,   /* Message type determines the payload used     */
};

enum
{
    LIFX_SET_COLOR_OFFSET_HUE           = 1,    /* 16-bit hue value                             */
    LIFX_SET_COLOR_OFFSET_SATURATION    = 3,    /* 16-bit saturation value                      */
    LIFX_SET_COLOR_OFFSET_BRIGHTNESS    = 5,    /* 16-bit brightness value                      */
    LIFX_SET_COLOR_OFFSET_KELVIN        = 7,    /* 16-bit kelvin value                          */
    LIFX_SET_COLOR_OFFSET_DURATION      = 9,    /* 32-bit brightness value                      */
};

class LIFXController
{
public:
    LIFXController(std::string ip, std::string name);
    ~LIFXController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    unsigned char       data_buf[49];
    unsigned char*      data;
    unsigned char       sequence;
    unsigned int        source;
    std::string         name;
    std::string         firmware_version;
    std::string         module_name;
    std::string         module_mac;
    std::string         location;
    net_port            port;

    /*-----------------------------------------------------*\
    | Functions for filling in LIFX header                  |
    \*-----------------------------------------------------*/
    void FrameHeader
        (
        unsigned short  size,
        bool            addressable,
        bool            tagged,
        unsigned char   origin,
        unsigned int    source
        );

    void FrameAddress
        (
        unsigned char * target,
        bool            res_required,
        bool            ack_required,
        unsigned char   sequence
        );

    void ProtocolAddress
        (
        unsigned short  type
        );
};
