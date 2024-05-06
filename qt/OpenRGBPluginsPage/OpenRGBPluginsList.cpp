/*---------------------------------------------------------*\
| OpenRGBPluginsList.cpp                                    |
|                                                           |
|   User interface entry for OpenRGB plugin list widget     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QMimeData>
#include <QUrl>
#include "OpenRGBPluginsList.h"

OpenRGBPluginsList::OpenRGBPluginsList(QWidget *parent) : QListWidget (parent)
{
    setAcceptDrops(true);
}

void OpenRGBPluginsList::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        std::vector<std::string> path_list;

        QList<QUrl> urls = mimeData->urls();

        for(const QUrl& url: urls)
        {
            path_list.push_back(url.toLocalFile().toStdString());
        }

        emit PluginsDropped(path_list);
    }
}

void OpenRGBPluginsList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void OpenRGBPluginsList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
