#ifndef OPENRGBPLUGINSPAGE_H
#define OPENRGBPLUGINSPAGE_H

#include <QWidget>
#include "OpenRGBPluginsEntry.h"
#include "ui_OpenRGBPluginsPage.h"
#include "PluginManager.h"

namespace Ui
{
class OpenRGBPluginsPage;
}

class Ui::OpenRGBPluginsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsPage(PluginManager* plugin_manager_ptr, QWidget *parent = nullptr);
    ~OpenRGBPluginsPage();

    void on_EnableButton_clicked(OpenRGBPluginsEntry* entry);

private slots:
    void changeEvent(QEvent *event);
    void on_InstallPluginButton_clicked();

    void on_RemovePluginButton_clicked();

    void on_PluginsList_itemSelectionChanged();

    void on_PluginsList_PluginsDropped(std::vector<std::string>);

private:
    Ui::OpenRGBPluginsPageUi*   ui;
    PluginManager*              plugin_manager;
    std::vector<OpenRGBPluginsEntry*> entries;

    bool InstallPlugin(std::string path);
    void RefreshList();
};

#endif // OPENRGBPLUGINSPAGE_H
