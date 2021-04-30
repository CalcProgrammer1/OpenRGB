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
    void Hide();
    void Show();

private:
    Ui::OpenRGBPluginContainerUi *ui;
    QWidget* plugin_widget;
};

#endif // OPENRGBPLUGINCONTAINER_H
