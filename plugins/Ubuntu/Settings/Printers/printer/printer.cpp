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

#include <QDebug>

Printer::Printer(QObject *parent)
    : QObject(parent)
{
    // TODO: remove this constructor.
}

Printer::Printer(PrinterBackend *backend, QObject *parent)
    : QObject(parent)
    , m_backend(backend)
{
    loadColorModel();
    loadPrintQualities();
}

Printer::~Printer()
{
    m_backend->deleteLater();
}

void Printer::loadColorModel()
{
    m_supportedColorModels = m_backend->printerGetSupportedColorModels(name());
    m_defaultColorModel = m_backend->printerGetDefaultColorModel(name());

    if (m_supportedColorModels.size() == 0) {
        m_supportedColorModels.append(m_defaultColorModel);
    }
}

void Printer::loadPrintQualities()
{
    m_supportedPrintQualities = m_backend->printerGetSupportedQualities(name());
    m_defaultPrintQuality = m_backend->printerGetDefaultQuality(name());

    if (m_supportedPrintQualities.size() == 0) {
        m_supportedPrintQualities.append(m_defaultPrintQuality);
    }
}

ColorModel Printer::defaultColorModel() const
{
    return m_defaultColorModel;
}

QList<ColorModel> Printer::supportedColorModels() const
{
    return m_supportedColorModels;
}

PrintQuality Printer::defaultPrintQuality() const
{
    return m_defaultPrintQuality;
}

QList<PrintQuality> Printer::supportedPrintQualities() const
{
    return m_supportedPrintQualities;
}

QList<PrinterEnum::DuplexMode> Printer::supportedDuplexModes() const
{
    return m_backend->supportedDuplexModes();
}

QStringList Printer::supportedDuplexStrings() const
{
    QStringList list;
    Q_FOREACH(const PrinterEnum::DuplexMode &mode, supportedDuplexModes()) {
        list << Utils::duplexModeToUIString(mode);
    }
    return list;
}

PrinterEnum::DuplexMode Printer::defaultDuplexMode() const
{
    return m_backend->defaultDuplexMode();
}

PrinterJob *Printer::job()
{
    return new PrinterJob(this);
}

int Printer::printFile(const QString &filepath, const PrinterJob *options)
{
    auto dest = m_backend->makeDest(name(), options);  // options could be QMap<QString, QString> ?

    qDebug() << "Going to print:" << filepath << options->title();
    return m_backend->printFileToDest(filepath, options->title(), dest);
}

QString Printer::name() const
{
    return m_backend->printerName();
}

QString Printer::description() const
{
    return m_backend->description();
}

QPageSize Printer::defaultPageSize() const
{
    return m_backend->defaultPageSize();
}

QList<QPageSize> Printer::supportedPageSizes() const
{
    return m_backend->supportedPageSizes();
}

PrinterEnum::AccessControl Printer::accessControl() const
{
    // TODO: implement
    return PrinterEnum::AccessControl::AccessAllow;
}

PrinterEnum::ErrorPolicy Printer::errorPolicy() const
{
    // TODO: implement
    return PrinterEnum::ErrorPolicy::RetryOnError;
}

bool Printer::enabled() const
{
    // TODO: implement
    return true;
}

QStringList Printer::users() const
{
    // TODO: implement
    return QStringList();
}

PrinterEnum::State Printer::state() const
{
    return m_backend->state();
}

QString Printer::lastStateMessage() const
{
    // TODO: implement
    return QString();
}

bool Printer::isDefault()
{
    return name() == m_backend->defaultPrinterName();
}

bool Printer::isPdf()
{
    return m_backend->backendType() == PrinterBackend::BackendType::PdfType;
}

