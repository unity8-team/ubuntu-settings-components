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

#ifndef USC_PRINTER_H
#define USC_PRINTER_H

#include "enums.h"
#include "structs.h"

#include <QtCore/QObject>

class PrinterPrivate;
class Printer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printer)
    Q_ENUMS(PrintRange ColorMode Quality ErrorPolicy OperationPolicy
            AccessControl DuplexMode State CartridgeType)

    Q_PROPERTY(ColorMode colorMode READ colorMode WRITE setColorMode NOTIFY colorModeChanged)
    Q_PROPERTY(int copies READ copies WRITE setCopies NOTIFY copiesChanged)
    Q_PROPERTY(bool duplex READ duplex WRITE setDuplex NOTIFY duplexChanged)
    Q_PROPERTY(bool duplexSupported READ duplexSupported NOTIFY duplexSupportedChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString printRange READ printRange WRITE setPrintRange NOTIFY printRangeChanged)
    Q_PROPERTY(PrintRange printRangeMode READ printRangeMode WRITE setPrintRangeMode NOTIFY printRangeModeChanged)
    Q_PROPERTY(bool pdfMode READ pdfMode WRITE setPdfMode NOTIFY pdfModeChanged)
    Q_PROPERTY(Quality quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QPageSize pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged)
    Q_PROPERTY(QList<QPageSize> supportedPageSizes READ supportedPageSizes CONSTANT)

    // Whether or not the user list is denied or allowed printing.
    Q_PROPERTY(AccessControl accessControl READ accessControl WRITE setAccessControl NOTIFY accessControlChanged)

    // What to do on printing/printer error.
    Q_PROPERTY(ErrorPolicy errorPolicy READ errorPolicy WRITE setErrorPolicy NOTIFY errorPolicyChanged)

    // A list of users that are either denied or allowed printing.
    Q_PROPERTY(QStringList users READ users NOTIFY usersChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString lastStateMessage READ lastStateMessage NOTIFY lastStateMessageChanged)
public:

    explicit Printer(QObject *parent = 0);
    ~Printer();
    ColorMode colorMode() const;
    int copies() const;
    bool duplex() const;
    bool duplexSupported() const;
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

public slots:
    void setColorMode(const ColorMode &colorMode);
    void setCopies(const int &copies);
    void setDuplex(const bool duplex);
    void setDuplexSupported(const bool duplexSupported);
    void setName(const QString &name);
    void setPrintRange(const QString &printRange);
    void setPrintRangeMode(const PrintRange &printRangeMode);
    void setPdfMode(const bool pdfMode);
    void setQuality(const Quality &quality);
    void setPageSize(const QPageSize &pageSize);

    // Add user that is either denied or allowed printer. See AccessControl.
    void addUser(const QString &username);

    // Removes user. See addUser.
    void removeUser(const QString &username);

    // Requests ink levels for printer.
    void requestInkLevels(const QString &name);

signals:
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
    void qualityChanged();
    void lastStateMessageChanged();

    void inkLevelsRequestComplete(const InkLevels &inkLevels);
    void inkLevelsRequestFailed(const QString &reply);
}

#endif // USC_PRINTER_H
