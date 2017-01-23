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
#include "models/printermodel.h"
#include "printer/printer.h"
#include "printer/printerjob.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(PrinterBackend*)
Q_DECLARE_METATYPE(PrinterJob*)

class TestPrinterModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init()
    {
        m_backend = new MockPrinterBackend;
        m_model = new PrinterModel(m_backend, 100);
    }
    void cleanup()
    {
        QSignalSpy destroyedSpy(m_model, SIGNAL(destroyed(QObject*)));
        m_model->deleteLater();
        QTRY_COMPARE(destroyedSpy.count(), 1);
        delete m_backend;
    }
    void testInstantiation_data()
    {
        QTest::addColumn<PrinterBackend*>("backend");
        QTest::addColumn<int>("targetCount");

        {
            PrinterBackend* backend = new MockPrinterBackend;
            QTest::newRow("no printers") << backend  << 0;
        }
        {
            PrinterBackend* backend = new MockPrinterBackend;

            PrinterBackend *printerABackend = new MockPrinterBackend("a-printer");
            Printer* printerA = new Printer(printerABackend);

            ((MockPrinterBackend*) backend)->m_availablePrinters << printerA;

            QTest::newRow("some printers") << backend << 1;
        }
    }
    void testInstantiation()
    {
        QFETCH(PrinterBackend*, backend);
        QFETCH(int, targetCount);

        PrinterModel model(backend);
        QCOMPARE(model.count(), targetCount);
    }
    void testUpdateInsert()
    {
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy insertSpy(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)));

        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        Printer* printerA = new Printer(printerABackend);
        ((MockPrinterBackend*) m_backend)->m_availablePrinters << printerA;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 1, 500);
        QCOMPARE(countSpy.count(), 1);
        QCOMPARE(insertSpy.count(), 1);

        // Check item was inserted at end
        QList<QVariant> args = insertSpy.takeFirst();
        QCOMPARE(args.at(1).toInt(), 0);
        QCOMPARE(args.at(2).toInt(), 0);
    }
    void testUpdateMove()
    {
        // Setup two printers
        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        Printer* printerA = new Printer(printerABackend);
        PrinterBackend* printerBBackend = new MockPrinterBackend("b-printer");
        Printer* printerB = new Printer(printerBBackend);
        ((MockPrinterBackend*) m_backend)->m_availablePrinters << printerA << printerB;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 2, 500);

        // Setup spy and move a printer
        int from = 1;
        int to = 0;
        ((MockPrinterBackend*) m_backend)->m_availablePrinters.move(from, to);

        // Check signals were fired
        QSignalSpy moveSpy(m_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)));
        QTRY_COMPARE_WITH_TIMEOUT(moveSpy.count(), 1, 500);
        QCOMPARE(m_model->count(), 2);

        // Check item was moved from -> to
        QList<QVariant> args = moveSpy.at(0);
        QCOMPARE(args.at(1).toInt(), from);
        QCOMPARE(args.at(2).toInt(), from);
        QCOMPARE(args.at(4).toInt(), to);
    }
    void testUpdateRemove()
    {
        // Setup two printers
        PrinterBackend *printerABackend = new MockPrinterBackend("a-printer");
        Printer* printerA = new Printer(printerABackend);
        PrinterBackend *printerBBackend = new MockPrinterBackend("b-printer");
        Printer* printerB = new Printer(printerBBackend);
        ((MockPrinterBackend*) m_backend)->m_availablePrinters << printerA << printerB;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 2, 500);

        // Setup spy and remove a printer
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));
        ((MockPrinterBackend*) m_backend)->m_availablePrinters.removeLast();

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 1, 500);
        QCOMPARE(countSpy.count(), 1);
        QCOMPARE(removeSpy.count(), 1);

        // Check item was removed from end
        QList<QVariant> args = removeSpy.at(0);
        QCOMPARE(args.at(1).toInt(), 1);
        QCOMPARE(args.at(2).toInt(), 1);
    }
    void testUpdateRemoveMulti()
    {
        // Setup four printers
        MockPrinterBackend *printerABackend = new MockPrinterBackend("a-printer");
        Printer* printerA = new Printer(printerABackend);
        MockPrinterBackend *printerBBackend = new MockPrinterBackend("b-printer");
        Printer* printerB = new Printer(printerBBackend);
        MockPrinterBackend *printerCBackend = new MockPrinterBackend("c-printer");
        Printer* printerC = new Printer(printerCBackend);
        MockPrinterBackend *printerDBackend = new MockPrinterBackend("d-printer");
        Printer* printerD = new Printer(printerDBackend);
        ((MockPrinterBackend*) m_backend)->m_availablePrinters << printerA << printerB << printerC << printerD;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 4, 500);

        // Setup spy and remove middle two printers
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));
        ((MockPrinterBackend*) m_backend)->m_availablePrinters.removeAt(2);
        ((MockPrinterBackend*) m_backend)->m_availablePrinters.removeAt(1);

        // Wait until one count signal has been fired
        // (to ensure this is only one iteration of update)
        QTRY_COMPARE_WITH_TIMEOUT(countSpy.count(), 1, 500);
        QCOMPARE(m_model->count(), 2);
        QCOMPARE(removeSpy.count(), 2);

        // Check items were removed from the middle
        QList<QVariant> args;
        args = removeSpy.at(0);
        QCOMPARE(args.at(1).toInt(), 1);
        QCOMPARE(args.at(2).toInt(), 1);

        args = removeSpy.at(1);
        QCOMPARE(args.at(1).toInt(), 1);
        QCOMPARE(args.at(2).toInt(), 1);
    }
private:
    PrinterBackend *m_backend;
    PrinterModel *m_model;
};

QTEST_GUILESS_MAIN(TestPrinterModel)
#include "tst_printermodel.moc"

