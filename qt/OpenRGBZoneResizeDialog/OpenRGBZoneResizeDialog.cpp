/*---------------------------------------------------------*\
| OpenRGBZoneResizeDialog.cpp                               |
|                                                           |
|   User interface for resizing zones                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QLineEdit>
#include "OpenRGBZoneResizeDialog.h"
#include "ui_OpenRGBZoneResizeDialog.h"

OpenRGBZoneResizeDialog::OpenRGBZoneResizeDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBZoneResizeDialog)
{
    edit_dev      = edit_dev_ptr;
    edit_zone_idx = edit_zone_idx_val;

    unsigned int size_min     = edit_dev->zones[edit_zone_idx].leds_min;
    unsigned int size_max     = edit_dev->zones[edit_zone_idx].leds_max;
    unsigned int size_current = edit_dev->zones[edit_zone_idx].leds_count;

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QStringList header_labels;
    header_labels << "Name" << "Size" << "";
    ui->SegmentsTreeWidget->setHeaderLabels(header_labels);

    ui->ResizeSlider->setRange(size_min, size_max);
    ui->ResizeBox->setRange(size_min, size_max);

    ui->ResizeSlider->setValue(size_current);
    ui->ResizeBox->setValue(size_current);

    for(unsigned int segment_idx = 0; segment_idx < edit_dev->zones[edit_zone_idx].segments.size(); segment_idx++)
    {
        QTreeWidgetItem* new_item   = new QTreeWidgetItem(ui->SegmentsTreeWidget);

        QLineEdit* lineedit_name    = new QLineEdit(ui->SegmentsTreeWidget);
        QLineEdit* lineedit_length  = new QLineEdit(ui->SegmentsTreeWidget);
        QSlider*   slider_length    = new QSlider(Qt::Horizontal, ui->SegmentsTreeWidget);

        slider_length->setMaximum(edit_dev->zones[edit_zone_idx].leds_count);

        lineedit_name->setText(QString::fromStdString(edit_dev->zones[edit_zone_idx].segments[segment_idx].name));
        lineedit_length->setText(QString::number(edit_dev->zones[edit_zone_idx].segments[segment_idx].leds_count));
        slider_length->setSliderPosition(edit_dev->zones[edit_zone_idx].segments[segment_idx].leds_count);

        ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
        ui->SegmentsTreeWidget->setItemWidget(new_item, 1, lineedit_length);
        ui->SegmentsTreeWidget->setItemWidget(new_item, 2, slider_length);

        connect(slider_length, &QSlider::valueChanged, this, &OpenRGBZoneResizeDialog::on_segment_slider_valueChanged);
        connect(lineedit_length, &QLineEdit::textChanged, this, &OpenRGBZoneResizeDialog::on_segment_lineedit_textChanged);
    }
}

OpenRGBZoneResizeDialog::OpenRGBZoneResizeDialog(unsigned int edit_zone_min_val, unsigned int edit_zone_max_val, unsigned int edit_zone_current_val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBZoneResizeDialog)
{
    /*-----------------------------------------------------*\
    | This constructor does not use a device pointer.       |
    \*-----------------------------------------------------*/
    edit_dev = NULL;

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    /*-----------------------------------------------------*\
    | This constructor is used for resizing mode-specific   |
    | colors.  Segments are not used in this mode, so hide  |
    | the Segments tree view and buttons.                   |
    \*-----------------------------------------------------*/
    ui->SegmentsTreeWidget->hide();
    ui->AddSegmentButton->hide();
    ui->RemoveSegmentButton->hide();

    ui->ResizeSlider->setRange(edit_zone_min_val, edit_zone_max_val);
    ui->ResizeBox->setRange(edit_zone_min_val, edit_zone_max_val);

    ui->ResizeSlider->setValue(edit_zone_current_val);
    ui->ResizeBox->setValue(edit_zone_current_val);
}

OpenRGBZoneResizeDialog::~OpenRGBZoneResizeDialog()
{
    delete ui;
}

void OpenRGBZoneResizeDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBZoneResizeDialog::on_ResizeSlider_valueChanged(int value)
{
    ui->ResizeBox->blockSignals(true);
    ui->ResizeBox->setValue(value);
    ui->ResizeBox->blockSignals(false);

    /*-----------------------------------------------------*\
    | Set maximum value for all segment sliders to new zone |
    | size                                                  |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 2))->setMaximum(value);
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneResizeDialog::on_segment_lineedit_textChanged()
{
    /*-----------------------------------------------------*\
    | Update the Slider with the LineEdit value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        int lineedit_value = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->text().toInt();
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 2))->setValue(lineedit_value);
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneResizeDialog::on_segment_slider_valueChanged(int)
{
    /*-----------------------------------------------------*\
    | Update the LineEdit with the Slider value for each    |
    | segment                                               |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        int slider_value = ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 2))->value();
        ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->setText(QString::number(slider_value));
    }

    CheckSegmentsValidity();
}

void OpenRGBZoneResizeDialog::on_ResizeBox_valueChanged(int value)
{
    ui->ResizeSlider->blockSignals(true);
    ui->ResizeSlider->setValue(value);
    ui->ResizeSlider->blockSignals(false);

    /*-----------------------------------------------------*\
    | Set maximum value for all segment sliders to new zone |
    | size                                                  |
    \*-----------------------------------------------------*/
    for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
    {
        ((QSlider*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 2))->setMaximum(value);
    }

    CheckSegmentsValidity();
}

int OpenRGBZoneResizeDialog::show()
{
    int ret_val = 0;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        ret_val = -1;
    }
    else
    {
        ret_val = ui->ResizeBox->value();
    }

    if(ret_val >= 0 && edit_dev != NULL)
    {
        edit_dev->ResizeZone(edit_zone_idx, ret_val);

        edit_dev->ClearSegments(edit_zone_idx);

        unsigned int start_idx = 0;

        for(int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
        {
            segment new_segment;
            new_segment.type       = ZONE_TYPE_LINEAR;
            new_segment.name       = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 0))->text().toStdString();
            new_segment.start_idx  = start_idx;
            new_segment.leds_count = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->text().toInt();

            edit_dev->AddSegment(edit_zone_idx, new_segment);

            start_idx += new_segment.leds_count;
        }
    }

    return(ret_val);
}

void OpenRGBZoneResizeDialog::on_AddSegmentButton_clicked()
{
    /*---------------------------------------------------------*\
    | Create new line in segments list tree                     |
    \*---------------------------------------------------------*/
    QTreeWidgetItem* new_item   = new QTreeWidgetItem(ui->SegmentsTreeWidget);

    /*---------------------------------------------------------*\
    | Create new widgets for line                               |
    \*---------------------------------------------------------*/
    QLineEdit* lineedit_name    = new QLineEdit(ui->SegmentsTreeWidget);
    QLineEdit* lineedit_length  = new QLineEdit(ui->SegmentsTreeWidget);
    QSlider*   slider_length    = new QSlider(Qt::Horizontal, ui->SegmentsTreeWidget);

    /*---------------------------------------------------------*\
    | Fill in new Name field as "Segment X"                     |
    \*---------------------------------------------------------*/
    std::string new_name        = "Segment ";

    new_name.append(std::to_string(ui->SegmentsTreeWidget->topLevelItemCount() + 1));

    lineedit_name->setText(new_name.c_str());

    /*---------------------------------------------------------*\
    | Restrict slider maximum to zone size                      |
    \*---------------------------------------------------------*/
    slider_length->setMaximum(edit_dev->zones[edit_zone_idx].leds_count);

    /*---------------------------------------------------------*\
    | Add new widgets to tree                                   |
    \*---------------------------------------------------------*/
    ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 1, lineedit_length);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 2, slider_length);

    /*---------------------------------------------------------*\
    | Connect signals for handling slider and line edits        |
    \*---------------------------------------------------------*/
    connect(lineedit_name, &QLineEdit::textChanged, this, &OpenRGBZoneResizeDialog::on_segment_lineedit_textChanged);
    connect(slider_length, &QSlider::valueChanged, this, &OpenRGBZoneResizeDialog::on_segment_slider_valueChanged);
    connect(lineedit_length, &QLineEdit::textChanged, this, &OpenRGBZoneResizeDialog::on_segment_lineedit_textChanged);

    CheckSegmentsValidity();
}

void OpenRGBZoneResizeDialog::CheckSegmentsValidity()
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
            unsigned int segment_leds = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(segment_idx), 1))->text().toInt();

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

        if(total_segment_leds != ui->ResizeBox->value())
        {
            segments_valid = false;
        }
    }

    ui->ButtonBox->setEnabled(segments_valid);
}

void OpenRGBZoneResizeDialog::on_RemoveSegmentButton_clicked()
{
    ui->SegmentsTreeWidget->takeTopLevelItem(ui->SegmentsTreeWidget->topLevelItemCount() - 1);

    CheckSegmentsValidity();
}
