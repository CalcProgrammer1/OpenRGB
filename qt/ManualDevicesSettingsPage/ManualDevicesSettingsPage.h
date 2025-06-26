/*-----------------------------------------------------------------*\
| ManualDevicesSettingsPage.h                                       |
|                                                                   |
|   User interface for OpenRGB Manually Added Devices settings page |
|                                                                   |
|   This file is part of the OpenRGB project                        |
|   SPDX-License-Identifier: GPL-2.0-only                           |
\*-----------------------------------------------------------------*/

#pragma once

#include "BaseManualDeviceEntry.h"
#include "ManualDevicesTypeManager.h"
#include "nlohmann/json.hpp"
#include <QWidget>
#include <QMenu>

using json = nlohmann::json;

namespace Ui
{
    class ManualDevicesSettingsPage;
}

class ManualDevicesSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManualDevicesSettingsPage(QWidget *parent = nullptr);
    ~ManualDevicesSettingsPage();

private slots:
    void reloadList();
    void reloadMenu();
    void onAddDeviceItemSelected(QAction* action);

    void changeEvent(QEvent *event);

    void on_removeDeviceButton_clicked();
    void on_saveConfigurationButton_clicked();

    void on_deviceList_itemSelectionChanged();

private:
    Ui::ManualDevicesSettingsPage*        ui;
    std::vector<BaseManualDeviceEntry*>   entries;
    QMenu*                                addDeviceMenu;

    void addEntry(BaseManualDeviceEntry* entry);
    void addEntries(const ManualDeviceTypeBlock&);
    void clearList();
};
