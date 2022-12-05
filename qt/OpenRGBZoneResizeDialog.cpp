#include "OpenRGBZoneResizeDialog.h"

#include <QLineEdit>

using namespace Ui;

OpenRGBZoneResizeDialog::OpenRGBZoneResizeDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBZoneResizeDialogUi)
{
    edit_dev      = edit_dev_ptr;
    edit_zone_idx = edit_zone_idx_val;

    unsigned int size_min     = edit_dev->zones[edit_zone_idx].leds_min;
    unsigned int size_max     = edit_dev->zones[edit_zone_idx].leds_max;
    unsigned int size_current = edit_dev->zones[edit_zone_idx].leds_count;

    ui->setupUi(this);

    ui->ResizeSlider->setRange(size_min, size_max);
    ui->ResizeBox->setRange(size_min, size_max);

    ui->ResizeSlider->setValue(size_current);
    ui->ResizeBox->setValue(size_current);

    for(unsigned int segment_idx = 0; segment_idx < edit_dev->zones[edit_zone_idx].segments.size(); segment_idx++)
    {
        QTreeWidgetItem* new_item = new QTreeWidgetItem(ui->SegmentsTreeWidget);

        QLineEdit* lineedit_name = new QLineEdit(ui->SegmentsTreeWidget);
        QLineEdit* lineedit_start = new QLineEdit(ui->SegmentsTreeWidget);
        QLineEdit* lineedit_length = new QLineEdit(ui->SegmentsTreeWidget);

        lineedit_name->setText(QString::fromStdString(edit_dev->zones[edit_zone_idx].segments[segment_idx].name));
        lineedit_start->setText(QString::number(edit_dev->zones[edit_zone_idx].segments[segment_idx].start_idx));
        lineedit_length->setText(QString::number(edit_dev->zones[edit_zone_idx].segments[segment_idx].leds_count));

        ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
        ui->SegmentsTreeWidget->setItemWidget(new_item, 1, lineedit_start);
        ui->SegmentsTreeWidget->setItemWidget(new_item, 2, lineedit_length);
    }
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

void Ui::OpenRGBZoneResizeDialog::on_ResizeSlider_valueChanged(int value)
{
    ui->ResizeBox->blockSignals(true);
    ui->ResizeBox->setValue(value);
    ui->ResizeBox->blockSignals(false);
}

void Ui::OpenRGBZoneResizeDialog::on_ResizeBox_valueChanged(int value)
{
    ui->ResizeSlider->blockSignals(true);
    ui->ResizeSlider->setValue(value);
    ui->ResizeSlider->blockSignals(false);
}

int Ui::OpenRGBZoneResizeDialog::show()
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

    if(ret_val >= 0)
    {
        edit_dev->ResizeZone(edit_zone_idx, ret_val);

        edit_dev->zones[edit_zone_idx].segments.clear();

        for(unsigned int item_idx = 0; item_idx < ui->SegmentsTreeWidget->topLevelItemCount(); item_idx++)
        {
            segment new_segment;
            new_segment.type       = ZONE_TYPE_LINEAR;
            new_segment.name       = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 0))->text().toStdString();
            new_segment.start_idx  = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 1))->text().toInt();
            new_segment.leds_count = ((QLineEdit*)ui->SegmentsTreeWidget->itemWidget(ui->SegmentsTreeWidget->topLevelItem(item_idx), 2))->text().toInt();

            edit_dev->zones[edit_zone_idx].segments.push_back(new_segment);
        }
    }

    return(ret_val);
}

void Ui::OpenRGBZoneResizeDialog::on_AddSegmentButton_clicked()
{
	QTreeWidgetItem* new_item = new QTreeWidgetItem(ui->SegmentsTreeWidget);
    
    QLineEdit* lineedit_name = new QLineEdit(ui->SegmentsTreeWidget);
    QLineEdit* lineedit_start = new QLineEdit(ui->SegmentsTreeWidget);
    QLineEdit* lineedit_length = new QLineEdit(ui->SegmentsTreeWidget);

    ui->SegmentsTreeWidget->setItemWidget(new_item, 0, lineedit_name);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 1, lineedit_start);
    ui->SegmentsTreeWidget->setItemWidget(new_item, 2, lineedit_length);
}


void Ui::OpenRGBZoneResizeDialog::on_RemoveSegmentButton_clicked()
{
    ui->SegmentsTreeWidget->takeTopLevelItem(ui->SegmentsTreeWidget->topLevelItemCount() - 1);
}

