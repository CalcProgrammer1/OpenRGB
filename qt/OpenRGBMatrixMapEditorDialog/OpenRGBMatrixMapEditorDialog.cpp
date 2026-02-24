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

enum
{
    MATRIX_ORDER_HORIZONTAL_TOP_LEFT,
    MATRIX_ORDER_HORIZONTAL_TOP_LEFT_ZIGZAG,
    MATRIX_ORDER_HORIZONTAL_TOP_RIGHT,
    MATRIX_ORDER_HORIZONTAL_TOP_RIGHT_ZIGZAG,
    MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT,
    MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT_ZIGZAG,
    MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT,
    MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT_ZIGZAG,
    MATRIX_ORDER_VERTICAL_TOP_LEFT,
    MATRIX_ORDER_VERTICAL_TOP_LEFT_ZIGZAG,
    MATRIX_ORDER_VERTICAL_TOP_RIGHT,
    MATRIX_ORDER_VERTICAL_TOP_RIGHT_ZIGZAG,
    MATRIX_ORDER_VERTICAL_BOTTOM_LEFT,
    MATRIX_ORDER_VERTICAL_BOTTOM_LEFT_ZIGZAG,
    MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT,
    MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT_ZIGZAG
};

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
    | Populate Auto-Generate Combo Box                      |
    \*-----------------------------------------------------*/
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Top Left");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Top Left Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Top Right");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Top Right Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Bottom Left");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Bottom Left Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Bottom Right");
    ui->ComboBoxAutoGenerate->addItem("Horizontal from Bottom Right Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Top Left");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Top Left Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Top Right");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Top Right Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Bottom Left");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Bottom Left Zigzag");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Bottom Right");
    ui->ComboBoxAutoGenerate->addItem("Vertical from Bottom Right Zigzag");

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

