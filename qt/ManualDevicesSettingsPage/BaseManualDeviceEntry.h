/*-----------------------------------------------------------------*\
| BaseManualDeviceEntry.h                                           |
|                                                                   |
|   Base class to all user-defined device settings entries          |
|                                                                   |
|   This file is part of the OpenRGB project                        |
|   SPDX-License-Identifier: GPL-2.0-only                           |
\*-----------------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class BaseManualDeviceEntry: public QWidget
{
    Q_OBJECT

public:
    explicit BaseManualDeviceEntry(QWidget *parent = nullptr): QWidget(parent) {}
    virtual json saveSettings() = 0;
    virtual bool isDataValid() = 0;

    void setSettingsSection(const std::string& section);
    std::string getSettingsSection();

private:
    std::string settingsSection;
};

typedef std::function<BaseManualDeviceEntry*(const json& data)> ManualDeviceEntrySpawnFunction;

class ManualDeviceTypeRegistrator
{
public:
    ManualDeviceTypeRegistrator(const std::string& name, const std::string& settingsEntry, ManualDeviceEntrySpawnFunction entrySpawnFunction);
};

#define REGISTER_MANUAL_DEVICE_TYPE(name, settingsEntry, func) static ManualDeviceTypeRegistrator device_detector_obj_##func(name, settingsEntry, func)
