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

#include "backend/backend.h"
#include "printer/printer.h"

#include <QtCore/QObject>

class Printer;
class PrinterBackend;

class PRINTERS_DECL_EXPORT PrinterJob : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool collate READ collate WRITE setCollate NOTIFY collateChanged)
    Q_PROPERTY(int colorModel READ colorModel WRITE setColorModel NOTIFY colorModelChanged)
    Q_PROPERTY(PrinterEnum::ColorModelType colorModelType READ colorModelType NOTIFY colorModelTypeChanged)
    Q_PROPERTY(int copies READ copies WRITE setCopies NOTIFY copiesChanged)
    Q_PROPERTY(int duplexMode READ duplexMode WRITE setDuplexMode NOTIFY duplexModeChanged)
    Q_PROPERTY(bool isTwoSided READ isTwoSided NOTIFY isTwoSidedChanged)
    Q_PROPERTY(bool landscape READ landscape WRITE setLandscape NOTIFY landscapeChanged)
//    Q_PROPERTY(Printer *printer READ printer WRITE setPrinter NOTIFY printerChanged)
    Q_PROPERTY(QString printerName READ printerName WRITE setPrinterName NOTIFY printerNameChanged)
    Q_PROPERTY(QString printRange READ printRange WRITE setPrintRange NOTIFY printRangeChanged)
    Q_PROPERTY(PrinterEnum::PrintRange printRangeMode READ printRangeMode WRITE setPrintRangeMode NOTIFY printRangeModeChanged)
    Q_PROPERTY(int quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(bool reverse READ reverse WRITE setReverse NOTIFY reverseChanged)
    Q_PROPERTY(PrinterEnum::JobState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

    friend class PrinterCupsBackend;
public:
    explicit PrinterJob(QObject *parent=Q_NULLPTR);
    explicit PrinterJob(Printer *printer, QObject *parent=Q_NULLPTR);
    explicit PrinterJob(Printer *printer, PrinterBackend *backend,
                        QObject *parent=Q_NULLPTR);
    explicit PrinterJob(const QString &name, PrinterBackend *backend, int jobId, QObject *parent=Q_NULLPTR);
    ~PrinterJob();

    bool collate() const;
    int colorModel() const;
    PrinterEnum::ColorModelType colorModelType() const;
    int copies() const;
    int duplexMode() const;
    bool isTwoSided() const;
    int jobId() const;  // TODO: implement
    bool landscape() const;
//    Printer *printer() const;
    QString printerName() const;
    QString printRange() const;
    PrinterEnum::PrintRange printRangeMode() const;
    int quality() const;
    bool reverse() const;
    PrinterEnum::JobState state() const;
    QString title() const;
public Q_SLOTS:
    PrinterEnum::DuplexMode getDuplexMode() const;
    ColorModel getColorModel() const;
    PrintQuality getPrintQuality() const;
    Q_INVOKABLE void printFile(const QUrl &url);
    void setCollate(const bool collate);
    void setColorModel(const int colorModel);
    void setCopies(const int copies);
    void setDuplexMode(const int duplexMode);
    void setLandscape(const bool landscape);
//    void setPrinter(Printer *printer);
    void setPrinterName(const QString &printerName);
    void setPrintRange(const QString &printRange);
    void setPrintRangeMode(const PrinterEnum::PrintRange printRangeMode);
    void setQuality(const int quality);
    void setReverse(const bool reverse);
    void setTitle(const QString &title);

    bool updateFrom(QSharedPointer<PrinterJob> newPrinterJob);
private Q_SLOTS:
    void loadDefaults();
    void setIsTwoSided(const bool isTwoSided);
    void setState(const PrinterEnum::JobState &state);
Q_SIGNALS:
    void collateChanged();
    void colorModelChanged();
    void colorModelTypeChanged();
    void copiesChanged();
    void duplexModeChanged();
    void isTwoSidedChanged();
    void landscapeChanged();
//    void printerChanged();
    void printerNameChanged();
    void printRangeChanged();
    void printRangeModeChanged();
    void qualityChanged();
    void reverseChanged();
    void stateChanged();
    void titleChanged();
private:
    bool m_collate;
    int m_color_model;
    int m_copies;
    PrinterBackend *m_backend; // TODO: Maybe use the printer's backend?
    int m_duplex_mode;
    bool m_is_two_sided;
    int m_job_id;
    bool m_landscape;
    Printer *m_printer;
    QString m_printer_name;
    QString m_print_range;
    PrinterEnum::PrintRange m_print_range_mode;
    int m_quality;
    bool m_reverse;
    PrinterEnum::JobState m_state;
    QString m_title;
};

#endif // USC_PRINTERS_PRINTERJOB_H
