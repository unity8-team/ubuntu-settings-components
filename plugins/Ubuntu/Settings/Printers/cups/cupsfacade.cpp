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
#include <QThread>

#define __CUPS_ADD_OPTION(dest, name, value) dest->num_options = \
    cupsAddOption(name, value, dest->num_options, &dest->options);

CupsFacade::CupsFacade(QObject *parent) : QObject(parent)
{
}

CupsFacade::~CupsFacade()
{
    cancelPrinterDriverRequest();
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

    if (options->collate()) {
        __CUPS_ADD_OPTION(dest, "Collate", "True");
    } else {
        __CUPS_ADD_OPTION(dest, "Collate", "False");
    }

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

    if (options->reverse()) {
        __CUPS_ADD_OPTION(dest, "OutputOrder", "Reverse");
    } else {
        __CUPS_ADD_OPTION(dest, "OutputOrder", "Normal");
    }

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

void CupsFacade::requestPrinterDrivers(
    const QString &deviceId, const QString &language, const QString &makeModel,
    const QString &product, const QStringList &includeSchemes,
    const QStringList &excludeSchemes
)
{
    auto thread = new QThread;
    auto loader = new PrinterDriverLoader(deviceId, language, makeModel,
                                          product, includeSchemes,
                                          excludeSchemes);
    loader->moveToThread(thread);
    connect(loader, SIGNAL(error(const QString&)),
            this, SIGNAL(printerDriversFailedToLoad(const QString&)));
    connect(this, SIGNAL(requestPrinterDriverCancel()), loader, SLOT(cancel()));
    connect(thread, SIGNAL(started()), loader, SLOT(process()));
    connect(loader, SIGNAL(finished()), thread, SLOT(quit()));
    connect(loader, SIGNAL(finished()), loader, SLOT(deleteLater()));
    connect(loader, SIGNAL(loaded(const QList<PrinterDriver>&)),
            this, SIGNAL(printerDriversLoaded(const QList<PrinterDriver>&)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
 }

void CupsFacade::cancelPrinterDriverRequest()
{
    Q_EMIT requestPrinterDriverCancel();
}

PrinterDriverLoader::PrinterDriverLoader(
        const QString &deviceId, const QString &language,
        const QString &makeModel, const QString &product,
        const QStringList &includeSchemes, const QStringList &excludeSchemes)
    : m_deviceId(deviceId)
    , m_language(language)
    , m_makeModel(makeModel)
    , m_product(product)
    , m_includeSchemes(includeSchemes)
    , m_excludeSchemes(excludeSchemes)
{
}

PrinterDriverLoader::~PrinterDriverLoader()
{
}

void PrinterDriverLoader::process()
{
    m_running = true;

    ipp_t* response = helper.createPrinterDriversRequest(
        m_deviceId, m_language, m_makeModel, m_product, m_includeSchemes,
        m_excludeSchemes
    );

    // Note: if the response somehow fails, we return.
    // FIXME: use helper's own isReplyOk
    if (!response || ippGetStatusCode(response) > IPP_OK_CONFLICT) {
        QString err(cupsLastErrorString());
        qWarning() << __PRETTY_FUNCTION__ << "Cups HTTP error:" << err;

        if (response)
            ippDelete(response);

        Q_EMIT error(err);
        Q_EMIT finished();
        return;
    }

    ipp_attribute_t *attr;
    QByteArray ppdDeviceId;
    QByteArray ppdLanguage;
    QByteArray ppdMakeModel;
    QByteArray ppdName;

    // cups_option_t option;
    QList<PrinterDriver> drivers;

    for (attr = ippFirstAttribute(response); attr != NULL && m_running; attr = ippNextAttribute(response)) {

        while (attr != NULL && ippGetGroupTag(attr) != IPP_TAG_PRINTER)
            attr = ippNextAttribute(response);

        if (attr == NULL)
            break;

        // Pull the needed attributes from this PPD...
        ppdDeviceId = "NONE";
        ppdLanguage.clear();
        ppdMakeModel.clear();
        ppdName.clear();

        while (attr != NULL && ippGetGroupTag(attr) == IPP_TAG_PRINTER) {
            if (!strcmp(ippGetName(attr), "ppd-device-id") &&
                 ippGetValueTag(attr) == IPP_TAG_TEXT) {
                ppdDeviceId = ippGetString(attr, 0, NULL);
            } else if (!strcmp(ippGetName(attr), "ppd-natural-language") &&
                       ippGetValueTag(attr) == IPP_TAG_LANGUAGE) {
                ppdLanguage = ippGetString(attr, 0, NULL);

            } else if (!strcmp(ippGetName(attr), "ppd-make-and-model") &&
                       ippGetValueTag(attr) == IPP_TAG_TEXT) {
                ppdMakeModel = ippGetString(attr, 0, NULL);
            } else if (!strcmp(ippGetName(attr), "ppd-name") &&
                       ippGetValueTag(attr) == IPP_TAG_NAME) {

                ppdName = ippGetString(attr, 0, NULL);
            }

            attr = ippNextAttribute(response);
        }

        // See if we have everything needed...
        if (ppdLanguage.isEmpty() || ppdMakeModel.isEmpty() ||
            ppdName.isEmpty()) {
            if (attr == NULL)
                break;
            else
                continue;
        }

        PrinterDriver m;
        m.name = ppdName;
        m.deviceId = ppdDeviceId;
        m.makeModel = ppdMakeModel;
        m.language = ppdLanguage;

        drivers.append(m);
    }

    ippDelete(response);

    Q_EMIT loaded(drivers);
    Q_EMIT finished();
}

void PrinterDriverLoader::cancel()
{
    m_running = false;
}
