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

namespace
{
QSize devicePixelSize(const QSize &logicalSize, const qreal devicePixelRatio)
{
    return QSize(qMax(1, qRound(logicalSize.width() * devicePixelRatio)),
                 qMax(1, qRound(logicalSize.height() * devicePixelRatio)));
}
}

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
    | If within wheel region, update hue from point         |
    | position                                              |
    \*-----------------------------------------------------*/
    if(inWheel)
    {
        qreal hue = 0;
        const qreal center = qMin(width(), height()) / 2.0;
        if( point_x > center )
        {
            if(point_y < center )
            {
                //1
                hue = 90 - (qAtan2( (point_x - center) , (center - point_y) )  / 3.14 / 2 * 360);
            }
            else
            {
                //4
                hue = 270 + (qAtan2( (point_x - center) , (point_y - center ) )  / 3.14 / 2 * 360);
            }
        }
        else
        {
            if(point_y < center )
            {
                //2
                hue =  90 + (qAtan2( (center - point_x) , (center - point_y) )  / 3.14 / 2 * 360);
            }
            else
            {
                //3
                hue =  270 - (qAtan2( (center - point_x) , (point_y - center ))  / 3.14 / 2 * 360);
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
        int w = qMin(width(), height());

        // radius of outer circle
        qreal r = w/2 - margin;

        // radius of inner circle
        qreal ir = r - wheelWidth;

        // Leave a small gap between the square and the inner edge of the wheel
        qreal squareRadius = qMax(0.0, ir - squareGap);

        // left corner of square
        qreal m = w/2.0 - squareRadius/qSqrt(2);

        QPointF p = QPointF(point.x() - x_offset, point.y() - y_offset) - QPointF(m, m);
        qreal SquareWidth = 2*squareRadius/qSqrt(2);
        qreal saturation = qBound(0.0, p.x()/SquareWidth, 1.0);
        qreal value      = qBound(0.0, p.y()/SquareWidth, 1.0);

        return QColor::fromHsvF( current.hueF(),
                                 saturation,
                                 value);
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
    | Set the mouse down flag if the click started inside a |
    | selectable region                                     |
    \*-----------------------------------------------------*/
    mouseDown = inWheel || inSquare;
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
    | If dragging started in the wheel, continue processing |
    | hue from the cursor angle even outside the wheel      |
    \*-----------------------------------------------------*/
    if(inWheel)
    {
        QColor color = posColor(lastPos);
        hueChanged(color.hue());
    }

    /*-----------------------------------------------------*\
    | If dragging started in the square, continue processing|
    | saturation and value with clamped coordinates         |
    \*-----------------------------------------------------*/
    else if(inSquare)
    {
        QColor color = posColor(lastPos);
        svChanged(color);
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
    rebuildCache(event->size());
    update();
}

void ColorWheel::paintEvent(QPaintEvent *)
{
    if(wheel.isNull() || !qFuzzyCompare(wheel.devicePixelRatio(), devicePixelRatioF()))
    {
        rebuildCache(size());
    }

    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    composeWheel();
    painter.drawPixmap(0,0,wheel);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ColorWheel::rebuildCache(const QSize &newSize)
{
    const unsigned int size = qMin(newSize.width(), newSize.height());
    const qreal devicePixelRatio = devicePixelRatioF();

    wheelWidth = 0.1 * size;

    wheel = QPixmap(devicePixelSize(newSize, devicePixelRatio));
    wheel.setDevicePixelRatio(devicePixelRatio);
    wheel.fill(Qt::transparent);
    drawWheelImage(newSize);
    drawSquareImage(current.hue());
}

void ColorWheel::drawWheelImage(const QSize &newSize)
{
    /*-----------------------------------------------------*\
    | Create image canvas                                   |
    \*-----------------------------------------------------*/
    const qreal devicePixelRatio = devicePixelRatioF();
    wheelImage = QImage(devicePixelSize(newSize, devicePixelRatio), QImage::Format_ARGB32_Premultiplied);
    wheelImage.setDevicePixelRatio(devicePixelRatio);

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
    const qreal center = size / 2.0;
    const qreal outerRadius = center - margin;
    const qreal innerRadius = outerRadius - wheelWidth;

    QPainterPath painterpath;
    painterpath.addEllipse(QPointF(0,0), outerRadius, outerRadius);
    painterpath.addEllipse(QPointF(0,0), innerRadius, innerRadius);

    painter.translate(x_offset + center, y_offset + center);

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

    CleanWheel = QPixmap::fromImage(wheelImage);
    CleanWheel.setDevicePixelRatio(devicePixelRatio);
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

    // Leave a small gap between the square and the inner edge of the wheel
    qreal squareRadius = qMax(0.0, ir - squareGap);

    // left corner of square
    qreal m = w/2.0-squareRadius/qSqrt(2);

    /*-----------------------------------------------------*\
    | Create image canvas                                   |
    \*-----------------------------------------------------*/
    const qreal devicePixelRatio = devicePixelRatioF();
    const qreal SquareWidth = 2*squareRadius/qSqrt(2);
    const int squarePixelWidth = qMax(1, qRound(SquareWidth * devicePixelRatio));
    const int squarePixelCoordinateRange = qMax(1, squarePixelWidth - 1);
    QImage square(squarePixelWidth, squarePixelWidth, QImage::Format_ARGB32_Premultiplied);

    /*-----------------------------------------------------*\
    | Paint the square.  X axis is saturation and Y axis is |
    | value                                                 |
    \*-----------------------------------------------------*/
    QColor color;
    QRgb qrgb;

    for(int x = 0; x < squarePixelWidth; x++)
    {
        for(int y = 0; y < squarePixelWidth; y++)
        {
            const int saturation = qRound((x * 254.0) / squarePixelCoordinateRange);
            const int value = qRound((y * 254.0) / squarePixelCoordinateRange);
            color = QColor::fromHsv(hue, saturation, value);

            qrgb = qRgb(color.red(),color.green(),color.blue());

            square.setPixel(x, y, qrgb);
        }
    }

    square.setDevicePixelRatio(devicePixelRatio);

    /*-----------------------------------------------------*\
    | Store the square image at the target device resolution |
    \*-----------------------------------------------------*/
    squareImage = square;

    /*-----------------------------------------------------*\
    | Calculate square region                               |
    \*-----------------------------------------------------*/
    const int squareX = qRound(x_offset + m);
    const int squareY = qRound(y_offset + m);
    const int squareSize = qMax(1, qRound(SquareWidth));
    squareRegion = QRegion(squareX, squareY, squareSize, squareSize);
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
    const qreal size = qMin(height(), width());
    const qreal center = size / 2.0;
    const qreal outerRadius = center - margin;
    const qreal innerRadius = outerRadius - wheelWidth;
    const qreal indicatorRadius = (outerRadius + innerRadius) / 2.0;
    painter.translate(x_offset + center, y_offset + center);
    painter.rotate( -hue );
    painter.drawEllipse(QPointF(indicatorRadius,0.0),5,5);
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

    // Keep the picker geometry aligned with the square's reduced bounds
    qreal squareRadius = qMax(0.0, ir - squareGap);

    // left corner of square
    qreal m = w/2.0-squareRadius/qSqrt(2);

    painter.translate(x_offset + m-5, y_offset + m-5);

    qreal SquareWidth = 2*squareRadius/qSqrt(2);
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
