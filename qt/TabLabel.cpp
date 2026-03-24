/*---------------------------------------------------------*\
| TabLabel.cpp                                              |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QFontMetrics>
#include "TabLabel.h"
#include "OpenRGBFont.h"
#include "ui_TabLabel.h"

TabLabel::TabLabel(int icon, char* label, char* context, bool translatable) :
    QWidget(nullptr),
    ui(new Ui::TabLabel)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store initial widths for icon and name                |
    \*-----------------------------------------------------*/
    icon_initial_width = ui->icon->width();
    name_initial_width = ui->name->width();

    /*-----------------------------------------------------*\
    | Initialize icon font                                  |
    \*-----------------------------------------------------*/
    QFont font = OpenRGBFont::GetFont();
    font.setPointSize(18);

    /*-----------------------------------------------------*\
    | Set icon                                              |
    \*-----------------------------------------------------*/
    ui->icon->setFont(font);
    ui->icon->setText(OpenRGBFont::icon(icon));

    /*-----------------------------------------------------*\
    | Store data                                            |
    \*-----------------------------------------------------*/
    this->translatable  = translatable;
    this->label         = label;
    this->context       = context;

    /*-----------------------------------------------------*\
    | Update label                                          |
    \*-----------------------------------------------------*/
    UpdateLabel(true);

    SetTextHidden(false);
}

TabLabel::~TabLabel()
{
    delete ui;
}

void TabLabel::SetTextHidden(bool hidden)
{
    if(hidden)
    {
        ui->name->setFixedWidth(0);
        setFixedWidth(ui->icon->width());
    }
    else
    {
        ui->name->setFixedWidth(name_initial_width);
        setFixedWidth(ui->icon->width() + ui->name->width());
    }

    ui->name->setHidden(hidden);
}

void TabLabel::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        UpdateLabel(false);
    }
}

void TabLabel::UpdateLabel(bool in_constructor)
{
    if(translatable)
    {
        /*-------------------------------------------------*\
        | Storing the base string in label                  |
        |   enables switching between multiple languages    |
        | The context needs to be stored as the translation |
        |   file requires the originating context           |
        \*-------------------------------------------------*/
        QApplication* app = static_cast<QApplication *>(QApplication::instance());
        ui->name->setText(app->translate(context, label));
    }
    else if(in_constructor)
    {
        /*-------------------------------------------------*\
        | Must only be called during constructor if not     |
        | translatable as label buffer may not exist        |
        \*-------------------------------------------------*/
        ui->name->setText(label);
    }
}
