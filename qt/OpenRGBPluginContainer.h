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

    QWidget* plugin_widget;

private:
    Ui::OpenRGBPluginContainerUi *ui;
};

#endif // OPENRGBPLUGINCONTAINER_H
