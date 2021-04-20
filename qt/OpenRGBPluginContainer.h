#ifndef OPENRGBPLUGINCONTAINER_H
#define OPENRGBPLUGINCONTAINER_H

#include "ui_OpenRGBPluginContainer.h"

#include <QWidget>

namespace Ui
{
    class OpenRGBPluginContainer;
}

class Ui::OpenRGBPluginContainer : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginContainer(QWidget *plugin, QWidget *parent = nullptr);
    ~OpenRGBPluginContainer();

private slots:
    void on_ShowPluginButton_clicked();

private:
    Ui::OpenRGBPluginContainerUi *ui;
    QWidget* plugin_widget;
};

#endif // OPENRGBPLUGINCONTAINER_H
