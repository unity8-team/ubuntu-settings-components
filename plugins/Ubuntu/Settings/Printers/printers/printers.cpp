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

    // Let Qt be in charge of RAII.
    m_backend->setParent(this);
}

Printers::~Printers()
{
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

QString Printers::lastMessage() const
{
    return m_lastMessage;
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

bool Printers::addPrinter(const QString &name, const QString &ppd,
                          const QString &device, const QString &description,
                          const QString &location)
{
    QString reply = m_backend->printerAdd(name, device, ppd, description,
                                          location);
    if (!reply.isEmpty()) {
        m_lastMessage = reply;
        return false;
    }
    return true;
}

bool Printers::addPrinterWithPpdFile(const QString &name,
                                     const QString &ppdFileName,
                                     const QString &device,
                                     const QString &description,
                                     const QString &location)
{
    QString reply = m_backend->printerAddWithPpd(name, device, ppdFileName,
                                                 description, location);
    if (!reply.isEmpty()) {
        m_lastMessage = reply;
        return false;
    }
    return true;
}

bool Printers::removePrinter(const QString &name)
{

}
