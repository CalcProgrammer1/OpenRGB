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

#define OpenRGBPluginInterface_IID "com.OpenRGBPluginInterface"

struct OpenRGBPluginInfo
{
    std::string                 PluginName;
    std::string                 PluginDescription;
    std::string                 PluginLocation;

    bool                        HasCustom;
    QLabel                      *PluginLabel;
};

class OpenRGBPluginInterface
{
public:
    virtual                    ~OpenRGBPluginInterface() {}

    virtual OpenRGBPluginInfo   Initialize(bool dark_theme, ResourceManager* resource_manager_ptr)  = 0;

    virtual QWidget            *CreateGUI(QWidget* parent)                                          = 0;

    OpenRGBPluginInfo           info;
};

Q_DECLARE_INTERFACE(OpenRGBPluginInterface, OpenRGBPluginInterface_IID)
