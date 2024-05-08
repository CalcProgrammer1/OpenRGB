/*---------------------------------------------------------*\
| swatches.h                                                |
|                                                           |
|   Custom Colour Swatch widget that allows for adding user |
|   colours                                                 |
|                                                           |
|   Chris M (Dr_No)                             23 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QVector>
#include <QPen>

class Swatches : public QWidget
{
    Q_OBJECT
public:
    explicit Swatches(QWidget *parent = nullptr);

    virtual QSize   sizeHint () const;
    virtual QSize   minimumSizeHint () const;
    QColor          color();

signals:
    void            swatchChanged(const QColor color);
    //void            customSwatches(const QVector<QColor> swatch);

public slots:
    void            addCustomSwatch(const QColor &color);
    //void            addCustomSwatches(const QVector<QColor> &swatch);
    void            setCurrentColor(const QColor &color);

protected:
    void            mousePressEvent(QMouseEvent *event);
    void            mouseReleaseEvent(QMouseEvent *);
    void            resizeEvent(QResizeEvent *event);
    void            paintEvent(QPaintEvent *);

private:
    const int       minSize             = 20;
    const int       minRows             = 1;
    const int       minColumns          = 8;
    const int       margin              = 2;
    const float     corner_radius       = 0.0f;
    bool            mouseDown           = false;
    int             selected            = 0;
    int             width_inc_margin    = minSize + (margin * 2);
    int             height_inc_margin   = minSize + (margin * 2);
    int             min_swatches;

    struct swatch
    {
        QColor      color           = QColor(0, 0, 0);
        QRect       region          = QRect(0, 0, 25, 25);
    };

    swatch          add_swatch;
    QPen            border_pen      = QColor(128,128,128);  //Grey50
    QVector<swatch> swatch_list;
    QColor          picker_color;
    QSize           initSize;
    QPixmap         swatch_pixmap;
    QImage          swatch_image;

    void            drawSwatches(const QSize &newSize);
};
