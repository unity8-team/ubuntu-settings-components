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

#ifndef USC_PRINTERS_CUPSFACADE_IMPL_H
#define USC_PRINTERS_CUPSFACADE_IMPL_H

#include "cups/cupsfacade.h"
#include "cups/cupspkhelper.h"

class CupsFacadeImpl : public CupsFacade
{
    Q_OBJECT
public:
    explicit CupsFacadeImpl(QObject *parent = nullptr);
    virtual ~CupsFacadeImpl() override;
    virtual QString printerAdd(const QString &name,
                               const QUrl &uri,
                               const QUrl &ppdFile,
                               const QString &info,
                               const QString &location) override;

    virtual QString printerAddWithPpd(const QString &name,
                                      const QUrl &uri,
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

    virtual cups_dest_t* makeDest(const QString &name,
                                  const PrinterJob *options) override;

    virtual int printFileToDest(const QString &filepath,
                                const QString &title,
                                const cups_dest_t *dest) override;
private:
    CupsPkHelper helper;
};

#endif // USC_PRINTERS_CUPSFACADE_IMPL_H
