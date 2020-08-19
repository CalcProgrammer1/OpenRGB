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
#include <QMouseEvent>

DeviceView::DeviceView(QWidget *parent) :
    QWidget(parent),
    initSize(128,128),
    mouseDown(false)
{
    controller = NULL;
    setMouseTracking(1);

    size = width();
}

void DeviceView::setController(RGBController * controller_ptr)
{
    /*-----------------------------------------------------*\
    | Store the controller pointer                          |
    \*-----------------------------------------------------*/
    controller = controller_ptr;

    /*-----------------------------------------------------*\
    | Set the size of the selection flags vector            |
    \*-----------------------------------------------------*/
    selectionFlags.resize(controller->leds.size());

    /*-----------------------------------------------------*\
    | Set the size of the zone and LED position vectors     |
    \*-----------------------------------------------------*/
    zone_pos.resize(controller->zones.size());
    led_pos.resize(controller->leds.size());

    /*-----------------------------------------------------*\
    | Process position and size for zones                   |
    \*-----------------------------------------------------*/
    unsigned int maxWidth       = 0;
    unsigned int maxCols        = 20;
    unsigned int totalHeight    = 0;

    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        if((controller->zones[zone_idx].type == ZONE_TYPE_MATRIX) && (controller->zones[zone_idx].matrix_map))
        {
            totalHeight += controller->zones[zone_idx].matrix_map->height;
            zone_pos[zone_idx].matrix_w = controller->zones[zone_idx].matrix_map->width;
        }
        else
        {
            unsigned int count = controller->zones[zone_idx].leds_count;
            zone_pos[zone_idx].matrix_w = std::min(count, maxCols);
            totalHeight += count / maxCols + !!(count % maxCols); // Equivalent to ceil(float(count) / maxCols);
        }

        if(zone_pos[zone_idx].matrix_w > maxWidth)
        {
            maxWidth = zone_pos[zone_idx].matrix_w;
        }
    }

    /*-----------------------------------------------------*\
    | Add some space for zone names and padding             |
    \*-----------------------------------------------------*/
    totalHeight += controller->zones.size();

    float atom      = 1.0 / maxWidth;       // Atom is the width of a single square; if the whole thing becomes too tall, we ignore it and let the view widget take care of it
    float current_y = 0;                    // We will be descending, placing each zone one atom below the previous one
    matrix_h        = totalHeight * atom;

    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        zone_pos[zone_idx].matrix_x = (1.0 - (zone_pos[zone_idx].matrix_w * atom)) / 2;
        zone_pos[zone_idx].matrix_y = current_y + 0.5 * atom;
        zone_pos[zone_idx].matrix_w *= atom;
        zone_pos[zone_idx].matrix_h = 0.4 * atom;
        current_y += atom;

        /*-----------------------------------------------------*\
        | Now process the position and size for the LEDs        |
        \*-----------------------------------------------------*/
        if((controller->zones[zone_idx].type == ZONE_TYPE_MATRIX) && (controller->zones[zone_idx].matrix_map))
        {
            matrix_map_type * map = controller->zones[zone_idx].matrix_map;

            for(int led_x = 0; led_x < map->width; led_x++)
            {
                for(int led_y = 0; led_y < map->height; led_y++)
                {
                    unsigned int map_idx    = led_y * map->width + led_x;
                    unsigned int color_idx  = map->map[map_idx];

                    if(color_idx != 0xFFFFFFFF)
                    {
                        led_pos[color_idx].matrix_x = (zone_pos[zone_idx].matrix_x + led_x + 0.1) * atom;
                        led_pos[color_idx].matrix_y = (current_y + led_y + 0.1) * atom;
                        led_pos[color_idx].matrix_w = 0.8 * atom;
                        led_pos[color_idx].matrix_h = 0.8 * atom;
                    }
                }
            }
        }
        else
        {
            for(int i = 0; i < controller->zones[zone_idx].leds_count; i++)
            {
                led_pos[i + controller->zones[zone_idx].start_idx].matrix_x = zone_pos[zone_idx].matrix_x + (i % maxCols + 0.1) * atom;
                led_pos[i + controller->zones[zone_idx].start_idx].matrix_y = current_y + (i / maxCols + 0.1) * atom;
                led_pos[i + controller->zones[zone_idx].start_idx].matrix_w = 0.8 * atom;
                led_pos[i + controller->zones[zone_idx].start_idx].matrix_h = 0.8 * atom;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Update cached size and offset                         |
    \*-----------------------------------------------------*/
    size     = width();
    offset_x = 0;

    if(height() < size * matrix_h)
    {
        size     = height() / matrix_h;
        offset_x = (width() - size) / 2;
    }
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
    ctrlDown    = event->modifiers().testFlag(Qt::ControlModifier);
    mouseDown   = true;
    mouseMoved  = false;

    if(ctrlDown)
    {
        previousFlags = selectionFlags;
        previousSelection = selectedLeds;
    }

    /*-----------------------------------------------------*\
    | It's okay if the size becomes negative                |
    \*-----------------------------------------------------*/
    selectionRect.setLeft(event->x());
    selectionRect.setTop(event->y());
    selectionRect.setRight(event->x());
    selectionRect.setBottom(event->y());

    updateSelection();
    update();
}

void DeviceView::mouseMoveEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
    selectionRect.setRight(event->x());
    selectionRect.setBottom(event->y());

    if(mouseDown)
    {
        mouseMoved  = true;
        ctrlDown    = event->modifiers().testFlag(Qt::ControlModifier);

        /*-----------------------------------------------------*\
        | Clear the previous selection in case ctrl is released |
        \*-----------------------------------------------------*/
        if(!ctrlDown)
        {
            previousSelection.clear();
            previousFlags.clear();
            previousFlags.resize(controller->leds.size());
        }
        updateSelection();
    }
    update();
}

