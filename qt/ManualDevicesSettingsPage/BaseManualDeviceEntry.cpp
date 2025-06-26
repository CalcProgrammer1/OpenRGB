/*-----------------------------------------------------------------*\
| BaseManualDeviceEntry.cpp                                         |
|                                                                   |
|   Base class to all user-defined device settings entries          |
|                                                                   |
|   This file is part of the OpenRGB project                        |
|   SPDX-License-Identifier: GPL-2.0-only                           |
\*-----------------------------------------------------------------*/

#include "BaseManualDeviceEntry.h"

#include "ManualDevicesTypeManager.h"

void BaseManualDeviceEntry::setSettingsSection(const std::string& section)
{
	settingsSection = section;
}

std::string BaseManualDeviceEntry::getSettingsSection()
{
	return settingsSection;
}

ManualDeviceTypeRegistrator::ManualDeviceTypeRegistrator(const std::string& name, const std::string& settingsEntry, ManualDeviceEntrySpawnFunction entrySpawnFunction)
{
    ManualDevicesTypeManager::get()->registerType(name, settingsEntry, entrySpawnFunction);
};
