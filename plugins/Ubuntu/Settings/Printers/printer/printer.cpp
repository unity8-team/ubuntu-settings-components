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

#include "printer.h"
#include "printer_p.h"

#include <QDebug>

Printer::Printer(QObject *parent)
    : QObject(parent)
    , d_ptr(new PrinterPrivate(this))
{
}

Printer::Printer(PrinterInfo *info, CupsFacade *cups, QObject *parent)
    : QObject(parent)
    , d_ptr(new PrinterPrivate(this, info, cups))
{
}

Printer::~Printer()
{

}

PrinterPrivate::PrinterPrivate(Printer *q)
{
    loadColorModel();
    loadPrintQualities();
}

PrinterPrivate::PrinterPrivate(Printer *q, PrinterInfo *info, CupsFacade *cups)
{
    this->info = info;
    this->cups = cups;

    loadColorModel();
    loadPrintQualities();
}

PrinterPrivate::~PrinterPrivate()
{
    delete this->info;
}

void PrinterPrivate::loadColorModel()
{
    if (!this->info->isPdf()) {
        QStringList opts({"DefaultColorModel"});
        auto name = this->info->printerName();
        auto vals = this->cups->printerGetOptions(name, opts);
        m_defaultColorModel = vals["DefaultColorModel"].value<ColorModel>();

        m_supportedColorModels = this->cups->printerGetSupportedColorModels(name);

        if (m_supportedColorModels.size() == 0) {
            m_supportedColorModels.append(m_defaultColorModel);
        }
    } else {
        ColorModel rgb = ColorModel();
        rgb.colorType = PrinterEnum::ColorModelType::ColorType;
        rgb.name = "RGB";
        rgb.text = "Color";

        m_defaultColorModel = rgb;
        m_supportedColorModels = QList<ColorModel>{rgb};
    }
}

void PrinterPrivate::loadPrintQualities()
{
    QString name = this->info->printerName();

    m_defaultPrintQuality = this->cups->printerGetOption(
        name, "DefaultPrintQuality").value<PrintQuality>();
    m_supportedPrintQualities = this->cups->printerGetSupportedQualities(name);
}

ColorModel Printer::defaultColorModel() const
{
    Q_D(const Printer);
    return d->m_defaultColorModel;
}

QList<ColorModel> Printer::supportedColorModels() const
{
    Q_D(const Printer);
    return d->m_supportedColorModels;
}

PrintQuality Printer::defaultPrintQuality() const
{
    Q_D(const Printer);
    return d->m_defaultPrintQuality;
}

QList<PrintQuality> Printer::supportedPrintQualities() const
{
    Q_D(const Printer);
    return d->m_supportedPrintQualities;
}

QList<PrinterEnum::DuplexMode> Printer::supportedDuplexModes() const
{
    Q_D(const Printer);
    return d->info->supportedDuplexModes();
}

QStringList Printer::supportedDuplexStrings() const
{
    Q_D(const Printer);
    QStringList list;
    Q_FOREACH(const PrinterEnum::DuplexMode &mode, supportedDuplexModes()) {
        list << Utils::duplexModeToUIString(mode);
    }
    return list;
}

PrinterEnum::DuplexMode Printer::defaultDuplexMode() const
{
    Q_D(const Printer);
    return d->info->defaultDuplexMode();
}

PrinterJob *Printer::job()
{
    return new PrinterJob(this);
}

int Printer::printFile(const QString &filepath, const PrinterJob *options)
{
    Q_D(const Printer);

    auto dest = d->cups->makeDest(this->name(), options);  // options could be QMap<QString, QString> ?

    qDebug() << "Going to print:" << filepath << options->title();
    return d->cups->printFileToDest(filepath, options->title(), dest);
}

QString Printer::name() const
{
    Q_D(const Printer);
    return d->info->printerName();
}

QString Printer::description() const
{
    Q_D(const Printer);
    return d->info->description();
}

QPageSize Printer::defaultPageSize() const
{
    Q_D(const Printer);
    return d->info->defaultPageSize();
}

QList<QPageSize> Printer::supportedPageSizes() const
{
    Q_D(const Printer);
    return d->info->supportedPageSizes();
}

PrinterEnum::AccessControl Printer::accessControl() const
{

}

PrinterEnum::ErrorPolicy Printer::errorPolicy() const
{

}

bool Printer::enabled() const
{

}

QStringList Printer::users() const
{

}

PrinterEnum::State Printer::state() const
{

}

QString Printer::lastStateMessage() const
{

}

bool Printer::isDefault()
{
    Q_D(Printer);
    return name() == d->info->defaultPrinterName();
}

bool Printer::isPdf()
{
    Q_D(Printer);
    return d->info->isPdf();
}

void Printer::setDefaultColorModel(const ColorModel &colorModel)
{
    Q_D(Printer);

    if (defaultColorModel() == colorModel) {
        return;
    }

    if (!supportedColorModels().contains(colorModel)) {
        qWarning() << Q_FUNC_INFO << "color model not supported";
        return;
    }

    QStringList vals({colorModel.name});
    QString reply = d->cups->printerAddOption(name(), "ColorModel", vals);
    Q_UNUSED(reply);

    d->loadColorModel();
    Q_EMIT defaultColorModelChanged();
}

void Printer::setAccessControl(const PrinterEnum::AccessControl &accessControl)
{

}

void Printer::setDescription(const QString &description)
{
    Q_D(const Printer);
    QString answer = d->cups->printerSetInfo(d->info->printerName(), description);

    d->info->refresh();
    Q_EMIT descriptionChanged();
}

void Printer::setDefaultDuplexMode(const PrinterEnum::DuplexMode &duplexMode)
{
    Q_D(Printer);

    if (defaultDuplexMode() == duplexMode) {
        return;
    }

    if (!d->info->supportedDuplexModes().contains(duplexMode)) {
        qWarning() << Q_FUNC_INFO << "duplex mode not supported";
        return;
    }

    QStringList vals({Utils::duplexModeToPpdChoice(duplexMode)});
    QString reply = d->cups->printerAddOption(name(), "Duplex", vals);

    d->info->refresh();
    Q_EMIT defaultDuplexModeChanged();
}

void Printer::setEnabled(const bool enabled)
{

}

void Printer::setErrorPolicy(const PrinterEnum::ErrorPolicy &errorPolicy)
{

}

void Printer::setName(const QString &name)
{

}

void Printer::setDefaultPrintQuality(const PrintQuality &quality)
{
    Q_D(Printer);

    if (defaultPrintQuality() == quality) {
        return;
    }

    if (!supportedPrintQualities().contains(quality)) {
        qWarning() << Q_FUNC_INFO << "quality not supported.";
        return;
    }

    QStringList vals({quality.name});
    QString reply = d->cups->printerAddOption(name(), quality.originalOption, vals);
    d->loadPrintQualities();
}

void Printer::setDefaultPageSize(const QPageSize &pageSize)
{
    Q_D(Printer);

    if (defaultPageSize() == pageSize) {
        return;
    }

    if (!d->info->supportedPageSizes().contains(pageSize)) {
        qWarning() << Q_FUNC_INFO << "pagesize not supported.";
        return;
    }

    if (pageSize.key().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "pagesize does not expose a ppd key.";
        return;
    }

    QStringList vals({pageSize.key()});
    QString reply = d->cups->printerAddOption(name(), "PageSize", vals);

    d->info->refresh();
    Q_EMIT defaultPageSizeChanged();
}

void Printer::addUser(const QString &username)
{

}

void Printer::removeUser(const QString &username)
{

}

void Printer::requestInkLevels(const QString &name)
{

}
