typedef struct NetPacketHeader
{
    unsigned char       pkt_magic[4];               /* Magic value "ORGB" identifies beginning of packet    */
    unsigned int        pkt_dev_idx;                /* Device index                                         */
    unsigned int        pkt_id;                     /* Packet ID                                            */
    unsigned int        pkt_size;                   /* Packet size                                          */
};

enum
{
    /*----------------------------------------------------------------------------------------------------------*\
    | Network requests                                                                                           |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_REQUEST_CONTROLLER_COUNT      = 0,    /* Request RGBController device count from server       */
    NET_PACKET_ID_REQUEST_CONTROLLER_DATA       = 1,    /* Request RGBController data block                     */

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

NetPacketHeader * InitNetPacketHeader
    (
    unsigned int        pkt_dev_idx,
    unsigned int        pkt_id,
    unsigned int        pkt_size
    );