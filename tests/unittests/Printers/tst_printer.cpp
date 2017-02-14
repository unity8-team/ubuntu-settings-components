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

#include "mockbackend.h"

#include "backend/backend.h"
#include "backend/backend_pdf.h"
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
        m_backend = new MockPrinterBackend(m_printerName);
        m_instance = new Printer(m_backend);
    }
    void cleanup()
    {
        QSignalSpy destroyedSpy(m_instance, SIGNAL(destroyed(QObject*)));
        m_instance->deleteLater();
        QTRY_COMPARE(destroyedSpy.count(), 1);
        delete m_backend;
    }
    void testName()
    {
        QCOMPARE(m_backend->printerName(), m_instance->name());
    }
    void testDescription()
    {
        getBackend()->m_description = "some description";
        QCOMPARE(m_backend->description(), m_instance->description());
    }
    void testSetDescription()
    {
        QString desc("another description");
        m_instance->setDescription(desc);
        QCOMPARE(getBackend()->infos.value(m_printerName), desc);
    }
    void testSupportedDuplexModes_data()
    {
        QTest::addColumn<QList<PrinterEnum::DuplexMode>>("modes");
        QTest::addColumn<QStringList>("strings");
        {
            QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone});
            QStringList strings({"One Sided"});
            QTest::newRow("one") << modes << strings;
        }
        {
            QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone, PrinterEnum::DuplexMode::DuplexLongSide});
            QStringList strings({"One Sided", "Long Edge (Standard)"});
            QTest::newRow("multiple") << modes << strings;
        }
    }
    void testSupportedDuplexModes()
    {
        QFETCH(QList<PrinterEnum::DuplexMode>, modes);
        QFETCH(QStringList, strings);
        getBackend()->m_supportedDuplexModes = modes;
        QCOMPARE(m_instance->supportedDuplexModes(), modes);
        QCOMPARE(m_instance->supportedDuplexStrings(), strings);
    }
    void testDefaultDuplexMode()
    {
        QList<PrinterEnum::DuplexMode> modes({
            PrinterEnum::DuplexMode::DuplexNone,
            PrinterEnum::DuplexMode::DuplexLongSide,
            PrinterEnum::DuplexMode::DuplexShortSide});
        getBackend()->m_supportedDuplexModes = modes;

        m_instance->setDefaultDuplexMode(PrinterEnum::DuplexMode::DuplexLongSide);
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexLongSide);
        m_instance->setDefaultDuplexMode(PrinterEnum::DuplexMode::DuplexNone);
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexNone);
        m_instance->setDefaultDuplexMode(PrinterEnum::DuplexMode::DuplexShortSide);
        QCOMPARE(m_instance->defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexShortSide);
    }
    void testSetDefaultDuplexMode()
    {
        // Add support
        QList<PrinterEnum::DuplexMode> modes({PrinterEnum::DuplexMode::DuplexNone, PrinterEnum::DuplexMode::DuplexLongSide});
        getBackend()->m_supportedDuplexModes = modes;

        m_instance->setDefaultDuplexMode(PrinterEnum::DuplexMode::DuplexLongSide);

        QVariant duplexVar = getBackend()->printerOptions[m_printerName].value("Duplex");
        QStringList duplexVals = duplexVar.toStringList();
         QCOMPARE(
            duplexVals.at(0),
            (QString) Utils::duplexModeToPpdChoice(PrinterEnum::DuplexMode::DuplexLongSide)
        );
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
    void testDefaultPageSize()
    {
        auto targetSize = QPageSize(QPageSize::A4);
        getBackend()->m_defaultPageSize = targetSize;
        QCOMPARE(m_instance->defaultPageSize(), targetSize);
    }
    void testSupportedPageSizes_data()
    {
        auto supported = QList<QPageSize>({QPageSize(QPageSize::A4), QPageSize(QPageSize::Letter)});
        getBackend()->m_supportedPageSizes = supported;
        QCOMPARE(m_instance->supportedPageSizes(), supported);
    }
    void testSetDefaultPageSize()
    {
        QFETCH(QList<QPageSize>, sizes);
        QFETCH(QPageSize, size);
        QFETCH(bool, expectCupsCommunication);
        QFETCH(QString, expectedValue);
        getBackend()->m_supportedPageSizes = sizes;

        m_instance->setDefaultPageSize(size);

        QVariant pageSizeVar = getBackend()->printerOptions[m_printerName].value("PageSize");
        QStringList pageSizeVals = pageSizeVar.toStringList();

        if (expectCupsCommunication) {
            QCOMPARE(pageSizeVals.at(0), expectedValue);
        }
    }
    void testDefaultColorMode()
    {
        ColorModel colorModel;
        QCOMPARE(m_instance->defaultColorModel(), colorModel);
    }
    void testSupportedColorModels()
    {
        ColorModel a;
        a.text = "Gray";

        ColorModel b;
        b.text = "RBG";
        QList<ColorModel> models({a, b});

        PrinterBackend *backend = new MockPrinterBackend(m_printerName);
        ((MockPrinterBackend*) backend)->printerOptions[m_printerName].insert(
            "SupportedColorModels", QVariant::fromValue(models));
        Printer p(backend);
        QCOMPARE(p.supportedColorModels(), models);
    }
    void testDefaultPrintQuality()
    {
        PrintQuality quality;
        QCOMPARE(m_instance->defaultPrintQuality(), quality);
    }
    void testSupportedQualities()
    {
        PrintQuality a;
        a.name = "Poor";

        PrintQuality b;
        b.name = "Worse";
        QList<PrintQuality> qualities({a, b});

        PrinterBackend *backend = new MockPrinterBackend(m_printerName);
        ((MockPrinterBackend*) backend)->printerOptions[m_printerName].insert(
            "SupportedPrintQualities", QVariant::fromValue(qualities));
        Printer p(backend);
        QCOMPARE(p.supportedPrintQualities(), qualities);
    }
    void testPdfPrinter()
    {
        PrinterBackend *backend = new PrinterPdfBackend(m_printerName);
        Printer p(backend);
        QCOMPARE(p.defaultColorModel().name, QString("RGB"));
        QCOMPARE(p.defaultPageSize(), QPageSize(QPageSize::A4));
        QCOMPARE(p.defaultDuplexMode(), PrinterEnum::DuplexMode::DuplexNone);
        QCOMPARE(p.type(), PrinterEnum::PrinterType::PdfType);
    }
    void testState()
    {
        getBackend()->m_state = PrinterEnum::State::AbortedState;
        QCOMPARE(m_instance->state(), getBackend()->m_state);
    }
    void testEnabled()
    {

    }
    void testSetEnabled_data()
    {

    }
    void testSetEnabled()
    {

    }
    void testAcceptJobs()
    {

    }
    void testSetAcceptJobs_data()
    {

    }
    void testSetAcceptJobs()
    {

    }
    void testHoldsDefinition()
    {

    }

private:
    QString m_printerName = "my-printer";
    PrinterBackend *m_backend = nullptr;
    Printer *m_instance = nullptr;
    MockPrinterBackend* getBackend() {
        return (MockPrinterBackend*) m_backend;
    }
};

QTEST_GUILESS_MAIN(TestPrinter)
#include "tst_printer.moc"
