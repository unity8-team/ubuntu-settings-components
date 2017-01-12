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

#include "printers_global.h"

#include "enums.h"

#include <QObject>
#include <QString>
#include <QUrl>

class PRINTERS_DECL_EXPORT CupsFacade : public QObject
{
    Q_OBJECT
public:
    explicit CupsFacade(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~CupsFacade() {};
    virtual QString printerAdd(const QString &name,
                               const QUrl &uri,
                               const QUrl &ppdFile,
                               const QString &info,
                               const QString &location) = 0;

    virtual QString printerAddWithPpd(const QString &name,
                                      const QUrl &uri,
                                      const QString &ppdFileName,
                                      const QString &info,
                                      const QString &location) = 0;

    virtual QString printerDelete(const QString &name) = 0;

    virtual QString printerSetEnabled(const QString &name,
                                      const bool enabled) = 0;

    virtual QString printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason = QString::null) = 0;

    virtual QString printerSetInfo(const QString &name,
                                   const QString &info) = 0;

    virtual QString printerSetLocation(const QString &name,
                                       const QString &location) = 0;

    virtual QString printerSetShared(const QString &name,
                                     const bool shared) = 0;

    virtual QString printerSetJobSheets(const QString &name,
                                        const QString &start,
                                        const QString &end) = 0;

    virtual QString printerSetErrorPolicy(const QString &name,
                                          const ErrorPolicy &policy) = 0;

    virtual QString printerSetOpPolicy(const QString &name,
                                       const OperationPolicy &policy) = 0;

    virtual QString printerSetUsersAllowed(const QString &name,
                                           const QStringList &users) = 0;

    virtual QString printerSetUsersDenied(const QString &name,
                                          const QStringList &users) = 0;

    virtual QString printerAddOptionDefault(const QString &name,
                                            const QString &option,
                                            const QStringList &values) = 0;

    virtual QString printerDeleteOptionDefault(const QString &name,
                                               const QString &value) = 0;

    virtual QString printerAddOption(const QString &name,
                                     const QString &option,
                                     const QStringList &values) = 0;

Q_SIGNALS:
    void printerAdded(const QString &name);
    void printerModified(const QString &name, const bool ppdChanged);
    void printerDeleted(const QString &name);
    void printerStateChanged(const QString &name);
};

#endif // USC_PRINTERS_CUPSFACADE_H
