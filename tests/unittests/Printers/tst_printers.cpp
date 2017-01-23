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

#include "backend/backend.h"
#include "printers/printers.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(PrinterBackend*)
Q_DECLARE_METATYPE(QList<Printer*>)

class TestPrinters : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInstantiation_data()
    {
        QTest::addColumn<PrinterBackend*>("backend");

        {
            PrinterBackend* backend = new MockPrinterBackend;
            QTest::newRow("no printers") << backend;
        }
    }
    void testInstantiation()
    {
        QFETCH(PrinterBackend*, backend);
        Printers printers(backend);
    }
    void testAllPrintersFilter_data()
    {
        QTest::addColumn<QList<Printer*>>("in");
        QTest::addColumn<QList<Printer*>>("out");

        {
            auto in = QList<Printer*>();
            auto out = QList<Printer*>();

            auto aBackend = new MockPrinterBackend("printer-a");
            auto a = new Printer(aBackend);
            auto bBackend = new MockPrinterBackend("printer-b");
            auto b = new Printer(bBackend);

            in << a << b;
            out << a << b;

            QTest::newRow("no defaults") << in << out;
        }
        {
            auto in = QList<Printer*>();
            auto out = QList<Printer*>();

            auto aBackend = new MockPrinterBackend("printer-a");
            auto a = new Printer(aBackend);
            auto bBackend = new MockPrinterBackend("printer-b");
            auto b = new Printer(bBackend);
            bBackend->m_defaultPrinterName = "printer-b";

            in << a << b;
            out << b << a;

            QTest::newRow("have default") << in << out;
        }
    }
    void testAllPrintersFilter()
    {
        QFETCH(QList<Printer*>, in);
        QFETCH(QList<Printer*>, out);

        PrinterBackend* backend = new MockPrinterBackend;
        ((MockPrinterBackend*) backend)->m_availablePrinters = in;

        Printers printers(backend, 100);
        auto all = printers.allPrinters();

        QTRY_COMPARE_WITH_TIMEOUT(all->rowCount(), out.size(), 101);
        for (int i = 0; i < all->rowCount(); i++) {
            QCOMPARE(
                 all->data(all->index(i, 0)).toString(),
                 out.at(i)->name()
            );
        }
    }
};

QTEST_GUILESS_MAIN(TestPrinters)
#include "tst_printers.moc"
