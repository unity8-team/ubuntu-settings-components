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

#ifndef USC_PRINTERS_CUPS_BACKEND_H
#define USC_PRINTERS_CUPS_BACKEND_H

#include "backend/backend.h"
#include "cups/cupsfacade.h"
#include "cupsdnotifier.h" // Note: this file was generated.

#include <QPrinterInfo>

#define CUPSD_NOTIFIER_DBUS_PATH "/org/cups/cupsd/Notifier"

class PRINTERS_DECL_EXPORT PrinterCupsBackend : public PrinterBackend
{
    Q_OBJECT
public:
    explicit PrinterCupsBackend(QObject *parent = Q_NULLPTR);
    explicit PrinterCupsBackend(CupsFacade *cups, QPrinterInfo info,
                                OrgCupsCupsdNotifierInterface* notifier,
                                QObject *parent = Q_NULLPTR);
    virtual ~PrinterCupsBackend() override;

    virtual bool holdsDefinition() const override;

    virtual QString printerAdd(const QString &name,
                               const QString &uri,
                               const QString &ppdFile,
                               const QString &info,
                               const QString &location) override;
    virtual QString printerAddWithPpd(const QString &name,
                                      const QString &uri,
                                      const QString &ppdFileName,
                                      const QString &info,
                                      const QString &location) override;
    virtual QString printerDelete(const QString &name) override;
    virtual QString printerSetEnabled(const QString &name,
                                      const bool enabled) override;
    virtual QString printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason = QString::null) override;
    virtual QString printerSetInfo(const QString &name,
                                   const QString &info) override;
    virtual QString printerSetLocation(const QString &name,
                                       const QString &location) override;
    virtual QString printerSetShared(const QString &name,
                                     const bool shared) override;
    virtual QString printerSetJobSheets(const QString &name,
                                        const QString &start,
                                        const QString &end) override;
    virtual QString printerSetErrorPolicy(const QString &name,
                                          const PrinterEnum::ErrorPolicy &policy) override;

    virtual QString printerSetOpPolicy(const QString &name,
                                       const PrinterEnum::OperationPolicy &policy) override;
    virtual QString printerSetUsersAllowed(const QString &name,
                                           const QStringList &users) override;
    virtual QString printerSetUsersDenied(const QString &name,
                                          const QStringList &users) override;
    virtual QString printerAddOptionDefault(const QString &name,
                                            const QString &option,
                                            const QStringList &values) override;
    virtual QString printerDeleteOptionDefault(const QString &name,
                                               const QString &value) override;
    virtual QString printerAddOption(const QString &name,
                                     const QString &option,
                                     const QStringList &values) override;

    // TODO: const for both these getters (if possible)!
    virtual QVariant printerGetOption(const QString &name,
                                      const QString &option) const override;
    virtual QMap<QString, QVariant> printerGetOptions(
        const QString &name, const QStringList &options
    ) override;
    // FIXME: maybe have a PrinterDest iface that has a CupsDest impl?
    virtual cups_dest_t* makeDest(const QString &name,
                                  const PrinterJob *options) override;

    virtual QList<ColorModel> printerGetSupportedColorModels(
        const QString &name) const override;
    virtual ColorModel printerGetDefaultColorModel(const QString &name) const;
    virtual QList<PrintQuality> printerGetSupportedQualities(
        const QString &name) const override;
    virtual PrintQuality printerGetDefaultQuality(const QString &name) const;

    virtual void cancelJob(const QString &name, const int jobId) override;
    virtual int printFileToDest(const QString &filepath,
                                const QString &title,
                                const cups_dest_t *dest) override;
    virtual QList<QSharedPointer<PrinterJob>> printerGetJobs(const QString &name) override;

    virtual QString printerName() const override;
    virtual QString description() const override;
    virtual QString location() const override;
    virtual QString makeAndModel() const override;

    virtual PrinterEnum::State state() const override;
    virtual QList<QPageSize> supportedPageSizes() const override;
    virtual QPageSize defaultPageSize() const override;
    virtual bool supportsCustomPageSizes() const override;

    virtual QPageSize minimumPhysicalPageSize() const override;
    virtual QPageSize maximumPhysicalPageSize() const override;
    virtual QList<int> supportedResolutions() const override;
    virtual PrinterEnum::DuplexMode defaultDuplexMode() const override;
    virtual QList<PrinterEnum::DuplexMode> supportedDuplexModes() const override;

    virtual QList<QSharedPointer<Printer>> availablePrinters() override;
    virtual QStringList availablePrinterNames() override;
    virtual QSharedPointer<Printer> getPrinter(const QString &printerName) override;
    virtual QString defaultPrinterName() override;
    virtual void requestAvailablePrinters() override;
    virtual void requestAvailablePrinterDrivers() override;

public Q_SLOTS:
    virtual void refresh() override;
    void createSubscription();

Q_SIGNALS:
    void cancelWorkers();

private:
    void cancelSubscription();
    CupsFacade *m_cups;
    QPrinterInfo m_info;
    OrgCupsCupsdNotifierInterface *m_notifier;
    int m_cupsSubscriptionId = -1;
};

class PrintersLoader : public QObject
{
    Q_OBJECT
    CupsFacade *m_cups;
    OrgCupsCupsdNotifierInterface *m_notifier;
    bool m_running = false;
public:
    explicit PrintersLoader(CupsFacade *cups,
                            OrgCupsCupsdNotifierInterface* notifier,
                            QObject *parent = Q_NULLPTR);
    ~PrintersLoader();

public Q_SLOTS:
    void load();
    void cancel();

Q_SIGNALS:
    void finished();
    void loaded(QList<QSharedPointer<Printer>> printers);
};

#endif // USC_PRINTERS_CUPS_BACKEND_H
