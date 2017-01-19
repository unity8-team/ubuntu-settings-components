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
    void init()
    {
        m_mock_cups = new MockCupsFacade;
        m_mock_info = new MockPrinterInfo;
        m_model = new PrinterModel(m_mock_info, m_mock_cups, 100);
    }
    void cleanup()
    {
        QSignalSpy destroyedSpy(m_model, SIGNAL(destroyed(QObject*)));
        m_model->deleteLater();
        QTRY_COMPARE(destroyedSpy.count(), 1);

        delete m_mock_info;
        delete m_mock_cups;
    }
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
    void testUpdateInsert()
    {
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy insertSpy(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)));

        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters << printerA;

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
        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        PrinterInfo* printerB = new MockPrinterInfo("b-printer");
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters << printerA << printerB;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 2, 500);

        // Setup spy and move a printer
        int from = 1;
        int to = 0;
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters.move(from, to);

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
        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        PrinterInfo* printerB = new MockPrinterInfo("b-printer");
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters << printerA << printerB;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 2, 500);

        // Setup spy and remove a printer
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters.removeLast();

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
        PrinterInfo* printerA = new MockPrinterInfo("a-printer");
        PrinterInfo* printerB = new MockPrinterInfo("b-printer");
        PrinterInfo* printerC = new MockPrinterInfo("c-printer");
        PrinterInfo* printerD = new MockPrinterInfo("d-printer");
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters << printerA << printerB << printerC << printerD;

        QTRY_COMPARE_WITH_TIMEOUT(m_model->count(), 4, 500);

        // Setup spy and remove middle two printers
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters.removeAt(2);
        ((MockPrinterInfo*) m_mock_info)->m_availablePrinters.removeAt(1);

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
    CupsFacade *m_mock_cups;
    PrinterInfo *m_mock_info;
    PrinterModel *m_model;
};

QTEST_GUILESS_MAIN(TestPrinterModel)
#include "tst_printermodel.moc"

