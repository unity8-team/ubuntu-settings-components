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

#include "utils.h"

#include "cups/cupsfacade_impl.h"

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
    Q_UNUSED(name);
    Q_UNUSED(uri);
    Q_UNUSED(ppdFile);
    Q_UNUSED(info);
    Q_UNUSED(location);
    return QString();
}

QString CupsFacadeImpl::printerAddWithPpd(const QString &name,
                                          const QUrl &uri,
                                          const QString &ppdFileName,
                                          const QString &info,
                                          const QString &location)
{
    Q_UNUSED(name);
    Q_UNUSED(uri);
    Q_UNUSED(ppdFileName);
    Q_UNUSED(info);
    Q_UNUSED(location);
    return QString();
}

QString CupsFacadeImpl::printerDelete(const QString &name)
{
    Q_UNUSED(name);
    return QString();
}

QString CupsFacadeImpl::printerSetEnabled(const QString &name,
                                          const bool enabled)
{
    Q_UNUSED(name);
    Q_UNUSED(enabled);
    return QString();
}

QString CupsFacadeImpl::printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason)
{
    Q_UNUSED(name);
    Q_UNUSED(enabled);
    Q_UNUSED(reason);
    return QString();
}

QString CupsFacadeImpl::printerSetInfo(const QString &name,
                                       const QString &info)
{
    if (!helper.printerClassSetInfo(name, info)) {
        return helper.getLastError();
    }
    return QString();
}

QString CupsFacadeImpl::printerSetLocation(const QString &name,
                                           const QString &location)
{
    Q_UNUSED(name);
    Q_UNUSED(location);
    return QString();
}

QString CupsFacadeImpl::printerSetShared(const QString &name,
                                         const bool shared)
{
    Q_UNUSED(name);
    Q_UNUSED(shared);
    return QString();
}

QString CupsFacadeImpl::printerSetJobSheets(const QString &name,
                                            const QString &start,
                                            const QString &end)
{
    Q_UNUSED(name);
    Q_UNUSED(start);
    Q_UNUSED(end);
    return QString();
}

QString CupsFacadeImpl::printerSetErrorPolicy(const QString &name,
                                              const PrinterEnum::ErrorPolicy &policy)
{
    Q_UNUSED(name);
    Q_UNUSED(policy);
    return QString();
}

QString CupsFacadeImpl::printerSetOpPolicy(const QString &name,
                                           const PrinterEnum::OperationPolicy &policy)
{
    Q_UNUSED(name);
    Q_UNUSED(policy);
    return QString();
}

QString CupsFacadeImpl::printerSetUsersAllowed(const QString &name,
                                               const QStringList &users)
{
    Q_UNUSED(name);
    Q_UNUSED(users);
    return QString();
}

QString CupsFacadeImpl::printerSetUsersDenied(const QString &name,
                                              const QStringList &users)
{
    Q_UNUSED(name);
    Q_UNUSED(users);
    return QString();
}

QString CupsFacadeImpl::printerAddOptionDefault(const QString &name,
                                                const QString &option,
                                                const QStringList &values)
{
    Q_UNUSED(name);
    Q_UNUSED(option);
    Q_UNUSED(values);
    return QString();
}

QString CupsFacadeImpl::printerDeleteOptionDefault(const QString &name,
                                                   const QString &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
    return QString();
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

QVariant CupsFacadeImpl::printerGetOption(const QString &name,
                                          const QString &option)
{
    Q_UNUSED(name);
    Q_UNUSED(option);
    return QVariant();
}

QMap<QString, QVariant> CupsFacadeImpl::printerGetOptions(
    const QString &name, const QStringList &options)
{
    QMap<QString, QVariant> ret;

    QString printerName = getPrinterName(name);
    QString instance = getPrinterInstance(name);

    ppd_file_t* ppd;

    // We don't need a dest, really.
    cups_dest_t *dest = helper.getDest(printerName, instance);
    if (!dest) {
        qCritical() << "Could not get dest for" << printerName;
        return ret;
    }

    ppd = helper.getPpdFile(printerName, instance);
    if (!ppd) {
        qCritical() << "Could not get PPD for" << printerName;
        cupsFreeDests(1, dest);
        return ret;
    }

    Q_FOREACH(const QString &option, options) {
        if (option == "DefaultColorModel") {
            ColorModel model;

            ppd_option_t *defCModel = ppdFindOption(ppd, option.toUtf8());
            if (defCModel) {
                model = Utils::parsePpdColorModel(defCModel->choices[0].choice);
            } else {
                ppd_option_t *cModel = ppdFindOption(ppd, "ColorModel");
                if (cModel) {
                    model = Utils::parsePpdColorModel(cModel->defchoice);
                }
            }
            ret[option] = QVariant::fromValue(model);
        } else {
            ppd_option_t *val = ppdFindOption(ppd, option.toUtf8());

            if (val) {
                qWarning() << "asking for" << option << "returns" << val->text;
            } else {
                qWarning() << "option" << option << "yielded no option";
            }
        }
    }

    ppdClose(ppd);
    cupsFreeDests(1, dest);

    return ret;
}

QList<ColorModel> CupsFacadeImpl::printerGetSupportedColorModels(
    const QString &name) const
{
    QList<ColorModel> ret;
    ppd_file_t* ppd;

    ppd = helper.getPpdFile(getPrinterName(name), getPrinterInstance(name));
    if (!ppd) {
        qCritical() << "Could not get PPD for" << name;
        return ret;
    }

    ppd_option_t *colorModels = ppdFindOption(ppd, "ColorModel");
    if (colorModels) {
        for (int i = 0; i < colorModels->num_choices; ++i)
            ret.append(Utils::parsePpdColorModel(colorModels->choices[i].choice));
    }

    ppdClose(ppd);
    return ret;
}

QString CupsFacadeImpl::getPrinterName(const QString &name) const
{
    const auto parts = name.splitRef(QLatin1Char('/'));
    return parts.at(0).toString();
}

QString CupsFacadeImpl::getPrinterInstance(const QString &name) const
{
    const auto parts = name.splitRef(QLatin1Char('/'));
    QString instance;
    if (parts.size() > 1)
        instance = parts.at(1).toString();

    return instance;
}

cups_dest_t* CupsFacadeImpl::makeDest(const QString &name,
                                      const PrinterJob *options)
{
    // Get the cups dest
    cups_dest_t *dest = helper.getDest(getPrinterName(name), getPrinterInstance(name));

    if (options->copies() > 1) {
        __CUPS_ADD_OPTION(dest, "copies", QString::number(options->copies()).toLocal8Bit());
    }

    __CUPS_ADD_OPTION(dest, "ColorModel", options->getColorModel().name.toLocal8Bit());
    __CUPS_ADD_OPTION(dest, "Duplex", Utils::duplexModeToPpdChoice(options->getDuplexMode()).toLocal8Bit());

    if (options->landscape()) {
        __CUPS_ADD_OPTION(dest, "landscape", "");
    }

    if (options->printRangeMode() == PrinterEnum::PrintRange::PageRange
            && !options->printRange().isEmpty()) {
        __CUPS_ADD_OPTION(dest, "page-ranges", options->printRange().toLocal8Bit());
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
                         filepath.toLocal8Bit(),
                         title.toLocal8Bit(),
                         dest->num_options,
                         dest->options);
}
