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

#ifndef USC_PRINTERS_H
#define USC_PRINTERS_H

#include <QAbstractItemModel>
#include <QtCore/QObject>
#include <QtCore/QString>

class PrintersPrivate;
class Printers : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printers)
public:
    Q_PROPERTY (QAbstractItemModel* allPrinters
                READ getAllPrinters
                CONSTANT)

    Q_PROPERTY (QAbstractItemModel* recentPrinters
                READ getRecentPrinters
                CONSTANT)

    Q_PROPERTY (QAbstractItemModel* printJobs
                READ getPrintJobs
                CONSTANT)

    Q_PROPERTY (QString defaultPrinterName
                READ defaultPrinterName
                WRITE setDefaultPrinterName
                NOTIFY defaultPrinterNameChanged)
public:
    Q_INVOKABLE Printer* getPrinterByName(const QString &name);
    QString defaultPrinterName() const;
    void setDefaultPrinterName(const QString &name);
    void defaultPrinterNameChanged();

public slots:
    void addPrinter();

private slots:
    void updatePrinters();
}

#endif // USC_PRINTERS_H
