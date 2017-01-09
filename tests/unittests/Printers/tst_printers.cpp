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

class TestPrinters : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    // void init()
    // {
    //     m_mockcups = new MockCupsFacade;
    //     m_mockinfo = new MockPrinterInfo;
    //     m_instance = new Printers(m_mockinfo, m_mockcups);
    // }
    // void cleanup()
    // {
    //     QSignalSpy destroyedSpy(m_instance, SIGNAL(destroyed(QObject*)));
    //     m_instance->deleteLater();
    //     QTRY_COMPARE(destroyedSpy.count(), 1);
    //     delete m_mockinfo;
    //     delete m_mockcups;
    // }
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
};


QTEST_GUILESS_MAIN(TestPrinters)
#include "tst_printers.moc"
