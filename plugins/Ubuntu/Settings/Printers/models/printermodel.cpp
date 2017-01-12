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

#include "utils.h"

#include "cups/cupsfacade_impl.h"
#include "models/printermodel.h"
#include "models/printermodel_p.h"
#include "printer/printerinfo_impl.h"

#include <QDebug>

PrinterModel::PrinterModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this))
{
    update();
}

PrinterModel::PrinterModel(PrinterInfo *info, CupsFacade *cups, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this, info, cups))
{
    update();
}

PrinterModelPrivate::PrinterModelPrivate(PrinterModel *q)
{
    this->info = new PrinterInfoImpl;
    this->cups = new CupsFacadeImpl;
}

PrinterModelPrivate::PrinterModelPrivate(PrinterModel *q, PrinterInfo *info, CupsFacade *cups)
{
    this->info = info;
    this->cups = cups;
}

PrinterModel::~PrinterModel()
{

}

void PrinterModel::update()
{

    Q_D(PrinterModel);

    // TODO: Proper model semantics (insert, move, remove, etc).
    int oldCount = d->printers.size();
    d->refreshPrinters();
    if (oldCount != d->printers.size()) {
        Q_EMIT countChanged();
    }
}

int PrinterModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const PrinterModel);
    return d->printers.size();
}

int PrinterModel::count() const
{
    return rowCount();
}

QVariant PrinterModel::data(const QModelIndex &index, int role) const
{
    Q_D(const PrinterModel);
    QVariant ret;

    if ((0<=index.row()) && (index.row()<d->printers.size())) {

        auto printer = d->printers[index.row()];

        switch (role) {
        case NameRole:
        case Qt::DisplayRole:
            ret = printer->name();
            break;
        // case ColorModeRole:
        //     ret = printer->colorMode();
        //     break;
        // case CopiesRole:
        //     ret = printer->copies();
        //     break;
        // case DuplexRole:
        //     ret = printer->duplex();
        //     break;
        case SupportedDuplexModesRole:
            ret = QVariant::fromValue(printer->supportedDuplexStrings());
            break;
        // case PrintRangeRole:
        //     ret = printer->printRange();
        //     break;
        // case PrintRangeModeRole:
        //     ret = printer->printRangeMode();
        //     break;
        // case PdfModeRole:
        //     ret = printer->pdfMode();
        //     break;
        // case QualityRole:
        //     ret = printer->quality();
        //     break;
        case DescriptionRole:
            ret = printer->description();
            break;
        // case PageSizeRole:
        //     ret = printer->pageSize();
        //     break;
        // case QPageSizeRole:
        //     ret = printer->qPageSize();
        //     break;
        // case AccessControlRole:
        //     ret = printer->accessControl();
        //     break;
        // case ErrorPolicyRole:
        //     ret = printer->errorPolicy();
        //     break;
        // case UsersRole:
        //     ret = printer->users();
        //     break;
        // case StateRole:
        //     ret = printer->state();
        //     break;
        case PrinterRole:
            // TODO: figure out how crazy this is...
            ret = QVariant::fromValue(printer.data());
            break;
        // case LastStateMessageRole:
        //     ret = printer->lastStateMessage();
        //     break;
        }
    }

    return ret;
}

bool PrinterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(const PrinterModel);

    if ((0<=index.row()) && (index.row()<d->printers.size())) {

        auto printer = d->printers[index.row()];

        switch (role) {
        case DescriptionRole:
            printer->setDescription(value.toString());
            break;
        case SupportedDuplexModesRole:
            DuplexMode mode = Utils::ppdChoiceToDuplexMode(value.toString());
            printer->setDefaultDuplexMode(mode);
            break;
        }
    }

    return true;
}

QHash<int, QByteArray> PrinterModel::roleNames() const
{
    static QHash<int,QByteArray> names;

    if (Q_UNLIKELY(names.empty())) {
        names[Qt::DisplayRole] = "displayName";
        names[ColorModeRole] = "colorMode";
        names[CopiesRole] = "copies";
        names[DuplexRole] = "duplex";
        names[SupportedDuplexModesRole] = "supportedDuplexModes";
        names[NameRole] = "name";
        names[PrintRangeRole] = "printRange";
        names[PrintRangeModeRole] = "printRangeMode";
        names[PdfModeRole] = "pdfMode";
        names[QualityRole] = "quality";
        names[DescriptionRole] = "description";
        names[PageSizeRole] = "pageSize";
        names[QPageSizeRole] = "qPageSize";
        names[AccessControlRole] = "accessControl";
        names[ErrorPolicyRole] = "errorPolicy";
        names[UsersRole] = "users";
        names[StateRole] = "state";
        names[PrinterRole] = "printer";
        names[LastStateMessageRole] = "lastStateMessage";
    }

    return names;
}

QSharedPointer<Printer> PrinterModel::getPrinterFromName(const QString &name)
{

}

void PrinterModelPrivate::refreshPrinters()
{
    Q_FOREACH(PrinterInfo *printerInfo, info->availablePrinters()) {
        QSharedPointer<Printer> printer = QSharedPointer<Printer>(
            new Printer(printerInfo, this->cups)
        );
        printers.append(printer);
    }
}

void PrinterModelPrivate::init()
{
}

PrinterFilter::PrinterFilter()
{

}

PrinterFilter::~PrinterFilter()
{

}

void PrinterFilter::filterOnState(const State &state)
{

}

void PrinterFilter::filterOnRecent(const bool recent)
{

}

bool PrinterFilter::filterAcceptsRow(int, const QModelIndex&) const
{

}

bool PrinterFilter::lessThan(const QModelIndex&, const QModelIndex&) const
{

}
