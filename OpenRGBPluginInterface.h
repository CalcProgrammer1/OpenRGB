/*-----------------------------------------*\
|  OpenRGBPluginInterface.h                 |
|                                           |
|  OpenRGB Plugin Interface Class           |
|                                           |
|  herosilas12 (CoffeeIsLife)   12/11/2020  |
|  Adam Honse (CalcProgrammer1) 1/5/2021    |
\*-----------------------------------------*/

#pragma once

#include "ResourceManager.h"

#include <QtPlugin>
#include <QLabel>

#define OpenRGBPluginInterface_IID  "com.OpenRGBPluginInterface"

#define OPENRGB_PLUGIN_API_VERSION  1

struct OpenRGBPluginInfo
{
    /*-------------------------------------------------------------------------------------------------*\
    | Plugin Details                                                                                    |
    \*------------------------------------------------------------------------------------------------ */    
    std::string                 Name;           /* Plugin name string                                  */
    std::string                 Description;    /* Plugin description string                           */
    std::string                 Version;        /* Plugin version string                               */
    std::string                 Commit;         /* Plugin commit (git or otherwise) string             */
    std::string                 URL;            /* Plugin project URL string                           */
    QImage                      Icon;           /* Icon image (displayed 64x64)                        */

    /*-------------------------------------------------------------------------------------------------*\
    | Plugin Tab Configuration                                                                          |
    \*-------------------------------------------------------------------------------------------------*/
    std::string                 Location;       /* Plugin tab location.  This field is mandatory       */
                                                /* Options are "TopTabBar", "DevicesTab",              */
                                                /* "InformationTab", or "SettingsTab"                  */
    std::string                 Label;          /* Plugin tab label string                             */
    std::string                 TabIconString;  /* Plugin tab icon string, leave empty to use custom   */
    QImage                      TabIcon;        /* Custom tab icon image (displayed 16x16)             */
};

class OpenRGBPluginInterface
{
public:
    virtual                    ~OpenRGBPluginInterface() {}

    /*-------------------------------------------------------------------------------------------------*\
    | Plugin Information                                                                                |
    \*-------------------------------------------------------------------------------------------------*/
    virtual OpenRGBPluginInfo   GetPluginInfo()                                                     = 0;
    virtual unsigned int        GetPluginAPIVersion()                                               = 0;

    /*-------------------------------------------------------------------------------------------------*\
    | Plugin Functionality                                                                              |
    \*-------------------------------------------------------------------------------------------------*/
    virtual void                Initialize(bool dark_theme, ResourceManager* resource_manager_ptr)  = 0;
    virtual QWidget            *CreateGUI(QWidget* parent)                                          = 0;
    virtual void                Unload()                                                            = 0;
};

Q_DECLARE_INTERFACE(OpenRGBPluginInterface, OpenRGBPluginInterface_IID)
