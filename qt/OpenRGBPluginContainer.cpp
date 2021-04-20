#include "OpenRGBPluginContainer.h"
#include "ui_OpenRGBPluginContainer.h"

Ui::OpenRGBPluginContainer::OpenRGBPluginContainer(QWidget *plugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginContainerUi)
{
    ui->setupUi(this);

    plugin_widget = plugin;

    ui->PluginContainerLayout->layout()->addWidget(plugin_widget);

    plugin_widget->hide();
}

Ui::OpenRGBPluginContainer::~OpenRGBPluginContainer()
{
    delete ui;
}

void Ui::OpenRGBPluginContainer::on_ShowPluginButton_clicked()
{
    if(plugin_widget->isHidden())
    {
        plugin_widget->show();
    }
    else
    {
        plugin_widget->hide();
    }
}
