/*---------------------------------------------------------*\
| OpenRGBZoneEditorDialog.cpp                               |
|                                                           |
|   User interface for editing zones                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <fstream>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include "OpenRGBDynamicSettingsWidget.h"
#include "OpenRGBMatrixMapEditorDialog.h"
#include "OpenRGBSegmentExportDialog.h"
#include "OpenRGBZoneEditorDialog.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "ui_OpenRGBZoneEditorDialog.h"

static void Callback(void* this_ptr, std::string key, nlohmann::json settings)
{
    ((OpenRGBZoneEditorDialog*)this_ptr)->OnSettingChanged(key, settings);
}

OpenRGBZoneEditorDialog::OpenRGBZoneEditorDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBZoneEditorDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    /*-----------------------------------------------------*\
    | Save device and zone                                  |
    \*-----------------------------------------------------*/
    edit_dev                = edit_dev_ptr;
    edit_zone_idx           = edit_zone_idx_val;
    edit_zone               = edit_dev->GetZone(edit_zone_idx);

    /*-----------------------------------------------------*\
    | Append zone name to window title                      |
    \*-----------------------------------------------------*/
    QString currentTitle = windowTitle();

    QString newTitle = currentTitle + " - " + QString::fromStdString(edit_dev->GetZoneName(edit_zone_idx));

    setWindowTitle(newTitle);

    /*-----------------------------------------------------*\
    | Initialize zone name                                  |
    \*-----------------------------------------------------*/
    ui->LineEditZoneName->blockSignals(true);
    ui->LineEditZoneName->setText(QString::fromStdString(edit_zone.name));
    ui->LineEditZoneName->blockSignals(false);

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME) == 0)
    {
        ui->LineEditZoneName->setEnabled(false);
    }
    else if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME)
    {
        ui->LabelZoneName->setText("Zone Name (*):");
    }

    /*-----------------------------------------------------*\
    | Initialize zone size                                  |
    \*-----------------------------------------------------*/
    ui->SliderZoneSize->blockSignals(true);
    ui->SliderZoneSize->setRange(edit_zone.leds_min, edit_zone.leds_max);
    ui->SliderZoneSize->setValue(edit_zone.leds_count);
    ui->SliderZoneSize->blockSignals(false);

    ui->SpinBoxZoneSize->blockSignals(true);
    ui->SpinBoxZoneSize->setRange(edit_zone.leds_min, edit_zone.leds_max);
    ui->SpinBoxZoneSize->setValue(edit_zone.leds_count);
    ui->SpinBoxZoneSize->blockSignals(false);

    if((edit_zone.flags & (ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE | ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY)) == 0)
    {
        ui->SliderZoneSize->setEnabled(false);
        ui->SpinBoxZoneSize->setEnabled(false);
    }
    else if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE)
    {
        ui->LabelZoneSize->setText("Zone Size (*):");
    }

    /*-----------------------------------------------------*\
    | Initialize zone type                                  |
    \*-----------------------------------------------------*/
    ui->ComboBoxZoneType->blockSignals(true);
    ui->ComboBoxZoneType->addItem("Single");
    ui->ComboBoxZoneType->addItem("Linear");
    ui->ComboBoxZoneType->addItem("Matrix");
    ui->ComboBoxZoneType->addItem("Linear Loop");
    ui->ComboBoxZoneType->addItem("Matrix Loop X");
    ui->ComboBoxZoneType->addItem("Matrix Loop Y");
    ui->ComboBoxZoneType->addItem("Segmented");

    ui->ComboBoxZoneType->setCurrentIndex(edit_dev->GetZoneType(edit_zone_idx));
    ui->ComboBoxZoneType->blockSignals(false);

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE) == 0)
    {
        ui->ComboBoxZoneType->setEnabled(false);
    }
    else if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE)
    {
        ui->LabelZoneType->setText("Zone Type (*):");
    }

    /*-----------------------------------------------------*\
    | Initialize zone matrix map                            |
    \*-----------------------------------------------------*/
    ui->ButtonZoneMatrixMap->setText(QString::number(edit_zone.matrix_map.height) + "x" + QString::number(edit_zone.matrix_map.width));

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP) == 0)
    {
        ui->ButtonZoneMatrixMap->setEnabled(false);
    }
    else if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP)
    {
        ui->LabelZoneMatrixMap->setText("Zone Matrix Map (*):");
    }

    /*-----------------------------------------------------*\
    | Initialize segment list                               |
    \*-----------------------------------------------------*/
    QStringList header_labels;
    header_labels << "Name" << "Type" << "Matrix Map" << "Size" << "";
    ui->SegmentsTreeWidget->setHeaderLabels(header_labels);
    ui->SegmentsTreeWidget->setItemsExpandable(false);
    ui->SegmentsTreeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    SegmentTreeWidgetItem* group_start = NULL;

    for(std::size_t segment_idx = 0; segment_idx < edit_zone.segments.size(); segment_idx++)
    {
        if(edit_zone.segments[segment_idx].flags & SEGMENT_FLAG_GROUP_START)
        {
            group_start = AddSegmentGroupRow(QString::fromStdString(edit_zone.segments[segment_idx].name));
        }
        else if((edit_zone.segments[segment_idx].flags & SEGMENT_FLAG_GROUP_MEMBER) && (group_start != NULL))
        {
            AddSegmentRow(QString::fromStdString(edit_zone.segments[segment_idx].name), edit_zone.segments[segment_idx].leds_count, edit_zone.segments[segment_idx].type, edit_zone.segments[segment_idx].matrix_map, group_start);
        }
        else
        {
            AddSegmentRow(QString::fromStdString(edit_zone.segments[segment_idx].name), edit_zone.segments[segment_idx].leds_count, edit_zone.segments[segment_idx].type, edit_zone.segments[segment_idx].matrix_map, ui->SegmentsTreeWidget);
        }
    }

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS) == 0)
    {
        ui->AddSegmentButton->setEnabled(false);
        ui->RemoveSegmentButton->setEnabled(false);
    }
    else if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
    }

    /*-----------------------------------------------------*\
    | Initialize configuration list                         |
    \*-----------------------------------------------------*/
    nlohmann::json configuration_schema  = edit_dev->GetDeviceSpecificZoneConfigurationSchema(edit_zone_idx);
    nlohmann::json configuration_value   = edit_dev->GetDeviceSpecificZoneConfiguration(edit_zone_idx);

    ui->ListWidgetDeviceSpecificZoneConfiguration->setSelectionMode(QAbstractItemView::NoSelection);
    ui->ListWidgetDeviceSpecificZoneConfiguration->setFocusPolicy(Qt::NoFocus);

    /*-----------------------------------------------------*\
    | Loop through the schema and create an entry for each  |
    | configuration                                         |
    \*-----------------------------------------------------*/
    for(nlohmann::json::iterator json_iterator = configuration_schema.begin(); json_iterator != configuration_schema.end(); json_iterator++)
    {
        nlohmann::json                  schema_entry    = json_iterator.value();
        QListWidgetItem*                item            = new QListWidgetItem(ui->ListWidgetDeviceSpecificZoneConfiguration);
        OpenRGBDynamicSettingsWidget*   item_widget     = new OpenRGBDynamicSettingsWidget(json_iterator.key(), schema_entry, configuration_value);

        item_widget->SetCallback(Callback, this);
        item->setSizeHint(item_widget->sizeHint());
        ui->ListWidgetDeviceSpecificZoneConfiguration->setItemWidget(item, item_widget);
    }
}

