/*---------------------------------------------------------*\
| OpenRGBPluginsList.h                                      |
|                                                           |
|   User interface entry for OpenRGB plugin list widget     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QListWidget>
#include <QDropEvent>
#include <QDragEnterEvent>

class OpenRGBPluginsList : public QListWidget
{
    Q_OBJECT

public:
    OpenRGBPluginsList(QWidget *parent = nullptr);

signals:
    void PluginsDropped(std::vector<std::string>);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
};
