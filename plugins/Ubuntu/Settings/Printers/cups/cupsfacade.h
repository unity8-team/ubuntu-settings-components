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

#ifndef USC_PRINTERS_CUPSFACADE_H
#define USC_PRINTERS_CUPSFACADE_H

#include "enums.h"
#include "structs.h"

#include "cups/cupspkhelper.h"
#include "printer/printerjob.h"

#include <cups/cups.h>

#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

class PrinterJob;
class CupsFacade : public QObject
{
    Q_OBJECT
public:
    explicit CupsFacade(QObject *parent = Q_NULLPTR);
    ~CupsFacade();
    QString printerAdd(const QString &name,
                       const QString &uri,
                       const QString &ppdFile,
                       const QString &info,
                       const QString &location);
    QString printerAddWithPpd(const QString &name,
                              const QString &uri,
                              const QString &ppdFileName,
                              const QString &info,
                              const QString &location);
    QString printerDelete(const QString &name);
    QString printerSetEnabled(const QString &name, const bool enabled);
    QString printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason = QString::null);
    QString printerSetInfo(const QString &name, const QString &info);
    QString printerSetLocation(const QString &name, const QString &location);
    QString printerSetShared(const QString &name, const bool shared);
    QString printerSetJobSheets(const QString &name, const QString &start,
                                const QString &end);
    QString printerSetErrorPolicy(const QString &name,
                                  const PrinterEnum::ErrorPolicy &policy);

    QString printerSetOpPolicy(const QString &name,
                               const PrinterEnum::OperationPolicy &policy);
    QString printerSetUsersAllowed(const QString &name,
                                   const QStringList &users);
    QString printerSetUsersDenied(const QString &name,
                                  const QStringList &users);
    QString printerAddOptionDefault(const QString &name,
                                    const QString &option,
                                    const QStringList &values);
    QString printerDeleteOptionDefault(const QString &name,
                                       const QString &value);
    QString printerAddOption(const QString &name,
                             const QString &option,
                             const QStringList &values);

    // TODO: const for both these getters (if possible)!
    QVariant printerGetOption(const QString &name, const QString &option);
    QMap<QString, QVariant> printerGetOptions(const QString &name,
                                              const QStringList &options);
    // FIXME: maybe have a PrinterDest iface that has a CupsDest impl?
    cups_dest_t* makeDest(const QString &name, const PrinterJob *options);

    QList<ColorModel> printerGetSupportedColorModels(const QString &name) const;
    QList<PrintQuality> printerGetSupportedQualities(const QString &name) const;
    int printFileToDest(const QString &filepath, const QString &title,
                        const cups_dest_t *dest);
    QList<PrinterDriver> getPrinterDrivers(
        const QString &deviceId = "",
        const QString &language = "",
        const QString &makeModel = "",
        const QString &product = "",
        const QStringList &includeSchemes = QStringList(),
        const QStringList &excludeSchemes = QStringList()
    );

Q_SIGNALS:
    void printerAdded(const QString &name);
    void printerModified(const QString &name, const bool ppdChanged);
    void printerDeleted(const QString &name);
    void printerStateChanged(const QString &name);

private:
    QString getPrinterName(const QString &name) const;
    QString getPrinterInstance(const QString &name) const;
    QStringList parsePpdColorModel(const QString &colorModel);
    const QStringList m_knownQualityOptions = QStringList({
        "Quality", "PrintQuality", "HPPrintQuality", "StpQuality",
        "OutputMode",
    });
    CupsPkHelper helper;
};

#endif // USC_PRINTERS_CUPSFACADE_H
