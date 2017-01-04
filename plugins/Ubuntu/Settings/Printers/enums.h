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

enum class PrintRange
{
    AllPages,
    PageRange,
};

enum class ColorMode
{
    ColorMode,
    GrayscaleMode,
};

enum class Quality
{
    DraftQuality,
    NormalQuality,
    BestQuality,
    PhotoQuality,
};

enum class ErrorPolicy
{
    RetryOnError,
    AbortOnError,
    StopPrinterOnError,
    RetryCurrentOnError,
};

enum class OperationPolicy
{
    DefaultOperation,
    AuthenticatedOperation,
};

enum class AccessControl
{
    AccessAllow,
    AccessDeny,
};

enum class DuplexMode
{
    DuplexNone,
    DuplexAuto,
    DuplexLongSide,
    DuplexShortSide,
};

enum class State
{
    IdleState,
    ActiveState,
    AbortedState,
    ErrorState,
};

enum class CartridgeType
{
    BlackCartridge,
    CyanCartridge,
    MagentaCartridge,
    YellowCartridge,
    RedCartridge,
    GreenCartridge,
    BlueCartridge,
    UnknownCartridge,
    WhiteCartridge,
};
