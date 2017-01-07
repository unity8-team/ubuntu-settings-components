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

#ifndef USC_PRINTERS_MOCK_PRINTERINFO_H
#define USC_PRINTERS_MOCK_PRINTERINFO_H

#include "enums.h"
#include "printer/printer.h"
#include "printer/printerinfo.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QPageSize>

class MockPrinterInfo : public QObject, public PrinterInfo
{
    Q_OBJECT
public:
    explicit MockPrinterInfo(QObject* parent = 0)
    {

    }

    virtual ~MockPrinterInfo() override
    {

    }

    virtual QString printerName() const override
    {

    }

    virtual QString description() const override
    {

    }

    virtual QString location() const override
    {

    }

    virtual QString makeAndModel() const override
    {

    }


    virtual State state() const override
    {

    }

    virtual QList<QPageSize> supportedPageSizes() const override
    {

    }

    virtual QPageSize defaultPageSize() const override
    {

    }

    virtual bool supportsCustomPageSizes() const override
    {

    }

    virtual QPageSize minimumPhysicalPageSize() const override
    {

    }

    virtual QPageSize maximumPhysicalPageSize() const override
    {

    }

    virtual QList<int> supportedResolutions() const override
    {

    }

    virtual DuplexMode defaultDuplexMode() const override
    {

    }

    virtual QList<DuplexMode> supportedDuplexModes() const override
    {

    }
};

#endif // USC_PRINTERS_MOCK_PRINTERINFO_H
