/*---------------------------------------------------------*\
| ManualDevicesTypeManager.cpp                              |
|                                                           |
|   OpenRGB Manual Devices Type Manager registers available |
|   types of Manually Added devices and generates UI        |
|   elements for their settings                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ManualDevicesTypeManager.h"

ManualDeviceTypeBlock::ManualDeviceTypeBlock(const std::string& _name, const std::string& _settingsSection, ManualDeviceEntrySpawnFunction _entrySpawnFunction)
{
    name                = _name;
    settingsSection     = _settingsSection;
    entrySpawnFunction  = _entrySpawnFunction;
}

BaseManualDeviceEntry* ManualDeviceTypeBlock::spawn(const json& data) const
{
    BaseManualDeviceEntry* result = entrySpawnFunction(data);

    if(result)
    {
        result->setSettingsSection(settingsSection);
    }

    return result;
}

ManualDevicesTypeManager* ManualDevicesTypeManager::instance;

ManualDevicesTypeManager *ManualDevicesTypeManager::get()
{
    if(!instance)
    {
        instance = new ManualDevicesTypeManager();
    }

    return instance;
}

ManualDevicesTypeManager::ManualDevicesTypeManager()
{

}

void ManualDevicesTypeManager::registerType(const std::string& name, const std::string& settingsSection, ManualDeviceEntrySpawnFunction entrySpawnFunction)
{
    types.push_back(ManualDeviceTypeBlock(name, settingsSection, entrySpawnFunction));
}

std::vector<ManualDeviceTypeBlock> ManualDevicesTypeManager::getRegisteredTypes()
{
    return types;
}

std::vector<std::string> ManualDevicesTypeManager::getRegisteredTypeNames()
{
    std::vector<std::string> result;
    result.resize(types.size());

    for(std::size_t i = 0; i < types.size(); i++)
    {
        result[i] = types[i].name;
    }

    return result;
}

BaseManualDeviceEntry* ManualDevicesTypeManager::spawnByTypeName(const std::string& typeName, const json& data)
{
    for(std::size_t i = 0; i < types.size(); i++)
    {
        if(types[i].name == typeName)
        {
            return types[i].spawn(data);
        }
    }

    return nullptr;
}
