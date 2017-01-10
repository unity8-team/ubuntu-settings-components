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

#include "printers/printers.h"
#include "printers/printers_p.h"

Printers::Printers(QObject *parent)
    : QObject(parent)
    , d_ptr(new PrintersPrivate(this))
{

}

Printers::Printers(PrinterInfo *info, CupsFacade *cups, QObject *parent)
    : QObject(parent)
    , d_ptr(new PrintersPrivate(this, info, cups))
{

}

Printers::~Printers()
{

}

PrintersPrivate::PrintersPrivate(Printers *q)
{

}

PrintersPrivate::PrintersPrivate(Printers *q, PrinterInfo *info, CupsFacade *cups)
{
    this->info = info;
    this->cups = cups;
}

QAbstractItemModel* Printers::allPrinters() const
{

}

QAbstractItemModel* Printers::recentPrinters() const
{

}

QAbstractItemModel* Printers::printJobs() const
{

}

QString Printers::defaultPrinterName() const
{

}

void Printers::setDefaultPrinterName(const QString &name)
{

}

QSharedPointer<Printer> Printers::getPrinterByName(const QString &name)
{

}
QSharedPointer<Printer> Printers::getJobOwner(const int &jobId)
{

}

QSharedPointer<Printer> Printers::addPrinter(const QString &name,
                                             const QUrl &ppd,
                                             const QUrl &device,
                                             const QString &description,
                                             const QString &location)
{

}

QSharedPointer<Printer> Printers::addPrinter(const QString &name,
                                             const QUrl &device,
                                             const QString &description,
                                             const QString &location)
{

}

bool Printers::removePrinter(const QString &name)
{

}
