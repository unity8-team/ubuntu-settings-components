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
#include "structs.h"

#include <cups/ppd.h>

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

    static ColorSpace ppdColorSpaceToColorSpace(const ppd_cs_t &space)
    {
        switch (space) {
        case PPD_CS_CMY:
            return ColorSpace::CMYSpace;
        case PPD_CS_CMYK:
            return ColorSpace::CMYKSpace;
        case PPD_CS_GRAY:
            return ColorSpace::GraySpace;
        case PPD_CS_N:
            return ColorSpace::NSpace;
        case PPD_CS_RGB:
            return ColorSpace::RGBSpace;
        case PPD_CS_RGBK:
            return ColorSpace::RGBKSpace;
        }
    }

    /* For information about this method, please see
    <https://www.cups.org/doc/ref-ppdcfile.html#ColorModel>. */
    static ColorModel parsePpdColorModel(const QString &colorModel)
    {
        ColorModel ret;
        QStringList vals = colorModel.split(" ");

        if (vals.size() == 1) {
            if (vals[0].contains("/")) {
                QStringList nameText = vals[0].split("/");
                ret.name = nameText[0];
                ret.text = nameText[1];
            } else {
                ret.name = vals[0];
            }
        }

        if (ret.name == "Gray") {
            ret.colorType = ColorModelType::GrayType;
        } else {
            ret.colorType = ColorModelType::ColorType;
        }

        if (vals.size() == 2) {
            QString s = vals[1].toLower();
            if (s == "cmy") ret.colorSpace = ColorSpace::CMYSpace;
            else if (s == "cmyk") ret.colorSpace = ColorSpace::CMYKSpace;
            else if (s == "n") ret.colorSpace = ColorSpace::NSpace;
            else if (s == "rgb") ret.colorSpace = ColorSpace::RGBSpace;
            else if (s == "rgbk") ret.colorSpace = ColorSpace::RGBKSpace;
            else ret.colorSpace = ColorSpace::GraySpace;
        }

        // ColorOrganization
        if (vals.size() == 3) {
            QString o = vals[2].toLower();
            ColorOrganization org = ret.colorOrganization;
            if (vals[2] == "chunky") {
                org = ColorOrganization::ChunkyOrganization;
            } else if (vals[2] == "banded") {
                org = ColorOrganization::BandedOrganization;
            } else if (vals[3] == "planar") {
                org = ColorOrganization::PlanarOrganization;
            }
            ret.colorOrganization = org;
        }

        // Compression
        if (vals.size() == 4) {
            ret.compressionMode = vals[3];
        }
        return ret;
    }

    static QString colorModelToPpdColorModel(const ColorModel &model)
    {
        QString nameText = model.name;
        if (!model.text.isEmpty()) nameText += QString("/%1").arg(model.text);

        QString colorSpace;
        if (model.colorSpace != ColorSpace::UnknownSpace) {
            switch (model.colorSpace) {
            case ColorSpace::CMYSpace:
                colorSpace = "cmy";
                break;
            case ColorSpace::CMYKSpace:
                colorSpace = "cmyk";
                break;
            case ColorSpace::NSpace:
                colorSpace = "n";
                break;
            case ColorSpace::RGBSpace:
                colorSpace = "rgb";
                break;
            case ColorSpace::RGBKSpace:
                colorSpace = "rgbk";
                break;
            case ColorSpace::GraySpace:
            case ColorSpace::UnknownSpace:
                break;
            }
        }

        if (colorSpace.isEmpty()) {
            return nameText;
        }

        QString colorOrg;
        if (model.colorOrganization != ColorOrganization::UnknownOrganization) {
            switch (model.colorOrganization) {
            case ColorOrganization::ChunkyOrganization:
                colorOrg = "chunky";
                break;
            case ColorOrganization::BandedOrganization:
                colorOrg = "banded";
                break;
            case ColorOrganization::PlanarOrganization:
                colorOrg = "planar";
                break;
            case ColorOrganization::UnknownOrganization:
                break;
            }
        }

        if (colorOrg.isEmpty()) {
            return QString("%1 %2").arg(nameText).arg(colorSpace);
        }

        if (model.compressionMode.isEmpty()) {
            return QString("%1 %2 %3").arg(nameText).arg(colorSpace).arg(colorOrg);
        }

        return QString("%1 %2 %3 %4").arg(nameText).arg(colorSpace).arg(colorOrg).arg(model.compressionMode);
    }
};

#endif // USC_PRINTERS_UTILS_H
