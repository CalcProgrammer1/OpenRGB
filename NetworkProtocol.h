/*---------------------------------------------------------*\
| NetworkProtocol.h                                         |
|                                                           |
|   OpenRGB SDK network protocol                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------------------*\
| OpenRGB SDK protocol version                                          |
|                                                                       |
|   0:      Initial (unversioned) protocol                              |
|   1:      Add versioning, vendor string (Release 0.5)                 |
|   2:      Add profile controls (Release 0.6)                          |
|   3:      Add brightness field to modes (Release 0.7)                 |
|   4:      Add segments field to zones, network plugins (Release 0.9)  |
|   5:      Zone flags, controller flags, resizable effects-only zones  |
|               (Release Candidate 1.0rc2)                              |
|   6:      Server name, Features TBD (Release 1.0)                     |
\*---------------------------------------------------------------------*/
#define OPENRGB_SDK_PROTOCOL_VERSION    6

/*-----------------------------------------------------*\
| Default Interface to bind to.                         |
\*-----------------------------------------------------*/
#define OPENRGB_SDK_HOST "0.0.0.0"

/*-----------------------------------------------------*\
| Default OpenRGB SDK port is 6742                      |
| This is "ORGB" on a phone keypad                      |
\*-----------------------------------------------------*/
#define OPENRGB_SDK_PORT 6742

/*-----------------------------------------------------*\
| OpenRGB SDK Magic Value "ORGB"                        |
\*-----------------------------------------------------*/
#define OPENRGB_SDK_MAGIC_SIZE 4
extern const char openrgb_sdk_magic[OPENRGB_SDK_MAGIC_SIZE];

typedef struct NetPacketHeader
{
    char                pkt_magic[4];                   /* Magic value "ORGB" identifies beginning of packet    */
    unsigned int        pkt_dev_id;                     /* Device ID                                            */
    unsigned int        pkt_id;                         /* Packet ID                                            */
    unsigned int        pkt_size;                       /* Packet size                                          */
} NetPacketHeader;

enum
{
    NET_CLIENT_FLAG_SUPPORTS_RGBCONTROLLER      = ( 1 << 0 ),   /* Client supports RGBController API            */
    NET_CLIENT_FLAG_SUPPORTS_PROFILEMANAGER     = ( 1 << 1 ),   /* Client supports ProfileManager API           */
    NET_CLIENT_FLAG_SUPPORTS_PLUGINMANAGER      = ( 1 << 2 ),   /* Client supports PluginManager API            */
    NET_CLIENT_FLAG_SUPPORTS_SETTINGSMANAGER    = ( 1 << 3 ),   /* Client supports SettingsManager API          */

    NET_CLIENT_FLAG_REQUEST_LOCAL_CLIENT        = ( 1 << 16 ),  /* Request local client                         */
};

enum
{
    NET_SERVER_FLAG_SUPPORTS_RGBCONTROLLER      = ( 1 << 0 ),   /* Server supports RGBController API            */
    NET_SERVER_FLAG_SUPPORTS_PROFILEMANAGER     = ( 1 << 1 ),   /* Server supports ProfileManager API           */
    NET_SERVER_FLAG_SUPPORTS_PLUGINMANAGER      = ( 1 << 2 ),   /* Server supports PluginManager API            */
    NET_SERVER_FLAG_SUPPORTS_SETTINGSMANAGER    = ( 1 << 3 ),   /* Server supports SettingsManager API          */
    NET_SERVER_FLAG_SUPPORTS_DETECTION          = ( 1 << 4 ),   /* Server supports detection functions          */

    NET_SERVER_FLAG_LOCAL_CLIENT                = ( 1 << 16),   /* Confirm that client is local client          */
};

enum
{
    /*----------------------------------------------------------------------------------------------------------*\
    | Network requests                                                                                           |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_REQUEST_CONTROLLER_COUNT      = 0,    /* Request RGBController device count from server       */
    NET_PACKET_ID_REQUEST_CONTROLLER_DATA       = 1,    /* Request RGBController data block                     */

