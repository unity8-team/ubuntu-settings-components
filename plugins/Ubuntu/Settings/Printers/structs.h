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

#ifndef USC_PRINTERS_STRUCTS_H
#define USC_PRINTERS_STRUCTS_H

#include "enums.h"

#include <QtCore/QMap>
#include <QDebug>
#include <QMetaType>

struct InkLevels
{
public:
    QMap<CartridgeType, uint> levels;
};

struct ColorModel
{
public:
    QString name = QString::null; // Gray, RGB, CMYK or anything [1], really.
    QString text = QString::null;
    ColorModelType colorType = ColorModelType::UnknownType;
    ColorSpace colorSpace = ColorSpace::UnknownSpace;
    ColorOrganization colorOrganization
        = ColorOrganization::UnknownOrganization;
    QString compressionMode = QString::null;

    bool operator==(const ColorModel& a) const
    {
        return (name == a.name && text == a.text && colorType == a.colorType &&
                colorSpace == a.colorSpace &&
                colorOrganization == a.colorOrganization &&
                compressionMode == a.compressionMode);
    }
};

Q_DECLARE_TYPEINFO(ColorModel, Q_PRIMITIVE_TYPE);
Q_DECLARE_METATYPE(ColorModel)

// [1] https://www.cups.org/doc/ppd-compiler.html#COLOR

#endif // USC_PRINTERS_STRUCTS_H
