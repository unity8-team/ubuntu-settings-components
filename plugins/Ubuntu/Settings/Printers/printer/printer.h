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

#ifndef USC_PRINTERS_PRINTER_H
#define USC_PRINTERS_PRINTER_H

#include "printers_global.h"

#include "cups/cupsfacade.h"
#include "enums.h"
#include "printer/printerinfo.h"
#include "structs.h"

#include <QObject>
#include <QPageSize>
#include <QList>
#include <QScopedPointer>
#include <QString>
#include <QStringList>

class PrinterPrivate;
class PRINTERS_DECL_EXPORT Printer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printer)
    Q_PROPERTY(ColorMode colorMode READ colorMode WRITE setColorMode NOTIFY colorModeChanged)
    Q_PROPERTY(ColorMode defaultColorMode READ defaultColorMode CONSTANT)
    Q_PROPERTY(int copies READ copies WRITE setCopies NOTIFY copiesChanged)
    Q_PROPERTY(bool duplex READ duplex WRITE setDuplex NOTIFY duplexChanged)
    Q_PROPERTY(bool duplexSupported READ duplexSupported NOTIFY duplexSupportedChanged)
    Q_PROPERTY(DuplexMode defaultDuplexMode READ defaultDuplexMode CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString printRange READ printRange WRITE setPrintRange NOTIFY printRangeChanged)
    Q_PROPERTY(PrintRange printRangeMode READ printRangeMode WRITE setPrintRangeMode NOTIFY printRangeModeChanged)
    Q_PROPERTY(bool pdfMode READ pdfMode WRITE setPdfMode NOTIFY pdfModeChanged)
    Q_PROPERTY(Quality quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QPageSize pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(QList<QPageSize> supportedPageSizes READ supportedPageSizes CONSTANT)
    Q_PROPERTY(AccessControl accessControl READ accessControl WRITE setAccessControl NOTIFY accessControlChanged)
    Q_PROPERTY(ErrorPolicy errorPolicy READ errorPolicy WRITE setErrorPolicy NOTIFY errorPolicyChanged)
    Q_PROPERTY(QStringList users READ users NOTIFY usersChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString lastStateMessage READ lastStateMessage NOTIFY lastStateMessageChanged)

    QScopedPointer<PrinterPrivate> const d_ptr;
public:
    explicit Printer(QObject *parent = nullptr);
    explicit Printer(PrinterInfo *info, CupsFacade *cups, QObject *parent = nullptr);
    ~Printer();

    Q_ENUM(PrintRange)
    Q_ENUM(ColorMode)
    Q_ENUM(Quality)
    Q_ENUM(ErrorPolicy)
    Q_ENUM(OperationPolicy)
    Q_ENUM(AccessControl)
    Q_ENUM(DuplexMode)
    Q_ENUM(State)
    Q_ENUM(CartridgeType)

    ColorMode colorMode() const;
    ColorMode defaultColorMode() const;
    int copies() const;
    bool duplex() const;
    bool duplexSupported() const;
    DuplexMode defaultDuplexMode() const;
    QString name() const;
    QString printRange() const;
    PrintRange printRangeMode() const;
    bool pdfMode() const;
    Quality quality() const;
    QString description() const;
    QPageSize pageSize() const;
    QList<QPageSize> supportedPageSizes() const;
    AccessControl accessControl() const;
    ErrorPolicy errorPolicy() const;
    QStringList users() const;
    State state() const;
    QString lastStateMessage() const;

    void setAccessControl(const AccessControl &accessControl);
    void setColorMode(const ColorMode &colorMode);
    void setCopies(const int &copies);
    void setDescription(const QString &description);
    void setDuplex(const bool duplex);
    void setDuplexSupported(const bool duplexSupported);
    void setErrorPolicy(const ErrorPolicy &errorPolicy);
    void setName(const QString &name);
    void setPrintRange(const QString &printRange);
    void setPrintRangeMode(const PrintRange &printRangeMode);
    void setPdfMode(const bool pdfMode);
    void setQuality(const Quality &quality);
    void setPageSize(const QPageSize &pageSize);

public Q_SLOTS:
    // Add user that is either denied or allowed printer. See AccessControl.
    void addUser(const QString &username);

    // Removes user. See addUser.
    void removeUser(const QString &username);

    // Requests ink levels for printer.
    void requestInkLevels(const QString &name);

Q_SIGNALS:
    void nameChanged();
    void descriptionChanged();
    void pageSizeChanged();
    void duplexModeChanged();
    void colorModeChanged();
    void qualityChanged();
    void rangeChanged();
    void accessControlChanged();
    void errorPolicyChanged();
    void usersChanged();
    void stateChanged();
    void copiesChanged();
    void duplexChanged();
    void duplexSupportedChanged();
    void settingsChanged();
    void printRangeChanged();
    void printRangeModeChanged();
    void pdfModeChanged();
    void lastStateMessageChanged();

    void inkLevelsRequestComplete(const InkLevels &inkLevels);
    void inkLevelsRequestFailed(const QString &reply);

    // Signals that some printer setting was changed.
    void printerChanged();
};

#endif // USC_PRINTERS_PRINTER_H
