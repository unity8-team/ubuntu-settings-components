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
#include "printer/printerjob.h"
#include "structs.h"

#include <QObject>
#include <QPageSize>
#include <QList>
#include <QScopedPointer>
#include <QString>
#include <QStringList>

class CupsFacade;
class PrinterJob;

class PrinterPrivate;
class PRINTERS_DECL_EXPORT Printer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printer)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(ColorModel defaultColorModel READ defaultColorModel WRITE setDefaultColorModel NOTIFY defaultColorModelChanged)
    Q_PROPERTY(QStringList supportedDuplexStrings READ supportedDuplexStrings CONSTANT)
    Q_PROPERTY(PrinterEnum::DuplexMode defaultDuplexMode READ defaultDuplexMode WRITE setDefaultDuplexMode NOTIFY defaultDuplexModeChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QPageSize defaultPageSize READ defaultPageSize WRITE setDefaultPageSize NOTIFY defaultPageSizeChanged)
    Q_PROPERTY(QList<QPageSize> supportedPageSizes READ supportedPageSizes CONSTANT)
    Q_PROPERTY(PrinterEnum::AccessControl accessControl READ accessControl WRITE setAccessControl NOTIFY accessControlChanged)
    Q_PROPERTY(PrinterEnum::ErrorPolicy errorPolicy READ errorPolicy WRITE setErrorPolicy NOTIFY errorPolicyChanged)
    Q_PROPERTY(QStringList users READ users NOTIFY usersChanged)
    Q_PROPERTY(PrinterEnum::State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString lastStateMessage READ lastStateMessage NOTIFY lastStateMessageChanged)

    QScopedPointer<PrinterPrivate> const d_ptr;
public:
    explicit Printer(QObject *parent = nullptr);
    explicit Printer(PrinterInfo *info, CupsFacade *cups, QObject *parent = nullptr);
    ~Printer();

    bool enabled() const;
    ColorModel defaultColorModel() const;
    QList<ColorModel> supportedColorModels() const;
    QList<PrinterEnum::DuplexMode> supportedDuplexModes() const;
    QStringList supportedDuplexStrings() const;
    PrinterEnum::DuplexMode defaultDuplexMode() const;
    QString name() const;
    PrintQuality defaultPrintQuality() const;
    QList<PrintQuality> supportedPrintQualities() const;
    QString description() const;
    QPageSize defaultPageSize() const;
    QList<QPageSize> supportedPageSizes() const;
    PrinterEnum::AccessControl accessControl() const;
    PrinterEnum::ErrorPolicy errorPolicy() const;
    QStringList users() const;
    PrinterEnum::State state() const;
    QString lastStateMessage() const;
    bool isDefault();
    bool isPdf();

    void setAccessControl(const PrinterEnum::AccessControl &accessControl);
    void setDefaultColorModel(const ColorModel &colorModel);
    void setDescription(const QString &description);
    void setDefaultDuplexMode(const PrinterEnum::DuplexMode &duplexMode);
    void setEnabled(const bool enabled);
    void setErrorPolicy(const PrinterEnum::ErrorPolicy &errorPolicy);
    void setName(const QString &name);
    void setDefaultPrintQuality(const PrintQuality &quality);
    void setDefaultPageSize(const QPageSize &pageSize);

public Q_SLOTS:
    // Add user that is either denied or allowed printer. See AccessControl.
    void addUser(const QString &username);

    // Helper for managing a job on the printer
    PrinterJob *job();
    int printFile(const QString &filepath, const PrinterJob *options);

    // Removes user. See addUser.
    void removeUser(const QString &username);

    // Requests ink levels for printer.
    void requestInkLevels(const QString &name);

Q_SIGNALS:
    void nameChanged();
    void enabledChanged();
    void descriptionChanged();
    void defaultPageSizeChanged();
    void defaultDuplexModeChanged();
    void defaultColorModelChanged();
    void qualityChanged();
    void accessControlChanged();
    void errorPolicyChanged();
    void usersChanged();
    void stateChanged();
    void settingsChanged();
    void lastStateMessageChanged();

    void inkLevelsRequestComplete(const InkLevels &inkLevels);
    void inkLevelsRequestFailed(const QString &reply);

    // Signals that some printer setting was changed.
    void printerChanged();
};

// FIXME: not necessary outside tests
Q_DECLARE_METATYPE(QList<PrinterEnum::DuplexMode>)

#endif // USC_PRINTERS_PRINTER_H
