/*---------------------------------------------------------*\
| TabLabel.cpp                                              |
|                                                           |
|   Qt widget for OpenRGB tab label                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QFontMetrics>
#include "TabLabel.h"
#include "OpenRGBFont.h"
#include "ui_TabLabel.h"

TabLabel::TabLabel(int icon, QString name, char* original, char* context) :
    QWidget(nullptr),
    ui(new Ui::TabLabel)
{
    ui->setupUi(this);

    QFont font = OpenRGBFont::GetFont();
    font.setPointSize(18);

    ui->icon->setFont(font);
    ui->icon->setText(OpenRGBFont::icon(icon));

    ui->name->setText(name);

    label   = original;
    ctxt    = context;
}

TabLabel::~TabLabel()
{
    delete ui;
}

void TabLabel::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        /*-----------------------------------------------------*\
        | Storing the base string in label                      |
        |   enables switching between multiple languages        |
        | The context needs to be stored as the translation     |
        |   file requires the originating context               |
        \*-----------------------------------------------------*/
        QApplication* app       = static_cast<QApplication *>(QApplication::instance());

        ui->name->setText(app->translate(ctxt, label));
    }
}
