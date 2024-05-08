/*---------------------------------------------------------*\
| swatches.cpp                                              |
|                                                           |
|   Custom Colour Swatch widget that allows for adding user |
|   colours                                                 |
|                                                           |
|   Chris M (Dr_No)                             23 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include "swatches.h"

Swatches::Swatches(QWidget *parent) :
    QWidget(parent),
    initSize(width_inc_margin * minColumns, height_inc_margin * minRows)
{
    setBaseSize(initSize);
    setSizeIncrement(width_inc_margin, height_inc_margin);
    add_swatch.color.setRgb( 0, 0, 0, 0); //transparent

    /*-----------------------------------------------------*\
    | Add default swatches to the list                      |
    \*-----------------------------------------------------*/
    swatch black_swatch;
    black_swatch.color.setRgb(0, 0, 0, 255);
    swatch_list.push_back(black_swatch);

    swatch red_swatch;
    red_swatch.color.setRgb(255, 0, 0, 255);
    swatch_list.push_back(red_swatch);

    swatch yellow_swatch;
    yellow_swatch.color.setRgb(255, 255, 0, 255);
    swatch_list.push_back(yellow_swatch);

    swatch green_swatch;
    green_swatch.color.setRgb( 0, 255, 0, 255);
    swatch_list.push_back(green_swatch);

    swatch cyan_swatch;
    cyan_swatch.color.setRgb( 0, 255, 255, 255);
    swatch_list.push_back(cyan_swatch);

    swatch blue_swatch;
    blue_swatch.color.setRgb( 0, 0, 255, 255);
    swatch_list.push_back(blue_swatch);

    swatch magenta_swatch;
    magenta_swatch.color.setRgb(255, 0, 255, 255);
    swatch_list.push_back(magenta_swatch);

    swatch white_swatch;
    white_swatch.color.setRgb(255, 255, 255, 255);
    swatch_list.push_back(white_swatch);

    min_swatches = swatch_list.size();
}

QColor Swatches::color()
{
    return swatch_list[selected].color.toRgb();
}

QSize Swatches::sizeHint () const
{
    return QSize(width(), height());
}

QSize Swatches::minimumSizeHint () const
{
    return baseSize();
}

void Swatches::setCurrentColor(const QColor &color)
{
    if(color == picker_color)
    {
        return;
    }

    picker_color = color;
}

void Swatches::addCustomSwatch(const QColor &color)
{
    swatch new_swatch;
    new_swatch.color = color;
    swatch_list.push_back(new_swatch);
    update();
}

void Swatches::resizeEvent(QResizeEvent *event)
{
    swatch_pixmap   = QPixmap(event->size());
    swatch_pixmap.fill(Qt::transparent);
    drawSwatches(event->size());
    update();
}

void Swatches::mousePressEvent(QMouseEvent* /*event*/)
{
    mouseDown = true;
}

void Swatches::mouseReleaseEvent(QMouseEvent* event)
{
    if(!mouseDown)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Clear mouse down and in-region flags                  |
    \*-----------------------------------------------------*/
    mouseDown   = false;

    if(false)//add_swatch.region.contains(event->pos()))
    {
        addCustomSwatch(picker_color);
    }
    else
    {
        int swatch_count = swatch_list.size();
        for(int i = 0; i < swatch_count; i++)
        {
            if(swatch_list[i].region.contains(event->pos()))
            {
                emit swatchChanged(swatch_list[i].color.toRgb());
                break;
            }
        }
    }
}

void Swatches::drawSwatches(const QSize &newSize)
{
    /*-----------------------------------------------------*\
    | Create image canvas & paint background transparent    |
    \*-----------------------------------------------------*/
    swatch_image = QImage(newSize, QImage::Format_ARGB32_Premultiplied);
    swatch_image.fill(Qt::transparent);

    /*-----------------------------------------------------*\
    | Set up painter                                        |
    \*-----------------------------------------------------*/
    QPainter painter(&swatch_image);
    painter.setPen(border_pen);

    /*-----------------------------------------------------*\
    | Paint the swatch cluster                              |
    \*-----------------------------------------------------*/
    int width           = qMin(width_inc_margin, (newSize.width() / minColumns) - (margin * 2));
    int height          = minSize;
    width_inc_margin    = width + (margin * 2);
    height_inc_margin   = height + (margin * 2);
    int swatch_count    = swatch_list.size();

    QPoint pointNewSwatch(margin, margin);
    for(int i = 0; i < swatch_count; i++)
    {
        QBrush brush(swatch_list[i].color, Qt::SolidPattern);
        swatch_list[i].region.setSize(QSize(width, height));
        swatch_list[i].region.marginsAdded(QMargins(margin, margin, margin, margin));
        swatch_list[i].region.moveTo(((i % minColumns) * width_inc_margin), ((i / minColumns) * height_inc_margin));

        painter.setBrush(brush);
        painter.drawRect(swatch_list[i].region);
    }

    //QBrush brush(add_swatch.color);
    //add_swatch.region.setSize(QSize(width, height));
    //add_swatch.region.marginsAdded(QMargins(margin, margin, margin, margin));
    //add_swatch.region.moveTo(((swatch_count % minColumns) * width_inc_margin), ((swatch_count / minColumns) * height_inc_margin));

    //painter.setBrush(brush);
    //painter.drawRect(add_swatch.region);
    //painter.drawText(add_swatch.region, Qt::AlignCenter, QString("+"));

    swatch_pixmap = QPixmap().fromImage(swatch_image);
}


void Swatches::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    drawSwatches(this->size());             //This is the main draw function
    painter.drawPixmap(0,0,swatch_pixmap);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
