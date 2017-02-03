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

#include "backend/backend_cups.h"
#include "cups/cupsfacade.h"
#include "models/jobmodel.h"
#include "models/printermodel.h"

#include <QDebug>
#include <QQmlEngine>

PrinterModel::PrinterModel(QObject *parent)
    : PrinterModel(new PrinterCupsBackend, parent)
{
}

PrinterModel::PrinterModel(PrinterBackend *backend, QObject *parent)
    : QAbstractListModel(parent)
    , m_backend(backend)
{
    update();

    QObject::connect(m_backend, &PrinterBackend::printerAdded,
                     this, &PrinterModel::printerSignalCatchall);
    QObject::connect(m_backend, &PrinterBackend::printerModified,
                     this, &PrinterModel::printerSignalCatchall);
    QObject::connect(m_backend, &PrinterBackend::printerDeleted,
                     this, &PrinterModel::printerSignalCatchall);
}

PrinterModel::~PrinterModel()
{
}

void PrinterModel::printerSignalCatchall(
    const QString &text, const QString &printerUri,
    const QString &printerName, uint printerState,
    const QString &printerStateReason, bool acceptingJobs)
{
    Q_UNUSED(text);
    Q_UNUSED(printerUri);
    Q_UNUSED(printerName);
    Q_UNUSED(printerState);
    Q_UNUSED(printerStateReason);
    Q_UNUSED(acceptingJobs);
    update();
}

void PrinterModel::update()
{
    // Store the old count and get the new printers
    int oldCount = m_printers.size();
    QList<Printer*> newPrinters = m_backend->availablePrinters();

    /* If any printers returned from the backend are irrelevant, we delete
    them. This a list of indices that corresponds to printers scheduled for
    deletion in newPrinters. */
    QList<uint> forDeletion;

    // Go through the old model
    for (int i=0; i < m_printers.count(); i++) {
        // Determine if the old printer exists in the new model
        bool exists = false;

        Q_FOREACH(Printer *p, newPrinters) {
            if (p->name() == m_printers.at(i)->name()) {
                exists = true;

                // Ensure the other properties of the Printer are up to date
                if (m_printers.at(i)->updateFrom(p)) {
                    Q_EMIT dataChanged(index(i), index(i), roleNames().keys().toVector());
                }

                break;
            }
        }

        // If it doesn't exist then remove it from the old model
        if (!exists) {
            beginRemoveRows(QModelIndex(), i, i);
            Printer *p = m_printers.takeAt(i);
            JobModel *jobModel = m_job_models.take(p->name());

            p->deleteLater();
            jobModel->deleteLater();

            endRemoveRows();

            i--;  // as we have removed an item decrement
        }
    }

    // Go through the new model
    for (int i=0; i < newPrinters.count(); i++) {
        // Determine if the new printer exists in the old model
        bool exists = false;
        int j;

        for (j=0; j < m_printers.count(); j++) {
            if (m_printers.at(j)->name() == newPrinters.at(i)->name()) {
                exists = true;
                forDeletion << i;
                break;
            }
        }

        if (exists) {
            if (j == i) {  // New printer exists and in correct position
                continue;
            } else {
                // New printer does exist but needs to be moved in old model
                beginMoveRows(QModelIndex(), j, 1, QModelIndex(), i);
                m_printers.move(j, i);
                endMoveRows();
            }

            // We can safely delete the newPrinter as it already exists.
            forDeletion << i;
        } else {
            // New printer does not exist insert into model
            beginInsertRows(QModelIndex(), i, i);

            m_printers.insert(i, newPrinters.at(i));

            JobModel *model = new JobModel(newPrinters.at(i)->name(), m_backend, 5000, this);
            QQmlEngine::setObjectOwnership(model, QQmlEngine::CppOwnership);
            m_job_models.insert(newPrinters.at(i)->name(), model);

            endInsertRows();
        }
    }

    Q_FOREACH(const int &index, forDeletion) {
        newPrinters.at(index)->deleteLater();
    }

    if (oldCount != m_printers.size()) {
        Q_EMIT countChanged();
    }
}

int PrinterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_printers.size();
}

int PrinterModel::count() const
{
    return rowCount();
}

QVariant PrinterModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if ((0<=index.row()) && (index.row()<m_printers.size())) {

        auto printer = m_printers[index.row()];

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
        case PrintQualityRole:
            ret = printer->supportedPrintQualities().indexOf(printer->defaultPrintQuality());
            break;
        case SupportedPrintQualitiesRole: {
                QStringList qualities;
                Q_FOREACH(const PrintQuality &q, printer->supportedPrintQualities()) {
                    qualities.append(q.text.isEmpty() ? q.name : q.text);
                }
                ret = qualities;
            }
            break;
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
            ret = QVariant::fromValue(printer);
            break;
        case IsPdfRole:
            ret = printer->isPdf();
            break;
        case JobRole: {
            ret = QVariant::fromValue(m_job_models.value(printer->name()));
            break;
        }
        // case LastStateMessageRole:
        //     ret = printer->lastStateMessage();
        //     break;
        }
    }

    return ret;
}

bool PrinterModel::setData(const QModelIndex &index,
                           const QVariant &value, int role)
{
    if ((0<=index.row()) && (index.row()<m_printers.size())) {

        auto printer = m_printers[index.row()];

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
        case PrintQualityRole: {
                int index = value.toInt();
                QList<PrintQuality> quals = printer->supportedPrintQualities();
                if (index >= 0 && quals.size() > index) {
                    printer->setDefaultPrintQuality(quals.at(index));
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
        names[PrintQualityRole] = "printQuality";
        names[SupportedPrintQualitiesRole] = "supportedPrintQualities";
        names[DescriptionRole] = "description";
        names[PageSizeRole] = "pageSize";
        names[SupportedPageSizesRole] = "supportedPageSizes";
        names[AccessControlRole] = "accessControl";
        names[ErrorPolicyRole] = "errorPolicy";
        names[UsersRole] = "users";
        names[StateRole] = "state";
        names[PrinterRole] = "printer";
        names[IsPdfRole] = "isPdf";
        names[JobRole] = "jobs";
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

Printer* PrinterModel::getPrinterFromName(const QString &name)
{
    Q_UNUSED(name);
    return Q_NULLPTR;
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
    m_pdfEnabled = true;
    m_pdf = pdf;
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

            // If Pdf is also same then sort by name
            if (leftPdf == rightPdf) {
                QString leftName = sourceModel()->data(left, PrinterModel::NameRole).toString();
                QString rightName = sourceModel()->data(right, PrinterModel::NameRole).toString();

                return leftName > rightName;
            } else {
                return leftPdf > rightPdf;
            }
        } else {
            return leftData.toInt() < rightData.toInt();
        }
    } else {
        return leftData < rightData;
    }
}

