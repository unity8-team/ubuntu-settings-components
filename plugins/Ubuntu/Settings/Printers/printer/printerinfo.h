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

#ifndef USC_PRINTERS_PRINTERINFO_H
#define USC_PRINTERS_PRINTERINFO_H

#include "enums.h"

#include <QPageSize>
#include <QList>
#include <QString>
#include <QStringList>

class PrinterInfo
{
public:
    explicit PrinterInfo(const QString &name = QString::null)
        : m_printerName(name) {};
    virtual ~PrinterInfo() {};

    virtual bool holdsDefinition() const = 0;

    virtual QString printerName() const = 0;
    virtual QString description() const = 0;
    virtual QString location() const = 0;
    virtual QString makeAndModel() const = 0;

    virtual PrinterEnum::State state() const = 0;
    virtual QList<QPageSize> supportedPageSizes() const = 0;
    virtual QPageSize defaultPageSize() const = 0;
    virtual bool supportsCustomPageSizes() const = 0;

    virtual QPageSize minimumPhysicalPageSize() const = 0;
    virtual QPageSize maximumPhysicalPageSize() const = 0;
    virtual QList<int> supportedResolutions() const = 0;
    virtual PrinterEnum::DuplexMode defaultDuplexMode() const = 0;
    virtual QList<PrinterEnum::DuplexMode> supportedDuplexModes() const = 0;

    virtual QList<PrinterInfo*> availablePrinters() = 0;
    virtual QStringList availablePrinterNames() = 0;
    virtual PrinterInfo* printerInfo(const QString &printerName) = 0;
    virtual QString defaultPrinterName() = 0;

    virtual void refresh() = 0;
protected:
    const QString m_printerName;

};

Q_DECLARE_TYPEINFO(PrinterInfo, Q_MOVABLE_TYPE);

#endif // USC_PRINTERS_PRINTERINFO_H
