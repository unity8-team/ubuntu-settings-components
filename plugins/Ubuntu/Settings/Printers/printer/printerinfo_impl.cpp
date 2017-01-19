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

#include "printer/printerinfo_impl.h"

#include <QDebug>
#include <QPrinter>

PrinterInfoImpl::PrinterInfoImpl(const QString &name) : PrinterInfo(name)
{
    refresh();
}

PrinterInfoImpl::PrinterInfoImpl(QPrinterInfo info)
    : PrinterInfo(info.isNull() ? QString::null : info.printerName())
    , m_info(info)
{
}

PrinterInfoImpl::~PrinterInfoImpl()
{

}

bool PrinterInfoImpl::holdsDefinition() const
{
    return !m_info.isNull();
}

QString PrinterInfoImpl::printerName() const
{
    return m_info.printerName();
}

QString PrinterInfoImpl::description() const
{
    return m_info.description();
}

QString PrinterInfoImpl::location() const
{

}

QString PrinterInfoImpl::makeAndModel() const
{

}

PrinterEnum::State PrinterInfoImpl::state() const
{

}

QList<QPageSize> PrinterInfoImpl::supportedPageSizes() const
{
    return m_info.supportedPageSizes();
}

QPageSize PrinterInfoImpl::defaultPageSize() const
{
    return m_info.defaultPageSize();
}

bool PrinterInfoImpl::supportsCustomPageSizes() const
{

}

QPageSize PrinterInfoImpl::minimumPhysicalPageSize() const
{

}

QPageSize PrinterInfoImpl::maximumPhysicalPageSize() const
{

}

QList<int> PrinterInfoImpl::supportedResolutions() const
{

}

PrinterEnum::DuplexMode PrinterInfoImpl::defaultDuplexMode() const
{
    return Utils::qDuplexModeToDuplexMode(m_info.defaultDuplexMode());
}

QList<PrinterEnum::DuplexMode> PrinterInfoImpl::supportedDuplexModes() const
{
    QList<PrinterEnum::DuplexMode> list;
    Q_FOREACH(const QPrinter::DuplexMode mode, m_info.supportedDuplexModes()) {
        if (mode != QPrinter::DuplexAuto) {
            list.append(Utils::qDuplexModeToDuplexMode(mode));
        }
    }
    return list;
}

QList<PrinterInfo*> PrinterInfoImpl::availablePrinters()
{
    QList<PrinterInfo*> list;
    Q_FOREACH(QPrinterInfo info, QPrinterInfo::availablePrinters()) {
        list.append(new PrinterInfoImpl(info));
    }
    return list;
}

QStringList PrinterInfoImpl::availablePrinterNames()
{

}

PrinterInfo* PrinterInfoImpl::printerInfo(const QString &printerName)
{

}

QString PrinterInfoImpl::defaultPrinterName()
{
    return QPrinterInfo::defaultPrinterName();
}

void PrinterInfoImpl::refresh()
{
    if (m_printerName.isEmpty()) {
        m_info = QPrinterInfo();
    } else {
        m_info = QPrinterInfo::printerInfo(m_printerName);
    }
}
