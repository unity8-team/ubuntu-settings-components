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

#ifndef USC_PRINTER_MODEL_H
#define USC_PRINTER_MODEL_H

#include "printer/printer.h"

#include <QAbstractListModel>
#include <QByteArray>
#include <QModelIndex>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QVariant>

class PrinterModelPrivate;
class Q_DECL_EXPORT PrinterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PrinterModel)
    QScopedPointer<PrinterModelPrivate> const d_ptr;
public:
    explicit PrinterModel(QObject *parent = 0);
    explicit PrinterModel(PrinterInfo *info, CupsFacade *cups, QObject *parent = 0);
    ~PrinterModel();

    enum class Roles
    {
        // Qt::DisplayRole holds device name
        ColorModeRole = Qt::UserRole,
        CopiesRole,
        DuplexRole,
        DuplexSupportedRole,
        NameRole,
        PrintRangeRole,
        PrintRangeModeRole,
        PdfModeRole,
        QualityRole,
        DescriptionRole,
        PageSizeRole,
        QPageSizeRole,
        AccessControlRole,
        ErrorPolicyRole,
        UsersRole,
        StateRole,
        PrinterRole,
        LastStateMessageRole,
        LastRole = LastStateMessageRole,
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    QSharedPointer<Printer> getPrinterFromName(const QString &name);
};

class PrinterFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PrinterFilter();
    ~PrinterFilter();
    void filterOnState(const State &state);
    void filterOnRecent(const bool recent);

protected:
    virtual bool filterAcceptsRow(int, const QModelIndex&) const override;
    virtual bool lessThan(const QModelIndex&, const QModelIndex&) const override;
};

#endif // USC_PRINTER_MODEL_H
