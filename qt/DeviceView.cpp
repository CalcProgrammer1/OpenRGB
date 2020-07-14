/*-----------------------------------------------------*\
| DeviceView.cpp                                        |
|                                                       |
|  OpenRGB Device view widget for Qt                    |
|                                                       |
| Adam Honse (calcprogrammer1@gmail.com)                |
\*-----------------------------------------------------*/

#include "DeviceView.h"
#include "RGBController.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    initSize(128,128),
    mouseDown(false)
{
    controller = NULL;
}

void DeviceView::setController(RGBController * controller_ptr)
{
    controller = controller_ptr;
}

QSize DeviceView::sizeHint () const
{
    return QSize(height(),height());
}

QSize DeviceView::minimumSizeHint () const
{
    return initSize;
}

void DeviceView::mousePressEvent(QMouseEvent *event)
{

}

void DeviceView::mouseMoveEvent(QMouseEvent *event)
{

}

void DeviceView::mouseReleaseEvent(QMouseEvent *)
{

}

void DeviceView::resizeEvent(QResizeEvent *event)
{
    unsigned int size = 0;

    if(event->size().width() < event->size().height())
    {
        size = event->size().width();
    }
    else
    {
        size = event->size().height();
    }

    update();
}

void DeviceView::paintEvent(QPaintEvent *event)
{
    int width       = event->rect().width();
    int height      = event->rect().height();
    int row         = 0;
    int col         = 0;
    int box_size    = 20;
    int box_margin  = 2;
    int max_cols    = ( width / (box_size + box_margin) ) - 1;

    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);

    if(controller != NULL)
    {
        for(int zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
        {
            if((controller->zones[zone_idx].type == ZONE_TYPE_MATRIX)
             &&(controller->zones[zone_idx].matrix_map != NULL))
            {
                unsigned int x_count = controller->zones[zone_idx].matrix_map->width;
                unsigned int y_count = controller->zones[zone_idx].matrix_map->height;

                for(int x = 0; x < x_count; x++)
                {
                    for(int y = 0; y < y_count; y++)
                    {
                        unsigned int map_idx = (y * x_count) + x;
                        unsigned int color_idx = controller->zones[zone_idx].matrix_map->map[map_idx];

                        if( color_idx != 0xFFFFFFFF )
                        {
                            painter.fillRect((x * (box_size + box_margin)), ((y + row) * (box_size + box_margin)), box_size, box_size, QColor::fromRgb(RGBGetRValue(controller->zones[zone_idx].colors[color_idx]), RGBGetGValue(controller->zones[zone_idx].colors[color_idx]), RGBGetBValue(controller->zones[zone_idx].colors[color_idx])));
                            painter.drawRect((x * (box_size + box_margin)), ((y + row) * (box_size + box_margin)), box_size, box_size);
                        }
                    }
                }
                row += y_count;
            }
            else
            {
                for(int led_idx = 0; led_idx < controller->zones[zone_idx].leds_count; led_idx++)
                {
                    painter.fillRect((col * (box_size + box_margin)), (row * (box_size + box_margin)), box_size, box_size, QColor::fromRgb(RGBGetRValue(controller->zones[zone_idx].colors[led_idx]), RGBGetGValue(controller->zones[zone_idx].colors[led_idx]), RGBGetBValue(controller->zones[zone_idx].colors[led_idx])));
                    painter.drawRect((col * (box_size + box_margin)), (row * (box_size + box_margin)), box_size, box_size);
                    col++;

                    if(col > max_cols)
                    {
                        row++;
                        col = 0;
                    }
                }
            }

            row += 2;
            col  = 0;
        }
    }
}
