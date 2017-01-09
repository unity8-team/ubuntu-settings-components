/*
 * Copyright (C) 2017 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "models/printermodel.h"
#include "models/printermodel_p.h"

#include <QDebug>

PrinterModel::PrinterModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this))
{
}

PrinterModel::PrinterModel(PrinterInfo *info, CupsFacade *cups, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this, info, cups))
{
}

PrinterModel::~PrinterModel()
{

}

int PrinterModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const PrinterModel);
    return d->printers.size();
}

QVariant PrinterModel::data(const QModelIndex &index, int role) const
{

}

QHash<int, QByteArray> PrinterModel::roleNames() const
{

}

QSharedPointer<Printer> PrinterModel::getPrinterFromName(const QString &name)
{

}

PrinterModelPrivate::PrinterModelPrivate(PrinterModel *q)
{

}

PrinterModelPrivate::PrinterModelPrivate(PrinterModel *q, PrinterInfo *info, CupsFacade *cups)
{
    this->info = info;
    this->cups = cups;

    Q_FOREACH(const QString &printer, info->availablePrinterNames()) {
        qWarning() << printer;
    }
}

PrinterFilter::PrinterFilter()
{

}

PrinterFilter::~PrinterFilter()
{

}

void PrinterFilter::filterOnState(const State &state)
{

}

void PrinterFilter::filterOnRecent(const bool recent)
{

}

bool PrinterFilter::filterAcceptsRow(int, const QModelIndex&) const
{

}

bool PrinterFilter::lessThan(const QModelIndex&, const QModelIndex&) const
{

}
