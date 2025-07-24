/*---------------------------------------------------------*\
| OpenRGBPluginInterface.h                                  |
|                                                           |
|   OpenRGB Plugin API                                      |
|                                                           |
|   herosilas12 (CoffeeIsLife)                  11 Dec 2020 |
|   Adam Honse (CalcProgrammer1)                05 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QtPlugin>
#include <QLabel>
#include <QMenu>
#include "ResourceManagerInterface.h"

#define OpenRGBPluginInterface_IID  "com.OpenRGBPluginInterface"

/*-----------------------------------------------------------------------------------------------------*\
| OpenRGB Plugin API Versions                                                                           |
| 0:    OpenRGB 0.6     Unversioned, early plugin API.                                                  |
| 1:    OpenRGB 0.61    First versioned API, introduced with plugin settings changes                    |
| 2:    OpenRGB 0.7     First released versioned API, callback unregister functions in ResourceManager  |
| 3:    OpenRGB 0.9     Use filesystem::path for paths, Added segments                                  |
| 4:    OpenRGB 1.0rc2  Resizable effects-only zones, zone flags                                        |
| 5:    OpenRGB 1.0     RGBController API overhaul, protected members, features TBD                     |
\*-----------------------------------------------------------------------------------------------------*/
#define OPENRGB_PLUGIN_API_VERSION  5

/*---------------------------------------------------------*\
| Plugin Tab Location Values                                |
\*---------------------------------------------------------*/
enum
{
    OPENRGB_PLUGIN_LOCATION_TOP         = 0,    /* Top-level tab (no icon)                             */
    OPENRGB_PLUGIN_LOCATION_DEVICES     = 1,    /* Devices tab                                         */
    OPENRGB_PLUGIN_LOCATION_INFORMATION = 2,    /* Information tab                                     */
    OPENRGB_PLUGIN_LOCATION_SETTINGS    = 3,    /* Settings tab                                        */
};

struct OpenRGBPluginInfo
{
    /*-----------------------------------------------------*\
    | Plugin Details                                        |
    \*-----------------------------------------------------*/
    std::string                 Name;           /* Plugin name string                                  */
    std::string                 Description;    /* Plugin description string                           */
    std::string                 Version;        /* Plugin version string                               */
    std::string                 Commit;         /* Plugin commit (git or otherwise) string             */
    std::string                 URL;            /* Plugin project URL string                           */
    QImage                      Icon;           /* Icon image (displayed 64x64)                        */

    /*-----------------------------------------------------*\
    | Plugin Tab Configuration                              |
    \*-----------------------------------------------------*/
    unsigned int                Location;       /* Plugin tab location from Plugin Tab Location enum   */
                                                /* This field is mandatory, an invalid value will      */
                                                /* prevent plugin tab from being displayed             */
    std::string                 Label;          /* Plugin tab label string                             */
    std::string                 TabIconString;  /* Plugin tab icon string, leave empty to use custom   */
    QImage                      TabIcon;        /* Custom tab icon image (displayed 16x16)             */

    /*-----------------------------------------------------*\
    | Plugin SDK Details                                    |
    \*-----------------------------------------------------*/
    unsigned int                ProtocolVersion;/* Plugin SDK protocol version                         */
};

class OpenRGBPluginInterface
{
public:
    virtual                    ~OpenRGBPluginInterface() {}

    /*-----------------------------------------------------*\
    | Plugin Information                                    |
    \*-----------------------------------------------------*/
    virtual OpenRGBPluginInfo   GetPluginInfo()                                                     = 0;
    virtual unsigned int        GetPluginAPIVersion()                                               = 0;

    /*-----------------------------------------------------*\
    | Plugin Functionality                                  |
    \*-----------------------------------------------------*/
    virtual void                Load(ResourceManagerInterface* resource_manager_ptr)                = 0;
    virtual QWidget*            GetWidget()                                                         = 0;
    virtual QMenu*              GetTrayMenu()                                                       = 0;
    virtual void                Unload()                                                            = 0;
    virtual unsigned char*      OnSDKCommand(unsigned int pkt_id, unsigned char * pkt_data, unsigned int *pkt_size)      = 0;
};

Q_DECLARE_INTERFACE(OpenRGBPluginInterface, OpenRGBPluginInterface_IID)
