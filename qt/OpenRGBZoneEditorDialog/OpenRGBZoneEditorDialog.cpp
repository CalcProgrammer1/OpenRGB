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
#include "OpenRGBMatrixMapEditorDialog.h"
#include "OpenRGBSegmentExportDialog.h"
#include "OpenRGBZoneEditorDialog.h"
#include "ui_OpenRGBZoneEditorDialog.h"

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
    edit_zone_matrix_map    = edit_dev->GetZoneMatrixMap(edit_zone_idx);

    /*-----------------------------------------------------*\
    | Append zone name to window title                      |
    \*-----------------------------------------------------*/
    QString currentTitle = windowTitle();

    QString newTitle = currentTitle + " - " + QString::fromStdString(edit_dev->GetZoneName(edit_zone_idx));

    setWindowTitle(newTitle);

    /*-----------------------------------------------------*\
    | Read the zone flags                                   |
    \*-----------------------------------------------------*/
    zone_flags flags = edit_dev->GetZoneFlags(edit_zone_idx);

    if(edit_dev->GetZoneLEDsMin(edit_zone_idx) != edit_dev->GetZoneLEDsMax(edit_zone_idx))
    {
        if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY) == 0)
        {
            flags |= ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE | ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP | ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS;
        }

        flags |= ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE;
    }

    /*-----------------------------------------------------*\
    | Set up zone size range                                |
    \*-----------------------------------------------------*/
    unsigned int size_min     = edit_dev->GetZoneLEDsMin(edit_zone_idx);
    unsigned int size_max     = edit_dev->GetZoneLEDsMax(edit_zone_idx);
    unsigned int size_current = edit_dev->GetZoneLEDsCount(edit_zone_idx);

    /*-----------------------------------------------------*\
    | Initialize zone name                                  |
    \*-----------------------------------------------------*/
    ui->LineEditZoneName->setText(QString::fromStdString(edit_dev->GetZoneName(edit_zone_idx)));

    if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME) == 0)
    {
        ui->LineEditZoneName->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Initialize zone size                                  |
    \*-----------------------------------------------------*/
    ui->SliderZoneSize->setRange(size_min, size_max);
    ui->SpinBoxZoneSize->setRange(size_min, size_max);

    ui->SliderZoneSize->setValue(size_current);
    ui->SpinBoxZoneSize->setValue(size_current);

    if((flags & (ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE | ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY)) == 0)
    {
        ui->SliderZoneSize->setEnabled(false);
        ui->SpinBoxZoneSize->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Initialize zone type                                  |
    \*-----------------------------------------------------*/
    ui->ComboBoxZoneType->addItem("Single");
    ui->ComboBoxZoneType->addItem("Linear");
    ui->ComboBoxZoneType->addItem("Matrix");
    ui->ComboBoxZoneType->addItem("Linear Loop");
    ui->ComboBoxZoneType->addItem("Matrix Loop X");
    ui->ComboBoxZoneType->addItem("Matrix Loop Y");
    ui->ComboBoxZoneType->addItem("Segmented");

    ui->ComboBoxZoneType->setCurrentIndex(edit_dev->GetZoneType(edit_zone_idx));

    if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE) == 0)
    {
        ui->ComboBoxZoneType->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Initialize zone matrix map                            |
    \*-----------------------------------------------------*/
    ui->ButtonZoneMatrixMap->setText(QString::number(edit_zone_matrix_map.height) + "x" + QString::number(edit_zone_matrix_map.width));

    if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP) == 0)
    {
        ui->ButtonZoneMatrixMap->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Initialize zone color order                           |
    \*-----------------------------------------------------*/
    ui->ComboBoxZoneColorOrder->addItem("Default");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_RGB) ui->ComboBoxZoneColorOrder->addItem("RGB");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_RBG) ui->ComboBoxZoneColorOrder->addItem("RBG");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_GRB) ui->ComboBoxZoneColorOrder->addItem("GRB");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_GBR) ui->ComboBoxZoneColorOrder->addItem("GBR");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_BRG) ui->ComboBoxZoneColorOrder->addItem("BRG");
    if(flags & ZONE_FLAG_SUPPORTS_COLOR_ORDER_BGR) ui->ComboBoxZoneColorOrder->addItem("BGR");

    if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_COLOR_ORDER) == 0)
    {
        ui->ComboBoxZoneColorOrder->setEnabled(false);
    }

    /*-----------------------------------------------------*\
    | Initialize segment list                               |
    \*-----------------------------------------------------*/
    QStringList header_labels;
    header_labels << "Name" << "Type" << "Matrix Map" << "Size" << "";
    ui->SegmentsTreeWidget->setHeaderLabels(header_labels);

    for(unsigned int segment_idx = 0; segment_idx < edit_dev->GetZoneSegmentCount(edit_zone_idx); segment_idx++)
    {
        matrix_map_type new_matrix_map = edit_dev->GetZoneSegmentMatrixMap(edit_zone_idx, segment_idx);
        AddSegmentRow(QString::fromStdString(edit_dev->GetZoneSegmentName(edit_zone_idx, segment_idx)), edit_dev->GetZoneSegmentLEDsCount(edit_zone_idx, segment_idx), edit_dev->GetZoneSegmentType(edit_zone_idx, segment_idx), new_matrix_map);
    }

    if((flags & ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS) == 0)
    {
        ui->AddSegmentButton->setEnabled(false);
        ui->RemoveSegmentButton->setEnabled(false);
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
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 4))->setMaximum(value);
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged()
{
    /*-----------------------------------------------------*\
    | Update the Slider with the LineEdit value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
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
    /*-----------------------------------------------------*\
    | Update the LineEdit with the Slider value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        int slider_value = ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 4))->value();
        ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 3))->setText(QString::number(slider_value));
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
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 4))->setMaximum(value);
    }

    CheckSegmentsValidity();
}

int OpenRGBZoneEditorDialog::show()
{
    int ret_val = 0;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        ret_val = -1;
    }
    else
    {
        ret_val = ui->SpinBoxZoneSize->value();
    }

    if(ret_val >= 0 && edit_dev != NULL)
    {
        zone                new_zone;

        new_zone.name           = ui->LineEditZoneName->text().toStdString();
        new_zone.leds_count     = ui->SliderZoneSize->value();
        new_zone.type           = ui->ComboBoxZoneType->currentIndex();
        new_zone.color_order    = ui->ComboBoxZoneColorOrder->currentIndex();
        new_zone.flags          = ZONE_FLAG_MANUALLY_CONFIGURED_NAME | ZONE_FLAG_MANUALLY_CONFIGURED_SIZE | ZONE_FLAG_MANUALLY_CONFIGURED_TYPE | ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP | ZONE_FLAG_MANUALLY_CONFIGURED_COLOR_ORDER;
        new_zone.matrix_map     = edit_zone_matrix_map;

        edit_dev->ConfigureZone(edit_zone_idx, new_zone);

        edit_dev->ClearSegments(edit_zone_idx);

        unsigned int start_idx = 0;

        for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
        {
            segment new_segment;
            new_segment.type       = ((QComboBox*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->currentIndex();
            new_segment.name       = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 0))->text().toStdString();
            new_segment.start_idx  = start_idx;
            new_segment.leds_count = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 3))->text().toInt();
            new_segment.matrix_map = ((SegmentTreeWidgetItem*)(ui->SegmentsTreeWidget->topLevelItem(item_idx)))->matrix_map;

            edit_dev->AddSegment(edit_zone_idx, new_segment);

            start_idx += new_segment.leds_count;
        }
    }

    return(ret_val);
}

void OpenRGBZoneEditorDialog::AddSegmentRow(QString name, unsigned int length, zone_type type, matrix_map_type matrix_map)
{
    /*---------------------------------------------------------*\
    | Create new line in segments list tree                     |
    \*---------------------------------------------------------*/
    SegmentTreeWidgetItem* new_item     = new SegmentTreeWidgetItem(ui->SegmentsTreeWidget);

    /*---------------------------------------------------------*\
    | Set the matrix map                                        |
    \*---------------------------------------------------------*/
    new_item->matrix_map = matrix_map;

    /*---------------------------------------------------------*\
    | Create new widgets for line                               |
    \*---------------------------------------------------------*/
    QComboBox*      combobox_type       = new QComboBox(ui->SegmentsTreeWidget);
    QLineEdit*      lineedit_name       = new QLineEdit(ui->SegmentsTreeWidget);
    QLineEdit*      lineedit_length     = new QLineEdit(ui->SegmentsTreeWidget);
    QSlider*        slider_length       = new QSlider(Qt::Horizontal, ui->SegmentsTreeWidget);
    QPushButton*    button_matrix_map   = new QPushButton();

    button_matrix_map->setText(QString::number(new_item->matrix_map.height) + "x" + QString::number(new_item->matrix_map.width));

    /*---------------------------------------------------------*\
    | Fill in Name field                                        |
    \*---------------------------------------------------------*/
    lineedit_name->setText(name);

    /*---------------------------------------------------------*\
    | Set up segment type combo box                             |
    \*---------------------------------------------------------*/
    combobox_type->addItem("Single");
    combobox_type->addItem("Linear");
    combobox_type->addItem("Matrix");
    combobox_type->addItem("Linear Loop");
    combobox_type->addItem("Matrix Loop X");
    combobox_type->addItem("Matrix Loop Y");
    combobox_type->addItem("Segmented");

    combobox_type->setCurrentIndex(type);

    /*---------------------------------------------------------*\
    | Fill in Length field                                      |
    \*---------------------------------------------------------*/
    lineedit_length->setText(QString::number(length));

    /*---------------------------------------------------------*\
    | Fill in slider length and maximum                         |
    \*---------------------------------------------------------*/
    slider_length->setMaximum(edit_dev->GetZoneLEDsCount(edit_zone_idx));
    slider_length->setValue(length);

    /*---------------------------------------------------------*\
    | Add new widgets to tree                                   |
    \*---------------------------------------------------------*/
    ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 1, combobox_type);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 2, button_matrix_map);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 3, lineedit_length);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 4, slider_length);

    /*---------------------------------------------------------*\
    | Connect signals for handling slider and line edits        |
    \*---------------------------------------------------------*/
    connect(lineedit_name, &QLineEdit::textChanged, this, &OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged);
    connect(slider_length, &QSlider::valueChanged, this, &OpenRGBZoneEditorDialog::on_segment_slider_valueChanged);
    connect(lineedit_length, &QLineEdit::textChanged, this, &OpenRGBZoneEditorDialog::on_segment_lineedit_textChanged);
    connect(button_matrix_map, SIGNAL(clicked()), new_item, SLOT(on_button_matrix_map_clicked()));
}

