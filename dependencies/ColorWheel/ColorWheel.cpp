/*-----------------------------------------------------*\
| ColorWheel.cpp                                        |
|                                                       |
|  Color wheel selector widget for Qt                   |
|                                                       |
|  Original: https://github.com/liuyanghejerry/Qt-Plus  |
|                                                       |
|  Modified by Adam Honse (calcprogrammer1@gmail.com)   |
\*-----------------------------------------------------*/

#include "ColorWheel.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QPainterPath>

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent),
    initSize(128,128),
    mouseDown(false),
    margin(0),
    wheelWidth(10),
    current(Qt::red),
    inWheel(false),
    inSquare(false)
{
    current = current.toHsv();
}

QColor ColorWheel::color()
{
    return current;
}

void ColorWheel::setColor(const QColor &color)
{
    if(color == current) return;
    if(color.hue() != current.hue())
    {
        hueChanged(color.hue());
    }

    if((color.saturation() != current.saturation()) || (color.value() != current.value()))
    {
        svChanged(color);
    }

    update();
    emit colorChanged(color);
}


QColor ColorWheel::posColor(const QPoint &point)
{
    /*-----------------------------------------------------*\
    | Subtract offsets from point value                     |
    \*-----------------------------------------------------*/
    int point_x = point.x() - x_offset;
    int point_y = point.y() - y_offset;

    /*-----------------------------------------------------*\
    | If point is not within widget, don't process          |
    \*-----------------------------------------------------*/
    if(!wheel.rect().contains(point))
    {
        return QColor();
    }

    /*-----------------------------------------------------*\
    | If within wheel region, update hue from point         |
    | position                                              |
    \*-----------------------------------------------------*/
    if(inWheel)
    {
        qreal hue = 0;
        int r = qMin(width() - x_offset, height() - y_offset) / 2;
        if( point_x > r )
        {
            if(point_y < r )
            {
                //1
                hue = 90 - (qAtan2( (point_x - r) , (r - point_y) )  / 3.14 / 2 * 360);
            }
            else
            {
                //4
                hue = 270 + (qAtan2( (point_x - r) , (point_y - r ) )  / 3.14 / 2 * 360);
            }
        }
        else
        {
            if(point_y < r )
            {
                //2
                hue =  90 + (qAtan2( (r - point_x) , (r - point_y) )  / 3.14 / 2 * 360);
            }
            else
            {
                //3
                hue =  270 - (qAtan2( (r - point_x) , (point_y - r ))  / 3.14 / 2 * 360);
            }
        }

        /*-----------------------------------------------------*\
        | Restrict hue to range 0-359                           |
        \*-----------------------------------------------------*/
        hue = (hue > 359) ? 359 : hue;
        hue = hue < 0 ? 0 : hue;

        return QColor::fromHsv(hue,
                               current.saturation(),
                               current.value());
    }

    /*-----------------------------------------------------*\
    | If within square region, update saturation and value  |
    | from point position                                   |
    \*-----------------------------------------------------*/
    if(inSquare)
    {
        // region of the widget
        int w = qMin(width() - x_offset, height() - y_offset);

        // radius of outer circle
        qreal r = w/2 - margin;

        // radius of inner circle
        qreal ir = r - wheelWidth;

        // left corner of square
        qreal m = w/2.0 - ir/qSqrt(2);

        QPoint p = point - QPoint(x_offset, y_offset) - QPoint(m, m);
        qreal SquareWidth = 2*ir/qSqrt(2);
        return QColor::fromHsvF( current.hueF(),
                                 p.x()/SquareWidth,
                                 p.y()/SquareWidth);
    }
    return QColor();
}

QSize ColorWheel::sizeHint () const
{
    return QSize(height(),height());
}

