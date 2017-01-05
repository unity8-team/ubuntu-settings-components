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
#include <QDBusConnection>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QVector>

class PrinterModelPrivate;
class PrinterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PrinterModel)
public:
    explicit PrinterModel(QObject *parent = 0);
    explicit PrinterModel(QDBusConnection &dbus, QObject *parent = 0);
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

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    QSharedPointer<Printer> getPrinterFromName(const QString &name);
};

class PrinterFilterPrivate;
class PrinterFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PrinterFilter)
public:
    explicit PrinterFilter() {}
    virtual ~PrinterFilter() {}
    void filterOnState(const Printer::State &state);
    void filterOnRecent(const bool recent);

protected:
    virtual bool filterAcceptsRow(int, const QModelIndex&) const;
    virtual bool lessThan(const QModelIndex&, const QModelIndex&) const;
};

#endif // USC_PRINTER_MODEL_H
