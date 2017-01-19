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

#ifndef USC_PRINTERS_PRINTERINFO_PDFIMPL_H
#define USC_PRINTERS_PRINTERINFO_PDFIMPL_H

#include "printer/printerinfo.h"

class PrinterInfoPdfImpl : public PrinterInfo
{
public:
    explicit PrinterInfoPdfImpl(const QString &name = QString::null);
    virtual ~PrinterInfoPdfImpl() override;

    virtual bool holdsDefinition() const override;

    virtual QString printerName() const override;
    virtual QString description() const override;
    virtual QString location() const override;
    virtual QString makeAndModel() const override;

    virtual PrinterEnum::State state() const override {}
    virtual QList<QPageSize> supportedPageSizes() const override;
    virtual QPageSize defaultPageSize() const override;
    virtual bool supportsCustomPageSizes() const override {}

    virtual QPageSize minimumPhysicalPageSize() const override {}
    virtual QPageSize maximumPhysicalPageSize() const override {}
    virtual QList<int> supportedResolutions() const override {}
    virtual PrinterEnum::DuplexMode defaultDuplexMode() const override;
    virtual QList<PrinterEnum::DuplexMode> supportedDuplexModes() const override;
    virtual QList<PrinterInfo*> availablePrinters() override;
    virtual QStringList availablePrinterNames() override {}
    virtual PrinterInfo* printerInfo(const QString &printerName) override {}
    virtual QString defaultPrinterName() override;

    virtual bool isPdf() const override;

    virtual void refresh() override {}
};

#endif // USC_PRINTERS_PRINTERINFO_PDFIMPL_H