QSize ColorWheel::minimumSizeHint () const
{
    return initSize;
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    /*-----------------------------------------------------*\
    | Update last position                                  |
    \*-----------------------------------------------------*/
    lastPos = event->pos();

    /*-----------------------------------------------------*\
    | If mouse is within wheel region, process wheel (hue)  |
    \*-----------------------------------------------------*/
    if(wheelRegion.contains(lastPos))
    {
        inWheel = true;
        inSquare = false;
        QColor color = posColor(lastPos);
        hueChanged(color.hue());
    }

    /*-----------------------------------------------------*\
    | If mouse is within square region, process square      |
    | (saturation and value)                                |
    \*-----------------------------------------------------*/
    else if(squareRegion.contains(lastPos))
    {
        inWheel = false;
        inSquare = true;
        QColor color = posColor(lastPos);
        svChanged(color);
    }

    /*-----------------------------------------------------*\
    | Set the mouse down flag                               |
    \*-----------------------------------------------------*/
    mouseDown = true;
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    /*-----------------------------------------------------*\
    | Update last position                                  |
    \*-----------------------------------------------------*/
    lastPos = event->pos();

    /*-----------------------------------------------------*\
    | Don't process if mouse button is not down             |
    \*-----------------------------------------------------*/
    if(!mouseDown)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | If mouse is within wheel region, process wheel (hue)  |
    \*-----------------------------------------------------*/
    if(wheelRegion.contains(lastPos) && inWheel)
    {
        QColor color = posColor(lastPos);
        hueChanged(color.hue());
    }

    /*-----------------------------------------------------*\
    | If mouse is within square region, process square      |
    | (saturation and value)                                |
    \*-----------------------------------------------------*/
    else if(squareRegion.contains(lastPos) && inSquare)
    {
        QColor color = posColor(lastPos);
        svChanged(color);
    }
    else
    {
        // TODO: due with cursor out of region after press
        //        int length = qMin(width(), height());
        //        QPoint center(length/2, length/2);
        //        int R = qSqrt(qPow(qAbs(lastPos.x()), 2)
        //                      + qPow(qAbs(lastPos.y()), 2));
        //        if(inWheel){
        //            int r =  length / 2;
        //            r += qSqrt(qPow(center.x(), 2) + qPow(center.y(), 2));
        //            int x0 = r/R * qAbs(lastPos.x());
        //            int y0 = r/R * qAbs(lastPos.y());
        //            QColor color = posColor(QPoint(x0, y0));
        //            hueChanged(color.hue());
        //        }else if(inSquare){
        //            //
        //        }
    }
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *)
{
    /*-----------------------------------------------------*\
    | Clear mouse down and in-region flags                  |
    \*-----------------------------------------------------*/
    mouseDown   = false;
    inWheel     = false;
    inSquare    = false;
}

void ColorWheel::resizeEvent(QResizeEvent *event)
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

    wheelWidth = 0.1 * size;

    wheel = QPixmap(event->size());
    wheel.fill(Qt::transparent);
    drawWheelImage(event->size());
    drawSquareImage(current.hue());
    update();
}

void ColorWheel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    composeWheel();
    painter.drawPixmap(0,0,wheel);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ColorWheel::drawWheelImage(const QSize &newSize)
{
    /*-----------------------------------------------------*\
    | Create image canvas                                   |
    \*-----------------------------------------------------*/
    wheelImage = QImage(newSize, QImage::Format_ARGB32_Premultiplied);

    /*-----------------------------------------------------*\
    | Paint the background                                  |
    \*-----------------------------------------------------*/
    wheelImage.fill(Qt::transparent);

    /*-----------------------------------------------------*\
    | Create rainbow gradient for wheel                     |
    \*-----------------------------------------------------*/
    QConicalGradient conicalGradient(0, 0, 0);
    conicalGradient.setColorAt(0.0,           Qt::red);
    conicalGradient.setColorAt(60.0  / 360.0, Qt::yellow);
    conicalGradient.setColorAt(120.0 / 360.0, Qt::green);
    conicalGradient.setColorAt(180.0 / 360.0, Qt::cyan);
    conicalGradient.setColorAt(240.0 / 360.0, Qt::blue);
    conicalGradient.setColorAt(300.0 / 360.0, Qt::magenta);
    conicalGradient.setColorAt(1.0,           Qt::red);

    /*-----------------------------------------------------*\
    | Set up painter with antialiasing                      |
    \*-----------------------------------------------------*/
    QPainter painter(&wheelImage);
    painter.setRenderHint(QPainter::Antialiasing);

    /*-----------------------------------------------------*\
    | Paint the wheel                                       |
    \*-----------------------------------------------------*/
    int size = qMin(newSize.width(), newSize.height());
    x_offset = (newSize.width() - size) / 2;
    y_offset = (newSize.height() - size) / 2;
    int r = size;

    QPainterPath painterpath;
    painterpath.addEllipse(QPoint(0,0),r/2-margin,r/2-margin);
    painterpath.addEllipse(QPoint(0,0),r/2-margin-wheelWidth,r/2-margin-wheelWidth);

    painter.translate(x_offset + (size / 2), y_offset + (size / 2));

    QBrush brush(conicalGradient);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    painter.drawPath(painterpath);

    /*-----------------------------------------------------*\
    | Calculate wheel region and subtract out the inner     |
    | region                                                |
    \*-----------------------------------------------------*/
    wheelRegion = QRegion(r/2, r/2, r-2*margin, r-2*margin, QRegion::Ellipse);
    wheelRegion.translate(x_offset - (r-2*margin)/2, y_offset - (r-2*margin)/2);

    int tmp = 2*(margin+wheelWidth);
    QRegion subRe( r/2, r/2, r-tmp, r-tmp, QRegion::Ellipse );
    subRe.translate( x_offset - (r-tmp)/2, y_offset - (r-tmp)/2);
    wheelRegion -= subRe;

    CleanWheel = QPixmap().fromImage(wheelImage);
}

