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

#ifndef USC_PRINTERS_PDF_BACKEND_H
#define USC_PRINTERS_PDF_BACKEND_H

#include "backend/backend.h"

class PRINTERS_DECL_EXPORT PrinterPdfBackend : public PrinterBackend
{
public:
    explicit PrinterPdfBackend(const QString &printerName,
                               QObject *parent = Q_NULLPTR);
    virtual QList<ColorModel> printerGetSupportedColorModels(
        const QString &name) const override;
    virtual ColorModel printerGetDefaultColorModel(const QString &name) const;
    virtual QList<PrintQuality> printerGetSupportedQualities(
        const QString &name) const override;
    virtual PrintQuality printerGetDefaultQuality(const QString &name) const;

    virtual QString printerName() const override;
    virtual QString description() const override;
    virtual QString location() const override;
    virtual QString makeAndModel() const override;

    virtual PrinterEnum::State state() const override;
    virtual QList<QPageSize> supportedPageSizes() const override;
    virtual QPageSize defaultPageSize() const override;
    virtual bool supportsCustomPageSizes() const override;

    virtual QPageSize minimumPhysicalPageSize() const override;
    virtual QPageSize maximumPhysicalPageSize() const override;
    virtual QList<int> supportedResolutions() const override;
    virtual PrinterEnum::DuplexMode defaultDuplexMode() const override;
    virtual QList<PrinterEnum::DuplexMode> supportedDuplexModes() const override;

    virtual PrinterBackend::BackendType backendType() const override;
};

#endif // USC_PRINTERS_PDF_BACKEND_H
