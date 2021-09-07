#include "OpenRGBPluginsList.h"
#include <QMimeData>
#include <QUrl>

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
