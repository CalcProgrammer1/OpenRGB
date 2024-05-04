/*---------------------------------------------------------*\
| DetectorTableModel.h                                      |
|                                                           |
|   Table model for detector list                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QSortFilterProxyModel>
#include <QAbstractTableModel>
#include "ResourceManager.h"

typedef struct
{
    std::string key;
    bool        value;
} DetectorTableValue;

class DetectorTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    std::vector<DetectorTableValue> detectors;

public:
    DetectorTableModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex&) const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant&, int role) override;
    QVariant headerData(int index, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

public slots:
    void applySettings();
    void toggleAll(const bool state, QSortFilterProxyModel* detectorSortModel);
};
