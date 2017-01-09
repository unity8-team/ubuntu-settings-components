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

#ifndef USC_PRINTERS_PRINTERMODELPRIVATE_H
#define USC_PRINTERS_PRINTERMODELPRIVATE_H

struct PrinterModelPrivate
{
    Q_DISABLE_COPY(PrinterModelPrivate)
    Q_DECLARE_PUBLIC(PrinterModel)
    explicit PrinterModelPrivate(PrinterModel *q);
    explicit PrinterModelPrivate(PrinterModel *q, PrinterInfo *info, CupsFacade *cups);
    CupsFacade *cups;
    PrinterModel *q_ptr;
    PrinterInfo *info;
    QList<QSharedPointer<Printer>> printers;
};

#endif // USC_PRINTERS_PRINTERMODELPRIVATE_H
