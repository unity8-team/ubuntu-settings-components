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

#include "cups/cupsfacade_impl.h"

#include <cups/cups.h>
#include <cups/http.h>
#include <cups/ipp.h>
#include <cups/ppd.h>

#include <QDebug>

CupsFacadeImpl::CupsFacadeImpl(QObject *parent) : CupsFacade(parent)
{

}

CupsFacadeImpl::~CupsFacadeImpl()
{

}

QString CupsFacadeImpl::printerAdd(const QString &name,
                                   const QUrl &uri,
                                   const QUrl &ppdFile,
                                   const QString &info,
                                   const QString &location)
{

}

QString CupsFacadeImpl::printerAddWithPpd(const QString &name,
                                          const QUrl &uri,
                                          const QString &ppdFileName,
                                          const QString &info,
                                          const QString &location)
{

}

QString CupsFacadeImpl::printerDelete(const QString &name)
{

}

QString CupsFacadeImpl::printerSetEnabled(const QString &name,
                                          const bool enabled)
{

}

QString CupsFacadeImpl::printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason)
{

}

QString CupsFacadeImpl::printerSetInfo(const QString &name,
                                       const QString &info)
{
    helper.printerClassSetInfo(name, info);
}

QString CupsFacadeImpl::printerSetLocation(const QString &name,
                                           const QString &location)
{

}

QString CupsFacadeImpl::printerSetShared(const QString &name,
                                         const bool shared)
{

}

QString CupsFacadeImpl::printerSetJobSheets(const QString &name,
                                            const QString &start,
                                            const QString &end)
{

}

QString CupsFacadeImpl::printerSetErrorPolicy(const QString &name,
                                              const ErrorPolicy &policy)
{

}

QString CupsFacadeImpl::printerSetOpPolicy(const QString &name,
                                           const OperationPolicy &policy)
{

}

QString CupsFacadeImpl::printerSetUsersAllowed(const QString &name,
                                               const QStringList &users)
{

}

QString CupsFacadeImpl::printerSetUsersDenied(const QString &name,
                                              const QStringList &users)
{

}

QString CupsFacadeImpl::printerAddOptionDefault(const QString &name,
                                                const QString &option,
                                                const QStringList &values)
{
}

QString CupsFacadeImpl::printerDeleteOptionDefault(const QString &name,
                                                   const QString &value)
{

}

QString CupsFacadeImpl::printerAddOption(const QString &name,
                                         const QString &option,
                                         const QStringList &values)
{
    qWarning() << __PRETTY_FUNCTION__ << name << option << values;
    helper.printerClassSetOption(name, option, values);

}