OpenRGBZoneEditorDialog::~OpenRGBZoneEditorDialog()
{
    delete ui;
}

void OpenRGBZoneEditorDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBZoneEditorDialog::on_SliderZoneSize_valueChanged(int value)
{
    ui->SpinBoxZoneSize->blockSignals(true);
    ui->SpinBoxZoneSize->setValue(value);
    ui->SpinBoxZoneSize->blockSignals(false);

    /*-----------------------------------------------------*\
    | Set maximum value for all segment sliders to new zone |
    | size                                                  |
    \*-----------------------------------------------------*/
    QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);
    while(*tree_iterator)
    {
        SegmentTreeWidgetItem* item = (SegmentTreeWidgetItem*)*tree_iterator;

        if(!item->segment_group_header)
        {
            ((QSlider*)ui->SegmentsTreeWidget->itemWidget(item, 4))->setMaximum(value);
        }

        tree_iterator++;
    }

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        ui->LabelZoneSize->setText("Zone Size (*):");
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged()
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
    }

    /*-----------------------------------------------------*\
    | Update the Slider with the LineEdit value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        /*-------------------------------------------------*\
        | Ignore this item if it is a segment group header  |
        \*-------------------------------------------------*/
        bool segment_group_header = ((SegmentTreeWidgetItem*)(ui->SegmentsTreeWidget->topLevelItem(item_idx)))->segment_group_header;

        if(segment_group_header)
        {
            continue;
        }

        int lineedit_value = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 3))->text().toInt();
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 4))->setValue(lineedit_value);
    }

    CheckSegmentsValidity();
}

