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

#ifndef USC_PRINTERS_BACKEND_H
#define USC_PRINTERS_BACKEND_H

#include "printer/printer.h"
#include "printer/printerjob.h"

// TODO: remove cups specific things from this API
#include <cups/cups.h>

#include <QObject>
#include <QPageSize>
#include <QList>
#include <QString>
#include <QStringList>

class Printer;
class PrinterJob;
class PRINTERS_DECL_EXPORT PrinterBackend : public QObject
{
    Q_OBJECT
public:
    explicit PrinterBackend(QObject *parent = Q_NULLPTR);
    explicit PrinterBackend(const QString &printerName,
                            QObject *parent = Q_NULLPTR);
    virtual ~PrinterBackend();

    enum class BackendType
    {
        DefaultType = 0,
        CupsType,
        PdfType,
    };
    Q_ENUM(BackendType)

    virtual bool holdsDefinition() const;

    // Add a printer using an already existing ppd.
    virtual QString printerAdd(const QString &name,
                               const QString &uri,
                               const QString &ppdFile,
                               const QString &info,
                               const QString &location);

    // Add a printer and provide a ppd file.
    virtual QString printerAddWithPpd(const QString &name,
                                      const QString &uri,
                                      const QString &ppdFileName,
                                      const QString &info,
                                      const QString &location);
    virtual QString printerDelete(const QString &name);
    virtual QString printerSetEnabled(const QString &name,
                                      const bool enabled);
    virtual QString printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason = QString::null);
    virtual QString printerSetInfo(const QString &name,
                                   const QString &info);
    virtual QString printerSetLocation(const QString &name,
                                       const QString &location);
    virtual QString printerSetShared(const QString &name,
                                     const bool shared);
    virtual QString printerSetJobSheets(const QString &name,
                                        const QString &start,
                                        const QString &end);
    virtual QString printerSetErrorPolicy(const QString &name,
                                          const PrinterEnum::ErrorPolicy &policy);

    virtual QString printerSetOpPolicy(const QString &name,
                                       const PrinterEnum::OperationPolicy &policy);
    virtual QString printerSetUsersAllowed(const QString &name,
                                           const QStringList &users);
    virtual QString printerSetUsersDenied(const QString &name,
                                          const QStringList &users);
    virtual QString printerAddOptionDefault(const QString &name,
                                            const QString &option,
                                            const QStringList &values);
    virtual QString printerDeleteOptionDefault(const QString &name,
                                               const QString &value);
    virtual QString printerAddOption(const QString &name,
                                     const QString &option,
                                     const QStringList &values);

    // TODO: const for both these getters (if possible)!
    virtual QVariant printerGetOption(const QString &name,
                                      const QString &option) const;
    virtual QMap<QString, QVariant> printerGetOptions(
        const QString &name, const QStringList &options
    );
    // FIXME: maybe have a PrinterDest iface that has a CupsDest impl?
    virtual cups_dest_t* makeDest(const QString &name,
                                  const PrinterJob *options);

    virtual QList<ColorModel> printerGetSupportedColorModels(
        const QString &name) const;
    virtual ColorModel printerGetDefaultColorModel(const QString &name) const;
    virtual QList<PrintQuality> printerGetSupportedQualities(
        const QString &name) const;
    virtual PrintQuality printerGetDefaultQuality(const QString &name) const;
    virtual int printFileToDest(const QString &filepath,
                                const QString &title,
                                const cups_dest_t *dest);

    virtual QString printerName() const;
    virtual QString description() const;
    virtual QString location() const;
    virtual QString makeAndModel() const;

    virtual PrinterEnum::State state() const;
    virtual QList<QPageSize> supportedPageSizes() const;
    virtual QPageSize defaultPageSize() const;
    virtual bool supportsCustomPageSizes() const;

    virtual QPageSize minimumPhysicalPageSize() const;
    virtual QPageSize maximumPhysicalPageSize() const;
    virtual QList<int> supportedResolutions() const;
    virtual PrinterEnum::DuplexMode defaultDuplexMode() const;
    virtual QList<PrinterEnum::DuplexMode> supportedDuplexModes() const;

    virtual QList<Printer*> availablePrinters();
    virtual QStringList availablePrinterNames();
    virtual Printer* getPrinter(const QString &printerName);
    virtual QString defaultPrinterName();

    virtual BackendType backendType() const;

public Q_SLOTS:
    virtual void refresh();

Q_SIGNALS:
    void printerAdded(const QString &name);
    void printerModified(const QString &name, const bool ppdChanged);
    void printerDeleted(const QString &name);
    void printerStateChanged(const QString &name);

protected:
    const QString m_printerName;
};

#endif // USC_PRINTERS_BACKEND_H