void OpenRGBZoneEditorDialog::on_AddSegmentButton_clicked()
{
    /*---------------------------------------------------------*\
    | Create new empty row with name "Segment X"                |
    \*---------------------------------------------------------*/
    QString new_name = "Segment " + QString::number(ui->SegmentsTreeWidget->topLevelItemCount() + 1);

    matrix_map_type new_matrix_map;
    AddSegmentRow(new_name, 0, ZONE_TYPE_LINEAR, new_matrix_map);

    CheckSegmentsValidity();
}

void OpenRGBZoneEditorDialog::CheckSegmentsValidity()
{
    bool segments_valid = true;

    /*---------------------------------------------------------*\
    | Only check validity if segments are configured            |
    \*---------------------------------------------------------*/
    if(ui->SegmentsTreeWidget->topLevelItemCount() != 0)
    {
        /*-----------------------------------------------------*\
        | Verify all segments add up to zone size               |
        \*-----------------------------------------------------*/
        int total_segment_leds = 0;

        for(int segment_idx = 0; segment_idx < ui->SegmentsTreeWidget->topLevelItemCount(); segment_idx++)
        {
            unsigned int segment_leds = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(segment_idx), 3))->text().toInt();

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
            if(((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(segment_idx), 0))->text().isEmpty())
            {
                segments_valid = false;
            }
        }

        if(total_segment_leds != ui->SpinBoxZoneSize->value())
        {
            segments_valid = false;
        }
    }

    ui->ButtonBox->setEnabled(segments_valid);
}