void ColorWheel::drawSquareImage(const int &hue)
{
//    QPainter painter(&squarePixmap);
//    painter.setRenderHint(QPainter::Antialiasing);

    /*-----------------------------------------------------*\
    | Calculate dimensions                                  |
    \*-----------------------------------------------------*/
    int w = qMin(width(), height());

    // radius of outer circle
    qreal r = w/2-margin;

    // radius of inner circle
    qreal ir = r-wheelWidth;

    // left corner of square
    qreal m = w/2.0-ir/qSqrt(2);

    /*-----------------------------------------------------*\
    | Create image canvas                                   |
    \*-----------------------------------------------------*/
    QImage square(255,255, QImage::Format_ARGB32_Premultiplied);

    /*-----------------------------------------------------*\
    | Paint the square.  X axis is saturation and Y axis is |
    | value                                                 |
    \*-----------------------------------------------------*/
    QColor color;
    QRgb qrgb;

    for(int x = 0; x < 255; x++)
    {
        for(int y = 0; y < 255; y++)
        {
            color = QColor::fromHsv(hue, x, y);

            qrgb = qRgb(color.red(),color.green(),color.blue());

            square.setPixel(x, y, qrgb);
        }
    }

    /*-----------------------------------------------------*\
    | Copy the fixed-size square image on to the scaled     |
    | canvas                                                |
    \*-----------------------------------------------------*/
    qreal SquareWidth = 2*ir/qSqrt(2);
    squareImage = square.scaled(SquareWidth, SquareWidth);

    /*-----------------------------------------------------*\
    | Calculate square region                               |
    \*-----------------------------------------------------*/
    squareRegion = QRegion(x_offset + m, y_offset + m, SquareWidth, SquareWidth);
    CleanSquare = squareImage;
}

void ColorWheel::drawIndicator(const int &hue)
{
    QPainter painter(&wheel);
    painter.setRenderHint(QPainter::Antialiasing);
    if(hue > 20 && hue < 200)
    {
        painter.setPen(Qt::black);
    }
    else
    {
        painter.setPen(Qt::white);
    }
    painter.setBrush(Qt::NoBrush);

    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);
    qreal r = qMin(height(), width()) / 2.0;
    painter.translate(x_offset + r, y_offset + r);
    painter.rotate( -hue );
    r = qMin(height(), width()) / 2.0 - margin - wheelWidth/2;
    painter.drawEllipse(QPointF(r,0.0),5,5);
}

void ColorWheel::drawPicker(const QColor &color)
{
    QPainter painter(&wheel);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;

    // region of the widget
    int w = qMin(width(), height());

    // radius of outer circle
    qreal r = w/2-margin;

    // radius of inner circle
    qreal ir = r-wheelWidth;

    // left corner of square
    qreal m = w/2.0-ir/qSqrt(2);

    painter.translate(x_offset + m-5, y_offset + m-5);

    qreal SquareWidth = 2*ir/qSqrt(2);
    qreal S = color.saturationF()*SquareWidth;
    qreal V = color.valueF()*SquareWidth;

    if(color.saturation() > 30 ||color.value() < 50)
    {
        pen.setColor(Qt::white);
    }

    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawEllipse(S,V,10,10);
}

void ColorWheel::composeWheel()
{
    wheel = CleanWheel;
    squareImage = CleanSquare;
    QPainter composePainter(&wheel);
    composePainter.drawImage(0, 0, wheelImage);
    composePainter.drawImage(squareRegion.boundingRect().topLeft(), squareImage);
    composePainter.end();
    drawIndicator(current.hue());
    drawPicker(current);
}

void ColorWheel::hueChanged(const int &hue)
{
    if((hue < 0) || (hue > 359))
    {
        return;
    }

    int s = current.saturation();
    int v = current.value();
    current.setHsv(hue, s, v);

    drawSquareImage(hue);

    if(!isVisible())
    {
        return;
    }
    repaint();

    emit colorChanged(current);
}

void ColorWheel::svChanged(const QColor &newcolor)
{
    int hue = current.hue();

    current.setHsv
            (
            hue,
            newcolor.saturation(),
            newcolor.value()
            );

    if(!isVisible())
    {
        return;
    }

    repaint();

    emit colorChanged(current);
}
