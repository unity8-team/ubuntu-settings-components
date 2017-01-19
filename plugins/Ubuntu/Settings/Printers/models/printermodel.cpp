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
#include "printer/printerinfo_allimpl.h"

#include <QDebug>

PrinterModel::PrinterModel(int timerMsec, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this))
{
    update();
    startUpdateTimer(timerMsec);
}

PrinterModel::PrinterModel(PrinterInfo *info, CupsFacade *cups, int timerMsec, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PrinterModelPrivate(this, info, cups))
{
    update();
    startUpdateTimer(timerMsec);
}

PrinterModelPrivate::PrinterModelPrivate(PrinterModel *q)
{
    this->info = new PrinterInfoAllImpl;
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

void PrinterModel::startUpdateTimer(int msecs)
{
    // Start a timer to poll for changes in the printers
    m_update_timer.setParent(this);
    connect(&m_update_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_update_timer.start(msecs);
}

void PrinterModel::update()
{
    Q_D(PrinterModel);

    // Store the old count and get the new printers
    int oldCount = d->printers.size();
    QList<PrinterInfo *> newPrinters = d->info->availablePrinters();

    // Go through the old model
    for (int i=0; i < d->printers.count(); i++) {
        // Determine if the old printer exists in the new model
        bool exists = false;

        Q_FOREACH(PrinterInfo *p, newPrinters) {
            if (p->printerName() == d->printers.at(i)->name()) {
                exists = true;
                break;
            }
        }

        // If it doesn't exist then remove it from the old model
        if (!exists) {
            beginRemoveRows(QModelIndex(), i, i);
            d->printers.removeAt(i);
            endRemoveRows();

            i--;  // as we have removed an item decrement
        }
    }

    // Go through the new model
    for (int i=0; i < newPrinters.count(); i++) {
        // Determine if the new printer exists in the old model
        bool exists = false;
        int j;

        for (j=0; j < d->printers.count(); j++) {
            if (d->printers.at(j)->name() == newPrinters.at(i)->printerName()) {
                exists = true;
                break;
            }
        }

        if (exists) {
            if (j == i) {  // New printer exists and in correct position
                continue;
            } else {
                // New printer does exist but needs to be moved in old model
                beginMoveRows(QModelIndex(), j, 1, QModelIndex(), i);
                d->printers.move(j, i);
                endMoveRows();
            }
        } else {
            // New printer does not exist insert into model
            beginInsertRows(QModelIndex(), i, i);
            QSharedPointer<Printer> printer = QSharedPointer<Printer>(
                new Printer(newPrinters.at(i), d->cups)
            );
            d->printers.insert(i, printer);
            endInsertRows();
        }
    }

    if (oldCount != d->printers.size()) {
        Q_EMIT countChanged();
    }
}

int PrinterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
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
        case ColorModelRole:
            ret = printer->supportedColorModels().indexOf(printer->defaultColorModel());
            break;
        case SupportedColorModelsRole: {
                QStringList models;
                Q_FOREACH(const ColorModel &m, printer->supportedColorModels()) {
                    models.append(m.text.isEmpty() ? m.name : m.text);
                }
                ret = models;
            }
            break;
        // case CopiesRole:
        //     ret = printer->copies();
        //     break;
        case DefaultPrinterRole:
            ret = printer->isDefault();
            break;
        case DuplexRole:
            ret = printer->supportedDuplexModes().indexOf(printer->defaultDuplexMode());
            break;
        case SupportedDuplexModesRole:
            ret = printer->supportedDuplexStrings();
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
        case PageSizeRole:
            ret = printer->defaultPageSize().name();
            break;
        case SupportedPageSizesRole: {
                QStringList sizes;
                Q_FOREACH(const QPageSize &s, printer->supportedPageSizes()) {
                    sizes << s.name();
                }
                ret = sizes;
            }
            break;

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
        case IsPdfRole:
            ret = printer->isPdf();
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
        case ColorModelRole: {
                int index = value.toInt();
                auto modes = printer->supportedColorModels();
                if (index >= 0 && modes.size() > index) {
                    printer->setDefaultColorModel(modes.at(index));
                }
            }
            break;
        case DescriptionRole:
            printer->setDescription(value.toString());
            break;
        case DuplexRole: {
                int index = value.toInt();
                auto modes = printer->supportedDuplexModes();
                if (index >= 0 && modes.size() > index) {
                    printer->setDefaultDuplexMode(modes.at(index));
                }
            }
            break;
        case PageSizeRole: {
                int index = value.toInt();
                QList<QPageSize> sizes = printer->supportedPageSizes();
                if (index >= 0 && sizes.size() > index) {
                    printer->setDefaultPageSize(sizes.at(index));
                }
            }
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
        names[ColorModelRole] = "colorModel";
        names[SupportedColorModelsRole] = "supportedColorModels";
        names[CopiesRole] = "copies";
        names[DefaultPrinterRole] = "default";
        names[DuplexRole] = "duplexMode";
        names[SupportedDuplexModesRole] = "supportedDuplexModes";
        names[NameRole] = "name";
        names[PrintRangeRole] = "printRange";
        names[PrintRangeModeRole] = "printRangeMode";
        names[PdfModeRole] = "pdfMode";
        names[QualityRole] = "quality";
        names[DescriptionRole] = "description";
        names[PageSizeRole] = "pageSize";
        names[SupportedPageSizesRole] = "supportedPageSizes";
        names[AccessControlRole] = "accessControl";
        names[ErrorPolicyRole] = "errorPolicy";
        names[UsersRole] = "users";
        names[StateRole] = "state";
        names[PrinterRole] = "printer";
        names[IsPdfRole] = "isPdf";
        names[LastStateMessageRole] = "lastStateMessage";
    }

    return names;
}

