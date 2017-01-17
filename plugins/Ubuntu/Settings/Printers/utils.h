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

#ifndef USC_PRINTERS_UTILS_H
#define USC_PRINTERS_UTILS_H

#include "enums.h"

#include <QString>
#include <QPrinter>

class Utils
{
public:
    static PrinterEnum::DuplexMode ppdChoiceToDuplexMode(const QString &choice)
    {
        if (choice == "DuplexTumble")
            return PrinterEnum::DuplexMode::DuplexShortSide;
        else if (choice == "DuplexNoTumble")
            return PrinterEnum::DuplexMode::DuplexLongSide;
        else if (choice == "Auto")
            return PrinterEnum::DuplexMode::DuplexAuto;
        else
            return PrinterEnum::DuplexMode::DuplexNone;
    }

    static const QString duplexModeToPpdChoice(const PrinterEnum::DuplexMode &mode)
    {
        switch (mode) {
        case PrinterEnum::DuplexMode::DuplexNone:
            return "None";
        case PrinterEnum::DuplexMode::DuplexAuto:
            return "Auto";
        case PrinterEnum::DuplexMode::DuplexShortSide:
            return "DuplexTumble";
        case PrinterEnum::DuplexMode::DuplexLongSide:
            return "DuplexNoTumble";
        }
    }

    static PrinterEnum::DuplexMode qDuplexModeToDuplexMode(const QPrinter::DuplexMode &mode)
    {
        switch(mode) {
        case QPrinter::DuplexNone:
            return PrinterEnum::DuplexMode::DuplexNone;
        case QPrinter::DuplexAuto:
            return PrinterEnum::DuplexMode::DuplexAuto;
        case QPrinter::DuplexLongSide:
            return PrinterEnum::DuplexMode::DuplexLongSide;
        case QPrinter::DuplexShortSide:
            return PrinterEnum::DuplexMode::DuplexShortSide;
        }
    }
};

#endif // USC_PRINTERS_UTILS_H
