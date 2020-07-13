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
    mouseDown(false),
    margin(0),
    wheelWidth(10),
    current(Qt::red),
    inWheel(false),
    inSquare(false)
{
    controller = NULL;
    current = current.toHsv();
}

QColor DeviceView::color()
{
    return current;
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
    /*-----------------------------------------------------*\
    | Clear mouse down and in-region flags                  |
    \*-----------------------------------------------------*/
    mouseDown   = false;
    inWheel     = false;
    inSquare    = false;
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

    wheel = QPixmap(event->size());
    wheel.fill(Qt::transparent);
    update();
}

void DeviceView::paintEvent(QPaintEvent *)
{
    #define MAX_COLS 100

    int row         = 0;
    int col         = 0;
    int box_size    = 20;
    int box_margin  = 2;

    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);

    if(controller != NULL)
    {
        for(int zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
        {
            for(int led_idx = 0; led_idx < controller->zones[zone_idx].leds_count; led_idx++)
            {
                painter.fillRect((col * (box_size + box_margin)), (row * (box_size + box_margin)), box_size, box_size, QColor::fromRgb(RGBGetRValue(controller->zones[zone_idx].colors[led_idx]), RGBGetGValue(controller->zones[zone_idx].colors[led_idx]), RGBGetBValue(controller->zones[zone_idx].colors[led_idx])));
                painter.drawRect((col * (box_size + box_margin)), (row * (box_size + box_margin)), box_size, box_size);
                col++;

                if(col > MAX_COLS)
                {
                    row++;
                    col = 0;
                }
            }

            row += 2;
            col  = 0;
        }
    }
}