void DeviceView::mouseReleaseEvent(QMouseEvent* event)
{
    selectionRect = selectionRect.normalized();
    mouseDown = false;

    /*-----------------------------------------------------*\
    | Check if the user clicked a zone name & select it     |
    \*-----------------------------------------------------*/
    if(!mouseMoved)
    {
        int size     = width();
        int offset_x = 0;

        if(height() < size * matrix_h)
        {
            size     = height() / matrix_h;
            offset_x = (width() - size) / 2;
        }

        for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
        {
            int posx = zone_pos[zone_idx].matrix_x * size + offset_x;
            int posy = zone_pos[zone_idx].matrix_y * size;
            int posw = zone_pos[zone_idx].matrix_w * size;
            int posh = zone_pos[zone_idx].matrix_h * size;

            QRect rect = {posx, posy, posw, posh};

            if(rect.contains(event->pos()))
            {
                selectZone(zone_idx, ctrlDown);
            }
        }
    }
    update();
}

void DeviceView::resizeEvent(QResizeEvent *event)
{
    size     = width();
    offset_x = 0;

    if(height() < size * matrix_h)
    {
        size     = height() / matrix_h;
        offset_x = (width() - size) / 2;
    }
    update();
}

void DeviceView::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QFont font = painter.font();

    /*-----------------------------------------------------*\
    | LED rectangles                                        |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < controller->leds.size(); led_idx++)
    {
        int posx = led_pos[led_idx].matrix_x * size + offset_x;
        int posy = led_pos[led_idx].matrix_y * size;
        int posw = led_pos[led_idx].matrix_w * size;
        int posh = led_pos[led_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        /*-----------------------------------------------------*\
        | Fill color                                            |
        \*-----------------------------------------------------*/
        QColor currentColor = QColor::fromRgb(
                    RGBGetRValue(controller->colors[led_idx]),
                    RGBGetGValue(controller->colors[led_idx]),
                    RGBGetBValue(controller->colors[led_idx]));
        painter.setBrush(currentColor);

        /*-----------------------------------------------------*\
        | Border color                                          |
        \*-----------------------------------------------------*/
        if(selectionFlags[led_idx])
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().dark().color());
        }
        painter.drawRect(rect);

        /*-----------------------------------------------------*\
        | Label                                                 |
        | Set the font color so that the text is visible        |
        \*-----------------------------------------------------*/
        font.setPixelSize(posh / 2);
        painter.setFont(font);

        if(currentColor.value() > 127)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::white);
        }
        //painter.drawText(rect, Qt::AlignVCenter | Qt::AlignHCenter, QString(controller->leds[led_idx].label.c_str()));
    }

    font.setPixelSize(12);
    painter.setFont(font);

    /*-----------------------------------------------------*\
    | Zone names                                            |
    \*-----------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
    {
        int posx = zone_pos[zone_idx].matrix_x * size + offset_x;
        int posy = zone_pos[zone_idx].matrix_y * size;
        int posw = zone_pos[zone_idx].matrix_w * size;
        int posh = zone_pos[zone_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};
        
        if(rect.contains(lastMousePos))
        {
            painter.setPen(palette().highlight().color());
        }
        else
        {
            painter.setPen(palette().windowText().color());
        }
        painter.drawText(posx, posy + posh, QString(controller->zones[zone_idx].name.c_str()));
    }

    /*-----------------------------------------------------*\
    | Selection area                                        |
    \*-----------------------------------------------------*/
    if(mouseDown)
    {
        QRect rect = selectionRect.normalized();
        QColor color = palette().highlight().color();
        color.setAlpha(63);
        painter.fillRect(rect, color);
        color.setAlpha(127);
        painter.setBrush(color);
        painter.drawRect(rect);
    }
}

