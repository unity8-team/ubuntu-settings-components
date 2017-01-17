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

#include "cups/cupsfacade_impl.h"
#include "models/printermodel.h"
#include "printer/printerjob.h"
#include "printer/printerinfo_impl.h"

PrinterJob::PrinterJob(QObject *parent)
    : QObject(parent)
    , m_color_mode(PrinterEnum::ColorModel::ColorMode)
    , m_copies(1)
    , m_cups(new CupsFacadeImpl())
    , m_duplex(false)
    , m_printer(Q_NULLPTR)
    , m_print_range(QStringLiteral(""))
    , m_print_range_mode(PrinterEnum::PrintRange::AllPages)
    , m_quality(PrinterEnum::Quality::NormalQuality)
    // TODO: Do we need a separate Job state?
    // NotStarted, InQueue, Processing, Complete, Error ?
    , m_state(PrinterEnum::State::IdleState)
    , m_title(QStringLiteral(""))
{

}

PrinterJob::PrinterJob(Printer *printer, QObject *parent)
    : QObject(parent)
    , m_copies(1)
    , m_cups(new CupsFacadeImpl())
    , m_printer(printer)
    , m_print_range(QStringLiteral(""))
    , m_print_range_mode(PrinterEnum::PrintRange::AllPages)
    // TODO: Do we need a separate Job state?
    // NotStarted, InQueue, Processing, Complete, Error ?
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

PrinterEnum::ColorModel PrinterJob::colorMode() const
{
    return m_color_mode;
}

int PrinterJob::copies() const
{
    return m_copies;
}

bool PrinterJob::duplex() const
{
    return m_duplex;
}

bool PrinterJob::landscape() const
{
    return m_landscape;
}

void PrinterJob::loadDefaults()
{
    if (m_printer) {
        // Load defaults from printer
        setColorMode(m_printer->colorMode());
        setDuplex(m_printer->duplex());
        setQuality(m_printer->quality());
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

PrinterEnum::Quality PrinterJob::quality() const
{
    return m_quality;
}

PrinterEnum::State PrinterJob::state() const
{
    return m_state;
}

void PrinterJob::setColorMode(const PrinterEnum::ColorModel &colorMode)
{
    if (m_color_mode != colorMode) {
        m_color_mode = colorMode;

        Q_EMIT colorModeChanged();
    }
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

void PrinterJob::setDuplex(const bool duplex)
{
    if (m_duplex != duplex) {
        m_duplex = duplex;

        Q_EMIT duplexChanged();
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
    if (m_printer_name != printerName) {
        PrinterInfo *info = new PrinterInfoImpl(printerName);

        if (info->holdsDefinition()) {
            m_printer_name = printerName;
            m_printer = new Printer(info, m_cups);
            loadDefaults();

            Q_EMIT printerNameChanged();
        } else {
            qWarning() << "Unknown printer:" << printerName;
        }
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

void PrinterJob::setQuality(const PrinterEnum::Quality &quality)
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
