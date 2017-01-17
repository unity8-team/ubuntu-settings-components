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

#include "cups/cupsfacade.h"
#include "models/printermodel.h"
#include "printer/printer.h"
#include "printer/printerjob.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(CupsFacade*)
Q_DECLARE_METATYPE(PrinterInfo*)
Q_DECLARE_METATYPE(PrinterJob*)

class TestPrinterModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInstantiation_data()
    {
        QTest::addColumn<CupsFacade*>("cups");
        QTest::addColumn<PrinterInfo*>("info");
        QTest::addColumn<int>("targetCount");

        {
            CupsFacade* cups = new MockCupsFacade;
            PrinterInfo* info = new MockPrinterInfo;
            QTest::newRow("no printers") << cups << info << 0;
        }
        {
            CupsFacade* cups = new MockCupsFacade;
            PrinterInfo* info = new MockPrinterInfo;

            PrinterInfo* printerA = new MockPrinterInfo("a-printer");
            PrinterInfo* printerB = new MockPrinterInfo("b-printer");

            ((MockPrinterInfo*) info)->m_availablePrinters << printerA;

            QTest::newRow("some printers") << cups << info << 1;
        }
    }
    void testInstantiation()
    {
        QFETCH(CupsFacade*, cups);
        QFETCH(PrinterInfo*, info);
        QFETCH(int, targetCount);

        PrinterModel model(info, cups);
        QCOMPARE(model.count(), targetCount);
    }
};

QTEST_GUILESS_MAIN(TestPrinterModel)
#include "tst_printermodel.moc"

