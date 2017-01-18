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
#include "printers/printers.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(CupsFacade*)
Q_DECLARE_METATYPE(PrinterInfo*)
Q_DECLARE_METATYPE(QList<PrinterInfo*>)

class TestPrinters : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInstantiation_data()
    {
        QTest::addColumn<CupsFacade*>("cups");
        QTest::addColumn<PrinterInfo*>("info");

        {
            CupsFacade* cups = new MockCupsFacade;
            PrinterInfo* info = new MockPrinterInfo;
            QTest::newRow("no printers") << cups << info;
        }
    }
    void testInstantiation()
    {
        QFETCH(CupsFacade*, cups);
        QFETCH(PrinterInfo*, info);

        Printers printers(info, cups);
    }
    void testAllPrintersFilter_data()
    {
        QTest::addColumn<QList<PrinterInfo*>>("in");
        QTest::addColumn<QList<PrinterInfo*>>("out");

        {
            auto in = QList<PrinterInfo*>();
            auto out = QList<PrinterInfo*>();

            auto a = new MockPrinterInfo("printer-a");
            auto b = new MockPrinterInfo("printer-b");

            in << a << b;
            out << a << b;

            QTest::newRow("no defaults") << in << out;
        }
        {
            auto in = QList<PrinterInfo*>();
            auto out = QList<PrinterInfo*>();

            auto a = new MockPrinterInfo("printer-a");
            auto b = new MockPrinterInfo("printer-b");
            b->m_defaultPrinterName = "printer-b";

            in << a << b;
            out << b << a;

            QTest::newRow("have default") << in << out;
        }
    }
    void testAllPrintersFilter()
    {
        QFETCH(QList<PrinterInfo*>, in);
        QFETCH(QList<PrinterInfo*>, out);

        CupsFacade* cups = new MockCupsFacade;
        PrinterInfo* info = new MockPrinterInfo;

        ((MockPrinterInfo*) info)->m_availablePrinters = in;

        Printers printers(info, cups, 100);
        auto all = printers.allPrinters();

        QTRY_COMPARE_WITH_TIMEOUT(all->rowCount(), out.size(), 101);
        for (int i = 0; i < all->rowCount(); i++) {
            QCOMPARE(
                 all->data(all->index(i, 0)).toString(),
                 out.at(i)->printerName()
            );
        }
    }
};

QTEST_GUILESS_MAIN(TestPrinters)
#include "tst_printers.moc"
