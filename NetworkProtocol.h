/*-----------------------------------------*\
|  NetworkProtocol.h                        |
|                                           |
|  Protocol header for OpenRGB SDK          |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/9/2020    |
\*-----------------------------------------*/

#pragma once

/*-----------------------------------------*\
| Default OpenRGB SDK port is 6742          |
| This is "ORGB" on a phone keypad          |
\*-----------------------------------------*/
#define OPENRGB_SDK_PORT 6742

typedef struct NetPacketHeader
{
    char                pkt_magic[4];               /* Magic value "ORGB" identifies beginning of packet    */
    unsigned int        pkt_dev_idx;                /* Device index                                         */
    unsigned int        pkt_id;                     /* Packet ID                                            */
    unsigned int        pkt_size;                   /* Packet size                                          */
} NetPacketHeader;

enum
{
    /*----------------------------------------------------------------------------------------------------------*\
    | Network requests                                                                                           |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_REQUEST_CONTROLLER_COUNT      = 0,    /* Request RGBController device count from server       */
    NET_PACKET_ID_REQUEST_CONTROLLER_DATA       = 1,    /* Request RGBController data block                     */

    NET_PACKET_ID_SET_CLIENT_NAME               = 50,   /* Send client name string to server                    */

    NET_PACKET_ID_DEVICE_LIST_UPDATED           = 100,  /* Indicate to clients that device list has updated     */
    /*----------------------------------------------------------------------------------------------------------*\
    | RGBController class functions                                                                              |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE      = 1000, /* RGBController::ResizeZone()                          */

    NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS      = 1050, /* RGBController::UpdateLEDs()                          */
    NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS  = 1051, /* RGBController::UpdateZoneLEDs()                      */
    NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED = 1052, /* RGBController::UpdateSingleLED()                     */

    NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE   = 1100, /* RGBController::SetCustomMode()                       */
    NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE      = 1101, /* RGBController::UpdateMode()                          */
};
