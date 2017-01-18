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

#ifndef USC_PRINTERS_PRIVATE_H
#define USC_PRINTERS_PRIVATE_H

#include "models/printermodel.h"

struct PrintersPrivate
{
    Q_DISABLE_COPY(PrintersPrivate)
    Q_DECLARE_PUBLIC(Printers)
    explicit PrintersPrivate(Printers *q, int printerUpdateIntervalMSecs);
    explicit PrintersPrivate(Printers *q, PrinterInfo *info, CupsFacade *cups,
                             int printerUpdateIntervalMSecs);
    ~PrintersPrivate();
    CupsFacade *cups;
    Printers *q_ptr;
    PrinterInfo *info;
    PrinterModel model;
    PrinterFilter allPrinters;
    PrinterFilter recentPrinters;
};

#endif // USC_PRINTERS_PRIVATE_H
