/*---------------------------------------------------------*\
| ManualDevicesTypeManager.h                                |
|                                                           |
|   OpenRGB Manual Devices Type Manager registers UI        |
|   classes for managing Manually Added devices             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"

#include <vector>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;


class ManualDeviceTypeBlock
{
public:
    ManualDeviceTypeBlock(const std::string& name, const std::string& settingsSection, ManualDeviceEntrySpawnFunction entrySpawnFunction);
    std::string name; // Name as listed in the drop-down list
    std::string settingsSection; // Settings Section name, as listed in Config file
    BaseManualDeviceEntry* spawn(const json &data) const;

private:
    ManualDeviceEntrySpawnFunction entrySpawnFunction;
};

class ManualDevicesTypeManager
{
	public:
        static ManualDevicesTypeManager* get();
        void registerType(const std::string& name, const std::string& settingsSection, ManualDeviceEntrySpawnFunction entrySpawnFunction);

        std::vector<ManualDeviceTypeBlock> getRegisteredTypes();
        std::vector<std::string> getRegisteredTypeNames();
        BaseManualDeviceEntry* spawnByTypeName(const std::string& typeName, const json& data);
		
	private:
		static ManualDevicesTypeManager* instance;
        std::vector<ManualDeviceTypeBlock> types;

        ManualDevicesTypeManager();
};