void SegmentTreeWidgetItem::on_button_matrix_map_clicked()
{
    QString             segment_name        = ((QLineEdit*)(treeWidget()->itemWidget(this, 0)))->text();
    unsigned int        segment_led_count   = ((QLineEdit*)(treeWidget()->itemWidget(this, 3)))->text().toInt();

    OpenRGBMatrixMapEditorDialog dialog(segment_name, &matrix_map, segment_led_count);

    dialog.show();

    ((QPushButton*)(treeWidget()->itemWidget(this, 2)))->setText(QString::number(matrix_map.height) + "x" + QString::number(matrix_map.width));
}

void OpenRGBZoneEditorDialog::on_segment_slider_valueChanged(int)
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
    }

    /*-----------------------------------------------------*\
    | Update the LineEdit with the Slider value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);
    while(*tree_iterator)
    {
        SegmentTreeWidgetItem* item = (SegmentTreeWidgetItem*)*tree_iterator;

        if(!item->segment_group_header)
        {
            int slider_value = ((QSlider*)ui->SegmentsTreeWidget->itemWidget(item, 4))->value();
            ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 3))->setText(QString::number(slider_value));
        }

        tree_iterator++;
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::on_SpinBoxZoneSize_valueChanged(int value)
{
    ui->SliderZoneSize->blockSignals(true);
    ui->SliderZoneSize->setValue(value);
    ui->SliderZoneSize->blockSignals(false);

    /*-----------------------------------------------------*\
    | Set maximum value for all segment sliders to new zone |
    | size                                                  |
    \*-----------------------------------------------------*/
    QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);
    while(*tree_iterator)
    {
        SegmentTreeWidgetItem* item = (SegmentTreeWidgetItem*)*tree_iterator;

        if(!item->segment_group_header)
        {
            ((QSlider*)ui->SegmentsTreeWidget->itemWidget(item, 4))->setMaximum(value);
        }

        tree_iterator++;
    }

    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SIZE) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SIZE;
        ui->LabelZoneSize->setText("Zone Size (*):");
    }

    CheckSegmentsValidity();
}

int OpenRGBZoneEditorDialog::show()
{
    /*-----------------------------------------------------*\
    | Execute this dialog                                   |
    \*-----------------------------------------------------*/
    int ret_val                 = 0;
    int result                  = this->exec();

    /*-----------------------------------------------------*\
    | Return -1 if cancelled or edit device is invalid      |
    \*-----------------------------------------------------*/
    if(result == QDialog::Rejected || edit_dev == NULL)
    {
        ret_val                 = -1;
    }
    else
    {
        /*-------------------------------------------------*\
        | Update zone with new settings                     |
        \*-------------------------------------------------*/
        edit_zone.name                              = ui->LineEditZoneName->text().toStdString();
        edit_zone.leds_count                        = ui->SliderZoneSize->value();
        edit_zone.type                              = ui->ComboBoxZoneType->currentIndex();

        if(edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS)
        {
            unsigned int start_idx                  = 0;

            edit_zone.segments.clear();

            QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);
            while(*tree_iterator)
            {
                segment new_segment;

                SegmentTreeWidgetItem* item         = (SegmentTreeWidgetItem*)*tree_iterator;

                bool segment_group_header           = item->segment_group_header;
                bool segment_group_member           = item->segment_group_member;

                new_segment.name                    = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 0))->text().toStdString();

                if(!segment_group_header)
                {
                    new_segment.type                = ((QComboBox*)ui->SegmentsTreeWidget->itemWidget(item, 1))->currentIndex();
                    new_segment.start_idx           = start_idx;
                    new_segment.leds_count          = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 3))->text().toInt();
                    new_segment.matrix_map          = item->matrix_map;
                    if(segment_group_member)
                    {
                        new_segment.flags           = SEGMENT_FLAG_GROUP_MEMBER;
                    }
                    else
                    {
                        new_segment.flags           = 0;
                    }
                    start_idx                      += new_segment.leds_count;
                }
                else
                {
                    new_segment.flags               = SEGMENT_FLAG_GROUP_START;
                }

                edit_zone.segments.push_back(new_segment);

                tree_iterator++;
            }
        }

        /*-------------------------------------------------*\
        | Configure the zone                                |
        \*-------------------------------------------------*/
        edit_dev->ConfigureZone(edit_zone_idx, edit_zone);

        /*-------------------------------------------------*\
        | Apply zone configuration                          |
        \*-------------------------------------------------*/
        edit_dev->SetDeviceSpecificZoneConfiguration(edit_zone_idx, zone_configuration);

        /*-------------------------------------------------*\
        | Save the configuration                            |
        \*-------------------------------------------------*/
        ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

        if(profile_manager != NULL)
        {
            profile_manager->SaveConfiguration();
        }

        /*-------------------------------------------------*\
        | Return new zone size                              |
        \*-------------------------------------------------*/
        ret_val                 = edit_zone.leds_count;
    }

    return(ret_val);
}

