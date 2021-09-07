#ifndef OPENRGBPLUGINSLIST_H
#define OPENRGBPLUGINSLIST_H

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

#endif // OPENRGBPLUGINSLIST_H
