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

#ifndef USC_PRINTERS_ENUMS_H
#define USC_PRINTERS_ENUMS_H

#include <QtCore/QObject>

#include "printers_global.h"

class PRINTERS_DECL_EXPORT PrinterEnum
{
    Q_GADGET

public:
    enum class AccessControl
    {
        AccessAllow = 0,
        AccessDeny,
    };
    Q_ENUM(AccessControl)

    enum class CartridgeType
    {
        BlackCartridge = 0,
        CyanCartridge,
        MagentaCartridge,
        YellowCartridge,
        RedCartridge,
        GreenCartridge,
        BlueCartridge,
        UnknownCartridge,
        WhiteCartridge,
    };
    Q_ENUM(CartridgeType)

    enum class ColorModel
    {
        ColorMode = 0,
        GrayscaleMode,
    };
    Q_ENUM(ColorModel)

    enum class DuplexMode
    {
        DuplexNone = 0,
        DuplexAuto,
        DuplexLongSide,
        DuplexShortSide,
    };
    Q_ENUM(DuplexMode)

    enum class ErrorPolicy
    {
        RetryOnError = 0,
        AbortOnError,
        StopPrinterOnError,
        RetryCurrentOnError,
    };
    Q_ENUM(ErrorPolicy)

    enum class OperationPolicy
    {
        DefaultOperation = 0,
        AuthenticatedOperation,
    };
    Q_ENUM(OperationPolicy)

    enum class PrintRange
    {
        AllPages = 0,
        PageRange,
    };
    Q_ENUM(PrintRange)

    enum class Quality
    {
        DraftQuality = 0,
        NormalQuality,
        BestQuality,
        PhotoQuality,
    };
    Q_ENUM(Quality)

    enum class State
    {
        IdleState = 0,
        ActiveState,
        AbortedState,
        ErrorState,
    };
    Q_ENUM(State)
};

#endif // USC_PRINTERS_ENUMS_H
