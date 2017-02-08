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
#include "cupsdnotifier.h" // Note: this file was generated.

#include <QDBusConnection>
#include <QPrinterInfo>
#include <QQmlEngine>

Printers::Printers(QObject *parent)
    : Printers(new PrinterCupsBackend(new IppClient(), QPrinterInfo(),
        new OrgCupsCupsdNotifierInterface("", CUPSD_NOTIFIER_DBUS_PATH,
                                          QDBusConnection::systemBus())),
       parent)
{
}

Printers::Printers(PrinterBackend *backend, QObject *parent)
    : QObject(parent)
    , m_backend(backend)
    , m_drivers(backend)
    , m_model(backend)
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

    connect(&m_drivers, SIGNAL(filterComplete()),
            this, SIGNAL(driverFilterChanged()));

    if (m_backend->type() == PrinterEnum::PrinterType::CupsType) {
        ((PrinterCupsBackend*) m_backend)->createSubscription();
    }
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
    // TODO: implement
    return Q_NULLPTR;
}

QAbstractItemModel* Printers::printJobs()
{
    // TODO: implement
    return Q_NULLPTR;
}

QAbstractItemModel* Printers::drivers()
{
    auto ret = &m_drivers;
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

QString Printers::driverFilter() const
{
    return m_drivers.filter();
}

void Printers::setDriverFilter(const QString &filter)
{
    m_drivers.setFilter(filter);
}

QString Printers::defaultPrinterName() const
{
    // TODO: implement
    return QString();
}

QString Printers::lastMessage() const
{
    return m_lastMessage;
}

PrinterJob* Printers::createJob(const QString &printerName)
{
    return new PrinterJob(m_backend->getPrinter(printerName), m_backend);
}

void Printers::cancelJob(const QString &printerName, const int jobId)
{
    m_backend->cancelJob(printerName, jobId);
}

void Printers::setDefaultPrinterName(const QString &name)
{
    // TODO: implement
    Q_UNUSED(name);
}

QSharedPointer<Printer> Printers::getPrinterByName(const QString &name)
{
    // TODO: implement
    Q_UNUSED(name);

    return QSharedPointer<Printer>(Q_NULLPTR);
}
QSharedPointer<Printer> Printers::getJobOwner(const int &jobId)
{
    // TODO: implement
    Q_UNUSED(jobId);

    return QSharedPointer<Printer>(Q_NULLPTR);
}

void Printers::prepareToAddPrinter()
{
    m_drivers.load();
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
    QString reply = m_backend->printerDelete(name);

    if (!reply.isEmpty()) {
        m_lastMessage = reply;
        return false;
    }
    return true;
}
