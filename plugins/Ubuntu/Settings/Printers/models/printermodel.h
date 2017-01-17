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

#include "printers_global.h"

#include "printer/printer.h"

#include <QAbstractListModel>
#include <QByteArray>
#include <QModelIndex>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QVariant>

class PrinterModelPrivate;
class PRINTERS_DECL_EXPORT PrinterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PrinterModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit PrinterModel(int timerMsec=5000, QObject *parent = 0);
    explicit PrinterModel(PrinterInfo *info, CupsFacade *cups, int timerMsec=5000, QObject *parent = 0);
    ~PrinterModel();

    enum Roles
    {
        // Qt::DisplayRole holds device name
        ColorModelRole = Qt::UserRole,
        SupportedColorModelsRole,
        CopiesRole,
        DuplexRole,
        SupportedDuplexModesRole,
        NameRole,
        PrintRangeRole,
        PrintRangeModeRole,
        PdfModeRole,
        QualityRole,
        DescriptionRole,
        PageSizeRole,
        SupportedPageSizesRole,
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
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;

    QSharedPointer<Printer> getPrinterFromName(const QString &name);

private:
    QScopedPointer<PrinterModelPrivate> const d_ptr;
    QTimer *m_update_timer;

private Q_SLOTS:
    void update();

Q_SIGNALS:
    void countChanged();
};

class PrinterFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PrinterFilter();
    ~PrinterFilter();
    void filterOnState(const PrinterEnum::State &state);
    void filterOnRecent(const bool recent);

protected:
    virtual bool filterAcceptsRow(int, const QModelIndex&) const override;
    virtual bool lessThan(const QModelIndex&, const QModelIndex&) const override;
};

#endif // USC_PRINTER_MODEL_H
