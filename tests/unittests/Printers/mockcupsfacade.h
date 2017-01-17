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

#ifndef USC_PRINTERS_MOCKCUPSFACADE_H
#define USC_PRINTERS_MOCKCUPSFACADE_H

#include "cups/cupsfacade.h"

#include <QMap>

class MockCupsFacade : public CupsFacade
{
    Q_OBJECT
public:
    virtual ~MockCupsFacade() override {}
    virtual QString printerAdd(const QString &name,
                               const QUrl &uri,
                               const QUrl &ppdFile,
                               const QString &info,
                               const QString &location) override
    {
        Q_UNUSED(name);
        Q_UNUSED(uri);
        Q_UNUSED(ppdFile);
        Q_UNUSED(info);
        Q_UNUSED(location);
        return returnValue;
    }

    virtual QString printerAddWithPpd(const QString &name,
                                      const QUrl &uri,
                                      const QString &ppdFileName,
                                      const QString &info,
                                      const QString &location) override
    {
        Q_UNUSED(name);
        Q_UNUSED(uri);
        Q_UNUSED(ppdFileName);
        Q_UNUSED(info);
        Q_UNUSED(location);
        return returnValue;
    }

    virtual QString printerDelete(const QString &name) override
    {
        Q_UNUSED(name);
        return returnValue;
    }

    virtual QString printerSetEnabled(const QString &name,
                                      const bool enabled) override
    {
        enableds.insert(name, enabled);
        return returnValue;

    }

    virtual QString printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason = QString::null) override
    {
        Q_UNUSED(name);
        return returnValue;

    }

    virtual QString printerSetInfo(const QString &name,
                                   const QString &info) override
    {
        infos.insert(name, info);
        return returnValue;

    }

    virtual QString printerSetLocation(const QString &name,
                                       const QString &location) override
    {
        locations.insert(name, location);
        return returnValue;

    }

    virtual QString printerSetShared(const QString &name,
                                     const bool shared) override
    {
        shareds.insert(name, shared);
        return returnValue;

    }

    virtual QString printerSetJobSheets(const QString &name,
                                        const QString &start,
                                        const QString &end) override
    {
        Q_UNUSED(name);
        Q_UNUSED(start);
        Q_UNUSED(end);
        return returnValue;

    }

    virtual QString printerSetErrorPolicy(const QString &name,
                                          const PrinterEnum::ErrorPolicy &policy) override
    {
        errorPolicies.insert(name, policy);
        return returnValue;

    }

    virtual QString printerSetOpPolicy(const QString &name,
                                       const PrinterEnum::OperationPolicy &policy) override
    {
        operationPolicies.insert(name, policy);
        return returnValue;

    }

    virtual QString printerSetUsersAllowed(const QString &name,
                                           const QStringList &users) override
    {
        printerOptions[name].insert("users-allowed", QVariant::fromValue(users));
        return returnValue;

    }

    virtual QString printerSetUsersDenied(const QString &name,
                                          const QStringList &users) override
    {
        printerOptions[name].insert("users-denied", QVariant::fromValue(users));
        return returnValue;

    }

    virtual QString printerAddOptionDefault(const QString &name,
                                            const QString &option,
                                            const QStringList &values) override
    {
        Q_UNUSED(name);
        Q_UNUSED(option);
        Q_UNUSED(values);
        return returnValue;
    }

    virtual QString printerDeleteOptionDefault(const QString &name,
                                               const QString &value) override
    {
        Q_UNUSED(name);
        Q_UNUSED(value);
        return returnValue;

    }

    virtual QString printerAddOption(const QString &name,
                                     const QString &option,
                                     const QStringList &values) override
    {
        printerOptions[name].insert(option, values);
        return returnValue;
    }

    virtual QVariant printerGetOption(const QString &name,
                                     const QString &option) override
    {
        // FIXME: if we're to return a variant, check that it can be converted
        return printerOptions[name].value(option);
    }

    virtual QMap<QString, QVariant> printerGetOptions(
        const QString &name, const QStringList &options) override
    {
        QMap<QString, QVariant> opts;
        Q_FOREACH(const QString &option, options) {
            opts[option] = printerGetOption(name, option);
        }
        return opts;
    }

    virtual QList<ColorModel> printerGetSupportedColorModels(
        const QString &name) const override
    {
        return printerOptions[name].value("ColorModels").value<QList<ColorModel>>();
    }

    virtual cups_dest_t* makeDest(const QString &name,
                                  const PrinterJob *options) override
    {

    }

    virtual int printFileToDest(const QString &filepath,
                                const QString &title,
                                const cups_dest_t *dest) override
    {

    }

    void mockPrinterAdded(const QString &name)
    {
        Q_EMIT printerAdded(name);
    }

    void mockPrinterModified(const QString &name, const bool ppdChanged)
    {
        Q_EMIT printerModified(name, ppdChanged);
    }

    void mockPrinterDeleted(const QString &name)
    {
        Q_EMIT printerDeleted(name);
    }

    void mockPrinterStateChanged(const QString &name)
    {
        Q_EMIT printerStateChanged(name);
    }

    QString returnValue = QString::null;

    // Map from printer to key/val.
    QMap<QString, QMap<QString, QVariant>> printerOptions;

    QMap<QString, bool> enableds;
    QMap<QString, bool> shareds;
    QMap<QString, QString> infos;
    QMap<QString, QString> locations;
    QMap<QString, PrinterEnum::ErrorPolicy> errorPolicies;
    QMap<QString, PrinterEnum::OperationPolicy> operationPolicies;
};

#endif // USC_PRINTERS_MOCKCUPSFACADE_H