void OpenRGBZoneEditorDialog::AddSegmentRow(QString name, unsigned int length, zone_type type, matrix_map_type matrix_map, QTreeWidget* parent)
{
    SegmentTreeWidgetItem* new_item     = new SegmentTreeWidgetItem(parent);
    new_item->setExpanded(true);

    AddSegmentRowInternal(name, length, type, matrix_map, new_item);
}

void OpenRGBZoneEditorDialog::AddSegmentRow(QString name, unsigned int length, zone_type type, matrix_map_type matrix_map, QTreeWidgetItem* parent)
{
    SegmentTreeWidgetItem* new_item     = new SegmentTreeWidgetItem(parent);
    new_item->segment_group_member      = true;
    new_item->setExpanded(true);

    AddSegmentRowInternal(name, length, type, matrix_map, new_item);
}

void OpenRGBZoneEditorDialog::AddSegmentRowInternal(QString name, unsigned int length, zone_type type, matrix_map_type matrix_map, SegmentTreeWidgetItem* new_item)
{
    /*-----------------------------------------------------*\
    | Set the matrix map                                    |
    \*-----------------------------------------------------*/
    new_item->matrix_map = matrix_map;

    /*-----------------------------------------------------*\
    | Create new widgets for line                           |
    \*-----------------------------------------------------*/
    QComboBox*      combobox_type       = new QComboBox(ui->SegmentsTreeWidget);
    QLineEdit*      lineedit_name       = new QLineEdit(ui->SegmentsTreeWidget);
    QLineEdit*      lineedit_length     = new QLineEdit(ui->SegmentsTreeWidget);
    QSlider*        slider_length       = new QSlider(Qt::Horizontal, ui->SegmentsTreeWidget);
    QPushButton*    button_matrix_map   = new QPushButton();

    button_matrix_map->setText(QString::number(new_item->matrix_map.height) + "x" + QString::number(new_item->matrix_map.width));

    /*-----------------------------------------------------*\
    | Fill in Name field                                    |
    \*-----------------------------------------------------*/
    lineedit_name->setText(name);

    /*-----------------------------------------------------*\
    | Set up segment type combo box                         |
    \*-----------------------------------------------------*/
    combobox_type->addItem("Single");
    combobox_type->addItem("Linear");
    combobox_type->addItem("Matrix");
    combobox_type->addItem("Linear Loop");
    combobox_type->addItem("Matrix Loop X");
    combobox_type->addItem("Matrix Loop Y");
    combobox_type->addItem("Segmented");

    combobox_type->setCurrentIndex(type);

    /*-----------------------------------------------------*\
    | Fill in Length field                                  |
    \*-----------------------------------------------------*/
    lineedit_length->setText(QString::number(length));

    /*-----------------------------------------------------*\
    | Fill in slider length and maximum                     |
    \*-----------------------------------------------------*/
    slider_length->setMaximum(edit_dev->GetZoneLEDsCount(edit_zone_idx));
    slider_length->setValue(length);

    /*-----------------------------------------------------*\
    | Add new widgets to tree                               |
    \*-----------------------------------------------------*/
    ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 1, combobox_type);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 2, button_matrix_map);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 3, lineedit_length);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 4, slider_length);

    /*-----------------------------------------------------*\
    | Connect signals for handling slider and line edits    |
    \*-----------------------------------------------------*/
    connect(lineedit_name, &QLineEdit::textChanged, this, &OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged);
    connect(slider_length, &QSlider::valueChanged, this, &OpenRGBZoneEditorDialog::on_segment_slider_valueChanged);
    connect(lineedit_length, &QLineEdit::textChanged, this, &OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged);
    connect(button_matrix_map, SIGNAL(clicked()), new_item, SLOT(on_button_matrix_map_clicked()));

    ui->SegmentsTreeWidget->expandAll();
}

