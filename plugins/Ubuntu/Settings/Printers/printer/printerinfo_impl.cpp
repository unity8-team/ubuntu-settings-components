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

#include "printer/printerinfo_impl.h"

PrinterInfoImpl::PrinterInfoImpl(QObject* parent) : QObject(parent)
{

}

PrinterInfoImpl::~PrinterInfoImpl()
{

}

QString PrinterInfoImpl::printerName() const
{

}

QString PrinterInfoImpl::description() const
{

}

QString PrinterInfoImpl::location() const
{

}

QString PrinterInfoImpl::makeAndModel() const
{

}


State PrinterInfoImpl::state() const
{

}

QList<QPageSize> PrinterInfoImpl::supportedPageSizes() const
{

}

QPageSize PrinterInfoImpl::defaultPageSize() const
{

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

DuplexMode PrinterInfoImpl::defaultDuplexMode() const
{

}

QList<DuplexMode> PrinterInfoImpl::supportedDuplexModes() const
{

}

QStringList PrinterInfoImpl::availablePrinterNames()
{

}

PrinterInfo* PrinterInfoImpl::printerInfo(const QString &printerName)
{

}

