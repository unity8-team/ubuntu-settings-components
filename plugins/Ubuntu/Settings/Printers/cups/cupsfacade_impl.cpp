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
    if (!helper.printerClassSetOption(name, option, values)) {
        return helper.getLastError();
    }

    Q_EMIT printerModified(name, true);
    return QString();
}

QVariant CupsFacadeImpl::printerGetOption(const QString &name,
                                          const QString &option)
{

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
                free(defCModel);
            }

            model.colorType = ppd->color_device ? ColorModelType::ColorType
                                                : ColorModelType::GrayType;
            model.colorSpace = Utils::ppdColorSpaceToColorSpace(ppd->colorspace);
            ret[option] = QVariant::fromValue(model);
        } else {
            ppd_option_t *val = ppdFindOption(ppd, option.toUtf8());

            if (val) {
                qWarning() << "asking for" << option << "returns" << val->text;
                free(val);
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

    // If there were no ColorModels in the ppd, append the guessed/default one:
    ColorModel model;
    ppd_option_t *defCModel = ppdFindOption(ppd, "DefaultColorModel");
    if (defCModel) {
        model = Utils::parsePpdColorModel(defCModel->choices[0].choice);
        free(defCModel);
    }
    model.colorType = ppd->color_device ? ColorModelType::ColorType
                                        : ColorModelType::GrayType;
    model.colorSpace = Utils::ppdColorSpaceToColorSpace(ppd->colorspace);
    if (model.name.isEmpty()) {
        model.name = ppd->color_device ? "Color" : "Gray"; // Translate? Improve?
    }
    ret.append(model);

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
