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

// #include <QObject>
#include <QPageSize>
#include <QList>
#include <QString>
#include <QStringList>

/* PrinterInfo uses the bridge pattern to accommodate different
implementations, like QPrinterInfo; and future and testable
implementations. */
class PrinterInfo
{

//protected:
    //explicit PrinterInfo(QObject* parent = 0);

public:
    virtual ~PrinterInfo() {};

    virtual QString printerName() const = 0;
    virtual QString description() const = 0;
    virtual QString location() const = 0;
    virtual QString makeAndModel() const = 0;

    virtual State state() const = 0;
    virtual QList<QPageSize> supportedPageSizes() const = 0;
    virtual QPageSize defaultPageSize() const = 0;
    virtual bool supportsCustomPageSizes() const = 0;

    virtual QPageSize minimumPhysicalPageSize() const = 0;
    virtual QPageSize maximumPhysicalPageSize() const = 0;
    virtual QList<int> supportedResolutions() const = 0;
    virtual DuplexMode defaultDuplexMode() const = 0;
    virtual QList<DuplexMode> supportedDuplexModes() const = 0;
};

#endif // USC_PRINTERS_PRINTERINFO_H
