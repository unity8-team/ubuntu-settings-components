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
#include "backend/backend_pdf.h"
#include "i18n.h"
#include "utils.h"

#include <QDBusConnection>

PrinterCupsBackend::PrinterCupsBackend(QObject *parent)
    : PrinterCupsBackend(new CupsFacade(), QPrinterInfo(),
                         new OrgCupsCupsdNotifierInterface("",
                                                           CUPSD_NOTIFIER_DBUS_PATH,
                                                           QDBusConnection::systemBus()),
                         parent)
{
    // Use proper RAII of things we create:
    m_cups->setParent(this);
    m_notifier->setParent(this);
}

PrinterCupsBackend::PrinterCupsBackend(CupsFacade *cups, QPrinterInfo info,
                                       OrgCupsCupsdNotifierInterface *notifier,
                                       QObject *parent)
    : PrinterBackend(info.printerName(), parent)
    , m_cups(cups)
    , m_info(info)
    , m_notifier(notifier)
{
    connect(m_cups, SIGNAL(printerDriversLoaded(const QList<PrinterDriver>&)),
            this, SIGNAL(printerDriversLoaded(const QList<PrinterDriver>&)));
    connect(m_cups, SIGNAL(printerDriversFailedToLoad(const QString&)),
            this, SIGNAL(printerDriversFailedToLoad(const QString&)));

    connect(m_notifier, SIGNAL(PrinterAdded(const QString&, const QString&,
                                            const QString&, uint,
                                            const QString&, bool)),
            this, SIGNAL(printerAdded(const QString&, const QString&,
                                      const QString&, uint,
                                      const QString&, bool)));
    connect(m_notifier, SIGNAL(PrinterDeleted(const QString&, const QString&,
                                              const QString&, uint,
                                              const QString&, bool)),
            this, SIGNAL(printerDeleted(const QString&, const QString&,
                                        const QString&, uint,
                                        const QString&, bool)));
    connect(m_notifier, SIGNAL(PrinterModified(const QString&, const QString&,
                                               const QString&, uint,
                                               const QString&, bool)),
            this, SIGNAL(printerModified(const QString&, const QString&,
                                         const QString&, uint,
                                         const QString&, bool)));
}

PrinterCupsBackend::~PrinterCupsBackend()
{
    cancelSubscription();
}

QString PrinterCupsBackend::printerAdd(const QString &name,
                                       const QString &uri,
                                       const QString &ppdFile,
                                       const QString &info,
                                       const QString &location)
{
    return m_cups->printerAdd(name, uri, ppdFile, info, location);
}

QString PrinterCupsBackend::printerAddWithPpd(const QString &name,
                                              const QString &uri,
                                              const QString &ppdFileName,
                                              const QString &info,
                                              const QString &location)
{
    return m_cups->printerAddWithPpd(name, uri, ppdFileName, info, location);
}

bool PrinterCupsBackend::holdsDefinition() const
{
    return !m_info.isNull();
}

QString PrinterCupsBackend::printerDelete(const QString &name)
{

}

QString PrinterCupsBackend::printerSetEnabled(const QString &name,
                                              const bool enabled)
{

}

QString PrinterCupsBackend::printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason)
{

}

QString PrinterCupsBackend::printerSetInfo(const QString &name,
                                           const QString &info)
{
    return m_cups->printerSetInfo(name, info);
}

QString PrinterCupsBackend::printerSetLocation(const QString &name,
                                               const QString &location)
{
    return m_cups->printerSetLocation(name, location);
}

QString PrinterCupsBackend::printerSetShared(const QString &name,
                                             const bool shared)
{
    return m_cups->printerSetShared(name, shared);
}

QString PrinterCupsBackend::printerSetJobSheets(const QString &name,
                                                const QString &start,
                                                const QString &end)
{
    return m_cups->printerSetJobSheets(name, start, end);
}

QString PrinterCupsBackend::printerSetErrorPolicy(const QString &name,
                                                  const PrinterEnum::ErrorPolicy &policy)
{
    return m_cups->printerSetErrorPolicy(name, policy);
}

QString PrinterCupsBackend::printerSetOpPolicy(const QString &name,
                                               const PrinterEnum::OperationPolicy &policy)
{
    return m_cups->printerSetOpPolicy(name, policy);
}

QString PrinterCupsBackend::printerSetUsersAllowed(const QString &name,
                                                   const QStringList &users)
{
    return m_cups->printerSetUsersAllowed(name, users);
}

QString PrinterCupsBackend::printerSetUsersDenied(const QString &name,
                                                  const QStringList &users)
{
    return m_cups->printerSetUsersDenied(name, users);
}

QString PrinterCupsBackend::printerAddOptionDefault(const QString &name,
                                                    const QString &option,
                                                    const QStringList &values)
{
    return m_cups->printerAddOptionDefault(name, option, values);
}

QString PrinterCupsBackend::printerDeleteOptionDefault(const QString &name,
                                                       const QString &value)
{
    return m_cups->printerDeleteOptionDefault(name, value);
}

QString PrinterCupsBackend::printerAddOption(const QString &name,
                                             const QString &option,
                                             const QStringList &values)
{
    return m_cups->printerAddOption(name, option, values);
}

    // TODO: const for both these getters (if possible)!