SegmentTreeWidgetItem* OpenRGBZoneEditorDialog::AddSegmentGroupRow(QString name)
{
    /*-----------------------------------------------------*\
    | Create new line in segments list tree                 |
    \*-----------------------------------------------------*/
    SegmentTreeWidgetItem* new_item     = new SegmentTreeWidgetItem(ui->SegmentsTreeWidget);

    /*-----------------------------------------------------*\
    | Mark this line as a group header                      |
    \*-----------------------------------------------------*/
    new_item->segment_group_header      = true;

    /*-----------------------------------------------------*\
    | Create new widgets for line                           |
    \*-----------------------------------------------------*/
    QLineEdit*      lineedit_name       = new QLineEdit(ui->SegmentsTreeWidget);

    /*-----------------------------------------------------*\
    | Fill in Name field                                    |
    \*-----------------------------------------------------*/
    lineedit_name->setText(name);

    /*-----------------------------------------------------*\
    | Add new widgets to tree                               |
    \*-----------------------------------------------------*/
    ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);

    return(new_item);
}

void OpenRGBZoneEditorDialog::on_AddSegmentButton_clicked()
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
    }

    /*-----------------------------------------------------*\
    | Create new empty row with name "Segment X"            |
    \*-----------------------------------------------------*/
    QString new_name = "Segment " + QString::number(ui->SegmentsTreeWidget->topLevelItemCount() + 1);

    matrix_map_type new_matrix_map;

    /*-----------------------------------------------------*\
    | Get selected item                                     |
    \*-----------------------------------------------------*/
    QList<QTreeWidgetItem *> items = ui->SegmentsTreeWidget->selectedItems();

    /*-----------------------------------------------------*\
    | Create new line under selected group header or as top |
    | level otherwise                                       |
    \*-----------------------------------------------------*/
    if((items.size() == 1) && ((SegmentTreeWidgetItem*)items[0])->segment_group_header)
    {
        AddSegmentRow(new_name, 0, ZONE_TYPE_LINEAR, new_matrix_map, items[0]);
    }
    else
    {
        AddSegmentRow(new_name, 0, ZONE_TYPE_LINEAR, new_matrix_map, ui->SegmentsTreeWidget);
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::on_AddSegmentGroupButton_clicked()
{
    /*-----------------------------------------------------*\
    | Create new empty row with name "Segment X"            |
    \*-----------------------------------------------------*/
    QString new_name = "Segment Group " + QString::number(ui->SegmentsTreeWidget->topLevelItemCount() + 1);
    AddSegmentGroupRow(new_name);
}

void OpenRGBZoneEditorDialog::CheckSegmentsValidity()
{
    bool segments_valid = true;
    unsigned int total_segment_leds = 0;

    QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);

    if(!(*tree_iterator))
    {
        ui->ButtonBox->setEnabled(segments_valid);
        return;
    }

    while(*tree_iterator)
    {
        SegmentTreeWidgetItem* item = (SegmentTreeWidgetItem*)*tree_iterator;

        /*-------------------------------------------------*\
        | Ignore this item if it is a segment group header  |
        \*-------------------------------------------------*/
        bool segment_group_header = item->segment_group_header;

        if(segment_group_header)
        {
            tree_iterator++;
            continue;
        }

        unsigned int segment_leds = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 3))->text().toInt();

        /*-------------------------------------------------*\
        | Zero-length segment is not allowed                |
        \*-------------------------------------------------*/
        if(segment_leds == 0)
        {
            segments_valid = false;
        }

        total_segment_leds += segment_leds;

        /*-------------------------------------------------*\
        | Empty name is not allowed                         |
        \*-------------------------------------------------*/
        if(((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 0))->text().isEmpty())
        {
            segments_valid = false;
        }

        tree_iterator++;
    }

    if(total_segment_leds != (unsigned int)ui->SpinBoxZoneSize->value())
    {
        segments_valid = false;
    }

    ui->ButtonBox->setEnabled(segments_valid);
}

