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

#include "printer/printerinfo_pdfimpl.h"

#include <QPageSize>

PrinterInfoPdfImpl::PrinterInfoPdfImpl(const QString &name) : PrinterInfo(name)
{

}

PrinterInfoPdfImpl::~PrinterInfoPdfImpl()
{

}

QList<PrinterInfo*> PrinterInfoPdfImpl::availablePrinters()
{
    QList<PrinterInfo*> list;
    list.append(new PrinterInfoPdfImpl("Create PDF"));
    return list;
}

PrinterEnum::DuplexMode PrinterInfoPdfImpl::defaultDuplexMode() const
{
    return PrinterEnum::DuplexMode::DuplexNone;
}

QPageSize PrinterInfoPdfImpl::defaultPageSize() const
{
    return QPageSize(QPageSize::A4);
}

QString PrinterInfoPdfImpl::defaultPrinterName()
{
    return "";
}

QString PrinterInfoPdfImpl::description() const
{
    return QStringLiteral("");
}

bool PrinterInfoPdfImpl::holdsDefinition() const
{
    return true;
}

bool PrinterInfoPdfImpl::isPdf() const
{
    return true;
}

QString PrinterInfoPdfImpl::location() const
{
    return QStringLiteral("");
}

QString PrinterInfoPdfImpl::makeAndModel() const
{
    return QStringLiteral("");
}

QString PrinterInfoPdfImpl::printerName() const
{
    return m_printerName;
}

QList<PrinterEnum::DuplexMode> PrinterInfoPdfImpl::supportedDuplexModes() const
{
    return QList<PrinterEnum::DuplexMode>{PrinterEnum::DuplexMode::DuplexNone};
}

QList<QPageSize> PrinterInfoPdfImpl::supportedPageSizes() const
{
    return QList<QPageSize>{QPageSize(QPageSize::A4)};
}
