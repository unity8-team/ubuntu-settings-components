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

#include "printer/printerinfo_allimpl.h"

#include "printer/printerinfo_impl.h"
#include "printer/printerinfo_pdfimpl.h"

PrinterInfoAllImpl::PrinterInfoAllImpl(const QString &name) : PrinterInfo(name)
{
    m_pdf_printers = new PrinterInfoPdfImpl();
    m_qt_printers = new PrinterInfoImpl();
}

PrinterInfoAllImpl::~PrinterInfoAllImpl()
{

}

QList<PrinterInfo*> PrinterInfoAllImpl::availablePrinters()
{
    QList<PrinterInfo*> list;
    list += m_pdf_printers->availablePrinters();
    list += m_qt_printers->availablePrinters();
    return list;
}
