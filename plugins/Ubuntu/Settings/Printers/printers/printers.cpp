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

#include "backend/backend_cups.h"
#include "printers/printers.h"

#include <QQmlEngine>

Printers::Printers(int printerUpdateIntervalMSecs, QObject *parent)
    : Printers(new PrinterCupsBackend, printerUpdateIntervalMSecs, parent)
{
}

Printers::Printers(PrinterBackend *backend, int printerUpdateIntervalMSecs,
                   QObject *parent)
    : QObject(parent)
    , m_backend(backend)
    , m_model(backend, printerUpdateIntervalMSecs)
{
    m_allPrinters.setSourceModel(&m_model);
    m_allPrinters.setSortRole(PrinterModel::Roles::DefaultPrinterRole);
    m_allPrinters.filterOnPdf(false);
    m_allPrinters.sort(0, Qt::DescendingOrder);

    m_allPrintersWithPdf.setSourceModel(&m_model);
    m_allPrintersWithPdf.setSortRole(PrinterModel::Roles::DefaultPrinterRole);
    m_allPrintersWithPdf.sort(0, Qt::DescendingOrder);
}

Printers::~Printers()
{
    delete m_backend;
}

QAbstractItemModel* Printers::allPrinters()
{
    auto ret = &m_allPrinters;
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

QAbstractItemModel* Printers::allPrintersWithPdf()
{
    auto ret = &m_allPrintersWithPdf;
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
