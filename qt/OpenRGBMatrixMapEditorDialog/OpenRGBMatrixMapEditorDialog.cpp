/*---------------------------------------------------------*\
| OpenRGBMatrixMapEditorDialog.cpp                          |
|                                                           |
|   User interface for editing matrix maps                  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      21 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <fstream>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include "OpenRGBMatrixMapEditorDialog.h"
#include "ui_OpenRGBMatrixMapEditorDialog.h"

OpenRGBMatrixMapEditorDialog::OpenRGBMatrixMapEditorDialog(QString name, matrix_map_type* edit_map_ptr, unsigned int led_count, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBMatrixMapEditorDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    /*-----------------------------------------------------*\
    | Save matrix map pointer                               |
    \*-----------------------------------------------------*/
    edit_map        = edit_map_ptr;
    edit_led_count  = led_count;

    /*-----------------------------------------------------*\
    | Append name to window title                           |
    \*-----------------------------------------------------*/
    if(name != "")
    {
        QString currentTitle = windowTitle();

        QString newTitle = currentTitle + " - " + name;

        setWindowTitle(newTitle);
    }

    /*-----------------------------------------------------*\
    | Initialize matrix map table                           |
    \*-----------------------------------------------------*/
    ui->LineEditHeight->setText(QString::number(edit_map->height));
    ui->LineEditWidth->setText(QString::number(edit_map->width));

    ui->TableWidgetMatrixMap->setRowCount(edit_map->height);
    ui->TableWidgetMatrixMap->setColumnCount(edit_map->width);

    for(int row = 0; row < ui->TableWidgetMatrixMap->rowCount(); row++)
    {
        for(int column = 0; column < ui->TableWidgetMatrixMap->columnCount(); column++)
        {
            unsigned int        table_value = edit_map->map[(row * edit_map->width) + column];
            QTableWidgetItem*   table_item  = new QTableWidgetItem;

            if(table_value < edit_led_count)
            {
                table_item->setText(QString::number(table_value));
            }

            ui->TableWidgetMatrixMap->setItem(row, column, table_item);
        }
    }
}

OpenRGBMatrixMapEditorDialog::~OpenRGBMatrixMapEditorDialog()
{
    delete ui;
}

void OpenRGBMatrixMapEditorDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

int OpenRGBMatrixMapEditorDialog::show()
{
    int ret_val = 0;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        ret_val = -1;
    }
    else
    {
        edit_map->height    = ui->LineEditHeight->text().toInt();
        edit_map->width     = ui->LineEditWidth->text().toInt();

        edit_map->map.resize(edit_map->height * edit_map->width);

        for(int row = 0; row < ui->TableWidgetMatrixMap->rowCount(); row++)
        {
            for(int column = 0; column < ui->TableWidgetMatrixMap->columnCount(); column++)
            {
                bool                valid           = false;
                QTableWidgetItem*   table_item      = ui->TableWidgetMatrixMap->item(row, column);
                QString             table_string    = "";
                unsigned int        table_number    = 0xFFFFFFFF;

                if(table_item)
                {
                    table_string                    = table_item->text();
                    table_number                    = table_string.toUInt(&valid);
                }

                if(!valid || (table_number >= edit_led_count))
                {
                    table_number = 0xFFFFFFFF;
                }

                edit_map->map[(row * edit_map->width) + column] = table_number;
            }
        }
    }

    return(ret_val);
}

void OpenRGBMatrixMapEditorDialog::on_LineEditHeight_textChanged(const QString &arg1)
{
    ui->TableWidgetMatrixMap->setRowCount(arg1.toInt());
}

void OpenRGBMatrixMapEditorDialog::on_LineEditWidth_textChanged(const QString &arg1)
{
    ui->TableWidgetMatrixMap->setColumnCount(arg1.toInt());
}