void OpenRGBMatrixMapEditorDialog::on_ButtonAutoGenerate_clicked()
{
    /*-----------------------------------------------------*\
    | Loop control variables                                |
    \*-----------------------------------------------------*/
    unsigned int    led_idx             = 0;
    unsigned int    height              = ui->LineEditHeight->text().toInt();
    unsigned int    width               = ui->LineEditWidth->text().toInt();
    int             matrix_order        = ui->ComboBoxAutoGenerate->currentIndex();
    bool            outer_loop_y        = true;
    bool            outer_loop_inverted = false;
    bool            inner_loop_inverted = false;
    bool            zigzag              = false;

    /*-----------------------------------------------------*\
    | Initialize loop control variables                     |
    \*-----------------------------------------------------*/
    switch(matrix_order)
    {
        case MATRIX_ORDER_HORIZONTAL_TOP_LEFT:
        case MATRIX_ORDER_HORIZONTAL_TOP_LEFT_ZIGZAG:
            outer_loop_y                = true;
            outer_loop_inverted         = false;
            inner_loop_inverted         = false;
            break;

        case MATRIX_ORDER_HORIZONTAL_TOP_RIGHT:
        case MATRIX_ORDER_HORIZONTAL_TOP_RIGHT_ZIGZAG:
            outer_loop_y                = true;
            outer_loop_inverted         = false;
            inner_loop_inverted         = true;
            break;

        case MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT_ZIGZAG:
            outer_loop_y                = true;
            outer_loop_inverted         = true;
            inner_loop_inverted         = false;
            break;

        case MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT_ZIGZAG:
            outer_loop_y                = true;
            outer_loop_inverted         = true;
            inner_loop_inverted         = true;
            break;

        case MATRIX_ORDER_VERTICAL_TOP_LEFT:
        case MATRIX_ORDER_VERTICAL_TOP_LEFT_ZIGZAG:
            outer_loop_y                = false;
            outer_loop_inverted         = false;
            inner_loop_inverted         = false;
            break;

        case MATRIX_ORDER_VERTICAL_TOP_RIGHT:
        case MATRIX_ORDER_VERTICAL_TOP_RIGHT_ZIGZAG:
            outer_loop_y                = false;
            outer_loop_inverted         = true;
            inner_loop_inverted         = false;
            break;

        case MATRIX_ORDER_VERTICAL_BOTTOM_LEFT:
        case MATRIX_ORDER_VERTICAL_BOTTOM_LEFT_ZIGZAG:
            outer_loop_y                = false;
            outer_loop_inverted         = false;
            inner_loop_inverted         = true;
            break;

        case MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT:
        case MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT_ZIGZAG:
            outer_loop_y                = false;
            outer_loop_inverted         = true;
            inner_loop_inverted         = true;
            break;
    }

    /*-----------------------------------------------------*\
    | Initialize zigzag flag                                |
    \*-----------------------------------------------------*/
    switch(matrix_order)
    {
        case MATRIX_ORDER_HORIZONTAL_TOP_LEFT:
        case MATRIX_ORDER_HORIZONTAL_TOP_RIGHT:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT:
        case MATRIX_ORDER_VERTICAL_TOP_LEFT:
        case MATRIX_ORDER_VERTICAL_TOP_RIGHT:
        case MATRIX_ORDER_VERTICAL_BOTTOM_LEFT:
        case MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT:
            zigzag = false;
            break;

        case MATRIX_ORDER_HORIZONTAL_TOP_LEFT_ZIGZAG:
        case MATRIX_ORDER_HORIZONTAL_TOP_RIGHT_ZIGZAG:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT_ZIGZAG:
        case MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT_ZIGZAG:
        case MATRIX_ORDER_VERTICAL_TOP_LEFT_ZIGZAG:
        case MATRIX_ORDER_VERTICAL_TOP_RIGHT_ZIGZAG:
        case MATRIX_ORDER_VERTICAL_BOTTOM_LEFT_ZIGZAG:
        case MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT_ZIGZAG:
            zigzag = true;
            break;
    }

    /*-----------------------------------------------------*\
    | Initialize and clear table                            |
    \*-----------------------------------------------------*/
    ui->TableWidgetMatrixMap->setRowCount(height);
    ui->TableWidgetMatrixMap->setColumnCount(width);

    ui->TableWidgetMatrixMap->clearContents();

    /*-----------------------------------------------------*\
    | Generate matrix map                                   |
    \*-----------------------------------------------------*/
    for(int outer = (outer_loop_inverted ? ((outer_loop_y ? height : width) - 1) : 0);
        (outer_loop_inverted ? (outer >= 0) : (outer < (int)(outer_loop_y ? height : width))) ;
        (outer_loop_inverted ? (outer--) : (outer++)))
    {
        for(int inner = (inner_loop_inverted ? ((outer_loop_y ? width : height) - 1) : 0);
        (inner_loop_inverted ? (inner >= 0) : (inner < (int)(outer_loop_y ? width : height))) ;
        (inner_loop_inverted ? (inner--) : (inner++)))
        {
            QTableWidgetItem*   table_item  = new QTableWidgetItem;

            if(led_idx < edit_led_count)
            {
                table_item->setText(QString::number(led_idx));
            }

            if(outer_loop_y)
            {
                ui->TableWidgetMatrixMap->setItem(outer, inner, table_item);
            }
            else
            {
                ui->TableWidgetMatrixMap->setItem(inner, outer, table_item);
            }

            led_idx++;
        }

        if(zigzag)
        {
            inner_loop_inverted = !inner_loop_inverted;
        }
    }
}

void OpenRGBMatrixMapEditorDialog::on_LineEditHeight_textChanged(const QString &arg1)
{
    ui->TableWidgetMatrixMap->setRowCount(arg1.toInt());
}

void OpenRGBMatrixMapEditorDialog::on_LineEditWidth_textChanged(const QString &arg1)
{
    ui->TableWidgetMatrixMap->setColumnCount(arg1.toInt());
}
