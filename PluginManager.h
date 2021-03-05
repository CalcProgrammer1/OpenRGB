#pragma once

#include "OpenRGBPluginInterface.h"
#include "ResourceManager.h"

#include <QPluginLoader>
#include <QLabel>
#include <QtPlugin>
#include <QDir>

#include <string>
#include <iostream>

class PluginManager
{
public:
    std::vector<OpenRGBPluginInterface*> ActivePlugins;

    void ScanAndLoadPlugins(bool dark_theme);
};