void DeviceView::updateSelection()
{
    selectedLeds.clear();
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());

    QRect sel              = selectionRect.normalized();
    std::vector<led>& leds = controller->leds;

    for(std::size_t led_idx = 0; led_idx < leds.size(); ++led_idx)
    {
        /*-----------------------------------------------------*\
        | Check intersection                                    |
        \*-----------------------------------------------------*/
        int posx = led_pos[led_idx].matrix_x * size + offset_x;
        int posy = led_pos[led_idx].matrix_y * size;
        int posw = led_pos[led_idx].matrix_w * size;
        int posh = led_pos[led_idx].matrix_h * size;

        QRect rect = {posx, posy, posw, posh};

        selectionFlags[led_idx] = 0;

        if(sel.intersects(rect))
        {
            selectionFlags[led_idx] = 1;
        }
        if(ctrlDown)
        {
            selectionFlags[led_idx] ^= previousFlags[led_idx];

            if(selectionFlags[led_idx])
            {
                selectedLeds.push_back(led_idx);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);
}

bool DeviceView::selectLed(int target)
{
    if(target < 0 || target >= controller->leds.size())
    {
        return false;
    }

    selectedLeds.resize(1);
    selectedLeds[0] = target;
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());
    selectionFlags[target] = 1;

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

bool DeviceView::selectLeds(QVector<int> target)
{
    for(int item: target)
    {
        if(item < 0 || item >= controller->leds.size())
        {
            return false;
        }
    }

    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());

    for(int item: target)
    {
        selectionFlags[item] = 1;
    }

    //selectedLeds = target;

    /*-----------------------------------------------------*\
    | Filter out duplicate items                            |
    \*-----------------------------------------------------*/
    selectedLeds.clear();

    for(int i = 0; i < selectionFlags.size(); ++i)
    {
        if(selectionFlags[i])
        {
            selectedLeds.push_back(i);
        }
    }

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

bool DeviceView::selectZone(int zone, bool add)
{
    if(zone < 0 || zone >= controller->zones.size())
    {
        return false;
    }

    if(!add)
    {
        selectedLeds.clear();
        selectionFlags.clear();
        selectionFlags.resize(controller->leds.size());
    }

    int zoneStart = controller->zones[zone].start_idx;

    for(std::size_t led_idx = 0; led_idx < controller->zones[zone].leds_count; ++led_idx)
    {
        if(!selectionFlags[zoneStart + led_idx])
        {
            selectedLeds.push_back(zoneStart + led_idx);
            selectionFlags[zoneStart + led_idx] = 1;
        }
    }

    /*-----------------------------------------------------*\
    | Send selection changed signal                         |
    \*-----------------------------------------------------*/
    emit selectionChanged(selectedLeds);

    return true;
}

void DeviceView::clearSelection()
{
    /*-----------------------------------------------------*\
    | Same as selecting the entire device                   |
    \*-----------------------------------------------------*/
    selectedLeds.clear();
    selectionFlags.clear();
    selectionFlags.resize(controller->leds.size());
}

void DeviceView::setSelectionColor(RGBColor color)
{
    if(selectedLeds.isEmpty())
    {
        controller->SetAllLEDs(color);
    }
    else
    {
        for(int led_idx: selectedLeds)
        {
            controller->SetLED(led_idx, color);
        }
    }
    controller->UpdateLEDs();
}