void OpenRGBZoneEditorDialog::on_RemoveSegmentButton_clicked()
{
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
                    unsigned int total_leds_count = ui->SliderZoneSize->value();

                    for(std::size_t segment_idx = 0; segment_idx < config_json["segments"].size(); segment_idx++)
                    {
                        unsigned int    segment_leds_count  = 0;
                        matrix_map_type segment_matrix_map;
                        QString         segment_name        = "";
                        zone_type       segment_type        = ZONE_TYPE_LINEAR;

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

                        matrix_map_type new_matrix_map;
                        new_matrix_map = RGBController::SetMatrixMapDescriptionJSON(config_json["segments"][segment_idx]["matrix_map"]);

                        AddSegmentRow(segment_name, segment_leds_count, segment_type, new_matrix_map);

                        total_leds_count += segment_leds_count;
                    }


                    ui->SliderZoneSize->setValue(total_leds_count);
                    ui->SpinBoxZoneSize->setValue(total_leds_count);
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
            unsigned int start_idx = 0;

            for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
            {
                segment new_segment;
                new_segment.type       = ((QComboBox*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->currentIndex();
                new_segment.name       = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 0))->text().toStdString();
                new_segment.start_idx  = start_idx;
                new_segment.leds_count = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 3))->text().toInt();
                new_segment.matrix_map = ((SegmentTreeWidgetItem*)(ui->SegmentsTreeWidget->topLevelItem(item_idx)))->matrix_map;

                config_json["segments"][item_idx] = RGBController::GetSegmentDescriptionJSON(new_segment);

                start_idx += new_segment.leds_count;
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

    OpenRGBMatrixMapEditorDialog dialog(QString::fromStdString(edit_dev->GetZoneName(edit_zone_idx)), &edit_zone_matrix_map, total_leds_count);

    dialog.show();

    ui->ButtonZoneMatrixMap->setText(QString::number(edit_zone_matrix_map.height) + "x" + QString::number(edit_zone_matrix_map.width));
}

void OpenRGBZoneEditorDialog::on_ButtonResetZoneConfiguration_clicked()
{

}
