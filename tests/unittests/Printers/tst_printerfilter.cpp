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

#include "mockbackend.h"

#include "models/printermodel.h"

#include <QObject>
#include <QSignalSpy>
#include <QScopedPointer>
#include <QTest>

Q_DECLARE_METATYPE(PrinterBackend*)
Q_DECLARE_METATYPE(PrinterJob*)

class TestPrinterFilter : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testEmptyCount()
    {
        QScopedPointer<PrinterBackend> backend(new MockPrinterBackend);
        PrinterModel model(backend.data());

        PrinterFilter filter;
        filter.setSourceModel(&model);
        QCOMPARE(filter.count(), model.count());
    }
    void testNonEmptyCount()
    {
        QScopedPointer<PrinterBackend> backend(new MockPrinterBackend);
        PrinterModel model(backend.data());

        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        PrinterBackend* printerBBackend = new MockPrinterBackend("b-printer");
        Printer printerA(printerABackend);
        Printer printerB(printerBBackend);

        ((MockPrinterBackend*) backend.data())->m_availablePrinters << &printerA << &printerB;

        PrinterFilter filter;
        filter.setSourceModel(&model);
        ((MockPrinterBackend*) backend.data())->mockPrinterAdded("Test added printer", "", printerA.name(), 0, "", true);

        QCOMPARE(filter.count(), 2);
    }
    void testCountChanged()
    {
        QScopedPointer<PrinterBackend> backend(new MockPrinterBackend);
        PrinterModel model(backend.data());

        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        PrinterBackend* printerBBackend = new MockPrinterBackend("b-printer");
        Printer printerA(printerABackend);
        Printer printerB(printerBBackend);

        ((MockPrinterBackend*) backend.data())->m_availablePrinters << &printerA << &printerB;

        PrinterFilter filter;
        filter.setSourceModel(&model);

        QSignalSpy modelCountSpy(&model, SIGNAL(countChanged()));
        QSignalSpy filterCountSpy(&filter, SIGNAL(countChanged()));
        ((MockPrinterBackend*) backend.data())->mockPrinterAdded("Test added printer", "", printerA.name(), 0, "", true);
        QCOMPARE(modelCountSpy.count(), 1);
        QCOMPARE(filterCountSpy.count(), 1);
    }
};

QTEST_GUILESS_MAIN(TestPrinterFilter)
#include "tst_printerfilter.moc"

