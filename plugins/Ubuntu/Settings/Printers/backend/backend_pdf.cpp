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

#include "i18n.h"
#include "backend/backend_pdf.h"

PrinterPdfBackend::PrinterPdfBackend(const QString &printerName,
                                     QObject *parent)
    : PrinterBackend(printerName, parent)
{
}

QList<ColorModel> PrinterPdfBackend::printerGetSupportedColorModels(
    const QString &name) const
{
    return QList<ColorModel>{printerGetDefaultColorModel(name)};
}

ColorModel PrinterPdfBackend::printerGetDefaultColorModel(
    const QString &name) const
{
    Q_UNUSED(name);
    ColorModel rgb;
    rgb.colorType = PrinterEnum::ColorModelType::ColorType;
    rgb.name = "RGB";
    rgb.text = __("Color");
    return rgb;
}

QList<PrintQuality> PrinterPdfBackend::printerGetSupportedQualities(
    const QString &name) const
{
    return QList<PrintQuality>({printerGetDefaultQuality(name)});
}

PrintQuality PrinterPdfBackend::printerGetDefaultQuality(
        const QString &name) const
{
    Q_UNUSED(name);
    PrintQuality quality;
    quality.name = __("Normal");
    return quality;
}

QString PrinterPdfBackend::printerName() const
{
    return m_printerName;
}

QString PrinterPdfBackend::description() const
{
    return QStringLiteral("");
}

QString PrinterPdfBackend::location() const
{
    return QStringLiteral("");
}

QString PrinterPdfBackend::makeAndModel() const
{
    return QStringLiteral("");
}

PrinterEnum::State PrinterPdfBackend::state() const
{
    return PrinterEnum::State::IdleState;
}

QList<QPageSize> PrinterPdfBackend::supportedPageSizes() const
{
    return QList<QPageSize>{QPageSize(QPageSize::A4)};
}

QPageSize PrinterPdfBackend::defaultPageSize() const
{
    return QPageSize(QPageSize::A4);
}

bool PrinterPdfBackend::supportsCustomPageSizes() const
{
    return false;
}


QPageSize PrinterPdfBackend::minimumPhysicalPageSize() const
{
    return QPageSize(QPageSize::A4);
}

QPageSize PrinterPdfBackend::maximumPhysicalPageSize() const
{
    return QPageSize(QPageSize::A4);
}

QList<int> PrinterPdfBackend::supportedResolutions() const
{

}

PrinterEnum::DuplexMode PrinterPdfBackend::defaultDuplexMode() const
{
    return PrinterEnum::DuplexMode::DuplexNone;
}

QList<PrinterEnum::DuplexMode> PrinterPdfBackend::supportedDuplexModes() const
{
    return QList<PrinterEnum::DuplexMode>{PrinterEnum::DuplexMode::DuplexNone};
}

PrinterBackend::BackendType PrinterPdfBackend::backendType() const
{
    return PrinterBackend::BackendType::PdfType;
}
