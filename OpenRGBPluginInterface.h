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
    std::string                 Name;
    std::string                 Description;
    std::string                 Version;
    std::string                 Commit;
    std::string                 URL;

    std::string                 Location;
    std::string                 Label;

    QImage                      Icon;
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