void Printer::setDefaultColorModel(const ColorModel &colorModel)
{
    if (defaultColorModel() == colorModel) {
        return;
    }

    if (!supportedColorModels().contains(colorModel)) {
        qWarning() << Q_FUNC_INFO << "color model not supported";
        return;
    }

    QStringList vals({colorModel.name});
    QString reply = m_backend->printerAddOption(name(), "ColorModel", vals);
    Q_UNUSED(reply);
}

void Printer::setAccessControl(const PrinterEnum::AccessControl &accessControl)
{
    // TODO: implement
    Q_UNUSED(accessControl);
}

void Printer::setDescription(const QString &description)
{
    QString answer = m_backend->printerSetInfo(name(), description);
}

void Printer::setDefaultDuplexMode(const PrinterEnum::DuplexMode &duplexMode)
{
    if (defaultDuplexMode() == duplexMode) {
        return;
    }

    if (!m_backend->supportedDuplexModes().contains(duplexMode)) {
        qWarning() << Q_FUNC_INFO << "duplex mode not supported";
        return;
    }

    QStringList vals({Utils::duplexModeToPpdChoice(duplexMode)});
    QString reply = m_backend->printerAddOption(name(), "Duplex", vals);
}

void Printer::setEnabled(const bool enabled)
{
    // TODO: implement
    Q_UNUSED(enabled);
}

void Printer::setErrorPolicy(const PrinterEnum::ErrorPolicy &errorPolicy)
{
    // TODO: implement
    Q_UNUSED(errorPolicy);
}

void Printer::setName(const QString &name)
{
    // TODO: implement
    Q_UNUSED(name);
}

void Printer::setDefaultPrintQuality(const PrintQuality &quality)
{
    if (defaultPrintQuality() == quality) {
        return;
    }

    if (!supportedPrintQualities().contains(quality)) {
        qWarning() << Q_FUNC_INFO << "quality not supported.";
        return;
    }

    QStringList vals({quality.name});
    QString reply = m_backend->printerAddOption(name(), quality.originalOption, vals);
}

void Printer::setDefaultPageSize(const QPageSize &pageSize)
{
    if (defaultPageSize() == pageSize) {
        return;
    }

    if (!m_backend->supportedPageSizes().contains(pageSize)) {
        qWarning() << Q_FUNC_INFO << "pagesize not supported.";
        return;
    }

    if (pageSize.key().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "pagesize does not expose a ppd key.";
        return;
    }

    QStringList vals({pageSize.key()});
    QString reply = m_backend->printerAddOption(name(), "PageSize", vals);

    m_backend->refresh();
    Q_EMIT defaultPageSizeChanged();
}

void Printer::addUser(const QString &username)
{
    // TODO: implement
    Q_UNUSED(username);
}

void Printer::removeUser(const QString &username)
{
    // TODO: implement
    Q_UNUSED(username);
}

void Printer::requestInkLevels(const QString &name)
{
    // TODO: implement
    Q_UNUSED(name);
}

bool Printer::deepCompare(Printer *other) const
{
    bool changed = false;

    changed |= defaultColorModel() != other->defaultColorModel();
    changed |= defaultPrintQuality() != other->defaultPrintQuality();
    changed |= description() != other->description();
    changed |= defaultDuplexMode() != other->defaultDuplexMode();
    changed |= defaultPageSize() != other->defaultPageSize();
    changed |= state() != other->state();

    // TODO: accessControl
    // TODO: enabled
    // TODO: errorPolicy

    // Return true if they are the same, so no change
    return changed == false;
}

void Printer::updateFrom(Printer* newPrinter)
{
    Q_UNUSED(newPrinter);

    m_backend->refresh();

    loadColorModel();
    loadPrintQualities();

    Q_EMIT descriptionChanged();
    Q_EMIT defaultColorModelChanged();
    Q_EMIT defaultDuplexModeChanged();
    Q_EMIT defaultPageSizeChanged();
    Q_EMIT defaultPrintQualityChanged();
    Q_EMIT stateChanged();

    // TODO: accessControl
    // TODO: enabled
    // TODO: errorPolicy
}
