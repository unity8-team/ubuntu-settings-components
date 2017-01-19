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

#include "mockcupsfacade.h"
#include "mockprinterinfo.h"

#include "models/printermodel.h"

#include <QObject>
#include <QSignalSpy>
#include <QScopedPointer>
#include <QTest>

Q_DECLARE_METATYPE(CupsFacade*)
Q_DECLARE_METATYPE(PrinterInfo*)
Q_DECLARE_METATYPE(PrinterJob*)

class TestPrinterFilter : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testEmptyCount()
    {
        QScopedPointer<CupsFacade> cups(new MockCupsFacade);
        QScopedPointer<PrinterInfo> info(new MockPrinterInfo);
        PrinterModel model(info.data(), cups.data(), 100);

        PrinterFilter filter;
        filter.setSourceModel(&model);
        QCOMPARE(filter.count(), model.count());
    }
    void testNonEmptyCount()
    {
        QScopedPointer<CupsFacade> cups(new MockCupsFacade);
        QScopedPointer<PrinterInfo> info(new MockPrinterInfo);
        PrinterModel model(info.data(), cups.data(), 100);

        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        PrinterInfo* printerB = new MockPrinterInfo("b-printer");

        ((MockPrinterInfo*) info.data())->m_availablePrinters << printerA << printerB;

        PrinterFilter filter;
        filter.setSourceModel(&model);

        QTRY_COMPARE_WITH_TIMEOUT(filter.count(), 2, 101);
    }
    void testCountChanged()
    {
        QScopedPointer<CupsFacade> cups(new MockCupsFacade);
        QScopedPointer<PrinterInfo> info(new MockPrinterInfo);
        PrinterModel model(info.data(), cups.data(), 100);

        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        PrinterInfo* printerB = new MockPrinterInfo("b-printer");

        ((MockPrinterInfo*) info.data())->m_availablePrinters << printerA << printerB;

        PrinterFilter filter;
        filter.setSourceModel(&model);

        QSignalSpy modelCountSpy(&model, SIGNAL(countChanged()));
        QSignalSpy filterCountSpy(&filter, SIGNAL(countChanged()));
        QTRY_COMPARE_WITH_TIMEOUT(modelCountSpy.count(), 1, 101);
        QTRY_COMPARE_WITH_TIMEOUT(filterCountSpy.count(), 1, 101);
    }
};

QTEST_GUILESS_MAIN(TestPrinterFilter)
#include "tst_printerfilter.moc"

