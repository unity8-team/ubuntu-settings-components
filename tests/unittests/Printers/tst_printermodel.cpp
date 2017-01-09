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

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(CupsFacade*)
Q_DECLARE_METATYPE(PrinterInfo*)

class TestPrinterModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInstantiation_data()
    {
        QTest::addColumn<CupsFacade*>("cups");
        QTest::addColumn<PrinterInfo*>("info");
        QTest::addColumn<int>("rowCount");

        {
            CupsFacade* cups = new MockCupsFacade;
            PrinterInfo* info = new MockPrinterInfo;
            QTest::newRow("no printers") << cups << info << 0;
        }
        {
            CupsFacade* cups = new MockCupsFacade;
            PrinterInfo* info = new MockPrinterInfo;
            ((MockPrinterInfo*) info)->m_availablePrinterNames << "my-printer";
            QTest::newRow("some printers") << cups << info << 1;
        }
    }
    void testInstantiation()
    {
        QFETCH(CupsFacade*, cups);
        QFETCH(PrinterInfo*, info);
        QFETCH(int, rowCount);

        PrinterModel model(info, cups);
        QCOMPARE(model.rowCount(), rowCount);
    }
};

QTEST_GUILESS_MAIN(TestPrinterModel)
#include "tst_printermodel.moc"

