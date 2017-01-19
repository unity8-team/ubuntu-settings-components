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

#include "cups/cupsfacade_impl.h"
#include "printer/printerinfo_allimpl.h"
#include "printers/printers.h"
#include "printers/printers_p.h"

#include <QQmlEngine>

Printers::Printers(int printerUpdateIntervalMSecs, QObject *parent)
    : QObject(parent)
    , d_ptr(new PrintersPrivate(this, printerUpdateIntervalMSecs))
{
}

Printers::Printers(PrinterInfo *info, CupsFacade *cups,
                   int printerUpdateIntervalMSecs, QObject *parent)
    : QObject(parent)
    , d_ptr(new PrintersPrivate(this, info, cups, printerUpdateIntervalMSecs))
{
}

Printers::~Printers()
{
}

PrintersPrivate::PrintersPrivate(Printers *q, int printerUpdateIntervalMSecs)
    : PrintersPrivate(q, new PrinterInfoAllImpl, new CupsFacadeImpl,
                      printerUpdateIntervalMSecs)
{
}

PrintersPrivate::PrintersPrivate(Printers *q, PrinterInfo *info,
                                 CupsFacade *cups,
                                 int printerUpdateIntervalMSecs)
    : model(info, cups, printerUpdateIntervalMSecs)
{
    this->info = info;
    this->cups = cups;

    allPrinters.setSourceModel(&model);
    allPrinters.setSortRole(PrinterModel::Roles::DefaultPrinterRole);
    allPrinters.filterOnPdf(true);
    allPrinters.sort(0, Qt::DescendingOrder);

    allPrintersWithPdf.setSourceModel(&model);
    allPrintersWithPdf.setSortRole(PrinterModel::Roles::DefaultPrinterRole);
    allPrintersWithPdf.filterOnPdf(false);
    allPrintersWithPdf.sort(0, Qt::DescendingOrder);
}

PrintersPrivate::~PrintersPrivate()
{
    delete cups;
    delete info;
}

QAbstractItemModel* Printers::allPrinters()
{
    Q_D(Printers);
    auto ret = &d->allPrinters;
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

QAbstractItemModel* Printers::allPrintersWithPdf()
{
    Q_D(Printers);
    auto ret = &d->allPrintersWithPdf;
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

QAbstractItemModel* Printers::recentPrinters()
{

}

QAbstractItemModel* Printers::printJobs()
{

}

QString Printers::defaultPrinterName() const
{

}

void Printers::setDefaultPrinterName(const QString &name)
{

}

QSharedPointer<Printer> Printers::getPrinterByName(const QString &name)
{

}
QSharedPointer<Printer> Printers::getJobOwner(const int &jobId)
{

}

QSharedPointer<Printer> Printers::addPrinter(const QString &name,
                                             const QUrl &ppd,
                                             const QUrl &device,
                                             const QString &description,
                                             const QString &location)
{

}

QSharedPointer<Printer> Printers::addPrinter(const QString &name,
                                             const QUrl &device,
                                             const QString &description,
                                             const QString &location)
{

}

bool Printers::removePrinter(const QString &name)
{

}
