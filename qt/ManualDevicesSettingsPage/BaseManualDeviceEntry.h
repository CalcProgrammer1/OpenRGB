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
	virtual void loadFromSettings(const json& data) = 0;
    virtual json saveSettings() = 0;
    virtual const char* settingsSection() = 0;
};
