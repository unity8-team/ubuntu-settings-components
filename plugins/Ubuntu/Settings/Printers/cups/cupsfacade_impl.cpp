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

#define __CUPS_ADD_OPTION(dest, name, value) dest->num_options = \
    cupsAddOption(name, value, dest->num_options, &dest->options);

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
                                              const PrinterEnum::ErrorPolicy &policy)
{

}

QString CupsFacadeImpl::printerSetOpPolicy(const QString &name,
                                           const PrinterEnum::OperationPolicy &policy)
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
    if (!helper.printerClassSetOption(name, option, values)) {
        return helper.getLastError();
    }

    Q_EMIT printerModified(name, true);
    return QString();
}

cups_dest_t* CupsFacadeImpl::makeDest(const QString &name,
                                      const PrinterJob *options)
{
    // Extract the cups name and instance from the m_name
    QStringList split = name.split("/");

    QString cups_name = split.takeFirst();
    QString cups_instance = split.isEmpty() ? "" : split.takeFirst();

    // Get the cups dest
    cups_dest_t *dest = cupsGetNamedDest(CUPS_HTTP_DEFAULT,
                                         cups_name.toLocal8Bit().data(),
                                         cups_instance.toLocal8Bit().data());

    if (options->copies() > 1) {
        __CUPS_ADD_OPTION(dest, "copies", QString::number(options->copies()).toLocal8Bit());
    }

    // FIXME: needs to know correct color models of printer?
    // is this a reason for converting PrinterJob to QMap in Printer?
    switch (options->colorMode()) {
    case PrinterEnum::ColorModel::ColorMode:
        __CUPS_ADD_OPTION(dest, "ColorModel", "RGB");
        break;
    case PrinterEnum::ColorModel::GrayscaleMode:
    default:
        __CUPS_ADD_OPTION(dest, "ColorModel", "KGray");
        break;
    }

    if (options->duplex()) {
        __CUPS_ADD_OPTION(dest, "Duplex", "DuplexAuto");
    } else {
        __CUPS_ADD_OPTION(dest, "Duplex", "DuplexNone");
    }

    if (options->landscape()) {
        __CUPS_ADD_OPTION(dest, "landscape", "");
    }

    if (options->printRangeMode() == PrinterEnum::PrintRange::PageRange
            && !options->printRange().isEmpty()) {
        __CUPS_ADD_OPTION(dest, "page-ranges", options->printRange().toLocal8Bit().data());
    }

    QString printQuality = "";
    switch (options->quality()) {
    case PrinterEnum::Quality::DraftQuality:
        printQuality = "FastDraft";
        break;
    case PrinterEnum::Quality::BestQuality:
        printQuality = "Best";
        break;
    case PrinterEnum::Quality::PhotoQuality:
        printQuality = "Photo";
        break;
    case PrinterEnum::Quality::NormalQuality:
    default:
        printQuality = "Normal";
        break;
    }
    __CUPS_ADD_OPTION(dest, "OutputMode", printQuality.toLocal8Bit());

    return dest;
}

int CupsFacadeImpl::printFileToDest(const QString &filepath,
                                    const QString &title,
                                    const cups_dest_t *dest)
{
    qDebug() << "Printing:" << filepath << title << dest->name << dest->num_options;
    return cupsPrintFile(dest->name,
                         filepath.toLocal8Bit().data(),
                         title.toLocal8Bit().data(),
                         dest->num_options,
                         dest->options);
}
