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

#ifndef USC_PRINTERS_H
#define USC_PRINTERS_H

#include "printers_global.h"

#include "cups/cupsfacade.h"
#include "models/printermodel.h"
#include "printer/printer.h"
#include "printer/printerinfo.h"

#include <QAbstractItemModel>
#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QString>
#include <QUrl>

class PrintersPrivate;
class PRINTERS_DECL_EXPORT Printers : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printers)
    Q_PROPERTY(QAbstractItemModel* allPrinters READ allPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* recentPrinters READ recentPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* printJobs READ printJobs CONSTANT)
    Q_PROPERTY(QString defaultPrinterName READ defaultPrinterName WRITE setDefaultPrinterName NOTIFY defaultPrinterNameChanged)

    QScopedPointer<PrintersPrivate> const d_ptr;

public:
    explicit Printers(QObject *parent = nullptr);
    explicit Printers(PrinterInfo *info, CupsFacade *cups, QObject *parent = nullptr);
    ~Printers();

    QAbstractItemModel* allPrinters() const;
    QAbstractItemModel* recentPrinters() const;
    QAbstractItemModel* printJobs() const;
    QString defaultPrinterName() const;

    void setDefaultPrinterName(const QString &name);

public Q_SLOTS:
    QSharedPointer<Printer> getPrinterByName(const QString &name);
    QSharedPointer<Printer> getJobOwner(const int &jobId);

    QSharedPointer<Printer> addPrinter(const QString &name,
                                       const QUrl &ppd,
                                       const QUrl &device,
                                       const QString &description,
                                       const QString &location);

    QSharedPointer<Printer> addPrinter(const QString &name,
                                       const QUrl &device,
                                       const QString &description,
                                       const QString &location);

    bool removePrinter(const QString &name);

Q_SIGNALS:
    void defaultPrinterNameChanged();
};

#endif // USC_PRINTERS_H
