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

#include "utils.h"

#include "mockcupsfacade.h"
#include "mockprinterinfo.h"

#include "cups/cupsfacade.h"
#include "printer/printer.h"

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(QList<QPageSize>)

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
    void testSupportedDuplexModes_data()
    {
        QTest::addColumn<QList<PrinterEnum::DuplexMode>>("modes");
        QTest::addColumn<QStringList>("strings");
        {
            QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone});
            QStringList strings({"None"});
            QTest::newRow("one") << modes << strings;
        }
        {
            QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone, PrinterEnum::DuplexMode::DuplexLongSide});
            QStringList strings({"None", "Long edge (standard)"});
            QTest::newRow("multiple") << modes << strings;
        }
    }
    void testSupportedDuplexModes()
    {
        QFETCH(QList<PrinterEnum::DuplexMode>, modes);
        QFETCH(QStringList, strings);
        ((MockPrinterInfo*) m_mockinfo)->m_supportedDuplexModes = modes;
        QCOMPARE(m_instance->supportedDuplexModes(), modes);
        QCOMPARE(m_instance->supportedDuplexStrings(), strings);
    }
    void testDefaultDuplexMode()
    {
        ((MockPrinterInfo*) m_mockinfo)->m_defaultDuplexMode = PrinterEnum::DuplexMode::DuplexLongSide;
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexLongSide);
        ((MockPrinterInfo*) m_mockinfo)->m_defaultDuplexMode = PrinterEnum::DuplexMode::DuplexNone;
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexNone);
        ((MockPrinterInfo*) m_mockinfo)->m_defaultDuplexMode = PrinterEnum::DuplexMode::DuplexShortSide;
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexShortSide);
    }
    void testSetDefaultDuplexMode()
    {
        ((MockPrinterInfo*) m_mockinfo)->m_defaultDuplexMode = PrinterEnum::DuplexMode::DuplexNone;

        // Add support
        QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone, PrinterEnum::DuplexMode::DuplexLongSide});
        ((MockPrinterInfo*) m_mockinfo)->m_supportedDuplexModes = modes;

        m_instance->setDefaultDuplexMode(PrinterEnum::DuplexMode::DuplexLongSide);

        QVariant duplexVar = ((MockCupsFacade*) m_mockcups)->printerOptions[m_printerName].value("Duplex");
        QStringList duplexVals = duplexVar.toStringList();
         QCOMPARE(
            duplexVals.at(0),
            (QString) Utils::duplexModeToPpdChoice(PrinterEnum::DuplexMode::DuplexLongSide)
        );
    }
    void testDefaultPageSize()
    {
        auto targetSize = QPageSize(QPageSize::A4);
        ((MockPrinterInfo*) m_mockinfo)->m_defaultPageSize = targetSize;
        QCOMPARE(m_instance->defaultPageSize(), targetSize);
    }
    void testSupportedPageSizes_data()
    {
        auto supported = QList<QPageSize>({QPageSize(QPageSize::A4), QPageSize(QPageSize::Letter)});
        ((MockPrinterInfo*) m_mockinfo)->m_supportedPageSizes = supported;
        QCOMPARE(m_instance->supportedPageSizes(), supported);
    }
    void testSetDefaultPageSize_data()
    {
        QTest::addColumn<QList<QPageSize>>("sizes");
        QTest::addColumn<QPageSize>("size");
        QTest::addColumn<bool>("expectCupsCommunication");
        QTest::addColumn<QString>("expectedValue");
        {
            QList<QPageSize> sizes; // none supported
            QPageSize size(QPageSize::A4);
            QTest::newRow("unsupported") << sizes << size << false << "";
        }
        {
            QList<QPageSize> sizes({QPageSize(QPageSize::A4)});
            QPageSize size(QPageSize::A4);
            QTest::newRow("supported") << sizes << size << true << "A4";
        }
        {
            QPageSize custom(QSize(100, 100), "foo");
            QList<QPageSize> sizes({custom});
            QTest::newRow("supported, but non-ppd size") << sizes << custom << false << "";
        }
    }
    void testSetDefaultPageSize()
    {
        QFETCH(QList<QPageSize>, sizes);
        QFETCH(QPageSize, size);
        QFETCH(bool, expectCupsCommunication);
        QFETCH(QString, expectedValue);
        ((MockPrinterInfo*) m_mockinfo)->m_supportedPageSizes = sizes;

        m_instance->setDefaultPageSize(size);

        QVariant pageSizeVar = ((MockCupsFacade*) m_mockcups)->printerOptions[m_printerName].value("PageSize");
        QStringList pageSizeVals = pageSizeVar.toStringList();

        if (expectCupsCommunication) {
            QCOMPARE(pageSizeVals.at(0), expectedValue);
        }
    }
    void testDefaultColorMode()
    {
        ColorModel colorModel;
        ((MockCupsFacade*) m_mockcups)->printerOptions[m_printerName].insert(
            "DefaultColorModel", QVariant::fromValue(colorModel));

        QCOMPARE(m_instance->defaultColorModel(), colorModel);
    }
    void testSupportedColorModels()
    {
        ColorModel a;
        a.text = "Gray";

        ColorModel b;
        b.text = "RBG";
        QList<ColorModel> models({a, b});

        PrinterInfo *info = new MockPrinterInfo(m_printerName);
        CupsFacade *cups = new MockCupsFacade;
        ((MockCupsFacade*) cups)->printerOptions[m_printerName].insert(
            "ColorModels", QVariant::fromValue(models));
        Printer p(info, cups);
        QCOMPARE(p.supportedColorModels(), models);
    }
    void testIsDefault_data()
    {
        QTest::addColumn<bool>("isDefault");
        QTest::newRow("default") << true;
        QTest::newRow("not default") << false;
    }
    void testIsDefault()
    {
        QFETCH(bool, isDefault);
        ((MockPrinterInfo*) m_mockinfo)->m_defaultPrinterName = isDefault ? m_printerName : "some-other";
        QCOMPARE(m_instance->isDefault(), isDefault);
    }
private:
    QString m_printerName = "my-printer";
    CupsFacade *m_mockcups = nullptr;
    Printer *m_instance = nullptr;
    PrinterInfo *m_mockinfo = nullptr;
};

QTEST_GUILESS_MAIN(TestPrinter)
#include "tst_printer.moc"

