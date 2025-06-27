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
#include <QListWidgetItem>

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

public slots:
    void reloadList();
    void reloadMenu();

private slots:
    void onTextEditChanged(); // Slot connected to all text edits in all entries; for validation & marking dirty changes
    void onAddDeviceItemSelected(QAction* action);

    void changeEvent(QEvent *event);

    void on_removeDeviceButton_clicked();
    void on_deviceList_itemSelectionChanged();
    void on_ActionSaveNoRescan_triggered();
    void on_ActionSaveAndRescan_triggered();

private:
    Ui::ManualDevicesSettingsPage*        ui;
    std::vector<BaseManualDeviceEntry*>   entries;
    QMenu*                                addDeviceMenu;
    bool                                  unsavedChanges;

    QListWidgetItem* addEntry(BaseManualDeviceEntry* entry);
    void addEntries(const ManualDeviceTypeBlock&);
    void clearList();
    void saveSettings();
    void setUnsavedChanges(bool v);
    bool checkValidToSave();
};
