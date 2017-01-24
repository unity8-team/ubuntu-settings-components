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

#include <QAbstractItemModel>
#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QString>

class PRINTERS_DECL_EXPORT Printers : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* allPrinters READ allPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* allPrintersWithPdf READ allPrintersWithPdf CONSTANT)
    Q_PROPERTY(QAbstractItemModel* recentPrinters READ recentPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* printJobs READ printJobs CONSTANT)
    Q_PROPERTY(QString defaultPrinterName READ defaultPrinterName WRITE setDefaultPrinterName NOTIFY defaultPrinterNameChanged)

public:
    explicit Printers(int printerUpdateIntervalMSecs = 5000, QObject *parent = nullptr);

    // Note: Printers takes ownership of backend.
    explicit Printers(PrinterBackend *backend,
                      int printerUpdateIntervalMSecs = 5000,
                      QObject *parent = nullptr);
    ~Printers();

    QAbstractItemModel* allPrinters();
    QAbstractItemModel* allPrintersWithPdf();
    QAbstractItemModel* recentPrinters();
    QAbstractItemModel* printJobs();
    QString defaultPrinterName() const;

    void setDefaultPrinterName(const QString &name);

public Q_SLOTS:
    QSharedPointer<Printer> getPrinterByName(const QString &name);
    QSharedPointer<Printer> getJobOwner(const int &jobId);

    QSharedPointer<Printer> addPrinter(const QString &name,
                                       const QString &ppd,
                                       const QString &device,
                                       const QString &description,
                                       const QString &location);
    QSharedPointer<Printer> addPrinterWithPpdFile(const QString &name,
                                                  const QString &ppdFileName,
                                                  const QString &device,
                                                  const QString &description,
                                                  const QString &location);

    bool removePrinter(const QString &name);

Q_SIGNALS:
    void defaultPrinterNameChanged();

private:
    PrinterBackend *m_backend;
    PrinterModel m_model;
    PrinterFilter m_allPrinters;
    PrinterFilter m_allPrintersWithPdf;
    PrinterFilter m_recentPrinters;
};

#endif // USC_PRINTERS_H