void OpenRGBZoneEditorDialog::on_RemoveSegmentButton_clicked()
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
    }

    ui->SegmentsTreeWidget->takeTopLevelItem(ui->SegmentsTreeWidget->topLevelItemCount() - 1);

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::on_ImportConfigurationButton_clicked()
{
    QFileDialog file_dialog(this);

    /*-----------------------------------------------------*\
    | Show the file chooser dialog                          |
    \*-----------------------------------------------------*/
    file_dialog.setFileMode(QFileDialog::ExistingFile);
    file_dialog.setNameFilter("*.json");
    file_dialog.setWindowTitle("Import Configuration");

    if(file_dialog.exec())
    {
        QStringList     selected_files  = file_dialog.selectedFiles();
        QString         filename        = selected_files[0];
        std::ifstream   config_file(filename.toStdString(), std::ios::in);
        nlohmann::json  config_json;

        /*-------------------------------------------------*\
        | Import config file if a valid file was opened     |
        \*-------------------------------------------------*/
        if(config_file)
        {
            try
            {
                config_file >> config_json;

                /*-----------------------------------------*\
                | Read in all segments                      |
                \*-----------------------------------------*/
                if(config_json.contains("segments"))
                {
                    unsigned int        total_leds_count    = ui->SliderZoneSize->value();

                    SegmentTreeWidgetItem* group_start      = NULL;
                    for(std::size_t segment_idx = 0; segment_idx < config_json["segments"].size(); segment_idx++)
                    {
                        unsigned int    segment_leds_count  = 0;
                        matrix_map_type segment_matrix_map;
                        QString         segment_name        = "";
                        zone_type       segment_type        = ZONE_TYPE_LINEAR;
                        unsigned int    segment_flags       = 0;

                        if(config_json["segments"][segment_idx].contains("name"))
                        {
                            segment_name = QString::fromStdString(config_json["segments"][segment_idx]["name"]);
                        }
                        if(config_json["segments"][segment_idx].contains("leds_count"))
                        {
                            segment_leds_count = config_json["segments"][segment_idx]["leds_count"];
                        }
                        if(config_json["segments"][segment_idx].contains("type"))
                        {
                            segment_type = config_json["segments"][segment_idx]["type"];
                        }
                        if(config_json["segments"][segment_idx].contains("flags"))
                        {
                            segment_flags = config_json["segments"][segment_idx]["flags"];
                        }

                        matrix_map_type new_matrix_map;
                        new_matrix_map = RGBController::SetMatrixMapDescriptionJSON(config_json["segments"][segment_idx]["matrix_map"]);

                        if(segment_flags & SEGMENT_FLAG_GROUP_START)
                        {
                            group_start = AddSegmentGroupRow(segment_name);
                        }
                        else if((segment_flags & SEGMENT_FLAG_GROUP_MEMBER) && (group_start != NULL))
                        {
                            AddSegmentRow(segment_name, segment_leds_count, segment_type, new_matrix_map, group_start);
                        }
                        else
                        {
                            AddSegmentRow(segment_name, segment_leds_count, segment_type, new_matrix_map, ui->SegmentsTreeWidget);
                        }

                        total_leds_count += segment_leds_count;
                    }

                    ui->SliderZoneSize->setValue(total_leds_count);
                    ui->SpinBoxZoneSize->setValue(total_leds_count);

                    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS) == 0)
                    {
                        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS;
                        ui->GroupBoxSegments->setTitle("Segments Configuration (*)");
                    }
                }
            }
            catch(const std::exception& e)
            {
            }
        }
    }
}

