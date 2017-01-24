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

#include "cups/cupsfacade.h"

#include <cups/http.h>
#include <cups/ipp.h>
#include <cups/ppd.h>

#include <QDebug>

#define __CUPS_ADD_OPTION(dest, name, value) dest->num_options = \
    cupsAddOption(name, value, dest->num_options, &dest->options);

CupsFacade::CupsFacade(QObject *parent) : QObject(parent)
{
}

CupsFacade::~CupsFacade()
{

}

QString CupsFacade::printerAdd(const QString &name,
                               const QString &uri,
                               const QString &ppdFile,
                               const QString &info,
                               const QString &location)
{
    if (!helper.printerAdd(name, uri, ppdFile, info, location)) {
        return helper.getLastError();
    }
    return QString();
}

QString CupsFacade::printerAddWithPpd(const QString &name,
                                      const QString &uri,
                                      const QString &ppdFileName,
                                      const QString &info,
                                      const QString &location)
{
    if (!helper.printerAddWithPpdFile(name, uri, ppdFileName, info, location)) {
        return helper.getLastError();
    }
    return QString();
}

QString CupsFacade::printerDelete(const QString &name)
{
    Q_UNUSED(name);
    return QString();
}

QString CupsFacade::printerSetEnabled(const QString &name, const bool enabled)
{
    Q_UNUSED(name);
    Q_UNUSED(enabled);
    return QString();
}

QString CupsFacade::printerSetAcceptJobs(
        const QString &name,
        const bool enabled,
        const QString &reason)
{
    Q_UNUSED(name);
    Q_UNUSED(enabled);
    Q_UNUSED(reason);
    return QString();
}

QString CupsFacade::printerSetInfo(const QString &name, const QString &info)
{
    if (!helper.printerClassSetInfo(name, info)) {
        return helper.getLastError();
    }
    return QString();
}

QString CupsFacade::printerSetLocation(const QString &name,
                                       const QString &location)
{
    Q_UNUSED(name);
    Q_UNUSED(location);
    return QString();
}

QString CupsFacade::printerSetShared(const QString &name, const bool shared)
{
    Q_UNUSED(name);
    Q_UNUSED(shared);
    return QString();
}

QString CupsFacade::printerSetJobSheets(const QString &name,
                                        const QString &start,
                                        const QString &end)
{
    Q_UNUSED(name);
    Q_UNUSED(start);
    Q_UNUSED(end);
    return QString();
}

QString CupsFacade::printerSetErrorPolicy(const QString &name,
                                          const PrinterEnum::ErrorPolicy &policy)
{
    Q_UNUSED(name);
    Q_UNUSED(policy);
    return QString();
}

QString CupsFacade::printerSetOpPolicy(const QString &name,
                                       const PrinterEnum::OperationPolicy &policy)
{
    Q_UNUSED(name);
    Q_UNUSED(policy);
    return QString();
}

QString CupsFacade::printerSetUsersAllowed(const QString &name,
                                           const QStringList &users)
{
    Q_UNUSED(name);
    Q_UNUSED(users);
    return QString();
}

QString CupsFacade::printerSetUsersDenied(const QString &name,
                                          const QStringList &users)
{
    Q_UNUSED(name);
    Q_UNUSED(users);
    return QString();
}

QString CupsFacade::printerAddOptionDefault(const QString &name,
                                            const QString &option,
                                            const QStringList &values)
{
    Q_UNUSED(name);
    Q_UNUSED(option);
    Q_UNUSED(values);
    return QString();
}

QString CupsFacade::printerDeleteOptionDefault(const QString &name,
                                               const QString &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
    return QString();
}

QString CupsFacade::printerAddOption(const QString &name,
                                     const QString &option,
                                     const QStringList &values)
{
    if (!helper.printerClassSetOption(name, option, values)) {
        return helper.getLastError();
    }

    Q_EMIT printerModified(name, true);
    return QString();
}

QVariant CupsFacade::printerGetOption(const QString &name,
                                      const QString &option)
{
    QStringList opts({option});
    auto res = printerGetOptions(name, opts);
    return res[option];
}

QMap<QString, QVariant> CupsFacade::printerGetOptions(
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
            ppd_option_t *ppdColorModel = ppdFindOption(ppd, "ColorModel");
            if (ppdColorModel) {
                ppd_choice_t* def = ppdFindChoice(ppdColorModel,
                                                  ppdColorModel->defchoice);
                if (def) {
                    model = Utils::parsePpdColorModel(def->choice,
                                                      def->text,
                                                      "ColorModel");
                }
            }
            ret[option] = QVariant::fromValue(model);
        } else if (option == "DefaultPrintQuality") {
            PrintQuality quality;
            Q_FOREACH(const QString opt, m_knownQualityOptions) {
                ppd_option_t *ppdQuality = ppdFindOption(ppd, opt.toUtf8());
                if (ppdQuality) {
                    ppd_choice_t* def = ppdFindChoice(ppdQuality,
                                                      ppdQuality->defchoice);
                    if (def) {
                        quality = Utils::parsePpdPrintQuality(def->choice,
                                                              def->text, opt);
                    }
                }
            }
            ret[option] = QVariant::fromValue(quality);
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

QList<ColorModel> CupsFacade::printerGetSupportedColorModels(
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
        for (int i = 0; i < colorModels->num_choices; ++i) {
            ret.append(Utils::parsePpdColorModel(colorModels->choices[i].choice,
                                                 colorModels->choices[i].text,
                                                 "ColorModel"));
        }
    }

    ppdClose(ppd);
    return ret;
}

QList<PrintQuality> CupsFacade::printerGetSupportedQualities(
    const QString &name) const
{
    QList<PrintQuality> ret;
    ppd_file_t* ppd;

    ppd = helper.getPpdFile(getPrinterName(name), getPrinterInstance(name));
    if (!ppd) {
        qCritical() << "Could not get PPD for" << name;
        return ret;
    }

    Q_FOREACH(const QString &opt, m_knownQualityOptions) {
        ppd_option_t *qualityOpt = ppdFindOption(ppd, opt.toUtf8());
        if (qualityOpt) {
            for (int i = 0; i < qualityOpt->num_choices; ++i)
                ret.append(Utils::parsePpdPrintQuality(qualityOpt->choices[i].choice,
                                                       qualityOpt->choices[i].text,
                                                       opt));
        }
    }

    ppdClose(ppd);
    return ret;
}

QString CupsFacade::getPrinterName(const QString &name) const
{
    const auto parts = name.splitRef(QLatin1Char('/'));
    return parts.at(0).toString();
}

QString CupsFacade::getPrinterInstance(const QString &name) const
{
    const auto parts = name.splitRef(QLatin1Char('/'));
    QString instance;
    if (parts.size() > 1)
        instance = parts.at(1).toString();

    return instance;
}

cups_dest_t* CupsFacade::makeDest(const QString &name,
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

    PrintQuality quality = options->getPrintQuality();
    __CUPS_ADD_OPTION(dest, quality.originalOption.toLocal8Bit(),
                      quality.name.toLocal8Bit());

    return dest;
}

int CupsFacade::printFileToDest(const QString &filepath, const QString &title,
                                const cups_dest_t *dest)
{
    qDebug() << "Printing:" << filepath << title << dest->name << dest->num_options;
    return cupsPrintFile(dest->name,
                         filepath.toLocal8Bit(),
                         title.toLocal8Bit(),
                         dest->num_options,
                         dest->options);
}