    NET_PACKET_ID_REQUEST_PROTOCOL_VERSION      = 40,   /* Request OpenRGB SDK protocol version from server     */

    NET_PACKET_ID_SET_CLIENT_NAME               = 50,   /* Send client name string to server                    */
    NET_PACKET_ID_SET_SERVER_NAME               = 51,   /* Send server name string to client                    */
    NET_PACKET_ID_SET_CLIENT_FLAGS              = 52,   /* Send client flags to server                          */
    NET_PACKET_ID_SET_SERVER_FLAGS              = 53,   /* Send server flags to client                          */

    /*----------------------------------------------------------------------------------------------------------*\
    | Detection functions                                                                                        |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_DEVICE_LIST_UPDATED           = 100,  /* Indicate to clients that device list has updated     */
    NET_PACKET_ID_DETECTION_STARTED             = 101,  /* Indicate to clients that detection started           */
    NET_PACKET_ID_DETECTION_PROGRESS_CHANGED    = 102,  /* Indicate to clients that detection progress changed  */
    NET_PACKET_ID_DETECTION_COMPLETE            = 103,  /* Indicate to clients that detection completed         */

    NET_PACKET_ID_REQUEST_RESCAN_DEVICES        = 140,  /* Request rescan of devices                            */

    /*----------------------------------------------------------------------------------------------------------*\
    | ProfileManager functions                                                                                   |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST   = 150,  /* Get profile list                                 */
    NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE       = 151,  /* Save current configuration in a new profile      */
    NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE       = 152,  /* Load a given profile                             */
    NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE     = 153,  /* Delete a given profile                           */
    NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE     = 154,  /* Upload a profile to the server in JSON format    */
    NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE   = 155,  /* Download a profile from the server in JSON format*/
    NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE = 156,  /* Get the active profile name                      */
    NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED
                                                    = 157,  /* Indicate to clients active profile has changed   */
    NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED     = 158,  /* Notify to active client that profile has loaded  */
                                                            /* Forwards loaded profile data                     */
    NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD
                                                    = 159,  /* Indicate to clients profile about to load        */

    /*----------------------------------------------------------------------------------------------------------*\
    | PluginManager functions                                                                                    |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST     = 200,  /* Get list of plugins                              */
    NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC     = 201,  /* Interact with a plugin                           */

    /*----------------------------------------------------------------------------------------------------------*\
    | SettingsManager functions                                                                                  |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS      = 250,  /* Get settings for a given key in JSON format      */
    NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS      = 251,  /* Set settings for a given key in JSON format      */
    NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS     = 252,  /* Save settings                                    */

    /*----------------------------------------------------------------------------------------------------------*\
    | RGBController functions                                                                                    |
    \*----------------------------------------------------------------------------------------------------------*/
    NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE          = 1000, /* RGBController::ResizeZone()                      */
    NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS       = 1001, /* RGBController::ClearSegments()                   */
    NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT          = 1002, /* RGBController::AddSegment()                      */

    NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS          = 1050, /* RGBController::UpdateLEDs()                      */
    NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS      = 1051, /* RGBController::UpdateZoneLEDs()                  */
    NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED     = 1052, /* RGBController::UpdateSingleLED()                 */

    NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE       = 1100, /* RGBController::SetCustomMode()                   */
    NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE          = 1101, /* RGBController::UpdateMode()                      */
    NET_PACKET_ID_RGBCONTROLLER_SAVEMODE            = 1102, /* RGBController::SaveMode()                        */
    NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE      = 1103, /* RGBController::UpdateZoneMode()                  */

    NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE        = 1150, /* RGBController::SignalUpdate()                    */
};

void InitNetPacketHeader
    (
    NetPacketHeader *   pkt_hdr,
    unsigned int        pkt_dev_idx,
    unsigned int        pkt_id,
    unsigned int        pkt_size
    );
