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
        m_model = new PrinterModel(m_backend);
    }
    void cleanup()
    {
        QSignalSpy destroyedSpy(m_model, SIGNAL(destroyed(QObject*)));
        m_model->deleteLater();
        QTRY_COMPARE(destroyedSpy.count(), 1);
        delete m_backend;
    }
    void testInsertOnPrintersLoaded()
    {
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy insertSpy(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)));

        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        auto printerA = QSharedPointer<Printer>(new Printer(printerABackend));

        getBackend()->mockPrinterLoaded(printerA);

        QCOMPARE(m_model->count(), 1);
        QCOMPARE(countSpy.count(), 1);
        QCOMPARE(insertSpy.count(), 1);

        // Check item was inserted at end
        QList<QVariant> args = insertSpy.takeFirst();
        QCOMPARE(args.at(1).toInt(), 0);
        QCOMPARE(args.at(2).toInt(), 0);
    }
    void testInsertOnPrinterAddedFromNotification()
    {
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy insertSpy(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)));

        PrinterBackend* printerABackend = new MockPrinterBackend("a-printer");
        auto printerA = QSharedPointer<Printer>(new Printer(printerABackend));

        getBackend()->mockPrinterLoaded(printerA);

        QCOMPARE(m_model->count(), 1);
        QCOMPARE(countSpy.count(), 1);
        QCOMPARE(insertSpy.count(), 1);

        // Check item was inserted at end
        QList<QVariant> args = insertSpy.takeFirst();
        QCOMPARE(args.at(1).toInt(), 0);
        QCOMPARE(args.at(2).toInt(), 0);
    }
    void testRemoveOnPrinterDeletedFromNotification()
    {
        // Setup two printers
        PrinterBackend *printerABackend = new MockPrinterBackend("a-printer");
        auto printerA = QSharedPointer<Printer>(new Printer(printerABackend));
        PrinterBackend *printerBBackend = new MockPrinterBackend("b-printer");
        auto printerB = QSharedPointer<Printer>(new Printer(printerBBackend));

        getBackend()->mockPrinterLoaded(printerA);
        getBackend()->mockPrinterLoaded(printerB);

        QCOMPARE(m_model->count(), 2);

        // Setup spy and remove a printer
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

        getBackend()->mockPrinterDeleted("Test removed printer", "", printerB->name(), 0, "", true);

        QCOMPARE(m_model->count(), 1);
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
        auto printerA = QSharedPointer<Printer>(new Printer(printerABackend));
        MockPrinterBackend *printerBBackend = new MockPrinterBackend("b-printer");
        auto printerB = QSharedPointer<Printer>(new Printer(printerBBackend));
        MockPrinterBackend *printerCBackend = new MockPrinterBackend("c-printer");
        auto printerC = QSharedPointer<Printer>(new Printer(printerCBackend));
        MockPrinterBackend *printerDBackend = new MockPrinterBackend("d-printer");
        auto printerD = QSharedPointer<Printer>(new Printer(printerDBackend));

        getBackend()->mockPrinterLoaded(printerA);
        getBackend()->mockPrinterLoaded(printerB);
        getBackend()->mockPrinterLoaded(printerC);
        getBackend()->mockPrinterLoaded(printerD);

        QCOMPARE(m_model->count(), 4);

        // Setup spy and remove middle two printers
        QSignalSpy countSpy(m_model, SIGNAL(countChanged()));
        QSignalSpy removeSpy(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)));

        getBackend()->mockPrinterDeleted("Test removed printer", "", printerB->name(), 0, "", true);
        getBackend()->mockPrinterDeleted("Test removed printer", "", printerC->name(), 0, "", true);

        QCOMPARE(countSpy.count(), 2);
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
    MockPrinterBackend* getBackend()
    {
        return (MockPrinterBackend*) m_backend;
    }
    PrinterBackend *m_backend;
    PrinterModel *m_model;
};

QTEST_GUILESS_MAIN(TestPrinterModel)
#include "tst_printermodel.moc"