QVariantMap PrinterModel::get(const int row) const
{
    QHashIterator<int, QByteArray> iterator(roleNames());
    QVariantMap result;
    QModelIndex modelIndex = index(row, 0);

    while (iterator.hasNext()) {
        iterator.next();
        result[iterator.value()] = modelIndex.data(iterator.key());
    }

    return result;
}

QSharedPointer<Printer> PrinterModel::getPrinterFromName(const QString &name)
{
    Q_UNUSED(name);
    return QSharedPointer<Printer>(nullptr);
}

PrinterFilter::PrinterFilter(QObject *parent) : QSortFilterProxyModel(parent)
{
    connect(this, SIGNAL(sourceModelChanged()), SLOT(onSourceModelChanged()));
}

PrinterFilter::~PrinterFilter()
{

}

QVariantMap PrinterFilter::get(const int row) const
{
    QHashIterator<int, QByteArray> iterator(roleNames());
    QVariantMap result;
    QModelIndex modelIndex = index(row, 0);

    while (iterator.hasNext()) {
        iterator.next();
        result[iterator.value()] = modelIndex.data(iterator.key());
    }

    return result;
}

void PrinterFilter::onSourceModelChanged()
{
    connect((PrinterModel*) sourceModel(),
            SIGNAL(countChanged()),
            this,
            SIGNAL(countChanged()));
}

void PrinterFilter::onSourceModelCountChanged()
{
    Q_EMIT countChanged();
}

int PrinterFilter::count() const
{
    return rowCount();
}

void PrinterFilter::filterOnState(const PrinterEnum::State &state)
{
    Q_UNUSED(state);
}

void PrinterFilter::filterOnRecent(const bool recent)
{
    Q_UNUSED(recent);
}

void PrinterFilter::filterOnPdf(const bool pdf)
{
    m_pdfEnabled = pdf;
}

bool PrinterFilter::filterAcceptsRow(int sourceRow,
                                     const QModelIndex &sourceParent) const
{
    bool accepts = true;
    QModelIndex childIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    if (accepts && m_recentEnabled) {
        // TODO: implement recent
    }

    // If pdfEnabled is false we only want real printers
    if (accepts && m_pdfEnabled) {
        bool isPdf = (bool) childIndex.model()->data(
            childIndex, PrinterModel::IsPdfRole).toBool();
        accepts = isPdf == m_pdf;
    }

    if (accepts && m_stateEnabled) {
        const PrinterEnum::State state =
            (PrinterEnum::State) childIndex.model()->data(
                childIndex, PrinterModel::StateRole
            ).toInt();
        accepts = m_state == state;
    }

    return accepts;
}
bool PrinterFilter::lessThan(const QModelIndex &left,
                             const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left, sortRole());
    QVariant rightData = sourceModel()->data(right, sortRole());
    if ((QMetaType::Type) leftData.type() == QMetaType::Bool) {
        // FIXME: hack to put Pdf printer at the bottom
        if (leftData.toInt() == rightData.toInt()) {
            int leftPdf = sourceModel()->data(left, PrinterModel::IsPdfRole).toInt();
            int rightPdf = sourceModel()->data(right, PrinterModel::IsPdfRole).toInt();

            return leftPdf > rightPdf;
        } else {
            return leftData.toInt() < rightData.toInt();
        }
    } else {
        return leftData < rightData;
    }
}

