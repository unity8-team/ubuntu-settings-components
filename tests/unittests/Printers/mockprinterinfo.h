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
        return m_printerName;
    }

    virtual QString description() const override
    {
        return m_description;
    }

    virtual QString location() const override
    {
        return m_location;
    }

    virtual QString makeAndModel() const override
    {
        return m_makeAndModel;
    }

    virtual State state() const override
    {
        return m_state;
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

    virtual QStringList availablePrinterNames() override
    {
        return m_availablePrinterNames;
    }

    virtual PrinterInfo* printerInfo(const QString &printerName) override
    {

    }

    QString m_printerName = QString::null;
    QString m_description = QString::null;
    QString m_location = QString::null;
    QString m_makeAndModel = QString::null;
    State m_state = State::IdleState;
    QStringList m_availablePrinterNames;
};

#endif // USC_PRINTERS_MOCK_PRINTERINFO_H
