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

#ifndef USC_PRINTERS_PRINTERPRIVATE_H
#define USC_PRINTERS_PRINTERPRIVATE_H

struct PrinterPrivate
{
    Q_DISABLE_COPY(PrinterPrivate)
    Q_DECLARE_PUBLIC(Printer)
    explicit PrinterPrivate(Printer *q);
    explicit PrinterPrivate(Printer *q, PrinterInfo *info, CupsFacade *cups);
    CupsFacade *cups;
    Printer *q_ptr;
    PrinterInfo *info;
};

#endif // USC_PRINTERS_PRINTERPRIVATE_H
