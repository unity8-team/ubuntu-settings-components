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
#include "printer/printer.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

class TestPrinter : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init()
    {
        m_mockcups = new MockCupsFacade;
        m_mockinfo = new MockPrinterInfo(m_printerName);
        m_instance = new Printer(m_mockinfo, m_mockcups);
    }
    void cleanup()
    {
        QSignalSpy destroyedSpy(m_instance, SIGNAL(destroyed(QObject*)));
        m_instance->deleteLater();
        QTRY_COMPARE(destroyedSpy.count(), 1);
        delete m_mockinfo;
        delete m_mockcups;
    }
    void testName()
    {
        QCOMPARE(m_mockinfo->printerName(), m_instance->name());
    }
    void testDescription()
    {
        ((MockPrinterInfo*) m_mockinfo)->m_description = "some description";
        QCOMPARE(m_mockinfo->description(), m_instance->description());
    }
    void testSetDescription()
    {
        QString desc("another description");
        m_instance->setDescription(desc);
        QCOMPARE(((MockCupsFacade*) m_mockcups)->infos.value(m_printerName), desc);
    }
private:
    QString m_printerName = "my-printer";
    CupsFacade *m_mockcups = nullptr;
    Printer *m_instance = nullptr;
    PrinterInfo *m_mockinfo = nullptr;
};

QTEST_GUILESS_MAIN(TestPrinter)
#include "tst_printer.moc"

