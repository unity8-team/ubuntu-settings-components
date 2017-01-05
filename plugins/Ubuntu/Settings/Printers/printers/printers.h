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
#include <QObject>
#include <QSharedPointer>
#include <QString>

class PrintersPrivate;
class Printers : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Printers)
    Q_PROPERTY(QAbstractItemModel* allPrinters READ allPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* recentPrinters READ recentPrinters CONSTANT)
    Q_PROPERTY(QAbstractItemModel* printJobs READ printJobs CONSTANT)
    Q_PROPERTY(QString defaultPrinterName READ defaultPrinterName WRITE setDefaultPrinterName NOTIFY defaultPrinterNameChanged)

public:
    QAbstractItemModel* allPrinters() const;
    QAbstractItemModel* recentPrinters() const;
    QAbstractItemModel* printJobs() const;
    QString defaultPrinterName() const;

    void setDefaultPrinterName(const QString &name);

public slots:
    QSharedPointer<Printer> getPrinterByName(const QString &name);
    QSharedPointer<Printer> getJobOwner(const int &jobId);
    void addPrinter();
    void updatePrinters();

signals:
    void defaultPrinterNameChanged();
}

#endif // USC_PRINTERS_H
