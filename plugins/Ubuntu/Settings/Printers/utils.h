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
    static PrinterEnum::DuplexMode ppdChoiceToDuplexMode(const QString &choice)
    {
        if (choice == "DuplexTumble")
            return PrinterEnum::DuplexMode::DuplexShortSide;
        else if (choice == "DuplexNoTumble")
            return PrinterEnum::DuplexMode::DuplexLongSide;
        else
            return PrinterEnum::DuplexMode::DuplexNone;
    }

    static const QString duplexModeToPpdChoice(const PrinterEnum::DuplexMode &mode)
    {
        switch (mode) {
        case PrinterEnum::DuplexMode::DuplexNone:
            return "None";
        case PrinterEnum::DuplexMode::DuplexShortSide:
            return "DuplexTumble";
        case PrinterEnum::DuplexMode::DuplexLongSide:
            return "DuplexNoTumble";
        }
    }

    static const QString duplexModeToUIString(const PrinterEnum::DuplexMode &mode)
    {
        // TODO: translate
        switch (mode) {
        case PrinterEnum::DuplexMode::DuplexNone:
            return "None";
        case PrinterEnum::DuplexMode::DuplexShortSide:
            return "Short edge (flip)";
        case PrinterEnum::DuplexMode::DuplexLongSide:
            return "Long edge (standard)";
        }
    }

    static PrinterEnum::DuplexMode qDuplexModeToDuplexMode(const QPrinter::DuplexMode &mode)
    {
        switch(mode) {
        case QPrinter::DuplexNone:
            return PrinterEnum::DuplexMode::DuplexNone;
        case QPrinter::DuplexAuto:
        case QPrinter::DuplexLongSide:
            return PrinterEnum::DuplexMode::DuplexLongSide;
        case QPrinter::DuplexShortSide:
            return PrinterEnum::DuplexMode::DuplexShortSide;
        }
    }

    static PrinterEnum::ColorSpace ppdColorSpaceToColorSpace(const ppd_cs_t &space)
    {
        switch (space) {
        case PPD_CS_CMY:
            return PrinterEnum::ColorSpace::CMYSpace;
        case PPD_CS_CMYK:
            return PrinterEnum::ColorSpace::CMYKSpace;
        case PPD_CS_GRAY:
            return PrinterEnum::ColorSpace::GraySpace;
        case PPD_CS_N:
            return PrinterEnum::ColorSpace::NSpace;
        case PPD_CS_RGB:
            return PrinterEnum::ColorSpace::RGBSpace;
        case PPD_CS_RGBK:
            return PrinterEnum::ColorSpace::RGBKSpace;
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

        if (ret.name.contains("Gray")) {
            ret.colorType = PrinterEnum::ColorModelType::GrayType;
        } else {
            ret.colorType = PrinterEnum::ColorModelType::ColorType;
        }

        if (vals.size() == 2) {
            QString s = vals[1].toLower();
            if (s == "cmy") ret.colorSpace = PrinterEnum::ColorSpace::CMYSpace;
            else if (s == "cmyk") ret.colorSpace = PrinterEnum::ColorSpace::CMYKSpace;
            else if (s == "n") ret.colorSpace = PrinterEnum::ColorSpace::NSpace;
            else if (s == "rgb") ret.colorSpace = PrinterEnum::ColorSpace::RGBSpace;
            else if (s == "rgbk") ret.colorSpace = PrinterEnum::ColorSpace::RGBKSpace;
            else ret.colorSpace = PrinterEnum::ColorSpace::GraySpace;
        }

        // ColorOrganization
        if (vals.size() == 3) {
            QString o = vals[2].toLower();
            PrinterEnum::ColorOrganization org = ret.colorOrganization;
            if (vals[2] == "chunky") {
                org = PrinterEnum::ColorOrganization::ChunkyOrganization;
            } else if (vals[2] == "banded") {
                org = PrinterEnum::ColorOrganization::BandedOrganization;
            } else if (vals[3] == "planar") {
                org = PrinterEnum::ColorOrganization::PlanarOrganization;
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
        if (model.colorSpace != PrinterEnum::ColorSpace::UnknownSpace) {
            switch (model.colorSpace) {
            case PrinterEnum::ColorSpace::CMYSpace:
                colorSpace = "cmy";
                break;
            case PrinterEnum::ColorSpace::CMYKSpace:
                colorSpace = "cmyk";
                break;
            case PrinterEnum::ColorSpace::NSpace:
                colorSpace = "n";
                break;
            case PrinterEnum::ColorSpace::RGBSpace:
                colorSpace = "rgb";
                break;
            case PrinterEnum::ColorSpace::RGBKSpace:
                colorSpace = "rgbk";
                break;
            case PrinterEnum::ColorSpace::GraySpace:
            case PrinterEnum::ColorSpace::UnknownSpace:
                break;
            }
        }

        if (colorSpace.isEmpty()) {
            return nameText;
        }

        QString colorOrg;
        if (model.colorOrganization != PrinterEnum::ColorOrganization::UnknownOrganization) {
            switch (model.colorOrganization) {
            case PrinterEnum::ColorOrganization::ChunkyOrganization:
                colorOrg = "chunky";
                break;
            case PrinterEnum::ColorOrganization::BandedOrganization:
                colorOrg = "banded";
                break;
            case PrinterEnum::ColorOrganization::PlanarOrganization:
                colorOrg = "planar";
                break;
            case PrinterEnum::ColorOrganization::UnknownOrganization:
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
