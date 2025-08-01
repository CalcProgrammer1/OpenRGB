/*---------------------------------------------------------*\
| OpenRGBZonesBulkResizer.cpp                               |
|                                                           |
|   User interface for bulk resizing zones                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBZonesBulkResizer.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "LogManager.h"
#include "OpenRGBDialog.h"

#include "ui_OpenRGBZonesBulkResizer.h"

#include <QDialog>
#include <QFile>
#include <QSpinBox>

void OpenRGBZonesBulkResizer::RunChecks(QWidget *parent)
{
    /*---------------------------------------------------------*\
    | Determine if we should run (user setting)                 |
    \*---------------------------------------------------------*/
    SettingsManager* settings_manager = ResourceManager::get()->GetSettingsManager();
    json ui_settings = settings_manager->GetSettings("UserInterface");

    if(!ui_settings.is_null() && ui_settings.contains("RunZoneChecks"))
    {
        json run_zone_checks = ui_settings["RunZoneChecks"];

        if(!run_zone_checks.is_null() && run_zone_checks.is_boolean())
        {
            bool should_run = run_zone_checks;

            if(!should_run)
            {
                LOG_DEBUG("[ZonesBulkResizer] Skipping zones sizes checks.");
                return;
            }
        }
    }

    LOG_DEBUG("[ZonesBulkResizer] Running zones sizes checks...");

    /*---------------------------------------------------------*\
    | Collect the unconfigured zones                            |
    \*---------------------------------------------------------*/
    std::vector<RGBController*>& controllers = ResourceManager::get()->GetRGBControllers();

    std::vector<std::tuple<RGBController*, unsigned int>> zones;

    for(RGBController* controller: controllers)
    {
        for(unsigned int zone_index = 0; zone_index < controller->zones.size(); zone_index++)
        {
            // Consider unconfigured if 0 leds AND led_count < leds_max
            if(controller->zones[zone_index].leds_count == 0
                    && controller->zones[zone_index].leds_count < controller->zones[zone_index].leds_max)
            {
                zones.push_back({controller, zone_index});
            }
        }
    }

    LOG_DEBUG("[ZonesBulkResizer] Zones checks finished: %d unconfigured zone(s).", zones.size());

    /*---------------------------------------------------------*\
    | Show the configuration tool GUI if we have some           |
    | unconfigured zones                                        |
    \*---------------------------------------------------------*/
    if(!zones.empty())
    {
        QDialog* dialog = new QDialog(parent);
        dialog->setWindowTitle(tr("Resize the zones"));

        dialog->setMinimumSize(600,480);
        dialog->setModal(true);

        QVBoxLayout* dialog_layout = new QVBoxLayout(dialog);

        OpenRGBZonesBulkResizer* widget = new OpenRGBZonesBulkResizer(dialog, zones);

        dialog_layout->addWidget(widget);

        connect(widget, &OpenRGBZonesBulkResizer::CloseRequest, [=](){
            dialog->reject();
        });

        dialog->exec();
    }
}

OpenRGBZonesBulkResizer::OpenRGBZonesBulkResizer(QWidget *parent,  const std::vector<std::tuple<RGBController*, unsigned int>>& unconfigured_zones) :
    QWidget(parent),
    ui(new Ui::OpenRGBZonesBulkResizer),
    unconfigured_zones(unconfigured_zones)
{
    ui->setupUi(this);

    /*---------------------------------------------------------*\
    | Set the table view settings (headers, resize behavior...) |
    \*---------------------------------------------------------*/
    ui->zones_table->setColumnCount(3);
    ui->zones_table->verticalHeader()->setVisible(false);
    ui->zones_table->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    ui->zones_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    ui->zones_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->zones_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->zones_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    ui->zones_table->setHorizontalHeaderLabels({tr("Controller"), tr("Zone"), tr("Size")});

    /*---------------------------------------------------------*\
    | Fill the table with widgets (labels, spinbox)             |
    \*---------------------------------------------------------*/
    for(const std::tuple<RGBController*, unsigned int>& tuple: unconfigured_zones)
    {
        CreateZoneWidget(std::get<0>(tuple), std::get<1>(tuple));
    }
}

