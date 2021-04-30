#include "OpenRGBPluginContainer.h"
#include "ui_OpenRGBPluginContainer.h"

Ui::OpenRGBPluginContainer::OpenRGBPluginContainer(QWidget *plugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginContainerUi)
{
    ui->setupUi(this);

    plugin_widget = plugin;

    ui->PluginContainerLayout->layout()->addWidget(plugin_widget);

    Hide();
}

Ui::OpenRGBPluginContainer::~OpenRGBPluginContainer()
{
    delete ui;
}

void Ui::OpenRGBPluginContainer::Hide()
{
    plugin_widget->hide();
    ui->PluginContainerLayout->layout()->invalidate();
}

void Ui::OpenRGBPluginContainer::Show()
{
    plugin_widget->show();
    ui->PluginContainerLayout->layout()->invalidate();
}