QVariant PrinterCupsBackend::printerGetOption(const QString &name,
                                              const QString &option) const
{
    return m_cups->printerGetOption(name, option);
}
QMap<QString, QVariant> PrinterCupsBackend::printerGetOptions(
        const QString &name, const QStringList &options)
{
    return m_cups->printerGetOptions(name, options);
}

// FIXME: maybe have a PrinterDest iface that has a CupsDest impl?
cups_dest_t* PrinterCupsBackend::makeDest(const QString &name,
                                          const PrinterJob *options)
{
    return m_cups->makeDest(name, options);
}

QList<ColorModel> PrinterCupsBackend::printerGetSupportedColorModels(
        const QString &name) const
{
    return m_cups->printerGetSupportedColorModels(name);
}

ColorModel PrinterCupsBackend::printerGetDefaultColorModel(
    const QString &name) const
{
    return printerGetOption(name, "DefaultColorModel").value<ColorModel>();
}

QList<PrintQuality> PrinterCupsBackend::printerGetSupportedQualities(
        const QString &name) const
{
    return m_cups->printerGetSupportedQualities(name);
}

PrintQuality PrinterCupsBackend::printerGetDefaultQuality(
        const QString &name) const
{
    return printerGetOption(name, "DefaultPrintQuality").value<PrintQuality>();
}

int PrinterCupsBackend::printFileToDest(const QString &filepath,
                                        const QString &title,
                                        const cups_dest_t *dest)
{
    return m_cups->printFileToDest(filepath, title, dest);
}

QString PrinterCupsBackend::printerName() const
{
    return m_info.printerName();
}

QString PrinterCupsBackend::description() const
{
    return m_info.description();
}

QString PrinterCupsBackend::location() const
{

}

QString PrinterCupsBackend::makeAndModel() const
{

}

PrinterEnum::State PrinterCupsBackend::state() const
{
    switch (m_info.state()) {
        case QPrinter::Idle:
            return PrinterEnum::State::IdleState;
        case QPrinter::Active:
            return PrinterEnum::State::ActiveState;
        case QPrinter::Aborted:
            return PrinterEnum::State::AbortedState;
        case QPrinter::Error:
            return PrinterEnum::State::ErrorState;
    }
}

QList<QPageSize> PrinterCupsBackend::supportedPageSizes() const
{
    return m_info.supportedPageSizes();
}

QPageSize PrinterCupsBackend::defaultPageSize() const
{
    return m_info.defaultPageSize();
}

bool PrinterCupsBackend::supportsCustomPageSizes() const
{

}

QPageSize PrinterCupsBackend::minimumPhysicalPageSize() const
{

}

QPageSize PrinterCupsBackend::maximumPhysicalPageSize() const
{

}

QList<int> PrinterCupsBackend::supportedResolutions() const
{

}

PrinterEnum::DuplexMode PrinterCupsBackend::defaultDuplexMode() const
{
    return Utils::qDuplexModeToDuplexMode(m_info.defaultDuplexMode());
}

QList<PrinterEnum::DuplexMode> PrinterCupsBackend::supportedDuplexModes() const
{
    QList<PrinterEnum::DuplexMode> list;
    Q_FOREACH(const QPrinter::DuplexMode mode, m_info.supportedDuplexModes()) {
        if (mode != QPrinter::DuplexAuto) {
            list.append(Utils::qDuplexModeToDuplexMode(mode));
        }
    }
    return list;
}

QList<Printer*> PrinterCupsBackend::availablePrinters()
{
    QList<Printer*> list;

    // Use availablePrinterNames as this gives us a name for even null printers
    Q_FOREACH(QString name, QPrinterInfo::availablePrinterNames()) {
        QPrinterInfo info = QPrinterInfo::printerInfo(name);

        if (!info.isNull()) {
            list.append(new Printer(new PrinterCupsBackend(m_cups, info, m_notifier)));
        } else {
            qWarning() << "Printer is null so skipping (" << name << ")";
        }
    }

    // Cups allows a faux PDF printer.
    list.append(new Printer(new PrinterPdfBackend(__("Create PDF"))));

    return list;
}

QStringList PrinterCupsBackend::availablePrinterNames()
{
    return QPrinterInfo::availablePrinterNames();
}

Printer* PrinterCupsBackend::getPrinter(const QString &printerName)
{

}

QString PrinterCupsBackend::defaultPrinterName()
{
    return QPrinterInfo::defaultPrinterName();
}

void PrinterCupsBackend::requestAvailablePrinterDrivers()
{
    return m_cups->requestPrinterDrivers();
}

PrinterBackend::BackendType PrinterCupsBackend::backendType() const
{
    return PrinterBackend::BackendType::CupsType;
}

void PrinterCupsBackend::refresh()
{
    if (m_printerName.isEmpty()) {
        throw std::invalid_argument("Trying to refresh unnamed printer.");
    } else {
        m_info = QPrinterInfo::printerInfo(m_printerName);
    }
}

void PrinterCupsBackend::createSubscription()
{
    m_cupsSubscriptionId = m_cups->createSubscription();
    qWarning() << "cups subscription id" << m_cupsSubscriptionId;
}

void PrinterCupsBackend::renewSubscription()
{

}

void PrinterCupsBackend::cancelSubscription()
{
    if (m_cupsSubscriptionId > 0)
        m_cups->cancelSubscription(m_cupsSubscriptionId);
}