void OpenRGBZoneEditorDialog::on_ExportConfigurationButton_clicked()
{
    OpenRGBSegmentExportDialog dialog;

    /*-----------------------------------------------------*\
    | Show the segment export dialog                        |
    \*-----------------------------------------------------*/
    if(dialog.show())
    {
        std::ofstream   config_file(dialog.GetFilename().toStdString(), std::ios::out);
        nlohmann::json  config_json;

        if(config_file)
        {
            QString     device  = dialog.GetDevice();
            QString     vendor  = dialog.GetVendor();

            /*---------------------------------------------*\
            | Fill in device string in the JSON             |
            \*---------------------------------------------*/
            if(device != "")
            {
                config_json["device"] = device.toStdString();
            }

            /*---------------------------------------------*\
            | Fill in vendor string in the JSON             |
            \*---------------------------------------------*/
            if(vendor != "")
            {
                config_json["vendor"] = vendor.toStdString();
            }

            /*---------------------------------------------*\
            | Fill in segment data in the JSON              |
            \*---------------------------------------------*/
            unsigned int start_idx  = 0;
            unsigned int item_idx   = 0;

            QTreeWidgetItemIterator tree_iterator(ui->SegmentsTreeWidget);
            while(*tree_iterator)
            {
                SegmentTreeWidgetItem*  item    = (SegmentTreeWidgetItem*)*tree_iterator;
                segment                 new_segment;

                bool segment_group_header       = item->segment_group_header;
                bool segment_group_member       = item->segment_group_member;

                new_segment.name                = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 0))->text().toStdString();

                if(!segment_group_header)
                {
                    new_segment.type            = ((QComboBox*)ui->SegmentsTreeWidget->itemWidget(item, 1))->currentIndex();
                    new_segment.start_idx       = start_idx;
                    new_segment.leds_count      = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(item, 3))->text().toInt();
                    new_segment.matrix_map      = item->matrix_map;
                    if(segment_group_member)
                    {
                        new_segment.flags       = SEGMENT_FLAG_GROUP_MEMBER;
                    }
                    else
                    {
                        new_segment.flags       = 0;
                    }
                    start_idx                  += new_segment.leds_count;
                }
                else
                {
                    new_segment.flags          = SEGMENT_FLAG_GROUP_START;
                }

                config_json["segments"][item_idx] = RGBController::GetSegmentDescriptionJSON(new_segment);

                item_idx++;
                tree_iterator++;
            }

            /*---------------------------------------------*\
            | Write the JSON data to the file               |
            \*---------------------------------------------*/
            config_file << std::setw(4) << config_json << std::endl;

            /*---------------------------------------------*\
            | Close the file when done                      |
            \*---------------------------------------------*/
            config_file.close();
        }
    }
}

void OpenRGBZoneEditorDialog::on_ButtonZoneMatrixMap_clicked()
{
    unsigned int total_leds_count = ui->SliderZoneSize->value();

    OpenRGBMatrixMapEditorDialog dialog(QString::fromStdString(edit_dev->GetZoneName(edit_zone_idx)), &edit_zone.matrix_map, total_leds_count);

    int ret = dialog.show();

    ui->ButtonZoneMatrixMap->setText(QString::number(edit_zone.matrix_map.height) + "x" + QString::number(edit_zone.matrix_map.width));

    if(ret >= 0)
    {
        if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP) == 0)
        {
            edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP;
            ui->LabelZoneMatrixMap->setText("Zone Matrix Map (*):");
        }
    }
}

void OpenRGBZoneEditorDialog::on_ButtonResetZoneConfiguration_clicked()
{
    edit_zone.flags &= ~ZONE_FLAGS_MANUALLY_CONFIGURED;

    /*-------------------------------------------------*\
    | Configure the zone                                |
    \*-------------------------------------------------*/
    edit_dev->ConfigureZone(edit_zone_idx, edit_zone);

    /*-------------------------------------------------*\
    | Save the configuration                            |
    \*-------------------------------------------------*/
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        profile_manager->SaveConfiguration();
    }

    done(0);
}

void OpenRGBZoneEditorDialog::on_LineEditZoneName_textChanged(const QString& /*arg1*/)
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_NAME) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_NAME;
        ui->LabelZoneName->setText("Zone Name (*):");
    }
}

void OpenRGBZoneEditorDialog::on_ComboBoxZoneType_currentIndexChanged(int /*index*/)
{
    if((edit_zone.flags & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE) == 0)
    {
        edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_TYPE;
        ui->LabelZoneType->setText("Zone Type (*):");
    }
}

void OpenRGBZoneEditorDialog::OnSettingChanged(std::string /*key*/, nlohmann::json settings)
{
    edit_zone.flags |= ZONE_FLAG_MANUALLY_CONFIGURED_DEVICE_SPECIFIC;
    zone_configuration.update(settings, true);
}