OpenRGBZonesBulkResizer::~OpenRGBZonesBulkResizer()
{
    delete ui;
}

void OpenRGBZonesBulkResizer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBZonesBulkResizer::CreateZoneWidget(RGBController* controller, unsigned int zone_index)
{
    /*---------------------------------------------------------*\
    | Labels: controller name + zone name                       |
    \*---------------------------------------------------------*/
    QLabel* controller_label = new QLabel(this);
    controller_label->setText(QString::fromStdString(controller->GetName()));

    QLabel* zone_label = new QLabel(this);
    zone_label->setText(QString::fromStdString(controller->zones[zone_index].name));

    /*---------------------------------------------------------*\
    | Spin box: controls the zone size                          |
    \*---------------------------------------------------------*/
    QSpinBox* spin_box = new QSpinBox(this);
    spin_box->setValue(0);
    spin_box->setMinimum(controller->zones[zone_index].leds_min);
    spin_box->setMaximum(controller->zones[zone_index].leds_max);

    /*---------------------------------------------------------*\
    | Insert labels + spinbox                                   |
    \*---------------------------------------------------------*/
    int row = ui->zones_table->rowCount();

    ui->zones_table->insertRow(row);
    ui->zones_table->setCellWidget(row, 0, controller_label);
    ui->zones_table->setCellWidget(row, 1, zone_label);
    ui->zones_table->setCellWidget(row, 2, spin_box);
}

void OpenRGBZonesBulkResizer::on_save_button_clicked()
{
    bool has_changes = false;

    /*---------------------------------------------------------*\
    | Resize what needs to be resized                           |
    \*---------------------------------------------------------*/
    for(unsigned int i = 0; i < unconfigured_zones.size(); i++)
    {
        unsigned int new_size = ((QSpinBox*)ui->zones_table->cellWidget(i,2))->value();

        if(new_size > 0)
        {
            RGBController* controller = std::get<0>(unconfigured_zones[i]);
            unsigned int zone_index = std::get<1>(unconfigured_zones[i]);

            controller->ResizeZone(zone_index, new_size);

            has_changes = true;
        }
    }

    /*---------------------------------------------------------*\
    | Save the sizes if the user did any changes                |
    \*---------------------------------------------------------*/
    if(has_changes)
    {
        ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

        if(profile_manager != NULL)
        {
            /*---------------------------------------------------------*\
            | Save the profile                                          |
            \*---------------------------------------------------------*/
            profile_manager->SaveProfile("sizes", true);
        }
    }

    /*---------------------------------------------------------*\
    | Save the "Do not show again" checkbox state, then close   |
    \*---------------------------------------------------------*/
    SaveDoNotRunState();

    emit CloseRequest();
}

void OpenRGBZonesBulkResizer::on_ignore_button_clicked()
{
    /*---------------------------------------------------------*\
    | Save the "Do not show again" checkbox state, then close   |
    \*---------------------------------------------------------*/
    SaveDoNotRunState();

    emit CloseRequest();
}

void OpenRGBZonesBulkResizer::SaveDoNotRunState()
{
    /*---------------------------------------------------------*\
    | Save the "Do not show again" checkbox state in            |
    | settings manager                                          |
    \*---------------------------------------------------------*/
    SettingsManager* settings_manager = ResourceManager::get()->GetSettingsManager();
    json ui_settings = settings_manager->GetSettings("UserInterface");

    bool state = ui->do_not_show_again_checkbox->checkState() == Qt::Checked;

    ui_settings["RunZoneChecks"] = !state;

    settings_manager->SetSettings("UserInterface", ui_settings);
    settings_manager->SaveSettings();
}
