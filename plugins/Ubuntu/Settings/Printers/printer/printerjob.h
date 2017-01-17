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

#ifndef USC_PRINTERS_PRINTERJOB_H
#define USC_PRINTERS_PRINTERJOB_H

#include "printers_global.h"

#include "enums.h"
#include "structs.h"

#include "cups/cupsfacade.h"
#include "printer/printer.h"

#include <QtCore/QObject>

class CupsFacade;
class Printer;

class PRINTERS_DECL_EXPORT PrinterJob : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ColorModel colorModel READ colorModel WRITE setColorModel NOTIFY colorModelChanged)
    Q_PROPERTY(int copies READ copies WRITE setCopies NOTIFY copiesChanged)
    Q_PROPERTY(bool duplex READ duplex WRITE setDuplex NOTIFY duplexChanged)
    Q_PROPERTY(bool landscape READ landscape WRITE setLandscape NOTIFY landscapeChanged)
//    Q_PROPERTY(Printer *printer READ printer WRITE setPrinter NOTIFY printerChanged)
    Q_PROPERTY(QString printerName READ printerName WRITE setPrinterName NOTIFY printerNameChanged)
    Q_PROPERTY(QString printRange READ printRange WRITE setPrintRange NOTIFY printRangeChanged)
    Q_PROPERTY(PrinterEnum::PrintRange printRangeMode READ printRangeMode WRITE setPrintRangeMode NOTIFY printRangeModeChanged)
    Q_PROPERTY(PrinterEnum::Quality quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(PrinterEnum::State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
public:
    explicit PrinterJob(QObject *parent=Q_NULLPTR);
    explicit PrinterJob(Printer *printer, QObject *parent=Q_NULLPTR);
    ~PrinterJob();

    ColorModel colorModel() const;
    int copies() const;
    bool duplex() const;
    bool landscape() const;
//    Printer *printer() const;
    QString printerName() const;
    QString printRange() const;
    PrinterEnum::PrintRange printRangeMode() const;
    PrinterEnum::Quality quality() const;
    PrinterEnum::State state() const;
    QString title() const;
public Q_SLOTS:
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void printFile(const QUrl &url);
    void setColorModel(const ColorModel &colorModel);
    void setCopies(const int copies);
    void setDuplex(const bool duplex);
    void setLandscape(const bool landscape);
//    void setPrinter(Printer *printer);
    void setPrinterName(const QString &printerName);
    void setPrintRange(const QString &printRange);
    void setPrintRangeMode(const PrinterEnum::PrintRange printRangeMode);
    void setQuality(const PrinterEnum::Quality &quality);
    void setTitle(const QString &title);
private Q_SLOTS:
    void loadDefaults();
    void setState(const PrinterEnum::State &state);
Q_SIGNALS:
    void colorModelChanged();
    void copiesChanged();
    void duplexChanged();
    void landscapeChanged();
//    void printerChanged();
    void printerNameChanged();
    void printRangeChanged();
    void printRangeModeChanged();
    void qualityChanged();
    void stateChanged();
    void titleChanged();
private:
    ColorModel m_color_model;
    int m_copies;
    CupsFacade *m_cups;
    bool m_duplex;
    bool m_landscape;
    Printer *m_printer;
    QString m_printer_name;
    QString m_print_range;
    PrinterEnum::PrintRange m_print_range_mode;
    PrinterEnum::Quality m_quality;
    PrinterEnum::State m_state;
    QString m_title;
};

#endif // USC_PRINTERS_PRINTERJOB_H
