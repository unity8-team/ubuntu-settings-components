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
    static DuplexMode ppdChoiceToDuplexMode(const QString &choice)
    {
        if (choice == "DuplexTumble")
            return DuplexMode::DuplexShortSide;
        else if (choice == "DuplexNoTumble")
            return DuplexMode::DuplexLongSide;
        else if (choice == "Auto")
            return DuplexMode::DuplexAuto;
        else
            return DuplexMode::DuplexNone;
    }

    static const QString duplexModeToPpdChoice(const DuplexMode &mode)
    {
        switch (mode) {
        case DuplexMode::DuplexNone:
            return "None";
        case DuplexMode::DuplexAuto:
            return "Auto";
        case DuplexMode::DuplexShortSide:
            return "DuplexTumble";
        case DuplexMode::DuplexLongSide:
            return "DuplexNoTumble";
        }
    }

    static DuplexMode qDuplexModeToDuplexMode(const QPrinter::DuplexMode &mode)
    {
        switch(mode) {
        case QPrinter::DuplexNone:
            return DuplexMode::DuplexNone;
        case QPrinter::DuplexAuto:
            return DuplexMode::DuplexAuto;
        case QPrinter::DuplexLongSide:
            return DuplexMode::DuplexLongSide;
        case QPrinter::DuplexShortSide:
            return DuplexMode::DuplexShortSide;
        }
    }
};

#endif // USC_PRINTERS_UTILS_H
