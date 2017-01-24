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

#include <QtCore/QDebug>
#include <QUrl>

#include "backend/backend_cups.h"
#include "models/printermodel.h"
#include "printer/printerjob.h"

PrinterJob::PrinterJob(QObject *parent)
    : PrinterJob(Q_NULLPTR, parent)
{
}

PrinterJob::PrinterJob(Printer *printer, QObject *parent)
    : PrinterJob(printer, new PrinterCupsBackend, parent)
{
}

PrinterJob::PrinterJob(Printer *printer, PrinterBackend *backend,
                       QObject *parent)
    : QObject(parent)
    , m_color_model(0)
    , m_copies(1)
    , m_backend(backend)
    , m_duplex_mode(0)
    , m_printer(printer)
    , m_printer_name(QStringLiteral(""))
    , m_print_range(QStringLiteral(""))
    , m_print_range_mode(PrinterEnum::PrintRange::AllPages)
    , m_quality(0)
    , m_state(PrinterEnum::State::IdleState)
    , m_title(QStringLiteral(""))
{
    loadDefaults();
}


PrinterJob::~PrinterJob()
{

}

void PrinterJob::cancel()
{

}

int PrinterJob::colorModel() const
{
    return m_color_model;
}

PrinterEnum::ColorModelType PrinterJob::colorModelType() const
{
    return getColorModel().colorType;
}

int PrinterJob::copies() const
{
    return m_copies;
}

int PrinterJob::duplexMode() const
{
    return m_duplex_mode;
}

ColorModel PrinterJob::getColorModel() const
{
    if (m_printer && colorModel() > -1 && colorModel() < m_printer->supportedColorModels().count()) {
        return m_printer->supportedColorModels().at(colorModel());
    } else {
        return ColorModel();
    }
}

PrintQuality PrinterJob::getPrintQuality() const
{
    PrintQuality ret;
    if (m_printer && quality() > -1 && quality() < m_printer->supportedPrintQualities().count()) {
        ret = m_printer->supportedPrintQualities().at(quality());
    }
    return ret;
}

PrinterEnum::DuplexMode PrinterJob::getDuplexMode() const
{
    if (m_printer && duplexMode() > -1 && duplexMode() < m_printer->supportedDuplexModes().count()) {
        return m_printer->supportedDuplexModes().at(duplexMode());
    } else {
        return PrinterEnum::DuplexMode::DuplexNone;
    }
}

bool PrinterJob::landscape() const
{
    return m_landscape;
}

void PrinterJob::loadDefaults()
{
    if (m_printer) {
        // Load defaults from printer
        setColorModel(m_printer->supportedColorModels().indexOf(m_printer->defaultColorModel()));
        setDuplexMode(m_printer->supportedDuplexModes().indexOf(m_printer->defaultDuplexMode()));
        setQuality(m_printer->supportedPrintQualities().indexOf(m_printer->defaultPrintQuality()));
    }
}

//Printer *PrinterJob::printer() const
//{
//    return m_printer;
//}

QString PrinterJob::printerName() const
{
    return m_printer_name;
}

void PrinterJob::printFile(const QUrl &url)
{
    if (m_printer) {
        int jobId = m_printer->printFile(url.toLocalFile(), this);

        // TODO: should we track the job and state of it here?
        // so then we can do cancel() and show in the UI when the job is done?
        Q_UNUSED(jobId);
    } else {
        qWarning() << "No valid printer in PrinterJob";
    }
}

QString PrinterJob::printRange() const
{
    return m_print_range;
}

PrinterEnum::PrintRange PrinterJob::printRangeMode() const
{
    return m_print_range_mode;
}

int PrinterJob::quality() const
{
    return m_quality;
}

PrinterEnum::State PrinterJob::state() const
{
    return m_state;
}

void PrinterJob::setColorModel(const int colorModel)
{
    if (m_color_model != colorModel) {
        m_color_model = colorModel;

        Q_EMIT colorModelChanged();
    }

    // Always emit colorModeType changed, as the underlying model could have
    // changed but the int maybe the same
    // eg if it was RGB, KGray and m_color_model was 1, then changing to
    // KGray, RGB with m_color_model as 1, results in no change but colorModelType does.
    Q_EMIT colorModelTypeChanged();
}

void PrinterJob::setCopies(const int copies)
{
    if (m_copies != copies) {
        if (copies > 0) {
            m_copies = copies;

            Q_EMIT copiesChanged();
        } else {
            qWarning() << "Copies should be greater than 0.";
        }
    }
}

void PrinterJob::setDuplexMode(const int duplexMode)
{
    if (m_duplex_mode != duplexMode) {
        m_duplex_mode = duplexMode;

        Q_EMIT duplexModeChanged();
    }
}

void PrinterJob::setLandscape(const bool landscape)
{
    if (m_landscape != landscape) {
        m_landscape = landscape;

        Q_EMIT landscapeChanged();
    }
}

//void PrinterJob::setPrinter(Printer *printer)
//{
//    qDebug() << "Attempting to set printer!" << printer;

//    if (m_printer != printer) {
//        m_printer = printer;

//        Q_EMIT printerChanged();
//    }
//}

void PrinterJob::setPrinterName(const QString &printerName)
{
    // Please note the return inside the foreach.
    if (m_printer_name != printerName) {
        Q_FOREACH(Printer *printer, m_backend->availablePrinters()) {
            if (printer->name() == printerName) {
                m_printer_name = printerName;
                m_printer = printer;
                loadDefaults();
                Q_EMIT printerNameChanged();
                return;
            }
        }

        qWarning() << "Unknown printer:" << printerName;
    }
}

void PrinterJob::setPrintRange(const QString &printRange)
{
    if (m_print_range != printRange) {
        m_print_range = printRange;

        Q_EMIT printRangeChanged();
    }
}

void PrinterJob::setPrintRangeMode(const PrinterEnum::PrintRange printRangeMode)
{
    if (m_print_range_mode != printRangeMode) {
        m_print_range_mode = printRangeMode;

        Q_EMIT printRangeModeChanged();
    }
}

void PrinterJob::setQuality(const int quality)
{
    if (m_quality != quality) {
        m_quality = quality;

        Q_EMIT qualityChanged();
    }
}

void PrinterJob::setState(const PrinterEnum::State &state)
{
    if (m_state != state) {
        m_state = state;

        Q_EMIT stateChanged();
    }
}

void PrinterJob::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;

        Q_EMIT titleChanged();
    }
}

QString PrinterJob::title() const
{
    return m_title;
}
