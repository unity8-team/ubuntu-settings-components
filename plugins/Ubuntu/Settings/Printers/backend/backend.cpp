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

#include "backend/backend.h"

PrinterBackend::PrinterBackend(QObject *parent)
    : QObject(parent)
{
}

PrinterBackend::PrinterBackend(const QString &printerName, QObject *parent)
    : QObject(parent)
    , m_printerName(printerName)
{
}

PrinterBackend::~PrinterBackend()
{
}

bool PrinterBackend::holdsDefinition() const
{
    return false;
}

QString PrinterBackend::printerAdd(const QString &name,
                                   const QString &uri,
                                   const QString &ppdFile,
                                   const QString &info,
                                   const QString &location)
{
    return QString();
}

QString PrinterBackend::printerAddWithPpd(const QString &name,
                                          const QString &uri,
                                          const QString &ppdFileName,
                                          const QString &info,
                                          const QString &location)
{
    return QString();
}

QString PrinterBackend::printerDelete(const QString &name)
{
    return QString();
}

QString PrinterBackend::printerSetEnabled(const QString &name,
                                          const bool enabled)
{
    return QString();
}

QString PrinterBackend::printerSetAcceptJobs(
    const QString &name,
    const bool enabled,
    const QString &reason)
{
    return QString();
}

QString PrinterBackend::printerSetInfo(const QString &name,
                                       const QString &info)
{
    return QString();
}

QString PrinterBackend::printerSetLocation(const QString &name,
                                           const QString &location)
{
    return QString();
}

QString PrinterBackend::printerSetShared(const QString &name,
                                         const bool shared)
{
    return QString();
}

QString PrinterBackend::printerSetJobSheets(const QString &name,
                                            const QString &start,
                                            const QString &end)
{
    return QString();
}

QString PrinterBackend::printerSetErrorPolicy(const QString &name,
                                              const PrinterEnum::ErrorPolicy &policy)
{
    return QString();
}


QString PrinterBackend::printerSetOpPolicy(const QString &name,
                                           const PrinterEnum::OperationPolicy &policy)
{
    return QString();
}

QString PrinterBackend::printerSetUsersAllowed(const QString &name,
                                               const QStringList &users)
{
    return QString();
}

QString PrinterBackend::printerSetUsersDenied(const QString &name,
                                              const QStringList &users)
{
    return QString();
}

QString PrinterBackend::printerAddOptionDefault(const QString &name,
                                                const QString &option,
                                                const QStringList &values)
{
    return QString();
}

QString PrinterBackend::printerDeleteOptionDefault(const QString &name,
                                                   const QString &value)
{
    return QString();
}

QString PrinterBackend::printerAddOption(const QString &name,
                                         const QString &option,
                                         const QStringList &values)
{
    return QString();
}

QVariant PrinterBackend::printerGetOption(const QString &name,
                                          const QString &option) const
{
    return QVariant();
}

QMap<QString, QVariant> PrinterBackend::printerGetOptions(
    const QString &name, const QStringList &options
)
{
    return QMap<QString, QVariant>();
}

// FIXME: maybe have a PrinterDest iface that has a CupsDest impl?
cups_dest_t* PrinterBackend::makeDest(const QString &name,
                              const PrinterJob *options)
{
    return Q_NULLPTR;
}

QList<ColorModel> PrinterBackend::printerGetSupportedColorModels(
    const QString &name) const
{
    return QList<ColorModel>();
}

ColorModel PrinterBackend::printerGetDefaultColorModel(
    const QString &name) const
{
    Q_UNUSED(name);
    return ColorModel();
}

QList<PrintQuality> PrinterBackend::printerGetSupportedQualities(
    const QString &name) const
{
    return QList<PrintQuality>();
}

PrintQuality PrinterBackend::printerGetDefaultQuality(
        const QString &name) const
{
    Q_UNUSED(name);
    return PrintQuality();
}

void PrinterBackend::cancelJob(const QString &name, const int jobId)
{
    Q_UNUSED(jobId);
    Q_UNUSED(name);
}

int PrinterBackend::printFileToDest(const QString &filepath,
                            const QString &title,
                            const cups_dest_t *dest)
{
    return -1;
}

QList<PrinterJob *> PrinterBackend::printerGetJobs(const QString &name)
{
    Q_UNUSED(name);

    return QList<PrinterJob *>{};
}

QString PrinterBackend::printerName() const
{
    return QString();
}

QString PrinterBackend::description() const
{
    return QString();
}

QString PrinterBackend::location() const
{
    return QString();
}

QString PrinterBackend::makeAndModel() const
{
    return QString();
}

PrinterEnum::State PrinterBackend::state() const
{
    return PrinterEnum::State::IdleState;
}

QList<QPageSize> PrinterBackend::supportedPageSizes() const
{
    return QList<QPageSize>();
}

QPageSize PrinterBackend::defaultPageSize() const
{

}

bool PrinterBackend::supportsCustomPageSizes() const
{
    return false;
}

QPageSize PrinterBackend::minimumPhysicalPageSize() const
{
    return QPageSize();
}

QPageSize PrinterBackend::maximumPhysicalPageSize() const
{
    return QPageSize();
}

QList<int> PrinterBackend::supportedResolutions() const
{
    return QList<int>();
}

PrinterEnum::DuplexMode PrinterBackend::defaultDuplexMode() const
{
    return PrinterEnum::DuplexMode::DuplexNone;
}

QList<PrinterEnum::DuplexMode> PrinterBackend::supportedDuplexModes() const
{
    return QList<PrinterEnum::DuplexMode>();
}

QList<Printer*> PrinterBackend::availablePrinters()
{
    return QList<Printer*>();
}

QStringList PrinterBackend::availablePrinterNames()
{
    return QStringList();
}

Printer* PrinterBackend::getPrinter(const QString &printerName)
{
    return Q_NULLPTR;
}

QString PrinterBackend::defaultPrinterName()
{
    return QString();
}

void PrinterBackend::requestAvailablePrinterDrivers()
{
}

PrinterBackend::BackendType PrinterBackend::backendType() const
{
    return BackendType::DefaultType;
}

void PrinterBackend::refresh()
{
}
